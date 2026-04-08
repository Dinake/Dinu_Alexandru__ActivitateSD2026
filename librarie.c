#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Carte Carte;
typedef struct Nod Nod;

struct Carte {
    int id;
    char* titlu;
    char* autor;
    int anPublicare;
    int disponibila;
};

struct Nod {
    Carte info;
    Nod* prev;
    Nod* next;
};

char* copiazaText(const char* text) {
    char* copie = (char*)malloc(strlen(text) + 1);
    strcpy(copie, text);
    return copie;
}

Carte initCarte(int id, const char* titlu, const char* autor, int an, int disponibila) {
    Carte c;
    c.id = id;
    c.titlu = copiazaText(titlu);
    c.autor = copiazaText(autor);
    c.anPublicare = an;
    c.disponibila = disponibila;
    return c;
}

Carte copieCarte(Carte c) {
    return initCarte(c.id, c.titlu, c.autor, c.anPublicare, c.disponibila);
}

void afisareCarte(Carte c) {
    printf("%d %s %s %d %s\n", c.id, c.titlu, c.autor, c.anPublicare, c.disponibila ? "da" : "nu");
}

void inserareSfarsit(Nod** head, Carte c) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = c;
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
        afisareCarte(head->info);
        head = head->next;
    }
}

void traversareInapoi(Nod* head) {
    if (head == NULL) return;
    while (head->next != NULL) head = head->next;
    while (head != NULL) {
        afisareCarte(head->info);
        head = head->prev;
    }
}

int numaraDisponibile(Nod* head) {
    int count = 0;
    while (head != NULL) {
        if (head->info.disponibila) count++;
        head = head->next;
    }
    return count;
}

void stergeVechi(Nod** head, int anPrag) {
    Nod* temp = *head;
    while (temp != NULL) {
        if (temp->info.anPublicare < anPrag) {
            if (temp->prev != NULL) temp->prev->next = temp->next;
            else *head = temp->next;
            if (temp->next != NULL) temp->next->prev = temp->prev;
            Nod* aux = temp;
            temp = temp->next;
            free(aux->info.titlu);
            free(aux->info.autor);
            free(aux);
        } else {
            temp = temp->next;
        }
    }
}

Carte* copieVectorAutor(Nod* head, const char* autor, int* dim) {
    *dim = 0;
    Nod* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->info.autor, autor) == 0) (*dim)++;
        temp = temp->next;
    }
    if (*dim == 0) return NULL;
    Carte* v = (Carte*)malloc(sizeof(Carte) * (*dim));
    int i = 0;
    temp = head;
    while (temp != NULL) {
        if (strcmp(temp->info.autor, autor) == 0) v[i++] = copieCarte(temp->info);
        temp = temp->next;
    }
    return v;
}

void scrieFisier(Nod* head, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "w");
    while (head != NULL) {
        fprintf(f, "%d,%s,%s,%d,%d\n", head->info.id, head->info.titlu, head->info.autor, head->info.anPublicare, head->info.disponibila);
        head = head->next;
    }
    fclose(f);
}

void eliberareLista(Nod** head) {
    while (*head != NULL) {
        Nod* temp = *head;
        *head = (*head)->next;
        free(temp->info.titlu);
        free(temp->info.autor);
        free(temp);
    }
}

int main() {
    Nod* lista = NULL;
    inserareSfarsit(&lista, initCarte(1, "Algoritmi", "Cormen", 2009, 1));
    inserareSfarsit(&lista, initCarte(2, "CleanCode", "Martin", 2008, 0));
    inserareSfarsit(&lista, initCarte(3, "Structuri", "Cormen", 1995, 1));
    inserareSfarsit(&lista, initCarte(4, "Sisteme", "Tanenbaum", 2014, 1));
    inserareSfarsit(&lista, initCarte(5, "Retele", "Tanenbaum", 2010, 0));

    printf("Inainte\n");
    traversareInainte(lista);
    printf("Inapoi\n");
    traversareInapoi(lista);
    printf("Disponibile %d\n", numaraDisponibile(lista));

    stergeVechi(&lista, 2000);
    printf("Dupa stergere\n");
    traversareInainte(lista);

    int dim = 0;
    Carte* v = copieVectorAutor(lista, "Cormen", &dim);
    printf("Cormen %d\n", dim);
    for (int i = 0; i < dim; i++) afisareCarte(v[i]);
    for (int i = 0; i < dim; i++) { free(v[i].titlu); free(v[i].autor); }
    free(v);

    scrieFisier(lista, "carti.txt");
    eliberareLista(&lista);
    return 0;
}
