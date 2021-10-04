#include <stdio.h>
#include <string.h>
#define SIZE 25

/* Este programa tem como objetivo transformar um  ficheiro de texto para binário.

Nota: Total de 90 jogos (cada equipa defronta outra 2 vezes) + 10 equipas = 100 entidades
Também assumi que o nome do ficheiro de texto era recebido via parametro na função main.

 */
//Para ser mais organizado o codigo. Servirá para guardar os dados dos jogos
struct jogos{
    char equipa1[SIZE],equipa2[SIZE];
    int pontosEquipa1,pontosEquipa2;
};


int main(int argc, char *argv[]) {
    struct jogos jogos;

    FILE *fptr;
    int pontos1,pontos2;
    char equipa[SIZE], equipa1[SIZE],equipa2[SIZE];

    //Caso haja mais do que 1 parametro de entrada ou nenhum
    if(argc!=2){
        printf("Número errado de parâmetros");
        return 1;
    }

    if ((fptr = fopen(argv[1], "rb")) == NULL) {
        printf("Erro a abrir o ficheiro");
        return 0;
    }
    FILE *fptr2 = fopen("Equipas.bin", "wb");
    fscanf(fptr, "%s",equipa);
    //Obter as 10 equipas e meter no ficheiro binário
    while(strcmp(equipa,"Jogos")!=0){
        fwrite(equipa,SIZE,sizeof(char),fptr2);
        fscanf(fptr, "%s",equipa);

    }
    //Isto servirá para no código principal ser mais fácil separar as equipas dos jogos
    fwrite("Jogos",SIZE,sizeof(char),fptr2);

    //Enquanto não estivermos no fim do ficheiro de texto acrescentaremos uma struct no nosso ficheiro binário.
    while (!feof(fptr)){
        //Obter os valores para colocar na struct
        fscanf( fptr, "%s - %s %d-%d",equipa1,equipa2,&pontos1,&pontos2);

        //Preencher a struct
        strcpy(jogos.equipa1,equipa1);
        strcpy(jogos.equipa2,equipa2);
        jogos.pontosEquipa1 = pontos1;
        jogos.pontosEquipa2 = pontos2;
        fwrite(&jogos, 1, sizeof(struct jogos),fptr2);
    }

    fclose(fptr);
    fclose(fptr2);

    return 0;
}