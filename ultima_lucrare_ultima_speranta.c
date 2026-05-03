#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Proiect Proiect;
typedef struct NodLista NodLista;
typedef struct NodArbore NodArbore;
typedef struct NodGraf NodGraf;
typedef struct Muchie Muchie;
typedef struct Heap Heap;
typedef struct HashTable HashTable;

struct Proiect {
    int id;
    char* nume;
    float buget;
    int prioritate;
};

struct NodLista {
    Proiect info;
    NodLista* next;
};

struct NodArbore {
    Proiect info;
    int inaltime;
    NodArbore* stanga;
    NodArbore* dreapta;
};

struct Muchie {
    NodGraf* info;
    Muchie* next;
};

struct NodGraf {
    Proiect info;
    NodGraf* next;
    Muchie* vecini;
};

struct Heap {
    Proiect* vector;
    int dimensiune;
    int nrElemente;
};

struct HashTable {
    NodLista** vector;
    int dimensiune;
};

char* copiazaText(const char* text) {
    char* copie = (char*)malloc(strlen(text) + 1);
    strcpy(copie, text);
    return copie;
}

Proiect initProiect(int id, const char* nume, float buget, int prioritate) {
    Proiect p;
    p.id = id;
    p.nume = copiazaText(nume);
    p.buget = buget;
    p.prioritate = prioritate;
    return p;
}

Proiect copieProiect(Proiect p) {
    return initProiect(p.id, p.nume, p.buget, p.prioritate);
}

void afisareProiect(Proiect p) {
    printf("%d %s %.2f %d\n", p.id, p.nume, p.buget, p.prioritate);
}

void eliberareProiect(Proiect* p) {
    free(p->nume);
    p->nume = NULL;
}

void push(NodLista** stiva, Proiect p) {
    NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
    nou->info = p;
    nou->next = *stiva;
    *stiva = nou;
}

Proiect pop(NodLista** stiva) {
    NodLista* temp = *stiva;
    Proiect p = temp->info;
    *stiva = temp->next;
    free(temp);
    return p;
}

void enqueue(NodLista** prim, NodLista** ultim, Proiect p) {
    NodLista* nou = (NodLista*)malloc(sizeof(NodLista));
    nou->info = p;
    nou->next = NULL;
    if (*ultim != NULL) {
        (*ultim)->next = nou;
    } else {
        *prim = nou;
    }
    *ultim = nou;
}

Proiect dequeue(NodLista** prim, NodLista** ultim) {
    NodLista* temp = *prim;
    Proiect p = temp->info;
    *prim = temp->next;
    if (*prim == NULL) {
        *ultim = NULL;
    }
    free(temp);
    return p;
}

Heap initHeap(int dimensiune) {
    Heap h;
    h.vector = (Proiect*)malloc(sizeof(Proiect) * dimensiune);
    h.dimensiune = dimensiune;
    h.nrElemente = 0;
    return h;
}

void swapProiect(Proiect* a, Proiect* b) {
    Proiect temp = *a;
    *a = *b;
    *b = temp;
}

void filtrareSus(Heap* h, int index) {
    while (index > 0) {
        int parinte = (index - 1) / 2;
        if (h->vector[parinte].prioritate >= h->vector[index].prioritate) {
            break;
        }
        swapProiect(&h->vector[parinte], &h->vector[index]);
        index = parinte;
    }
}

void inserareHeap(Heap* h, Proiect p) {
    if (h->nrElemente < h->dimensiune) {
        h->vector[h->nrElemente] = p;
        filtrareSus(h, h->nrElemente);
        h->nrElemente++;
    }
}

HashTable initHashTable(int dimensiune) {
    HashTable t;
    t.dimensiune = dimensiune;
    t.vector = (NodLista**)malloc(sizeof(NodLista*) * dimensiune);
    for (int i = 0; i < dimensiune; i++) {
        t.vector[i] = NULL;
    }
    return t;
}

int hash(int dimensiune, int id) {
    return id % dimensiune;
}

void inserareHash(HashTable t, Proiect p) {
    int pozitie = hash(t.dimensiune, p.id);
    push(&t.vector[pozitie], p);
}

NodArbore* inserareABC(NodArbore* radacina, Proiect p) {
    if (radacina == NULL) {
        NodArbore* nou = (NodArbore*)malloc(sizeof(NodArbore));
        nou->info = p;
        nou->inaltime = 1;
        nou->stanga = NULL;
        nou->dreapta = NULL;
        return nou;
    }
    if (p.id < radacina->info.id) {
        radacina->stanga = inserareABC(radacina->stanga, p);
    } else if (p.id > radacina->info.id) {
        radacina->dreapta = inserareABC(radacina->dreapta, p);
    }
    return radacina;
}

int maxim(int a, int b) {
    return a > b ? a : b;
}

int inaltime(NodArbore* radacina) {
    return radacina == NULL ? 0 : radacina->inaltime;
}

int balans(NodArbore* radacina) {
    return radacina == NULL ? 0 : inaltime(radacina->stanga) - inaltime(radacina->dreapta);
}

NodArbore* rotireDreapta(NodArbore* y) {
    NodArbore* x = y->stanga;
    NodArbore* t = x->dreapta;
    x->dreapta = y;
    y->stanga = t;
    y->inaltime = maxim(inaltime(y->stanga), inaltime(y->dreapta)) + 1;
    x->inaltime = maxim(inaltime(x->stanga), inaltime(x->dreapta)) + 1;
    return x;
}

NodArbore* rotireStanga(NodArbore* x) {
    NodArbore* y = x->dreapta;
    NodArbore* t = y->stanga;
    y->stanga = x;
    x->dreapta = t;
    x->inaltime = maxim(inaltime(x->stanga), inaltime(x->dreapta)) + 1;
    y->inaltime = maxim(inaltime(y->stanga), inaltime(y->dreapta)) + 1;
    return y;
}

NodArbore* inserareAVL(NodArbore* radacina, Proiect p) {
    if (radacina == NULL) {
        return inserareABC(NULL, p);
    }
    if (p.id < radacina->info.id) {
        radacina->stanga = inserareAVL(radacina->stanga, p);
    } else if (p.id > radacina->info.id) {
        radacina->dreapta = inserareAVL(radacina->dreapta, p);
    } else {
        return radacina;
    }
    radacina->inaltime = maxim(inaltime(radacina->stanga), inaltime(radacina->dreapta)) + 1;
    int b = balans(radacina);
    if (b > 1 && p.id < radacina->stanga->info.id) {
        return rotireDreapta(radacina);
    }
    if (b < -1 && p.id > radacina->dreapta->info.id) {
        return rotireStanga(radacina);
    }
    if (b > 1 && p.id > radacina->stanga->info.id) {
        radacina->stanga = rotireStanga(radacina->stanga);
        return rotireDreapta(radacina);
    }
    if (b < -1 && p.id < radacina->dreapta->info.id) {
        radacina->dreapta = rotireDreapta(radacina->dreapta);
        return rotireStanga(radacina);
    }
    return radacina;
}

void afisareArbore(NodArbore* radacina) {
    if (radacina != NULL) {
        afisareArbore(radacina->stanga);
        afisareProiect(radacina->info);
        afisareArbore(radacina->dreapta);
    }
}

void inserareNodGraf(NodGraf** graf, Proiect p) {
    NodGraf* nou = (NodGraf*)malloc(sizeof(NodGraf));
    nou->info = p;
    nou->next = *graf;
    nou->vecini = NULL;
    *graf = nou;
}

NodGraf* cautareGraf(NodGraf* graf, int id) {
    while (graf != NULL && graf->info.id != id) {
        graf = graf->next;
    }
    return graf;
}

void inserareMuchie(Muchie** lista, NodGraf* nod) {
    Muchie* nou = (Muchie*)malloc(sizeof(Muchie));
    nou->info = nod;
    nou->next = *lista;
    *lista = nou;
}

void adaugaMuchie(NodGraf* graf, int id1, int id2) {
    NodGraf* n1 = cautareGraf(graf, id1);
    NodGraf* n2 = cautareGraf(graf, id2);
    if (n1 != NULL && n2 != NULL) {
        inserareMuchie(&n1->vecini, n2);
        inserareMuchie(&n2->vecini, n1);
    }
}

void scrieFisier(Proiect* proiecte, int nr, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "w");
    for (int i = 0; i < nr; i++) {
        fprintf(f, "%d,%s,%.2f,%d\n", proiecte[i].id, proiecte[i].nume, proiecte[i].buget, proiecte[i].prioritate);
    }
    fclose(f);
}

int main() {
    Proiect proiecte[] = {
        initProiect(30, "Aplicatie", 12000.0f, 3),
        initProiect(10, "Migrare", 8000.0f, 1),
        initProiect(50, "Raportare", 15000.0f, 4),
        initProiect(20, "Audit", 5000.0f, 2),
        initProiect(40, "Integrare", 20000.0f, 5)
    };
    NodLista* stiva = NULL;
    NodLista* prim = NULL;
    NodLista* ultim = NULL;
    Heap heap = initHeap(10);
    HashTable tabela = initHashTable(7);
    NodArbore* abc = NULL;
    NodArbore* avl = NULL;
    NodGraf* graf = NULL;
    for (int i = 0; i < 5; i++) {
        push(&stiva, copieProiect(proiecte[i]));
        enqueue(&prim, &ultim, copieProiect(proiecte[i]));
        inserareHeap(&heap, copieProiect(proiecte[i]));
        inserareHash(tabela, copieProiect(proiecte[i]));
        abc = inserareABC(abc, copieProiect(proiecte[i]));
        avl = inserareAVL(avl, copieProiect(proiecte[i]));
        inserareNodGraf(&graf, copieProiect(proiecte[i]));
    }
    adaugaMuchie(graf, 30, 10);
    adaugaMuchie(graf, 30, 50);
    adaugaMuchie(graf, 20, 40);
    printf("Heap\n");
    for (int i = 0; i < heap.nrElemente; i++) {
        afisareProiect(heap.vector[i]);
    }
    printf("ABC\n");
    afisareArbore(abc);
    printf("AVL\n");
    afisareArbore(avl);
    Proiect p1 = pop(&stiva);
    Proiect p2 = dequeue(&prim, &ultim);
    printf("Stiva si coada\n");
    afisareProiect(p1);
    afisareProiect(p2);
    scrieFisier(proiecte, 5, "proiecte_final.txt");
    eliberareProiect(&p1);
    eliberareProiect(&p2);
    for (int i = 0; i < 5; i++) {
        eliberareProiect(&proiecte[i]);
    }
    return 0;
}