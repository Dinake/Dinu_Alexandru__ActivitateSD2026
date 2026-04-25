#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cerere Cerere;
typedef struct Heap Heap;

struct Cerere {
    int id;
    char* solicitant;
    float suma;
    int prioritate;
};

struct Heap {
    Cerere* vector;
    int dimensiune;
    int nrElemente;
};

char* copiazaText(const char* text) {
    char* copie = (char*)malloc(strlen(text) + 1);
    strcpy(copie, text);
    return copie;
}

Cerere initCerere(int id, const char* solicitant, float suma, int prioritate) {
    Cerere c;
    c.id = id;
    c.solicitant = copiazaText(solicitant);
    c.suma = suma;
    c.prioritate = prioritate;
    return c;
}

Heap initHeap(int dimensiune) {
    Heap heap;
    heap.vector = (Cerere*)malloc(sizeof(Cerere) * dimensiune);
    heap.dimensiune = dimensiune;
    heap.nrElemente = 0;
    return heap;
}

void interschimba(Cerere* a, Cerere* b) {
    Cerere temp = *a;
    *a = *b;
    *b = temp;
}

void filtrareSus(Heap* heap, int index) {
    while (index > 0) {
        int parinte = (index - 1) / 2;
        if (heap->vector[parinte].prioritate >= heap->vector[index].prioritate) {
            break;
        }
        interschimba(&heap->vector[parinte], &heap->vector[index]);
        index = parinte;
    }
}

void filtrareJos(Heap* heap, int index) {
    int pozitie = index;
    int stanga = index * 2 + 1;
    int dreapta = index * 2 + 2;
    if (stanga < heap->nrElemente && heap->vector[stanga].prioritate > heap->vector[pozitie].prioritate) {
        pozitie = stanga;
    }
    if (dreapta < heap->nrElemente && heap->vector[dreapta].prioritate > heap->vector[pozitie].prioritate) {
        pozitie = dreapta;
    }
    if (pozitie != index) {
        interschimba(&heap->vector[index], &heap->vector[pozitie]);
        filtrareJos(heap, pozitie);
    }
}

void inserareHeap(Heap* heap, Cerere c) {
    if (heap->nrElemente < heap->dimensiune) {
        heap->vector[heap->nrElemente] = c;
        filtrareSus(heap, heap->nrElemente);
        heap->nrElemente++;
    }
}

Cerere extragereHeap(Heap* heap) {
    Cerere extras = heap->vector[0];
    heap->vector[0] = heap->vector[heap->nrElemente - 1];
    heap->nrElemente--;
    if (heap->nrElemente > 0) {
        filtrareJos(heap, 0);
    }
    return extras;
}

void afisareCerere(Cerere c) {
    printf("%d %s %.2f %d\n", c.id, c.solicitant, c.suma, c.prioritate);
}

void afisareHeap(Heap heap) {
    for (int i = 0; i < heap.nrElemente; i++) {
        afisareCerere(heap.vector[i]);
    }
}

void scrieFisier(Heap heap, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "w");
    for (int i = 0; i < heap.nrElemente; i++) {
        fprintf(f, "%d,%s,%.2f,%d\n", heap.vector[i].id, heap.vector[i].solicitant, heap.vector[i].suma, heap.vector[i].prioritate);
    }
    fclose(f);
}

void eliberareCerere(Cerere* c) {
    free(c->solicitant);
    c->solicitant = NULL;
}

void dezalocareHeap(Heap* heap) {
    for (int i = 0; i < heap->nrElemente; i++) {
        eliberareCerere(&heap->vector[i]);
    }
    free(heap->vector);
    heap->vector = NULL;
    heap->dimensiune = 0;
    heap->nrElemente = 0;
}

int main() {
    Heap heap = initHeap(10);
    inserareHeap(&heap, initCerere(1, "Ana", 1200.0f, 2));
    inserareHeap(&heap, initCerere(2, "Mihai", 800.0f, 1));
    inserareHeap(&heap, initCerere(3, "Daria", 2300.0f, 5));
    inserareHeap(&heap, initCerere(4, "Radu", 1700.0f, 3));
    afisareHeap(heap);
    Cerere extras = extragereHeap(&heap);
    printf("Extras\n");
    afisareCerere(extras);
    eliberareCerere(&extras);
    scrieFisier(heap, "cereri_heap.txt");
    dezalocareHeap(&heap);
    return 0;
}