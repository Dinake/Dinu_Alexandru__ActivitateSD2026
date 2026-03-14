#include<stdio.h>
#include<malloc.h>

typedef struct Depozit Depozit;

struct Depozit {
	int id;
	float suprafata;
	int volum;
	char* denumire;
	char clasa;
};
struct Depozit initializare(int id, float suprafata, int volum, const char* denumire, char clasa) {
	struct Depozit s;
	s.id = id;
	s.suprafata = suprafata;
	s.volum = volum;
	s.denumire = (char*)malloc(sizeof(char)*(strlen(denumire)+1));
	strcpy(s.denumire, denumire);
	s.clasa = clasa;
	return s;
}

Depozit copiaza(Depozit d) {
	return initializare(d.id,d.suprafata,d.volum,d.denumire,d.clasa);
}

void afisare(struct Depozit s) {
	printf("%d. Depozitul %s de clasa %c are o suprafata de %5.2f si un volum de %d.\n",
		s.id, s.denumire, s.clasa, s.suprafata, s.volum);
}

void afisareVector(struct Depozit* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(*(vector+i));		
	}
}

struct Depozit* copiazaPrimeleNElemente(Depozit* vector, int nrElemente, int nrElementeCopiate) {
	if (nrElementeCopiate < nrElemente) {
		struct Depozit* vectorNou = malloc(sizeof(Depozit) * nrElementeCopiate);
		for (int i = 0; i < nrElementeCopiate; i++) {
			vectorNou[i] = copiaza(vector[i]);
		}
		return vectorNou;
	}
	else {
		return NULL;
	}
}

void dezalocare(struct Depozit** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].denumire);
	}
	free(*vector);
	*nrElemente = 0;
	*vector = NULL;
}

void copiazaAnumiteElemente(struct Depozit* vector, char nrElemente, float prag, struct Depozit** vectorNou, int* dimensiune) {
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].suprafata < prag) {
			(*dimensiune)++;
		}
	}
	*vectorNou = malloc(sizeof(Depozit) * (*dimensiune));
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].suprafata < prag) {
			(*vectorNou)[*dimensiune] = copiaza(vector[i]);
			(*dimensiune)++;
		}
	}
}

struct Depozit getPrimulElementConditionat(struct Depozit* vector, int nrElemente, const char* numeCautat) {
	Depozit s;
	s.id = -1;
	s.denumire = NULL;
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(numeCautat, vector[i].denumire)==0) {
			s = copiaza(vector[i]);
		}
	}
	return s;
}
	


int main() {
	struct Depozit depozit = initializare(1, 34.3, 70, "Emag", 'A');
	afisare(depozit);

	int nrDepozite = 4;
	Depozit* vector = (Depozit*)malloc(sizeof(Depozit)*nrDepozite);
	vector[0] = initializare(2, 34, 45, "DPD", 'B');
	vector[1] = initializare(3, 56, 89, "Amazon", 'A');
	vector[2] = initializare(4, 45, 96, "DSC", 'A');
	vector[3] = initializare(5, 34, 67, "Emag", 'A');
	
	afisareVector(vector, nrDepozite);
	printf("\n\n");
	int nrCopiate=2;
	Depozit* scurt = copiazaPrimeleNElemente(vector, nrDepozite, nrCopiate);

	afisareVector(scurt, nrCopiate);
	dezalocare(&scurt, &nrCopiate);
	Depozit* filtrat = NULL;
	int dimensiune = 0;
	copiazaAnumiteElemente(vector, nrDepozite, 50, &filtrat, &dimensiune);
	printf("\nVector filtrat:\n");
	afisareVector(filtrat, dimensiune);
	dezalocare(&filtrat, &dimensiune);

	Depozit depozitCautat = getPrimulElementConditionat(vector, nrDepozite, "Amazon");
	printf("\nDepozit cautat:\n");
	afisare(depozitCautat);
	if (depozitCautat.id != -1) {
		free(depozitCautat.denumire);
	}
	free(depozit.denumire);
	dezalocare(&vector, &nrDepozite);
	return 0;
}