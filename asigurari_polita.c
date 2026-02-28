#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Polita {
    int numarPolita;
    char* numeAsigurat;
    char* tipAsigurare;
    float primaAsigurare;
    float sumaAsigurata;
};

struct Nod {
    struct Polita data;
    struct Nod* next;
};

char* copieString(const char* sursa) {
    if (sursa == NULL) return NULL;
    char* copie = (char*)malloc(sizeof(char) * (strlen(sursa) + 1));
    if (copie) strcpy(copie, sursa);
    return copie;
}

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
    printf("Polita #%-4d | Asigurat: %-20s | Tip: %-10s | Prima: %8.2f RON | Suma: %10.2f RON\n",
        p.numarPolita, p.numeAsigurat, p.tipAsigurare, p.primaAsigurare, p.sumaAsigurata);
}

void eliberarePolita(struct Polita* p) {
    if (p->numeAsigurat) { free(p->numeAsigurat); p->numeAsigurat = NULL; }
    if (p->tipAsigurare) { free(p->tipAsigurare); p->tipAsigurare = NULL; }
}

void inserareSfarsit(struct Nod** head, struct Polita p) {
    struct Nod* nou = (struct Nod*)malloc(sizeof(struct Nod));
    nou->data = p;
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
        afisarePolita(temp->data);
        temp = temp->next;
    }
}

struct Polita* cautaPolita(struct Nod* head, int numar) {
    struct Nod* temp = head;
    while (temp != NULL) {
        if (temp->data.numarPolita == numar)
            return &(temp->data);
        temp = temp->next;
    }
    return NULL;
}

int numarPolite(struct Nod* head) {
    int count = 0;
    struct Nod* temp = head;
    while (temp != NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

float totalPrime(struct Nod* head) {
    float total = 0;
    struct Nod* temp = head;
    while (temp != NULL) {
        total += temp->data.primaAsigurare;
        temp = temp->next;
    }
    return total;
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
        int numar;
        char nume[100], tip[50];
        float prima, suma;
        fscanf(f, "%d,%[^,],%[^,],%f,%f", &numar, nume, tip, &prima, &suma);
        fgetc(f);
        inserareSfarsit(&head, initPolita(numar, nume, tip, prima, suma));
    }
    fclose(f);
    return head;
}

void eliberareLista(struct Nod** head) {
    struct Nod* temp = *head;
    while (temp != NULL) {
        struct Nod* urm = temp->next;
        eliberarePolita(&temp->data);
        free(temp);
        temp = urm;
    }
    *head = NULL;
}

int main() {
    struct Nod* lista = NULL;

    lista = citireDinFisier("polite.txt");

    if (lista == NULL) {
        inserareSfarsit(&lista, initPolita(101, "Popescu Ion",       "RCA",      1200.00f,  50000.00f));
        inserareSfarsit(&lista, initPolita(102, "Ionescu Maria",     "CASCO",    3500.00f, 120000.00f));
        inserareSfarsit(&lista, initPolita(103, "Vasilescu Andrei",  "Locuinta",  800.00f, 200000.00f));
        inserareSfarsit(&lista, initPolita(104, "Dumitrescu Elena",  "RCA",      1100.00f,  50000.00f));
        inserareSfarsit(&lista, initPolita(105, "Marinescu Dan",     "CASCO",    4200.00f, 150000.00f));
        inserareSfarsit(&lista, initPolita(106, "Georgescu Ana",     "Viata",    2000.00f, 500000.00f));
    }

    printf("===== LISTA POLITE =====\n");
    afisareLista(lista);

    printf("\n===== ADAUGARE POLITA NOUA =====\n");
    inserareSfarsit(&lista, initPolita(107, "Stanescu Mihai", "RCA", 1350.00f, 50000.00f));
    afisareLista(lista);

    printf("\n===== CAUTARE POLITA #103 =====\n");
    struct Polita* gasit = cautaPolita(lista, 103);
    if (gasit)
        afisarePolita(*gasit);
    else
        printf("Polita nu a fost gasita.\n");

    printf("\n===== TOTAL =====\n");
    printf("Numar polite: %d\n", numarPolite(lista));
    printf("Total prime incasate: %.2f RON\n", totalPrime(lista));

    eliberareLista(&lista);
    return 0;
}
