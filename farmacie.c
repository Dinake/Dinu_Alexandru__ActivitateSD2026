#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. Structura Farmacie
struct Farmacie {
    char* denumire;        // denumire farmacie
    float suprafata;       // suprafata spatiu comercial in m^2
    unsigned char nrAngajati; // numar angajati
    char* adresa;          // adresa
};

// Nod pentru lista dubla
struct Nod {
    struct Farmacie data;
    struct Nod* prev;
    struct Nod* next;
};

// ====== Functii auxiliare ======

// Creaza o copie deep a unui string
char* copieString(const char* sursa) {
    if (sursa == NULL) return NULL;
    char* copie = (char*)malloc(sizeof(char) * (strlen(sursa) + 1));
    if (copie) strcpy(copie, sursa);
    return copie;
}

// Initializare Farmacie
struct Farmacie initFarmacie(const char* denumire, float suprafata, unsigned char nrAngajati, const char* adresa) {
    struct Farmacie f;
    f.denumire = copieString(denumire);
    f.suprafata = suprafata;
    f.nrAngajati = nrAngajati;
    f.adresa = copieString(adresa);
    return f;
}

// Copie profunda (deep copy) a unei Farmacii
struct Farmacie copieFarmacie(struct Farmacie sursa) {
    struct Farmacie copie;
    copie.denumire = copieString(sursa.denumire);
    copie.suprafata = sursa.suprafata;
    copie.nrAngajati = sursa.nrAngajati;
    copie.adresa = copieString(sursa.adresa);
    return copie;
}

// Afisare Farmacie
void afisareFarmacie(struct Farmacie f) {
    printf("  Denumire: %-20s | Suprafata: %6.1f m2 | Angajati: %3d | Adresa: %s\n",
        f.denumire, f.suprafata, (int)f.nrAngajati, f.adresa);
}

// Eliberare memorie Farmacie
void eliberareFarmacie(struct Farmacie* f) {
    if (f->denumire) { free(f->denumire); f->denumire = NULL; }
    if (f->adresa) { free(f->adresa); f->adresa = NULL; }
}

// ====== Operatii pe lista dubla ======

// Inserare la sfarsit
void inserareSfarsit(struct Nod** head, struct Farmacie f) {
    struct Nod* nou = (struct Nod*)malloc(sizeof(struct Nod));
    nou->data = f;
    nou->next = NULL;
    nou->prev = NULL;

    if (*head == NULL) {
        *head = nou;
    } else {
        struct Nod* temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = nou;
        nou->prev = temp;
    }
}

// Traversare lista dubla inainte (de la head la tail)
void traversareInainte(struct Nod* head) {
    if (head == NULL) {
        printf("  Lista este goala.\n");
        return;
    }
    struct Nod* temp = head;
    int i = 1;
    while (temp != NULL) {
        printf("  [%d] ", i++);
        afisareFarmacie(temp->data);
        temp = temp->next;
    }
}

// Traversare lista dubla inapoi (de la tail la head)
void traversareInapoi(struct Nod* head) {
    if (head == NULL) {
        printf("  Lista este goala.\n");
        return;
    }
    // Merg la ultimul nod
    struct Nod* temp = head;
    while (temp->next != NULL)
        temp = temp->next;
    int i = 1;
    while (temp != NULL) {
        printf("  [%d] ", i++);
        afisareFarmacie(temp->data);
        temp = temp->prev;
    }
}

// ====== 2. Numar farmacii cu suprafata < prag ======
int numaraFarmaciiSuprafata(struct Nod* head, float prag) {
    int count = 0;
    struct Nod* temp = head;
    while (temp != NULL) {
        if (temp->data.suprafata < prag)
            count++;
        temp = temp->next;
    }
    return count;
}

// ====== 3. Stergere farmacii cu nrAngajati > prag ======
void stergeFarmaciiAngajati(struct Nod** head, unsigned char prag) {
    struct Nod* temp = *head;
    while (temp != NULL) {
        if (temp->data.nrAngajati > prag) {
            struct Nod* deSters = temp;
            // Releg nodurile vecine
            if (deSters->prev != NULL)
                deSters->prev->next = deSters->next;
            else
                *head = deSters->next; // stergem primul nod

            if (deSters->next != NULL)
                deSters->next->prev = deSters->prev;

            temp = deSters->next;
            eliberareFarmacie(&deSters->data);
            free(deSters);
        } else {
            temp = temp->next;
        }
    }
}

// ====== 4. Copiere in vector dupa lant comercial ======
// Returneaza un vector alocat dinamic cu farmacii din acelasi lant comercial
// (farmacii a caror denumire contine lantul specificat).
// Vectorul NU partajeaza date cu lista (deep copy).
struct Farmacie* copieVectorLant(struct Nod* head, const char* lant, int* dimensiune) {
    *dimensiune = 0;
    // Numaram cate farmacii fac parte din lantul respectiv
    struct Nod* temp = head;
    while (temp != NULL) {
        if (strstr(temp->data.denumire, lant) != NULL)
            (*dimensiune)++;
        temp = temp->next;
    }

    if (*dimensiune == 0)
        return NULL;

    struct Farmacie* vector = (struct Farmacie*)malloc(sizeof(struct Farmacie) * (*dimensiune));
    temp = head;
    int idx = 0;
    while (temp != NULL) {
        if (strstr(temp->data.denumire, lant) != NULL) {
            vector[idx] = copieFarmacie(temp->data); // deep copy
            idx++;
        }
        temp = temp->next;
    }
    return vector;
}

// Eliberare memorie lista dubla
void eliberareLista(struct Nod** head) {
    struct Nod* temp = *head;
    while (temp != NULL) {
        struct Nod* urm = temp->next;
        eliberareFarmacie(&temp->data);
        free(temp);
        temp = urm;
    }
    *head = NULL;
}

// ====== MAIN ======
int main() {
    struct Nod* lista = NULL;

    // 1. Creare lista dubla cu cel putin 5 farmacii
    printf("===== 1. Creare lista dubla cu 6 farmacii =====\n\n");

    inserareSfarsit(&lista, initFarmacie("Catena Centru",       120.5,  8,  "Str. Victoriei 10, Bucuresti"));
    inserareSfarsit(&lista, initFarmacie("Catena Nord",          85.0,  5,  "Bd. Magheru 25, Bucuresti"));
    inserareSfarsit(&lista, initFarmacie("HelpNet Plaza",       200.0, 12,  "Calea Dorobanti 50, Bucuresti"));
    inserareSfarsit(&lista, initFarmacie("Sensiblu Mall",       150.3, 10,  "Str. Lipscani 3, Bucuresti"));
    inserareSfarsit(&lista, initFarmacie("Catena Sud",           60.0,  4,  "Bd. Unirii 18, Bucuresti"));
    inserareSfarsit(&lista, initFarmacie("HelpNet Residence",    95.7,  7,  "Str. Aviatorilor 5, Bucuresti"));

    printf("Traversare inainte:\n");
    traversareInainte(lista);
    printf("\nTraversare inapoi:\n");
    traversareInapoi(lista);

    // 2. Numara farmacii cu suprafata < prag
    printf("\n===== 2. Numar farmacii cu suprafata < 100 m2 =====\n\n");
    float pragSuprafata = 100.0f;
    int numar = numaraFarmaciiSuprafata(lista, pragSuprafata);
    printf("Farmacii cu suprafata mai mica de %.1f m2: %d\n", pragSuprafata, numar);

    // 3. Stergere farmacii cu nrAngajati > prag si validare prin traversare
    printf("\n===== 3. Stergere farmacii cu nr. angajati > 7 =====\n\n");
    unsigned char pragAngajati = 7;

    printf("INAINTE de stergere - traversare inainte:\n");
    traversareInainte(lista);
    printf("\nINAINTE de stergere - traversare inapoi:\n");
    traversareInapoi(lista);

    stergeFarmaciiAngajati(&lista, pragAngajati);

    printf("\nDUPA stergere - traversare inainte:\n");
    traversareInainte(lista);
    printf("\nDUPA stergere - traversare inapoi:\n");
    traversareInapoi(lista);

    // 4. Copiere in vector farmacii din acelasi lant comercial
    printf("\n===== 4. Copiere in vector farmacii din lantul 'Catena' =====\n\n");
    int dimVector = 0;
    struct Farmacie* vector = copieVectorLant(lista, "Catena", &dimVector);

    printf("S-au gasit %d farmacii din lantul Catena:\n", dimVector);
    for (int i = 0; i < dimVector; i++) {
        printf("  [%d] ", i + 1);
        afisareFarmacie(vector[i]);
    }

    // Eliberare vector
    for (int i = 0; i < dimVector; i++)
        eliberareFarmacie(&vector[i]);
    free(vector);

    // Eliberare lista
    eliberareLista(&lista);

    return 0;
}
