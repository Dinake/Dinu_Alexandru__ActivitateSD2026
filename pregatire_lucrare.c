#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================
//   HASH TABLE
// ============================================================

// fiecare nod din lista de coliziuni
// cand 2 chei au acelasi hash le punem in aceeasi lista
typedef struct NodHash {
    int cheie;
    int valoare;
    struct NodHash* urm; // urmatoru element din lista
} NodHash;

// tabela de dispersie
// are un vector de pointeri la liste inlantuite
typedef struct HashTable {
    NodHash** bucket; // vectoru de liste
    int marime;       // cate bucket-uri are tabela
} HashTable;

// functia de hash - returneaza indexu in tabela
// folosim modulo cu marimea tabelei
int hash(int cheie, int marime) {
    return cheie % marime; // daca cheia e negativa iese negativ, atentie!!
}

// creeaza o tabela noua cu marimea data
// alocam vectoru si initializam cu NULL
HashTable* creareHashTable(int marime) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    ht->bucket = (NodHash**)malloc(marime * sizeof(NodHash*));
    ht->marime = marime;
    for (int i = 0; i < marime; i++)
        ht->bucket[i] = NULL; // la inceput toate bucket-urile goale
    return ht;
}

// inserare in tabela
// calculam hashul si punem la inceputul listei din bucket-ul respectiv
void htInserare(HashTable* ht, int cheie, int valoare) {
    int idx = hash(cheie, ht->marime);
    // cautam daca cheia exista deja ca sa o actualizam
    NodHash* curent = ht->bucket[idx];
    while (curent != NULL) {
        if (curent->cheie == cheie) {
            curent->valoare = valoare; // actualizam valoarea
            return;
        }
        curent = curent->urm;
    }
    // cheia nu exista, cream nod nou si il punem la inceput
    NodHash* nou = (NodHash*)malloc(sizeof(NodHash));
    nou->cheie = cheie;
    nou->valoare = valoare;
    nou->urm = ht->bucket[idx]; // leaga la lista existenta
    ht->bucket[idx] = nou;
}

// cautare in tabela - returneaza valoarea sau -1 daca nu gaseste
int htCautare(HashTable* ht, int cheie) {
    int idx = hash(cheie, ht->marime);
    NodHash* curent = ht->bucket[idx];
    while (curent != NULL) {
        if (curent->cheie == cheie)
            return curent->valoare;
        curent = curent->urm;
    }
    return -1; // nu am gasit cheia
}

// stergere din tabela
void htStergere(HashTable* ht, int cheie) {
    int idx = hash(cheie, ht->marime);
    NodHash* curent = ht->bucket[idx];
    NodHash* anterior = NULL;
    while (curent != NULL) {
        if (curent->cheie == cheie) {
            if (anterior == NULL)
                ht->bucket[idx] = curent->urm; // era primul din lista
            else
                anterior->urm = curent->urm;
            free(curent);
            return;
        }
        anterior = curent;
        curent = curent->urm;
    }
    // daca nu gasim cheia nu facem nimic
}

// afisare tabela - fiecare bucket cu lista lui
void htAfisare(HashTable* ht) {
    printf("=== HashTable ===\n");
    for (int i = 0; i < ht->marime; i++) {
        printf("Bucket[%d]: ", i);
        NodHash* curent = ht->bucket[i];
        if (curent == NULL) {
            printf("(gol)");
        }
        while (curent != NULL) {
            printf("(%d->%d) ", curent->cheie, curent->valoare);
            curent = curent->urm;
        }
        printf("\n");
    }
}

// eliberare memorie tabela
void htEliberare(HashTable* ht) {
    for (int i = 0; i < ht->marime; i++) {
        NodHash* curent = ht->bucket[i];
        while (curent != NULL) {
            NodHash* temp = curent;
            curent = curent->urm;
            free(temp);
        }
    }
    free(ht->bucket);
    free(ht);
}


// ============================================================
//   HEAP (MIN-HEAP)
// ============================================================

// heap implementat ca vector
// parintele lui i e la (i-1)/2
// copii lui i sunt la 2*i+1 si 2*i+2
typedef struct Heap {
    int* date;    // vectoru care contine elementele
    int marime;   // cate elemente avem
    int capac;    // cat de mare e vectoru alocat
} Heap;

// creeaza heap gol cu capacitate data
Heap* creareHeap(int capac) {
    Heap* h = (Heap*)malloc(sizeof(Heap));
    h->date = (int*)malloc(capac * sizeof(int));
    h->marime = 0;
    h->capac = capac;
    return h;
}

// schimba doua valori din vector
void schimba(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// urcam elementul de la poz i pana la locul lui (pt inserare)
void heapifyUp(Heap* h, int i) {
    while (i > 0) {
        int parinte = (i - 1) / 2;
        if (h->date[parinte] > h->date[i]) {
            schimba(&h->date[parinte], &h->date[i]);
            i = parinte;
        } else {
            break; // e la locul lui
        }
    }
}

// coboram elementul de la poz i pana la locul lui (pt stergere)
void heapifyDown(Heap* h, int i) {
    while (1) {
        int stg = 2 * i + 1;
        int dr  = 2 * i + 2;
        int minim = i;

        if (stg < h->marime && h->date[stg] < h->date[minim])
            minim = stg;
        if (dr < h->marime && h->date[dr] < h->date[minim])
            minim = dr;

        if (minim != i) {
            schimba(&h->date[i], &h->date[minim]);
            i = minim;
        } else {
            break; // e la locul lui
        }
    }
}

// inserare element in heap
void heapInserare(Heap* h, int val) {
    if (h->marime == h->capac) {
        printf("Heap plin, nu mai putem insera!\n");
        return;
    }
    h->date[h->marime] = val; // punem la final
    h->marime++;
    heapifyUp(h, h->marime - 1); // urcam la locul potrivit
}

// scoate minimul (radacina) din heap
int heapExtractMin(Heap* h) {
    if (h->marime == 0) {
        printf("Heap gol!\n");
        return -1;
    }
    int minim = h->date[0];
    // punem ultimul element pe pozitia 0 si coboram
    h->date[0] = h->date[h->marime - 1];
    h->marime--;
    if (h->marime > 0)
        heapifyDown(h, 0);
    return minim;
}

// vedem minimul fara sa il scoatem
int heapPeek(Heap* h) {
    if (h->marime == 0) {
        printf("Heap gol!\n");
        return -1;
    }
    return h->date[0]; // radacina e intotdeauna minimul
}

// afisare heap ca vector
void heapAfisare(Heap* h) {
    printf("=== Heap (vector): ");
    for (int i = 0; i < h->marime; i++)
        printf("%d ", h->date[i]);
    printf("\n");
    printf("Minim (radacina): %d\n", heapPeek(h));
}

// sorteaza un vector folosind heap (heap sort)
// bag toate elem in heap si le scot in ordine
void heapSort(int* v, int n) {
    Heap* h = creareHeap(n);
    for (int i = 0; i < n; i++)
        heapInserare(h, v[i]);
    for (int i = 0; i < n; i++)
        v[i] = heapExtractMin(h);
    free(h->date);
    free(h);
}

// eliberare memorie heap
void heapEliberare(Heap* h) {
    free(h->date);
    free(h);
}


// ============================================================
//   MAIN - testare ambele structuri
// ============================================================

int main() {
    printf("==============================\n");
    printf("  TEST HASH TABLE\n");
    printf("==============================\n");

    HashTable* ht = creareHashTable(7); // tabela cu 7 bucket-uri

    htInserare(ht, 10, 100);
    htInserare(ht, 17, 200); // 17 % 7 = 3, coliziune cu 10 nu... 10%7=3, deci coliziune!
    htInserare(ht, 3,  300); // 3 % 7 = 3, inca o coliziune
    htInserare(ht, 5,  500);
    htInserare(ht, 25, 250); // 25 % 7 = 4

    htAfisare(ht);

    printf("Cautare cheie 17: %d\n", htCautare(ht, 17));
    printf("Cautare cheie 99: %d\n", htCautare(ht, 99)); // nu exista

    htStergere(ht, 17);
    printf("Dupa stergere cheie 17:\n");
    htAfisare(ht);

    htEliberare(ht);

    printf("\n==============================\n");
    printf("  TEST HEAP\n");
    printf("==============================\n");

    Heap* h = creareHeap(20);
    heapInserare(h, 15);
    heapInserare(h, 3);
    heapInserare(h, 8);
    heapInserare(h, 1);
    heapInserare(h, 22);
    heapInserare(h, 7);

    heapAfisare(h);

    printf("Extragem minimele in ordine: ");
    while (h->marime > 0)
        printf("%d ", heapExtractMin(h));
    printf("\n");

    heapEliberare(h);

    printf("\n==============================\n");
    printf("  TEST HEAP SORT\n");
    printf("==============================\n");

    int v[] = {64, 25, 12, 22, 11};
    int n = sizeof(v) / sizeof(v[0]);
    printf("Inainte: ");
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    heapSort(v, n);
    printf("\nDupa heapSort: ");
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");

    return 0;
}
