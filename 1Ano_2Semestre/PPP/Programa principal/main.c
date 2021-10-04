/*
  Código desenvolvido por Edgar Filipe Ferreira Duarte, número: 2019216077, email: edgartip2003@gmail.com

  NOTAS:
  Todas as funções usadas pelo main.c estarão no equipas_func.c e serão acedidas via o header equipasheader.h
  Todas as etapas que involvam código mais ambíguo ou complexo serão comentados.


  Todas as considerações que tomei:

    -Escolhi ter uma lista ligada com a struct equipa( char equipa ; int pontos, diferença_cestos, vitorias, derrotas); pois assim
será mais fácil compararmos as pontuações e facilitará-nos muito na ordenação da lista ligada;

    -Nos casos em que duas equipas empatem em total de pontos, ganha quem tiver maior diferença de cestos sobre os oponentes.
    -Se este valor for igual criei um jogo de desempate que vai defenir um vencedor aliatóriamente. Exemplo:

            -Académica: 100pontos, Porto:100 pontos, Benfica:45pontos, Sporting:45 pontos, Boavista:20pontos

            -Posições:
                -1º Académica 30pontos(+ 80 cestos)
                -1º Porto 30pontos(+80 cestos)
                -3º Benfica 18pontos(-32 cestos)
                -3º Sporting 18pontos( -24 cestos)
                -5º Boavista 12pontos(-104 cestos)

                -Problemas: Quem é relegado e quem é o vencedor do campeonato?
                -Jogos random:
                    -Académica - Porto  75-63 (Académica fica em primeiro)
                    É de notar que vou forçar para este jogo não empatar.

                -Decisão por pontos:
                    -Sporting fica em 3º e Benfica em 4º.


    -Outra solução teria sido comparar confronto direto, mas esta solução ficaria muito complexa, pois teria de guardar o resultado
de todos os jogos, guadando-os noutra lista e gastando não só muita memória como tempo de execução, tornando o programa menos efeciente.
    -Escolhi 10 equipas e cada equipa confrontará outra 2 vezes o que me dá um total de 90 jogos + 10 equipas = 100 entidades.

 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "equipasheader.h"

int main( int argc, char *argv[]) {
    int numeroEquipas=0;
    //Fazer com que o random (mais tarde utilizado para o jogo de desempate) não sejam sempre os mesmos valores
    srand(time(0));

    //Caso na linha de comando se escreva mais do que apenas o nome do ficheiro
    if(argc != 2){
        printf("Demasiados parâmetros!");
        return 1;
    }

    FILE *fichEntrada = fopen(argv[1], "rb");
    //Caso o ficheiro não exista
    if (fichEntrada == NULL){
        printf("Este ficheiro não existe!");
        return 1;
    }

    //Inserir as nossas equipas na lista ligada e obter o numero de equipas
    numeroEquipas = GetTeams(fichEntrada);
    //Controlar que a inserção foi bem feita
    if(numeroEquipas == 0){
      printf("Ocorreu um erro na inserção de equipas no programa!");
      return 1;
    }

    //Inserir os pontos na lista ligada
    GetPoints(fichEntrada);

    //Obter o ficheiro de saida e abri-lo como wb.
    char *stringretorno = ObterLinha("config.txt");
    //Ocorreu um erro
    if(stringretorno == NULL){
        return 1;
    }
    FILE *fichSaida = fopen( stringretorno,"wb");
    //Para boa prática e evitar erros dar free do espaço ocupado pela string
    free(stringretorno);

    //Ordenar as posições na lista ligada
    OrdenarPontos();

    //Escrever no ficheiro
    EscreveFicheiro (fichSaida, numeroEquipas);

    return 0;
}
