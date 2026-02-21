//#include<stdio.h>
//#include<malloc.h>
////declaram un alias pentru struct Depozit ca sa scriem simplu depozit nu struct depozit;
//typedef struct Depozit Depozit;
//
//struct Depozit {
//	int id;
//	int volum;
//	float suprafata;
//	char* denumire;
//	char clasa; //tipul depozitului clasa a; clasa b; etc.
//
////de heap se ocupa programatorul de stack se ocupa sistemul
//// char ocupa in stack 4 octeti pt infrastructura pe 32 biti si 8 octeti pentru 64 biti
//// in heap char ocupa cat dorim noi sa initializam
//
//};
//
//struct Depozit initializare(int id, int volum, float suprafata, char* denumire, char clasa) {
//	struct Depozit s;
//	//initializare structura 
//	s.id = id;
//	s.volum = volum;
//	s.suprafata = suprafata;
//	s.denumire = (char*)malloc(sizeof(char)*(strlen(denumire)+1));
//	strcpy(s.denumire, denumire); 
//	s.clasa = clasa;
//	
//	return s;
//}
//
//struct Depozit copiazainit(Depozit depozit) {
//	return initializare(depozit.id, depozit.volum, depozit.suprafata, depozit.denumire, depozit.clasa);
//
//}
//
//
//
//
//void afisare(struct Depozit s) {
//	//afisarea tuturor atributelor.
//	printf("%d. Depozitul %s de clasa %c are o suprafata de %5.2f si un volum de %d. \n"
//			,s.id,s.denumire,s.clasa,s.suprafata,s.volum);
//}
//
//void afisareVector(struct Depozit* vector, int nrElemente) {
//	//afisarea elementelor din vector apeland functia afisare
//	//deplasare + dereferentiere inseamna P[2]
//	for (int i = 0; i < nrElemente; i++) {
//		afisare(*(vector+i));
//	}
//}
//
//struct Depozit* copiazaPrimeleNElemente(struct Depozit* vector, int nrElemente, int nrElementeCopiate) {
//	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
//	if (nrElementeCopiate < nrElemente) {
//		struct Depozit* vectorNou = malloc(sizeof(Depozit) * nrElementeCopiate);
//		for (int i = 0; i < nrElementeCopiate; i++) {
//			vectorNou[i] = copiazainit(vector[i]);
//		}
//
//		return vectorNou;
//	}
//	else {
//		return NULL;
//
//	}
//
//}
//
//void dezalocare(struct Depozit** vector, int* nrElemente) {
//	//dezalocam elementele din vector si vectorul
//	for (int i = 0; i < *nrElemente; i++) {
//		free((*vector)[i].denumire);
//
//	}
//	free(*vector);
//	*nrElemente = 0;
//	*vector = NULL;
//
//}
//
//void copiazaAnumiteElemente(struct Depozit* vector, char nrElemente, float prag, struct Depozit** vectorNou, int* dimensiune) {
//	//parametrul prag poate fi modificat in functie de 
//	// tipul atributului ales pentru a indeplini o conditie
//	//este creat un nou vector cu elementele care indeplinesc acea conditie
//	*dimensiune = 0;
//	for (int i = 0; i < nrElemente; i++) {
//		if (vector[i].suprafata < prag) {
//			(*dimensiune)++;
//		}
//	}
//
//	//
//
//	*vectorNou = malloc(sizeof(Depozit) * (*dimensiune));
//	*dimensiune = 0;
//	for (int i = 0; i < nrElemente; i++) {
//		if (vector[i].suprafata < prag) {
//			(*vectorNou)[*dimensiune] = copiazainit(vector[i]);
//			(*dimensiune)++;
//		}
//	}
//}
//
//struct Depozit getPrimulElementConditionat(struct Depozit* vector, int nrElemente, const char* numeCautat) {
//	//trebuie cautat elementul care indeplineste o conditie
//	//dupa atributul de tip char*. Acesta este returnat.
//	struct Depozit s;
//	s.id = -1;
//	for (int i = 0; i < nrElemente; i++) {
//		if (strcmp(numeCautat, vector[i].denumire==0)) {
//			s = copiazainit(vector[i]);
//
//		}
//	}
//
//	return s;
//}
//	
//
//
//int main() {
//
//	struct Depozit depozit = initializare(1, 70, 34.3, "Emag", 'A');
//	afisare(depozit);
//
//	int nrDepozite = 4;
//	Depozit* vector = (Depozit*)malloc(sizeof(Depozit) * nrDepozite);
//	vector[0] = initializare(2, 34, 45, "DPD", 'B');
//	vector[1] = initializare(3, 56, 89, "Amazon", 'A');
//	vector[2] = initializare(4, 45, 96, "DSC", 'D');
//
//	//vector[3] = depozit; --> shallow copy
//
//	vector[3] = initializare(5, 120, 300, "Emag", 'A');
//
//	afisareVector(vector, nrDepozite);
//
//	printf("\n\n");
//	int nrCopiate = 2;
//	Depozit* scurt = copiazaPrimeleNElemente(vector, nrDepozite, nrCopiate);
//
//	afisareVector(scurt, nrCopiate);
//	dezalocare(&scurt, &nrCopiate);
//
//	Depozit* filtrat = NULL;
//	int dimensiune = 0;
//	copiazaAnumiteElemente(vector, nrDepozite, 50, &filtrat, &dimensiune);
//	printf("\n\n");
//
//	printf("\n");
//	printf("Vector filtrat:");
//	printf("\n");
//	afisareVector(filtrat, dimensiune);
//	dezalocare(&filtrat, &dimensiune);
//
//
//	Depozit depozitCautat = getPrimulElementConditionat(vector, nrDepozite, "Amazon");
//	printf("\nDepozit cautat:\n");
//	afisare(depozitCautat);
//
//
//	return 0;
//}