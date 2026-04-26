#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Curs Curs;
typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;
typedef struct NodInt NodInt;
typedef struct Coada Coada;

struct Curs {
    int id;
    char* denumire;
    int credite;
};

struct NodPrincipal {
    Curs info;
    NodPrincipal* next;
    NodSecundar* vecini;
};

struct NodSecundar {
    NodPrincipal* info;
    NodSecundar* next;
};

struct NodInt {
    int id;
    NodInt* next;
};

struct Coada {
    NodInt* prim;
    NodInt* ultim;
};

char* copiazaText(const char* text) {
    char* copie = (char*)malloc(strlen(text) + 1);
    strcpy(copie, text);
    return copie;
}

Curs initCurs(int id, const char* denumire, int credite) {
    Curs c;
    c.id = id;
    c.denumire = copiazaText(denumire);
    c.credite = credite;
    return c;
}

void inserareCurs(NodPrincipal** graf, Curs c) {
    NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
    nou->info = c;
    nou->next = NULL;
    nou->vecini = NULL;
    if (*graf == NULL) {
        *graf = nou;
    } else {
        NodPrincipal* aux = *graf;
        while (aux->next != NULL) {
            aux = aux->next;
        }
        aux->next = nou;
    }
}

NodPrincipal* cautareCurs(NodPrincipal* graf, int id) {
    while (graf != NULL && graf->info.id != id) {
        graf = graf->next;
    }
    return graf;
}

void inserareVecin(NodSecundar** lista, NodPrincipal* nod) {
    NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
    nou->info = nod;
    nou->next = *lista;
    *lista = nou;
}

void adaugaMuchie(NodPrincipal* graf, int id1, int id2) {
    NodPrincipal* nod1 = cautareCurs(graf, id1);
    NodPrincipal* nod2 = cautareCurs(graf, id2);
    if (nod1 != NULL && nod2 != NULL) {
        inserareVecin(&nod1->vecini, nod2);
        inserareVecin(&nod2->vecini, nod1);
    }
}

void afisareCurs(Curs c) {
    printf("%d %s %d\n", c.id, c.denumire, c.credite);
}

void afisareGraf(NodPrincipal* graf) {
    while (graf != NULL) {
        afisareCurs(graf->info);
        NodSecundar* vecin = graf->vecini;
        while (vecin != NULL) {
            printf("  %d %s\n", vecin->info->info.id, vecin->info->info.denumire);
            vecin = vecin->next;
        }
        graf = graf->next;
    }
}

void push(NodInt** stiva, int id) {
    NodInt* nou = (NodInt*)malloc(sizeof(NodInt));
    nou->id = id;
    nou->next = *stiva;
    *stiva = nou;
}

int pop(NodInt** stiva) {
    NodInt* temp = *stiva;
    int id = temp->id;
    *stiva = temp->next;
    free(temp);
    return id;
}

void enqueue(Coada* coada, int id) {
    NodInt* nou = (NodInt*)malloc(sizeof(NodInt));
    nou->id = id;
    nou->next = NULL;
    if (coada->ultim != NULL) {
        coada->ultim->next = nou;
    } else {
        coada->prim = nou;
    }
    coada->ultim = nou;
}

int dequeue(Coada* coada) {
    NodInt* temp = coada->prim;
    int id = temp->id;
    coada->prim = temp->next;
    if (coada->prim == NULL) {
        coada->ultim = NULL;
    }
    free(temp);
    return id;
}

void dfs(NodPrincipal* graf, int start, int nrNoduri) {
    int* vizitat = (int*)calloc(nrNoduri + 1, sizeof(int));
    NodInt* stiva = NULL;
    push(&stiva, start);
    vizitat[start] = 1;
    while (stiva != NULL) {
        int id = pop(&stiva);
        NodPrincipal* nod = cautareCurs(graf, id);
        afisareCurs(nod->info);
        NodSecundar* vecin = nod->vecini;
        while (vecin != NULL) {
            int idVecin = vecin->info->info.id;
            if (vizitat[idVecin] == 0) {
                push(&stiva, idVecin);
                vizitat[idVecin] = 1;
            }
            vecin = vecin->next;
        }
    }
    free(vizitat);
}

void bfs(NodPrincipal* graf, int start, int nrNoduri) {
    int* vizitat = (int*)calloc(nrNoduri + 1, sizeof(int));
    Coada coada;
    coada.prim = NULL;
    coada.ultim = NULL;
    enqueue(&coada, start);
    vizitat[start] = 1;
    while (coada.prim != NULL) {
        int id = dequeue(&coada);
        NodPrincipal* nod = cautareCurs(graf, id);
        afisareCurs(nod->info);
        NodSecundar* vecin = nod->vecini;
        while (vecin != NULL) {
            int idVecin = vecin->info->info.id;
            if (vizitat[idVecin] == 0) {
                enqueue(&coada, idVecin);
                vizitat[idVecin] = 1;
            }
            vecin = vecin->next;
        }
    }
    free(vizitat);
}

void scrieGraf(NodPrincipal* graf, const char* numeFisier) {
    FILE* f = fopen(numeFisier, "w");
    while (graf != NULL) {
        fprintf(f, "%d,%s,%d\n", graf->info.id, graf->info.denumire, graf->info.credite);
        graf = graf->next;
    }
    fclose(f);
}

void dezalocareGraf(NodPrincipal** graf) {
    while (*graf != NULL) {
        NodPrincipal* temp = *graf;
        *graf = (*graf)->next;
        NodSecundar* vecin = temp->vecini;
        while (vecin != NULL) {
            NodSecundar* aux = vecin;
            vecin = vecin->next;
            free(aux);
        }
        free(temp->info.denumire);
        free(temp);
    }
}

int main() {
    NodPrincipal* graf = NULL;
    inserareCurs(&graf, initCurs(1, "SDD", 5));
    inserareCurs(&graf, initCurs(2, "PAW", 5));
    inserareCurs(&graf, initCurs(3, "PEAG", 4));
    inserareCurs(&graf, initCurs(4, "JAVA", 5));
    inserareCurs(&graf, initCurs(5, "SGBD", 4));
    adaugaMuchie(graf, 1, 2);
    adaugaMuchie(graf, 1, 3);
    adaugaMuchie(graf, 2, 4);
    adaugaMuchie(graf, 3, 5);
    printf("Graf\n");
    afisareGraf(graf);
    printf("DFS\n");
    dfs(graf, 1, 5);
    printf("BFS\n");
    bfs(graf, 1, 5);
    scrieGraf(graf, "cursuri_graf.txt");
    dezalocareGraf(&graf);
    return 0;
}