#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Comanda Comanda;
typedef struct Nod Nod;

struct Comanda {
    int id;
    char* client;
    float valoare;
    int prioritate;
};

struct Nod {
    Comanda info;
    Nod* next;
};

char* copiazaText(const char* text) {
    char* copie = (char*)malloc(strlen(text) + 1);
    strcpy(copie, text);
    return copie;
}

Comanda initComanda(int id, const char* client, float valoare, int prioritate) {
    Comanda c;
    c.id = id;
    c.client = copiazaText(client);
    c.valoare = valoare;
    c.prioritate = prioritate;
    return c;
}

void afisareComanda(Comanda c) {
    printf("%d %s %.2f %d\n", c.id, c.client, c.valoare, c.prioritate);
}

void push(Nod** varf, Comanda c) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = c;
    nou->next = *varf;
    *varf = nou;
}

Comanda pop(Nod** varf) {
    Nod* temp = *varf;
    Comanda c = temp->info;
    *varf = temp->next;
    free(temp);
    return c;
}

void enqueue(Nod** prim, Nod** ultim, Comanda c) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = c;
    nou->next = NULL;
    if (*ultim != NULL) {
        (*ultim)->next = nou;
    } else {
        *prim = nou;
    }
    *ultim = nou;
}

Comanda dequeue(Nod** prim, Nod** ultim) {
    Nod* temp = *prim;
    Comanda c = temp->info;
    *prim = temp->next;
    if (*prim == NULL) {
        *ultim = NULL;
    }
    free(temp);
    return c;
}

void afisareLista(Nod* cap) {
    while (cap != NULL) {
        afisareComanda(cap->info);
        cap = cap->next;
    }
}

float valoareTotala(Nod* cap) {
    float suma = 0;
    while (cap != NULL) {
        suma += cap->info.valoare;
        cap = cap->next;
    }
    return suma;
}

void scrieFisier(const char* numeFisier, Nod* cap) {
    FILE* f = fopen(numeFisier, "w");
    while (cap != NULL) {
        fprintf(f, "%d,%s,%.2f,%d\n", cap->info.id, cap->info.client, cap->info.valoare, cap->info.prioritate);
        cap = cap->next;
    }
    fclose(f);
}

void eliberareComanda(Comanda* c) {
    free(c->client);
    c->client = NULL;
}

void eliberareLista(Nod** cap) {
    while (*cap != NULL) {
        Nod* temp = *cap;
        *cap = (*cap)->next;
        eliberareComanda(&temp->info);
        free(temp);
    }
}

int main() {
    Nod* stiva = NULL;
    Nod* prim = NULL;
    Nod* ultim = NULL;

    push(&stiva, initComanda(1, "Ana", 250.5f, 2));
    push(&stiva, initComanda(2, "Mihai", 120.0f, 1));
    push(&stiva, initComanda(3, "Daria", 410.3f, 3));

    enqueue(&prim, &ultim, initComanda(4, "Vlad", 90.0f, 1));
    enqueue(&prim, &ultim, initComanda(5, "Irina", 300.0f, 2));
    enqueue(&prim, &ultim, initComanda(6, "Radu", 510.7f, 3));

    printf("Stiva\n");
    afisareLista(stiva);
    printf("Coada\n");
    afisareLista(prim);
    printf("Total coada %.2f\n", valoareTotala(prim));

    Comanda c1 = pop(&stiva);
    printf("Pop\n");
    afisareComanda(c1);
    eliberareComanda(&c1);

    Comanda c2 = dequeue(&prim, &ultim);
    printf("Dequeue\n");
    afisareComanda(c2);
    eliberareComanda(&c2);

    scrieFisier("comenzi_coada.txt", prim);
    eliberareLista(&stiva);
    eliberareLista(&prim);
    return 0;
}