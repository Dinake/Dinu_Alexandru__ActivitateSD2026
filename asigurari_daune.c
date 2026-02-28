#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct DosarDauna {
    int numarDosar;
    int numarPolita;
    char* descriere;
    float valoareDauna;
    char* dataEveniment;
};

struct Nod {
    struct DosarDauna data;
    struct Nod* next;
};

char* copieString(const char* sursa) {
    if (sursa == NULL) return NULL;
    char* copie = (char*)malloc(sizeof(char) * (strlen(sursa) + 1));
    if (copie) strcpy(copie, sursa);
    return copie;
}

struct DosarDauna initDosar(int numarDosar, int numarPolita, const char* descriere,
    float valoare, const char* data) {
    struct DosarDauna d;
    d.numarDosar = numarDosar;
    d.numarPolita = numarPolita;
    d.descriere = copieString(descriere);
    d.valoareDauna = valoare;
    d.dataEveniment = copieString(data);
    return d;
}

void afisareDosar(struct DosarDauna d) {
    printf("Dosar #%-4d | Polita: #%-4d | Valoare: %9.2f RON | Data: %-12s | %s\n",
        d.numarDosar, d.numarPolita, d.valoareDauna, d.dataEveniment, d.descriere);
}

void eliberareDosar(struct DosarDauna* d) {
    if (d->descriere) { free(d->descriere); d->descriere = NULL; }
    if (d->dataEveniment) { free(d->dataEveniment); d->dataEveniment = NULL; }
}

void inserareSfarsit(struct Nod** head, struct DosarDauna dosar) {
    struct Nod* nou = (struct Nod*)malloc(sizeof(struct Nod));
    nou->data = dosar;
    nou->next = NULL;
    if (*head == NULL) {
        *head = nou;
    } else {
        struct Nod* temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = nou;
    }
}

void afisareLista(struct Nod* head) {
    if (head == NULL) {
        printf("Lista este goala.\n");
        return;
    }
    struct Nod* temp = head;
    int i = 1;
    while (temp != NULL) {
        printf("[%d] ", i++);
        afisareDosar(temp->data);
        temp = temp->next;
    }
}

struct DosarDauna* cautaDosar(struct Nod* head, int numarDosar) {
    struct Nod* temp = head;
    while (temp != NULL) {
        if (temp->data.numarDosar == numarDosar)
            return &(temp->data);
        temp = temp->next;
    }
    return NULL;
}

int numarDosare(struct Nod* head) {
    int count = 0;
    struct Nod* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

float totalDaune(struct Nod* head) {
    float total = 0;
    struct Nod* temp = head;
    while (temp != NULL) {
        total += temp->data.valoareDauna;
        temp = temp->next;
    }
    return total;
}

float calculRataDauna(struct Nod* head, float totalPrime) {
    if (totalPrime == 0) return 0;
    return (totalDaune(head) / totalPrime) * 100.0f;
}

struct Nod* citireDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (f == NULL) {
        printf("Fisierul %s nu a fost gasit.\n", numeFisier);
        return NULL;
    }
    struct Nod* head = NULL;
    int n;
    fscanf(f, "%d", &n);
    fgetc(f);
    for (int i = 0; i < n; i++) {
        int numarDosar, numarPolita;
        char descriere[200], data[20];
        float valoare;
        fscanf(f, "%d,%d,%[^,],%f,%s", &numarDosar, &numarPolita, descriere, &valoare, data);
        fgetc(f);
        inserareSfarsit(&head, initDosar(numarDosar, numarPolita, descriere, valoare, data));
    }
    fclose(f);
    return head;
}

void eliberareLista(struct Nod** head) {
    struct Nod* temp = *head;
    while (temp != NULL) {
        struct Nod* urm = temp->next;
        eliberareDosar(&temp->data);
        free(temp);
        temp = urm;
    }
    *head = NULL;
}

int main() {
    struct Nod* lista = NULL;

    lista = citireDinFisier("dosare.txt");

    if (lista == NULL) {
        inserareSfarsit(&lista, initDosar(1001, 101, "Accident rutier - tamponare spate",    8500.00f,  "15.01.2026"));
        inserareSfarsit(&lista, initDosar(1002, 102, "Furt partial - oglinzi si jante",     12000.00f,  "22.01.2026"));
        inserareSfarsit(&lista, initDosar(1003, 103, "Inundatie apartament - etaj 3",       35000.00f,  "03.02.2026"));
        inserareSfarsit(&lista, initDosar(1004, 101, "Accident rutier - intersectie",       15000.00f,  "10.02.2026"));
        inserareSfarsit(&lista, initDosar(1005, 105, "Grindina - caroserie avariata",       22000.00f,  "18.02.2026"));
    }

    printf("===== LISTA DOSARE DE DAUNA =====\n");
    afisareLista(lista);

    printf("\n===== ADAUGARE DOSAR NOU =====\n");
    inserareSfarsit(&lista, initDosar(1006, 104, "Accident rutier - Loss total", 42000.00f, "05.03.2026"));
    afisareLista(lista);

    printf("\n===== CAUTARE DOSAR #1003 =====\n");
    struct DosarDauna* gasit = cautaDosar(lista, 1003);
    if (gasit)
        afisareDosar(*gasit);
    else
        printf("Dosarul nu a fost gasit.\n");

    printf("\n===== CAUTARE DOSAR #9999 =====\n");
    gasit = cautaDosar(lista, 9999);
    if (gasit)
        afisareDosar(*gasit);
    else
        printf("Dosarul #9999 nu a fost gasit.\n");

    printf("\n===== CALCUL FINANCIAR =====\n");
    int nrDos = numarDosare(lista);
    float daune = totalDaune(lista);
    float primeIncasate = 14150.00f;

    printf("Numar dosare: %d\n", nrDos);
    printf("Total daune: %.2f RON\n", daune);
    printf("Dauna medie per dosar: %.2f RON\n", nrDos > 0 ? daune / nrDos : 0);

    printf("\n===== RATA DAUNA =====\n");
    float rata = calculRataDauna(lista, primeIncasate);
    printf("Rata dauna = (%.2f / %.2f) * 100 = %.2f%%\n", daune, primeIncasate, rata);

    if (rata < 60.0f)
        printf("Status: PROFITABIL\n");
    else if (rata < 100.0f)
        printf("Status: ATENTIE - rata dauna ridicata\n");
    else
        printf("Status: PIERDERE\n");

    eliberareLista(&lista);
    return 0;
}
