#include <stdio.h>
#include <stdlib.h>

// Structura unui nod din arborele binar de cautare (ABC)
typedef struct Nod {
    int cheie;
    struct Nod* stanga;
    struct Nod* dreapta;
} Nod;

// Creeaza un nod nou cu cheia data
Nod* creareNod(int cheie) {
    Nod* nod = (Nod*)malloc(sizeof(Nod));
    if (!nod) {
        printf("Eroare la alocare memorie!\n");
        exit(1);
    }
    nod->cheie = cheie;
    nod->stanga = NULL;
    nod->dreapta = NULL;
    return nod;
}

// Inserare nod in ABC
Nod* inserare(Nod* radacina, int cheie) {
    if (radacina == NULL)
        return creareNod(cheie);

    if (cheie < radacina->cheie)
        radacina->stanga = inserare(radacina->stanga, cheie);
    else if (cheie > radacina->cheie)
        radacina->dreapta = inserare(radacina->dreapta, cheie);
    // cheile duplicate nu sunt inserate
    return radacina;
}

// Cautare nod cu o cheie data
Nod* cautare(Nod* radacina, int cheie) {
    if (radacina == NULL || radacina->cheie == cheie)
        return radacina;

    if (cheie < radacina->cheie)
        return cautare(radacina->stanga, cheie);
    return cautare(radacina->dreapta, cheie);
}

// Parcurgere in-ordine (stanga - radacina - dreapta) => rezultat sortat
void inOrdine(Nod* radacina) {
    if (radacina != NULL) {
        inOrdine(radacina->stanga);
        printf("%d ", radacina->cheie);
        inOrdine(radacina->dreapta);
    }
}

// Parcurgere pre-ordine (radacina - stanga - dreapta)
void preOrdine(Nod* radacina) {
    if (radacina != NULL) {
        printf("%d ", radacina->cheie);
        preOrdine(radacina->stanga);
        preOrdine(radacina->dreapta);
    }
}

// Parcurgere post-ordine (stanga - dreapta - radacina)
void postOrdine(Nod* radacina) {
    if (radacina != NULL) {
        postOrdine(radacina->stanga);
        postOrdine(radacina->dreapta);
        printf("%d ", radacina->cheie);
    }
}

// Gaseste nodul cu valoarea minima (cel mai din stanga)
Nod* minimNod(Nod* nod) {
    Nod* curent = nod;
    while (curent && curent->stanga != NULL)
        curent = curent->stanga;
    return curent;
}

// Stergere nod din ABC
Nod* stergere(Nod* radacina, int cheie) {
    if (radacina == NULL)
        return radacina;

    if (cheie < radacina->cheie)
        radacina->stanga = stergere(radacina->stanga, cheie);
    else if (cheie > radacina->cheie)
        radacina->dreapta = stergere(radacina->dreapta, cheie);
    else {
        // Nodul are cel mult un copil
        if (radacina->stanga == NULL) {
            Nod* temp = radacina->dreapta;
            free(radacina);
            return temp;
        } else if (radacina->dreapta == NULL) {
            Nod* temp = radacina->stanga;
            free(radacina);
            return temp;
        }
        // Nodul are doi copii: gasim succesorul in-ordine (minimul din subarborele drept)
        Nod* temp = minimNod(radacina->dreapta);
        radacina->cheie = temp->cheie;
        radacina->dreapta = stergere(radacina->dreapta, temp->cheie);
    }
    return radacina;
}

// Calculeaza inaltimea arborelui
int inaltime(Nod* radacina) {
    if (radacina == NULL)
        return 0;
    int stg = inaltime(radacina->stanga);
    int dr = inaltime(radacina->dreapta);
    return (stg > dr ? stg : dr) + 1;
}

// Numara nodurile din arbore
int numarNoduri(Nod* radacina) {
    if (radacina == NULL)
        return 0;
    return 1 + numarNoduri(radacina->stanga) + numarNoduri(radacina->dreapta);
}

// Verifica daca arborele este un ABC valid
int esteABC(Nod* nod, int minim, int maxim) {
    if (nod == NULL)
        return 1;
    if (nod->cheie <= minim || nod->cheie >= maxim)
        return 0;
    return esteABC(nod->stanga, minim, nod->cheie) &&
           esteABC(nod->dreapta, nod->cheie, maxim);
}

// Afiseaza arborele vizual (rotit 90 grade, dreapta sus)
void afisareVizuala(Nod* radacina, int spatiu) {
    if (radacina == NULL)
        return;
    spatiu += 5;
    afisareVizuala(radacina->dreapta, spatiu);
    printf("\n");
    for (int i = 5; i < spatiu; i++)
        printf(" ");
    printf("%d\n", radacina->cheie);
    afisareVizuala(radacina->stanga, spatiu);
}

// Elibereaza memoria alocata pentru intreg arborele
void elibereaza(Nod* radacina) {
    if (radacina != NULL) {
        elibereaza(radacina->stanga);
        elibereaza(radacina->dreapta);
        free(radacina);
    }
}

int main() {
    Nod* radacina = NULL;
    int optiune, valoare;

    printf("=== Arbore Binar de Cautare (ABC) ===\n");

    // Inserare valori de test
    int valoriInitiale[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(valoriInitiale) / sizeof(valoriInitiale[0]);

    for (int i = 0; i < n; i++)
        radacina = inserare(radacina, valoriInitiale[i]);

    printf("\nArborele initial (valori inserate: 50 30 70 20 40 60 80)\n");
    printf("Parcurgere in-ordine:   ");
    inOrdine(radacina);
    printf("\nParcurgere pre-ordine:  ");
    preOrdine(radacina);
    printf("\nParcurgere post-ordine: ");
    postOrdine(radacina);
    printf("\n");

    printf("Inaltimea arborelui: %d\n", inaltime(radacina));
    printf("Numarul de noduri: %d\n", numarNoduri(radacina));

    do {
        printf("\n--- MENIU ---\n");
        printf("1. Inserare nod\n");
        printf("2. Stergere nod\n");
        printf("3. Cautare nod\n");
        printf("4. Afisare in-ordine\n");
        printf("5. Afisare pre-ordine\n");
        printf("6. Afisare post-ordine\n");
        printf("7. Inaltimea arborelui\n");
        printf("8. Numarul de noduri\n");
        printf("9. Afisare vizuala arbore\n");
        printf("10. Verifica daca e ABC valid\n");
        printf("0. Iesire\n");
        printf("Optiunea ta: ");
        scanf("%d", &optiune);

        switch (optiune) {
            case 1:
                printf("Valoare de inserat: ");
                scanf("%d", &valoare);
                radacina = inserare(radacina, valoare);
                printf("Nodul %d a fost inserat.\n", valoare);
                break;
            case 2:
                printf("Valoare de sters: ");
                scanf("%d", &valoare);
                radacina = stergere(radacina, valoare);
                printf("Nodul %d a fost sters (daca exista).\n", valoare);
                break;
            case 3:
                printf("Valoare de cautat: ");
                scanf("%d", &valoare);
                if (cautare(radacina, valoare))
                    printf("Nodul %d a fost GASIT in arbore.\n", valoare);
                else
                    printf("Nodul %d NU exista in arbore.\n", valoare);
                break;
            case 4:
                printf("In-ordine: ");
                inOrdine(radacina);
                printf("\n");
                break;
            case 5:
                printf("Pre-ordine: ");
                preOrdine(radacina);
                printf("\n");
                break;
            case 6:
                printf("Post-ordine: ");
                postOrdine(radacina);
                printf("\n");
                break;
            case 7:
                printf("Inaltimea arborelui: %d\n", inaltime(radacina));
                break;
            case 8:
                printf("Numarul de noduri: %d\n", numarNoduri(radacina));
                break;
            case 9:
                printf("Afisare vizuala arbore:\n");
                afisareVizuala(radacina, 0);
                printf("\n");
                break;
            case 10:
                if (esteABC(radacina, -2147483648, 2147483647))
                    printf("Arborele este un ABC valid.\n");
                else
                    printf("Arborele NU este un ABC valid.\n");
                break;
            case 0:
                printf("La revedere!\n");
                break;
            default:
                printf("Optiune invalida!\n");
        }
    } while (optiune != 0);

    elibereaza(radacina);
    return 0;
}
