#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ====== STRUCTURI ======

// Polita de asigurare
struct Polita {
    int numarPolita;
    char* numeAsigurat;
    char* tipAsigurare;      // RCA, CASCO, Locuinta, Viata
    float primaAsigurare;    // prima platita de asigurat
    float sumaAsigurata;     // suma maxima acoperita
};

// Dosar de dauna
struct DosarDauna {
    int numarDosar;
    int numarPolita;         // legatura cu polita
    char* descriere;
    float valoareDauna;
    char* dataEveniment;     // format: dd.mm.yyyy
};

// Nod lista simplu inlantuita pentru dosare
struct Nod {
    struct DosarDauna data;
    struct Nod* next;
};

// ====== FUNCTII AUXILIARE ======

char* copieString(const char* sursa) {
    if (sursa == NULL) return NULL;
    char* copie = (char*)malloc(sizeof(char) * (strlen(sursa) + 1));
    if (copie) strcpy(copie, sursa);
    return copie;
}

// ====== POLITE - VECTOR ======

struct Polita initPolita(int numar, const char* nume, const char* tip, float prima, float suma) {
    struct Polita p;
    p.numarPolita = numar;
    p.numeAsigurat = copieString(nume);
    p.tipAsigurare = copieString(tip);
    p.primaAsigurare = prima;
    p.sumaAsigurata = suma;
    return p;
}

void afisarePolita(struct Polita p) {
    printf("  Polita #%-4d | Asigurat: %-20s | Tip: %-10s | Prima: %8.2f RON | Suma: %10.2f RON\n",
        p.numarPolita, p.numeAsigurat, p.tipAsigurare, p.primaAsigurare, p.sumaAsigurata);
}

void afisarePolite(struct Polita* polite, int nrPolite) {
    if (nrPolite == 0) {
        printf("  Nu exista polite.\n");
        return;
    }
    for (int i = 0; i < nrPolite; i++) {
        printf("  [%d] ", i + 1);
        afisarePolita(polite[i]);
    }
}

void eliberarePolita(struct Polita* p) {
    if (p->numeAsigurat) { free(p->numeAsigurat); p->numeAsigurat = NULL; }
    if (p->tipAsigurare) { free(p->tipAsigurare); p->tipAsigurare = NULL; }
}

// Citire polite din fisier
// Format fisier: numarPolita,numeAsigurat,tipAsigurare,primaAsigurare,sumaAsigurata
struct Polita* citirePolitedinFisier(const char* numeFisier, int* nrPolite) {
    FILE* f = fopen(numeFisier, "r");
    if (f == NULL) {
        printf("Eroare la deschiderea fisierului %s\n", numeFisier);
        *nrPolite = 0;
        return NULL;
    }

    // Citim numarul de polite din prima linie
    fscanf(f, "%d", nrPolite);
    fgetc(f); // consumam newline

    struct Polita* polite = (struct Polita*)malloc(sizeof(struct Polita) * (*nrPolite));

    for (int i = 0; i < *nrPolite; i++) {
        int numar;
        char nume[100], tip[50];
        float prima, suma;

        fscanf(f, "%d,%[^,],%[^,],%f,%f", &numar, nume, tip, &prima, &suma);
        fgetc(f); // consumam newline

        polite[i] = initPolita(numar, nume, tip, prima, suma);
    }

    fclose(f);
    return polite;
}

// Adaugare polita in vector (realocare)
struct Polita* adaugaPolita(struct Polita* polite, int* nrPolite, struct Polita politaNoua) {
    (*nrPolite)++;
    polite = (struct Polita*)realloc(polite, sizeof(struct Polita) * (*nrPolite));
    polite[*nrPolite - 1] = politaNoua;
    return polite;
}

// Calcul total prime incasate
float totalPrime(struct Polita* polite, int nrPolite) {
    float total = 0;
    for (int i = 0; i < nrPolite; i++)
        total += polite[i].primaAsigurare;
    return total;
}

// ====== DOSARE DAUNA - LISTA SIMPLA INLANTUITA ======

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
    printf("  Dosar #%-4d | Polita: #%-4d | Valoare: %9.2f RON | Data: %-12s | %s\n",
        d.numarDosar, d.numarPolita, d.valoareDauna, d.dataEveniment, d.descriere);
}

void eliberareDosar(struct DosarDauna* d) {
    if (d->descriere) { free(d->descriere); d->descriere = NULL; }
    if (d->dataEveniment) { free(d->dataEveniment); d->dataEveniment = NULL; }
}

// Inserare dosar la sfarsitul listei simple
void adaugaDosar(struct Nod** head, struct DosarDauna dosar) {
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

// Afisare toate dosarele din lista
void afisareDosare(struct Nod* head) {
    if (head == NULL) {
        printf("  Nu exista dosare de dauna.\n");
        return;
    }
    struct Nod* temp = head;
    int i = 1;
    while (temp != NULL) {
        printf("  [%d] ", i++);
        afisareDosar(temp->data);
        temp = temp->next;
    }
}

// Citire dosar dupa numar
struct DosarDauna* cautaDosar(struct Nod* head, int numarDosar) {
    struct Nod* temp = head;
    while (temp != NULL) {
        if (temp->data.numarDosar == numarDosar)
            return &(temp->data);
        temp = temp->next;
    }
    return NULL;
}

// Citire dosare din fisier
// Format: numarDosar,numarPolita,descriere,valoareDauna,dataEveniment
struct Nod* citireDosaredinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (f == NULL) {
        printf("Eroare la deschiderea fisierului %s\n", numeFisier);
        return NULL;
    }

    struct Nod* head = NULL;
    int nrDosare;
    fscanf(f, "%d", &nrDosare);
    fgetc(f);

    for (int i = 0; i < nrDosare; i++) {
        int numarDosar, numarPolita;
        char descriere[200], data[20];
        float valoare;

        fscanf(f, "%d,%d,%[^,],%f,%s", &numarDosar, &numarPolita, descriere, &valoare, data);
        fgetc(f);

        adaugaDosar(&head, initDosar(numarDosar, numarPolita, descriere, valoare, data));
    }

    fclose(f);
    return head;
}

// Numar dosare in lista
int numarDosare(struct Nod* head) {
    int count = 0;
    struct Nod* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

// Calcul total daune
float totalDaune(struct Nod* head) {
    float total = 0;
    struct Nod* temp = head;
    while (temp != NULL) {
        total += temp->data.valoareDauna;
        temp = temp->next;
    }
    return total;
}

// ====== RATA DAUNA ======
// Rata dauna = (total daune / total prime) * 100
float calculRataDauna(struct Nod* headDosare, struct Polita* polite, int nrPolite) {
    float daune = totalDaune(headDosare);
    float prime = totalPrime(polite, nrPolite);
    if (prime == 0) return 0;
    return (daune / prime) * 100.0f;
}

// ====== ELIBERARE MEMORIE ======

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

void eliberareVectorPolite(struct Polita* polite, int nrPolite) {
    for (int i = 0; i < nrPolite; i++)
        eliberarePolita(&polite[i]);
    free(polite);
}

// ====== MAIN ======
int main() {

    // ====== 1. Creare vector de polite si lista de dosare ======
    printf("========== FIRMA DE ASIGURARI - Sistem de gestiune ==========\n\n");

    // --- Polite (vector) ---
    int nrPolite = 0;
    struct Polita* polite = NULL;

    // Incercam citirea din fisier
    polite = citirePolitedinFisier("polite.txt", &nrPolite);

    // Daca fisierul nu exista, cream date hardcodate
    if (polite == NULL) {
        printf("[INFO] Fisierul polite.txt nu a fost gasit. Se folosesc date predefinite.\n\n");
        nrPolite = 6;
        polite = (struct Polita*)malloc(sizeof(struct Polita) * nrPolite);
        polite[0] = initPolita(101, "Popescu Ion",       "RCA",      1200.00f,  50000.00f);
        polite[1] = initPolita(102, "Ionescu Maria",     "CASCO",    3500.00f, 120000.00f);
        polite[2] = initPolita(103, "Vasilescu Andrei",  "Locuinta",  800.00f, 200000.00f);
        polite[3] = initPolita(104, "Dumitrescu Elena",  "RCA",      1100.00f,  50000.00f);
        polite[4] = initPolita(105, "Marinescu Dan",     "CASCO",    4200.00f, 150000.00f);
        polite[5] = initPolita(106, "Georgescu Ana",     "Viata",    2000.00f, 500000.00f);
    }

    printf("===== POLITE DE ASIGURARE (vector) =====\n");
    afisarePolite(polite, nrPolite);

    // --- Dosare de dauna (lista simpla) ---
    struct Nod* dosare = NULL;

    // Incercam citirea din fisier
    dosare = citireDosaredinFisier("dosare.txt");

    // Daca fisierul nu exista, cream date hardcodate
    if (dosare == NULL) {
        printf("\n[INFO] Fisierul dosare.txt nu a fost gasit. Se folosesc date predefinite.\n\n");
        adaugaDosar(&dosare, initDosar(1001, 101, "Accident rutier - tamponare spate",    8500.00f,  "15.01.2026"));
        adaugaDosar(&dosare, initDosar(1002, 102, "Furt partial - oglinzi si jante",     12000.00f,  "22.01.2026"));
        adaugaDosar(&dosare, initDosar(1003, 103, "Inundatie apartament - etaj 3",       35000.00f,  "03.02.2026"));
        adaugaDosar(&dosare, initDosar(1004, 101, "Accident rutier - intersectie",       15000.00f,  "10.02.2026"));
        adaugaDosar(&dosare, initDosar(1005, 105, "Grindina - caroserie avariata",       22000.00f,  "18.02.2026"));
    }

    printf("===== DOSARE DE DAUNA (lista simpla inlantuita) =====\n");
    afisareDosare(dosare);

    // ====== 2. Adaugare dosar nou ======
    printf("\n===== ADAUGARE DOSAR NOU =====\n");
    struct DosarDauna dosarNou = initDosar(1006, 104, "Accident rutier - Loss total", 42000.00f, "05.03.2026");
    adaugaDosar(&dosare, dosarNou);
    printf("  Dosar #1006 adaugat cu succes.\n");

    printf("\n===== LISTA DOSARE DUPA ADAUGARE =====\n");
    afisareDosare(dosare);

    // ====== 3. Cautare / citire dosar dupa numar ======
    printf("\n===== CAUTARE DOSAR #1003 =====\n");
    struct DosarDauna* gasit = cautaDosar(dosare, 1003);
    if (gasit != NULL) {
        printf("  Dosar gasit:\n  ");
        afisareDosar(*gasit);
    } else {
        printf("  Dosarul nu a fost gasit.\n");
    }

    printf("\n===== CAUTARE DOSAR #9999 (inexistent) =====\n");
    gasit = cautaDosar(dosare, 9999);
    if (gasit != NULL) {
        printf("  Dosar gasit:\n  ");
        afisareDosar(*gasit);
    } else {
        printf("  Dosarul #9999 nu a fost gasit.\n");
    }

    // ====== 4. Adaugare polita noua in vector ======
    printf("\n===== ADAUGARE POLITA NOUA IN VECTOR =====\n");
    polite = adaugaPolita(polite, &nrPolite, initPolita(107, "Stanescu Mihai", "RCA", 1350.00f, 50000.00f));
    printf("  Polita #107 adaugata. Total polite: %d\n", nrPolite);

    printf("\n===== POLITE DUPA ADAUGARE =====\n");
    afisarePolite(polite, nrPolite);

    // ====== 5. Calcul total prime si total daune ======
    printf("\n===== CALCUL FINANCIAR =====\n");
    float prime = totalPrime(polite, nrPolite);
    float daune = totalDaune(dosare);
    int nrDos = numarDosare(dosare);

    printf("  Total prime incasate:      %10.2f RON\n", prime);
    printf("  Total daune platite:       %10.2f RON\n", daune);
    printf("  Numar dosare de dauna:     %d\n", nrDos);
    printf("  Dauna medie per dosar:     %10.2f RON\n", nrDos > 0 ? daune / nrDos : 0);

    // ====== 6. Calcul rata dauna ======
    printf("\n===== RATA DAUNA =====\n");
    float rata = calculRataDauna(dosare, polite, nrPolite);
    printf("  Rata dauna = (Total Daune / Total Prime) * 100\n");
    printf("  Rata dauna = (%.2f / %.2f) * 100 = %.2f%%\n", daune, prime, rata);

    if (rata < 60.0f)
        printf("  Status: PROFITABIL - rata dauna sub 60%%\n");
    else if (rata < 100.0f)
        printf("  Status: ATENTIE - rata dauna intre 60%% si 100%%\n");
    else
        printf("  Status: PIERDERE - rata dauna depaseste 100%%\n");

    // ====== Eliberare memorie ======
    eliberareLista(&dosare);
    eliberareVectorPolite(polite, nrPolite);

    return 0;
}
