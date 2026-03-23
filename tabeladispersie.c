#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Produs Produs;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Produs {
    int cod;
    char* denumire;
    float pret;
    int stoc;
};

struct Nod {
    Produs info;
    Nod* next;
};

struct HashTable {
    int dimensiune;
    Nod** vector;
};

char* copiazaText(const char* text) {
    char* copie = (char*)malloc(strlen(text) + 1);
    strcpy(copie, text);
    return copie;
}

Produs initProdus(int cod, const char* denumire, float pret, int stoc) {
    Produs p;
    p.cod = cod;
    p.denumire = copiazaText(denumire);
    p.pret = pret;
    p.stoc = stoc;
    return p;
}

HashTable initHashTable(int dimensiune) {
    HashTable tabela;
    tabela.dimensiune = dimensiune;
    tabela.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
    for (int i = 0; i < dimensiune; i++) {
        tabela.vector[i] = NULL;
    }
    return tabela;
}

int hash(int dimensiune, int cod) {
    return cod % dimensiune;
}

void inserareLista(Nod** cap, Produs p) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = p;
    nou->next = *cap;
    *cap = nou;
}

void inserareHashTable(HashTable tabela, Produs p) {
    int pozitie = hash(tabela.dimensiune, p.cod);
    inserareLista(&tabela.vector[pozitie], p);
}

void afisareProdus(Produs p) {
    printf("%d %s %.2f %d\n", p.cod, p.denumire, p.pret, p.stoc);
}

void afisareHashTable(HashTable tabela) {
    for (int i = 0; i < tabela.dimensiune; i++) {
        printf("Pozitie %d\n", i);
        Nod* cap = tabela.vector[i];
        while (cap != NULL) {
            afisareProdus(cap->info);
            cap = cap->next;
        }
    }
}

Produs* cautareProdus(HashTable tabela, int cod) {
    int pozitie = hash(tabela.dimensiune, cod);
    Nod* cap = tabela.vector[pozitie];
    while (cap != NULL) {
        if (cap->info.cod == cod) {
            return &cap->info;
        }
        cap = cap->next;
    }
    return NULL;
}

float valoareStoc(HashTable tabela) {
    float suma = 0;
    for (int i = 0; i < tabela.dimensiune; i++) {
        Nod* cap = tabela.vector[i];
        while (cap != NULL) {
            suma += cap->info.pret * cap->info.stoc;
            cap = cap->next;
        }
    }
    return suma;
}

void scrieFisier(HashTable tabela, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "w");
    for (int i = 0; i < tabela.dimensiune; i++) {
        Nod* cap = tabela.vector[i];
        while (cap != NULL) {
            fprintf(f, "%d,%s,%.2f,%d\n", cap->info.cod, cap->info.denumire, cap->info.pret, cap->info.stoc);
            cap = cap->next;
        }
    }
    fclose(f);
}

void dezalocareHashTable(HashTable* tabela) {
    for (int i = 0; i < tabela->dimensiune; i++) {
        Nod* cap = tabela->vector[i];
        while (cap != NULL) {
            Nod* temp = cap;
            cap = cap->next;
            free(temp->info.denumire);
            free(temp);
        }
    }
    free(tabela->vector);
    tabela->vector = NULL;
    tabela->dimensiune = 0;
}

int main() {
    HashTable tabela = initHashTable(5);
    inserareHashTable(tabela, initProdus(101, "Paracetamol", 12.5f, 40));
    inserareHashTable(tabela, initProdus(206, "Ibuprofen", 18.0f, 25));
    inserareHashTable(tabela, initProdus(311, "VitaminaC", 22.7f, 15));
    inserareHashTable(tabela, initProdus(416, "Calciu", 16.4f, 20));
    afisareHashTable(tabela);
    Produs* gasit = cautareProdus(tabela, 311);
    if (gasit != NULL) {
        printf("Gasit\n");
        afisareProdus(*gasit);
    }
    printf("Valoare %.2f\n", valoareStoc(tabela));
    scrieFisier(tabela, "produse_hash.txt");
    dezalocareHashTable(&tabela);
    return 0;
}