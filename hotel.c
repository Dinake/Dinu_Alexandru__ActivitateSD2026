#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Camera Camera;
typedef struct Nod Nod;

struct Camera {
    int numar;
    char* tip;
    float pret;
    int ocupata;
};

struct Nod {
    Camera info;
    Nod* prev;
    Nod* next;
};

char* copiazaText(const char* text) {
    char* copie = (char*)malloc(strlen(text) + 1);
    strcpy(copie, text);
    return copie;
}

Camera initCamera(int numar, const char* tip, float pret, int ocupata) {
    Camera c;
    c.numar = numar;
    c.tip = copiazaText(tip);
    c.pret = pret;
    c.ocupata = ocupata;
    return c;
}

Camera copieCamera(Camera c) {
    return initCamera(c.numar, c.tip, c.pret, c.ocupata);
}

void afisareCamera(Camera c) {
    printf("%d %s %.2f %s\n", c.numar, c.tip, c.pret, c.ocupata ? "ocupata" : "libera");
}

void inserareSfarsit(Nod** head, Camera c) {
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
        afisareCamera(head->info);
        head = head->next;
    }
}

void traversareInapoi(Nod* head) {
    if (head == NULL) return;
    while (head->next != NULL) head = head->next;
    while (head != NULL) {
        afisareCamera(head->info);
        head = head->prev;
    }
}

int numaraCamereLibere(Nod* head) {
    int count = 0;
    while (head != NULL) {
        if (!head->info.ocupata) count++;
        head = head->next;
    }
    return count;
}

void stergeOcupate(Nod** head) {
    Nod* temp = *head;
    while (temp != NULL) {
        if (temp->info.ocupata) {
            if (temp->prev != NULL) temp->prev->next = temp->next;
            else *head = temp->next;
            if (temp->next != NULL) temp->next->prev = temp->prev;
            Nod* aux = temp;
            temp = temp->next;
            free(aux->info.tip);
            free(aux);
        } else {
            temp = temp->next;
        }
    }
}

Camera* copieVectorTip(Nod* head, const char* tip, int* dim) {
    *dim = 0;
    Nod* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->info.tip, tip) == 0) (*dim)++;
        temp = temp->next;
    }
    if (*dim == 0) return NULL;
    Camera* v = (Camera*)malloc(sizeof(Camera) * (*dim));
    int i = 0;
    temp = head;
    while (temp != NULL) {
        if (strcmp(temp->info.tip, tip) == 0) v[i++] = copieCamera(temp->info);
        temp = temp->next;
    }
    return v;
}

void scrieFisier(Nod* head, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "w");
    while (head != NULL) {
        fprintf(f, "%d,%s,%.2f,%d\n", head->info.numar, head->info.tip, head->info.pret, head->info.ocupata);
        head = head->next;
    }
    fclose(f);
}

void eliberareLista(Nod** head) {
    while (*head != NULL) {
        Nod* temp = *head;
        *head = (*head)->next;
        free(temp->info.tip);
        free(temp);
    }
}

int main() {
    Nod* lista = NULL;
    inserareSfarsit(&lista, initCamera(101, "single", 150.0f, 0));
    inserareSfarsit(&lista, initCamera(102, "double", 250.0f, 1));
    inserareSfarsit(&lista, initCamera(103, "single", 150.0f, 1));
    inserareSfarsit(&lista, initCamera(201, "suite", 500.0f, 0));
    inserareSfarsit(&lista, initCamera(202, "double", 250.0f, 0));

    printf("Inainte\n");
    traversareInainte(lista);
    printf("Inapoi\n");
    traversareInapoi(lista);
    printf("Camere libere %d\n", numaraCamereLibere(lista));

    stergeOcupate(&lista);
    printf("Dupa stergere\n");
    traversareInainte(lista);

    int dim = 0;
    Camera* v = copieVectorTip(lista, "single", &dim);
    printf("Single %d\n", dim);
    for (int i = 0; i < dim; i++) afisareCamera(v[i]);
    for (int i = 0; i < dim; i++) free(v[i].tip);
    free(v);

    scrieFisier(lista, "camere.txt");
    eliberareLista(&lista);
    return 0;
}
