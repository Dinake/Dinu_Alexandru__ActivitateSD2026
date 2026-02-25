#include<stdio.h>
#include<stdlib.h>
#include<string.h> 

struct Telefon {
    int id;
    int RAM;
    char* producator;
    float pret;
    char serie;
};


struct Telefon initializare(int id, int ram,const char* producator, float pret, char serie) {
    struct Telefon t;
    //initializare structura
    t.id=id;
    t.RAM=ram;
    t.producator=(char*)malloc(sizeof(char)*(strlen(producator)+1));

    strcpy_s(t.producator,strlen(producator)+1,producator); 
    //primul parametru este destinatia - unde vreau sa copiez, apoi cate caractere sa copiez si ultimul este dursa - de unde vreau sa copiez
    t.pret=pret;
    t.serie=serie;

    return t;
}

void afisare(struct Telefon t) {
    printf("%d. Telefonul %s seria %c are %d Gb RAM si costa %5.2f RON.\n",
         t.id, t.producator, t.serie, t.RAM, t.pret);

}


void modificaPret (struct Telefon t, float noulPret) {
    if(noulPret>0){
        t.pret=noulPret;
    }

}








int main() {
    struct Telefon t;
    t=initializare(1,256,"Samsung",2000.5,'A');

    afisare(t);

    modificaPret(t,1000);
    afisare(t);

    return 0;

}