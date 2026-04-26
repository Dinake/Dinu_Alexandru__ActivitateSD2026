#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student Student;
typedef struct NodABC NodABC;
typedef struct NodAVL NodAVL;

struct Student {
    int id;
    char* nume;
    float medie;
};

struct NodABC {
    Student info;
    NodABC* stanga;
    NodABC* dreapta;
};

struct NodAVL {
    Student info;
    int inaltime;
    NodAVL* stanga;
    NodAVL* dreapta;
};

char* copiazaText(const char* text) {
    char* copie = (char*)malloc(strlen(text) + 1);
    strcpy(copie, text);
    return copie;
}

Student initStudent(int id, const char* nume, float medie) {
    Student s;
    s.id = id;
    s.nume = copiazaText(nume);
    s.medie = medie;
    return s;
}

Student copieStudent(Student s) {
    return initStudent(s.id, s.nume, s.medie);
}

void afisareStudent(Student s) {
    printf("%d %s %.2f\n", s.id, s.nume, s.medie);
}

NodABC* inserareABC(NodABC* radacina, Student s) {
    if (radacina == NULL) {
        NodABC* nou = (NodABC*)malloc(sizeof(NodABC));
        nou->info = s;
        nou->stanga = NULL;
        nou->dreapta = NULL;
        return nou;
    }
    if (s.id < radacina->info.id) {
        radacina->stanga = inserareABC(radacina->stanga, s);
    } else if (s.id > radacina->info.id) {
        radacina->dreapta = inserareABC(radacina->dreapta, s);
    }
    return radacina;
}

Student* cautareABC(NodABC* radacina, int id) {
    if (radacina == NULL) {
        return NULL;
    }
    if (radacina->info.id == id) {
        return &radacina->info;
    }
    if (id < radacina->info.id) {
        return cautareABC(radacina->stanga, id);
    }
    return cautareABC(radacina->dreapta, id);
}

void afisareABCInordine(NodABC* radacina) {
    if (radacina != NULL) {
        afisareABCInordine(radacina->stanga);
        afisareStudent(radacina->info);
        afisareABCInordine(radacina->dreapta);
    }
}

int maxim(int a, int b) {
    return a > b ? a : b;
}

int inaltimeAVL(NodAVL* radacina) {
    return radacina == NULL ? 0 : radacina->inaltime;
}

int balans(NodAVL* radacina) {
    return radacina == NULL ? 0 : inaltimeAVL(radacina->stanga) - inaltimeAVL(radacina->dreapta);
}

NodAVL* rotireDreapta(NodAVL* y) {
    NodAVL* x = y->stanga;
    NodAVL* t = x->dreapta;
    x->dreapta = y;
    y->stanga = t;
    y->inaltime = maxim(inaltimeAVL(y->stanga), inaltimeAVL(y->dreapta)) + 1;
    x->inaltime = maxim(inaltimeAVL(x->stanga), inaltimeAVL(x->dreapta)) + 1;
    return x;
}

NodAVL* rotireStanga(NodAVL* x) {
    NodAVL* y = x->dreapta;
    NodAVL* t = y->stanga;
    y->stanga = x;
    x->dreapta = t;
    x->inaltime = maxim(inaltimeAVL(x->stanga), inaltimeAVL(x->dreapta)) + 1;
    y->inaltime = maxim(inaltimeAVL(y->stanga), inaltimeAVL(y->dreapta)) + 1;
    return y;
}

NodAVL* inserareAVL(NodAVL* radacina, Student s) {
    if (radacina == NULL) {
        NodAVL* nou = (NodAVL*)malloc(sizeof(NodAVL));
        nou->info = s;
        nou->inaltime = 1;
        nou->stanga = NULL;
        nou->dreapta = NULL;
        return nou;
    }
    if (s.id < radacina->info.id) {
        radacina->stanga = inserareAVL(radacina->stanga, s);
    } else if (s.id > radacina->info.id) {
        radacina->dreapta = inserareAVL(radacina->dreapta, s);
    } else {
        return radacina;
    }
    radacina->inaltime = maxim(inaltimeAVL(radacina->stanga), inaltimeAVL(radacina->dreapta)) + 1;
    int b = balans(radacina);
    if (b > 1 && s.id < radacina->stanga->info.id) {
        return rotireDreapta(radacina);
    }
    if (b < -1 && s.id > radacina->dreapta->info.id) {
        return rotireStanga(radacina);
    }
    if (b > 1 && s.id > radacina->stanga->info.id) {
        radacina->stanga = rotireStanga(radacina->stanga);
        return rotireDreapta(radacina);
    }
    if (b < -1 && s.id < radacina->dreapta->info.id) {
        radacina->dreapta = rotireDreapta(radacina->dreapta);
        return rotireStanga(radacina);
    }
    return radacina;
}

void afisareAVLPreordine(NodAVL* radacina) {
    if (radacina != NULL) {
        afisareStudent(radacina->info);
        afisareAVLPreordine(radacina->stanga);
        afisareAVLPreordine(radacina->dreapta);
    }
}

void scrieABC(NodABC* radacina, FILE* f) {
    if (radacina != NULL) {
        scrieABC(radacina->stanga, f);
        fprintf(f, "%d,%s,%.2f\n", radacina->info.id, radacina->info.nume, radacina->info.medie);
        scrieABC(radacina->dreapta, f);
    }
}

void eliberareABC(NodABC** radacina) {
    if (*radacina != NULL) {
        eliberareABC(&(*radacina)->stanga);
        eliberareABC(&(*radacina)->dreapta);
        free((*radacina)->info.nume);
        free(*radacina);
        *radacina = NULL;
    }
}

void eliberareAVL(NodAVL** radacina) {
    if (*radacina != NULL) {
        eliberareAVL(&(*radacina)->stanga);
        eliberareAVL(&(*radacina)->dreapta);
        free((*radacina)->info.nume);
        free(*radacina);
        *radacina = NULL;
    }
}

int main() {
    NodABC* abc = NULL;
    NodAVL* avl = NULL;
    Student studenti[] = {
        initStudent(30, "Ana", 9.2f),
        initStudent(10, "Mihai", 8.1f),
        initStudent(40, "Daria", 9.7f),
        initStudent(5, "Radu", 7.6f),
        initStudent(20, "Irina", 8.9f)
    };
    for (int i = 0; i < 5; i++) {
        abc = inserareABC(abc, studenti[i]);
        avl = inserareAVL(avl, copieStudent(studenti[i]));
    }
    printf("ABC\n");
    afisareABCInordine(abc);
    Student* gasit = cautareABC(abc, 20);
    if (gasit != NULL) {
        printf("Gasit\n");
        afisareStudent(*gasit);
    }
    printf("AVL\n");
    afisareAVLPreordine(avl);
    FILE* f = fopen("studenti_abc.txt", "w");
    scrieABC(abc, f);
    fclose(f);
    eliberareABC(&abc);
    eliberareAVL(&avl);
    return 0;
}