//
// O header servirá para os modelos para as listas e para interconetar o main.c com o equipas_func.c
//

#ifndef PROJETOFINALPPP_PARTE_PRINCIPAL_EQUIPASHEADER_H
#define PROJETOFINALPPP_PARTE_PRINCIPAL_EQUIPASHEADER_H

#endif //PROJETOFINALPPP_PARTE_PRINCIPAL_EQUIPASHEADER_H

#define SIZE 25

//Contem a informacao dos elementos da lista
struct teams {
    char nome_equipa[SIZE];
    int pontos, diferencaCestos , vitorias, derrotas;
};

//Elementos da lista ligada
struct elemFila{
    struct teams Equipa;
    int prioridade;
    struct elemFila *proxEquipa;
};

//Esta struct apenas servirá para termos o código mais organizado na função GetPoints;
struct jogos {
    char nomeEquipa1[SIZE] ,nomeEquipa2[SIZE];
    int pontosEquipa1, pontosEquipa2;
};

#include <stdio.h>

int insert (struct teams c, int prioridade, struct elemFila **raiz);
int GetTeams( FILE *fichEntrada);
void printList();
void GetPoints(FILE *fichEntrada);
char * ObterLinha(char nomeFich[]);
void OrdenarPontos();
void TrocarOrdem (struct elemFila *atual );
int JogoDesempate(struct elemFila *atual);
void EscreveFicheiro (FILE *fichSaida, int numeroEquipas);
