#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pacient Pacient;
typedef struct Nod Nod;

struct Pacient {
    int id;
    char* nume;
    char* sectie;
    int varsta;
    float temperatura;
};

struct Nod {
    Pacient info;
    Nod* prev;
    Nod* next;
};

char* copiazaText(const char* text) {
    char* copie = (char*)malloc(strlen(text) + 1);
    strcpy(copie, text);
    return copie;
}

Pacient initPacient(int id, const char* nume, const char* sectie, int varsta, float temp) {
    Pacient p;
    p.id = id;
    p.nume = copiazaText(nume);
    p.sectie = copiazaText(sectie);
    p.varsta = varsta;
    p.temperatura = temp;
    return p;
}

Pacient copiePacient(Pacient p) {
    return initPacient(p.id, p.nume, p.sectie, p.varsta, p.temperatura);
}

void afisarePacient(Pacient p) {
    printf("%d %s %s %d %.1f\n", p.id, p.nume, p.sectie, p.varsta, p.temperatura);
}

void inserareSfarsit(Nod** head, Pacient p) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = p;
    nou->next = NULL;
    nou->prev = NULL;
    if (*head == NULL) {
        *head = nou;
    } else {
        Nod* temp = *head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = nou;
        nou->prev = temp;
    }
}

void traversareInainte(Nod* head) {
    while (head != NULL) {
        afisarePacient(head->info);
        head = head->next;
    }
}

void traversareInapoi(Nod* head) {
    if (head == NULL) return;
    while (head->next != NULL) head = head->next;
    while (head != NULL) {
        afisarePacient(head->info);
        head = head->prev;
    }
}

int numaraPacientiCuFebra(Nod* head, float prag) {
    int count = 0;
    while (head != NULL) {
        if (head->info.temperatura > prag) count++;
        head = head->next;
    }
    return count;
}

void stergeSectie(Nod** head, const char* sectie) {
    Nod* temp = *head;
    while (temp != NULL) {
        if (strcmp(temp->info.sectie, sectie) == 0) {
            if (temp->prev != NULL) temp->prev->next = temp->next;
            else *head = temp->next;
            if (temp->next != NULL) temp->next->prev = temp->prev;
            Nod* aux = temp;
            temp = temp->next;
            free(aux->info.nume);
            free(aux->info.sectie);
            free(aux);
        } else {
            temp = temp->next;
        }
    }
}

Pacient* copieVectorSectie(Nod* head, const char* sectie, int* dim) {
    *dim = 0;
    Nod* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->info.sectie, sectie) == 0) (*dim)++;
        temp = temp->next;
    }
    if (*dim == 0) return NULL;
    Pacient* v = (Pacient*)malloc(sizeof(Pacient) * (*dim));
    int i = 0;
    temp = head;
    while (temp != NULL) {
        if (strcmp(temp->info.sectie, sectie) == 0) v[i++] = copiePacient(temp->info);
        temp = temp->next;
    }
    return v;
}

void scrieFisier(Nod* head, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "w");
    while (head != NULL) {
        fprintf(f, "%d,%s,%s,%d,%.1f\n", head->info.id, head->info.nume, head->info.sectie, head->info.varsta, head->info.temperatura);
        head = head->next;
    }
    fclose(f);
}

void eliberareLista(Nod** head) {
    while (*head != NULL) {
        Nod* temp = *head;
        *head = (*head)->next;
        free(temp->info.nume);
        free(temp->info.sectie);
        free(temp);
    }
}

int main() {
    Nod* lista = NULL;
    inserareSfarsit(&lista, initPacient(1, "Ion", "cardiologie", 55, 36.8f));
    inserareSfarsit(&lista, initPacient(2, "Maria", "pediatrie", 8, 38.5f));
    inserareSfarsit(&lista, initPacient(3, "Gheorghe", "cardiologie", 70, 39.1f));
    inserareSfarsit(&lista, initPacient(4, "Ana", "neurologie", 42, 37.2f));
    inserareSfarsit(&lista, initPacient(5, "Radu", "pediatrie", 5, 40.0f));

    printf("Inainte\n");
    traversareInainte(lista);
    printf("Inapoi\n");
    traversareInapoi(lista);
    printf("Pacienti cu febra peste 38 %d\n", numaraPacientiCuFebra(lista, 38.0f));

    stergeSectie(&lista, "pediatrie");
    printf("Dupa stergere pediatrie\n");
    traversareInainte(lista);

    int dim = 0;
    Pacient* v = copieVectorSectie(lista, "cardiologie", &dim);
    printf("Cardiologie %d\n", dim);
    for (int i = 0; i < dim; i++) afisarePacient(v[i]);
    for (int i = 0; i < dim; i++) { free(v[i].nume); free(v[i].sectie); }
    free(v);

    scrieFisier(lista, "pacienti.txt");
    eliberareLista(&lista);
    return 0;
}
