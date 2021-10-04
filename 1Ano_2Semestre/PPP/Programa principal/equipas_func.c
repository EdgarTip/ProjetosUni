// Código desenvolvido por: EDGAR FILIPE FERREIRA DUARTE , NUMERO: 2019216077
// EMAIL: edgartip2003@gmail.com


// Este source file vai ter todas as funções utilizadas pelo main.c de forma a organizar ao máximo o nosso código.
// Tal como no código main.c comentarei todos os passos que poderão ter interpretações ambíguas ou linhas de código complexas.
// Antes de cada função terei um comentário a explicar como a função funciona e o seu propósito.

//NOTA: As minhas decisões aquando da interpretação do enunciado está no main.c e não serão explicadas de novo neste script!

#include "equipasheader.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//A lista ligada a usar nas funções
static struct elemFila *raizPrincipal = NULL;


/*Esta função serve para colocar elementos na nossa lista ligada. Está feita por ordem crescente de prioridade ,
embora apenas usemos prioridade = 1 neste codigo (um elemento inserido posteriormente será colocado no fim da lista). Assim
todos os elementos serao colocados numa posição seguinte ao elemento anterior

Nota: Só meti com prioridade para deixar o código mais global. Devido a sempre se introduzir a mesma prioridade deixa
partes deste código inutilizadas (indicados com * em comentario). Devido a esta procura de uma solução global perde-se alguma
eficiência.*/
int insert (struct teams c, int prioridade, struct elemFila **raiz) {

    struct elemFila *aux, *prox, *anterior;

    //Obter um endereço para colocar o nosso aux
    aux = (struct elemFila *) malloc(sizeof(struct elemFila));

    //Se ocorrer erro (não há espaço, o sistema operativo não permite mais alocacao ,etc)
    if (aux == NULL) {
        return 1;
    }
    aux->Equipa = c;
    aux->prioridade = prioridade;
    aux->proxEquipa = NULL;

    //Meter no ínicio da fila
    if (*raiz == NULL || ((*raiz)->prioridade > prioridade)){
        aux->proxEquipa = *raiz;
        *raiz = aux;
    }
        //Se nao for no início da fila
    else{

        anterior = *raiz;
        prox = (*raiz)->proxEquipa;

        //Encontrar a posição pretendida (Este while dará sempre o valor de prox = NULL)
        while (prox != NULL && prox->prioridade <= prioridade) {
            anterior = prox;
            prox = prox->proxEquipa;
        }

        anterior->proxEquipa = aux;
        //Essa posição não é a última *
        if (prox != NULL) {
            aux->proxEquipa = prox;
        }
        //É o último elemento
        else aux->proxEquipa =NULL;
    }
    return 0;
}

/*Esta função serve para incializar a nossa lista. Do ficheiro de entrada retiramos os nomes das equipas a participarem no
torneio. Como a função "insert" (onde vamos inserir estes dados), pede uma struct do tipo teams como parametro de entrada,
cria-se uma struct desse tipo com todos os valores exceto "nomeEquipa" a 0. O valor de retorno será o número de equipas que
participam no torneio, valor que nos será útil para outras funções.

 Nota: Eu para separar as equipas dos jogos no programa que transforma o ficheiro de texto para binário deixei a palavra
"Jogos" como separador entre estas duas */

int GetTeams( FILE *fichEntrada) {
    char nomeEquipa[SIZE];
    struct teams Equipa;
    int numeroEquipas = 0;

    //meter a 0 todos os elementos da struct exceto o nome
    Equipa.diferencaCestos = Equipa.pontos = Equipa.vitorias = Equipa.derrotas = 0;

    //inicializar nomeEquipa
    fread( nomeEquipa, SIZE, sizeof(char), fichEntrada );
    //Ler o nome das 10 equipas
    while(strcmp(nomeEquipa,"Jogos")!=0){
        strcpy( Equipa.nome_equipa , nomeEquipa );                    //Passar o nome da equipa para a struct
        int controlador = insert(Equipa, 1, &raizPrincipal); //Inserir a struct na lista
        if(controlador == 1) return 0;                                //Controlar que a inserção foi bem feita
        numeroEquipas++;                                              //Incrementar o numero de equipas
        fread( nomeEquipa, SIZE, sizeof(char), fichEntrada);          //retirar os elementos do nosso ficheiro binario
    }
    return numeroEquipas;
}

/*Esta função serve para dar-mos os respetivos valores às equipas. Uma vitória = 2 pontos , uma derrota = 1 ponto. Como,por
hipótese não existem empates, não contabilizamos essa possibilidade. Vamos obter as informações dos jogos do nosso ficheiro binário.

  Primeiramente, vemos a diferença de cestos entre a equipa1 e equipa2. Se este número for negativo sabemos que a equipa2 ganhou, senão
sabemos que a equipa1 ganhou. A partir destes dados atribui-mos os pontos às duas equipas. Também incrementamos as vitórias e derrotas
de ambas as equipas, para depois ficar mais legível na nossa tabela de pontuações finais.
  Para finalizar, adicionamos á equipa1 a diferença de cestos, enquanto á equipa2 subtrai-mos este valor. Este valor servirá para mais
tarde em caso de empates em pontos de 2 equipas, desempatar-mos. */
void GetPoints(FILE *fichEntrada){
    struct jogos Jornadas;
    int check=0;
    int diferencaCestos =0, n=0;

    //Enquanto o ficheiro não chega ao fim, vamos buscar dados ao ficheiro
    while(!feof(fichEntrada)) {
        struct elemFila *prox = raizPrincipal, *prox2 = raizPrincipal;

        //Obter cada linha do ficheiro
        n = fread(&Jornadas, 1, sizeof(struct jogos), fichEntrada);

        //Obter as posições na listsa ligada

        //Esta linha de código é muito importante para prevenir inevitáveis erros devido ao funcionamento do fread
        if (n != 0) {

            //Procurar até os nomes que se obtém no ficheiro serem iguas a nomes na nossa lista ligada. Se não existirem
            //significa que ocorreu um erro.
            while (prox != NULL) {
                if (strcmp(prox->Equipa.nome_equipa, Jornadas.nomeEquipa1) == 0) break;
                prox = prox->proxEquipa;
            }
            while (prox2 != NULL) {
                if (strcmp(prox2->Equipa.nome_equipa, Jornadas.nomeEquipa2) == 0) break;
                prox2 = prox2->proxEquipa;
            }

            if (prox == NULL || prox2 == NULL) {
                printf("Existem dados errados no ficheir de texto");
                return;
            }

            // No ficheiro o número de cestos está escrito na forma CestosEquipa1 - CestosEquipa2
            diferencaCestos = Jornadas.pontosEquipa1 - Jornadas.pontosEquipa2;

            //Se a diferença for maior de zero significa que a equipa1 marcou mais pontos que a equipa 2
            if (diferencaCestos > 0) {
                prox->Equipa.pontos += 2;   //Incrementamos os pontos da equipa1
                prox->Equipa.vitorias += 1;  //Incrementamos as vitorias da equipa1
                prox2->Equipa.pontos += 1;   //""            "" ""       "" equipa2
                prox2->Equipa.derrotas += 1; //""            "" derrotas da equipa2
            }
                //Fazemos o contrário caso a diferenca de cesto for menor do que zero ( a equipa 2 marca mais pontos)
            else {
                prox->Equipa.pontos += 1;
                prox->Equipa.derrotas += 1;
                prox2->Equipa.pontos += 2;
                prox2->Equipa.vitorias += 1;

            }

            prox->Equipa.diferencaCestos += diferencaCestos;
            prox2->Equipa.diferencaCestos -= diferencaCestos;
        }
    }
}


/*Esta função serve para ordenar as equipas por ordem de pontos . Se houver empate em pontos será feito o desempate por diferenca de pontos ou um jogo "random"

 A ordenação é feita segundo o método "bubble sort", ou seja vamos movendo as parcelas de menor valor para o fim da fila sucessivamente, obtendo uma lista organizada
de forma decrescente de pontos.
 Primeiro passamos o elemento com menos valor para a última posição da fila, na iteração seguinte do "do while" passamos o segundo valor menos alto para
a antepenúltima posição , ... , até ao fim da nossa lista.
 Se 2 equipas empatarem em pontos veremos a diferença de cestos. Se empatarmos ainda na diferença de ceste teremos um jogo de desempate com um resultado final random.
 */


void OrdenarPontos(){

    //O inteiro trocado vai-nos indicar se há algo que ainda foi trocado ou nao. Se não houver vamos concluir que já está tudo em ordem
    int trocado = 0, i=0;

    struct elemFila *atual;
    //esta variavel começa null pois queremos que a primeira vez que o código percorra o while , percorra toda a lista ligada.
    struct elemFila *controlador = NULL;

    //Se a raiz for nula (erro)
    if(raizPrincipal == NULL) return;

    do{
        //Dar reset ao nosso verificador de ateracoes
        trocado = 0;
        atual = raizPrincipal;

        //Percorrer todos os elementos
        while(atual->proxEquipa != controlador){
            //Se os pontos do atual sao menores do que os pontos do seguinte
            if(atual->Equipa.pontos < atual->proxEquipa->Equipa.pontos){
                //Dizer que houve alteracoes nesta iteracao
                trocado = 1;

                //Fazer a alteracao
                TrocarOrdem(atual);
            }
            //Se 2 equipas empatarem com a mesma quantidade de pontos veremos a diferença de cestos
            else if (atual->Equipa.pontos == atual->proxEquipa->Equipa.pontos){

                if(atual->Equipa.diferencaCestos < atual->proxEquipa->Equipa.diferencaCestos){
                    TrocarOrdem(atual);
                }

                //Se empatarem em número de cestos teremos o nosso jogo random que decidirá a equipa vencedora
                else if( atual->Equipa.diferencaCestos == atual->proxEquipa->Equipa.diferencaCestos){

                    int desempate = JogoDesempate( atual );  //Se devolver 0 a equipa atual ganhou e 1 se a equipa atual.proxEquipa ganhou

                    if(desempate) TrocarOrdem(atual); //Equipa atual perde
                }

            }
            //Mover 1 unidade na nossa raiz
            atual = atual->proxEquipa;
        }
        //Fazer com que o bloco final fique permanente ( pois será o maior)
        controlador = atual;

    }while(trocado);
    //Se houver uma iteração onde não tenha havido trocas, significa que já não há mais nada a trocar o que nos pode poupar muitos recursos

}

//Troca a ordem na nossa raiz entre 2 elementos consequtivos. Criada para evitar repetições sucessivas de código na funcao Ordenar Pontos.
void TrocarOrdem (struct elemFila *atual ){
    struct teams valor = atual->Equipa;
    atual->Equipa = atual->proxEquipa->Equipa;
    atual->proxEquipa->Equipa = valor;
}

/*Esta função serve para desempatar em caso de empate em pontos e diferença de cestos por 2 equipas. Cria-se duas variáveis
auxiliares que correspondem às 2 equipas empatadas. A seguir damos valores random de 0 a 186 às variáveis. Vemos qual
das variaveis é maior e em caso de empate voltamos a dar um valor para aux2. Se aux1 > aux 2 , a equipa que está à esquerda
na lista ligada "ganha" e não há trocas de posição, senão ocorre o oposto.


NOTA: Escolheu-se 186 como número máximo, pois é o máximo que uma equipa alguma vez alcançou no NBA (podia ser outro valor qualquer
desde que não seja 0 ,embora quanto maior for o número, menor a probabilidade de os números aux1 e aux2 darem iguais, ou seja
menor probabilidade de o código ficar preso no while durante muito tempo o que torna o programa mais eficiente).*/

int JogoDesempate(struct elemFila *atual){
    int aux1=0, aux2=0;
    //Meter valor random no nosso auxiliar 1
    aux1 = rand()%(186);
    //Meter valor random no nosso auxiliar 2 que tem de ser diferente do 1
    while(aux2 ==0 || aux2 ==aux1){
        aux2 =  rand()%(186);
    }
    //Devolver resultados
    if(aux1 > aux2) return 0;
    else return 1;

}

/*Função para testar o programa. Indica como está a organização da nossa lista ligada.
NOTA: Deixei a função no programa para ser mais fácil ver como a lista ligada evolui em caso de dúvida*/

/*void printList (){
    struct elemFila *prox;

    if(raizPrincipal == NULL){
        printf("Empty List\n");
    }
    else{
        printf("Teams:\n");
        prox = raizPrincipal;

        while(prox != NULL) {
            printf("EQUIPA: %s;  PONTOS: %d;  Difcestos: %d  Vitórias :%d;  Derrotas:%d\n", prox->Equipa.nome_equipa, prox->Equipa.pontos,
                                                                                            prox->Equipa.diferencaCestos,prox->Equipa.vitorias,prox->Equipa.derrotas);
            prox = prox->proxEquipa;
        }
    }
}
*/

/*Esta função permite-nos obter uma linha do ficheiro que nos é passado por parametro. Se o ficheiro não existir, devolvemos um
char nulo que por sua vez no main vai cancelar a execução do programa. Esta função faz getc(fich) (busca 1 carater) até encontrar
 um endline character (\n), ou até já não existirem mais carateres ou existir um erro (EOF). Depois devolve essa string. */

char * ObterLinha(char nomeFich[]){
    int i=0,k=0;
    char *str = malloc(SIZE);

    FILE *fich = fopen(nomeFich,"r");
    //Assegurar que o ficheiro existe senão devolvemos um char nulo (que nos vai parar o programa por completo).
    if (fich == NULL ){
        printf("Esse ficheiro não existe! Escolha um ficheiro válido se faz favor!");
        return NULL;
    }

    //Escrever carater a carater até chegar ao fim do ficheiro ou haver um erro ou encontrar um \n
    while ( (i= getc(fich)) != EOF) {
        if( i == '\n') break;
        str[k]=i;
        k++;
    }
    return str;
}

/*Esta função escreve no nosso ficheiro de texto final as informações pedidas pelo enunciado (Uma tabela de classificações das equipas
 e uma lista com as duas equipas promovidas e despromovidas. Para a tabela de pontuações o programa vai à lista ordenada anteriormente
 na função "OrdenarPontos" e para cada elemento escreve no ficheiro os seus valores. Para as equipas promovidas e despromovidas, sabe-
 mos que as 2 equipas promovidas terao as posicoes 1 e 2 na lista enquanto as despromovidas terao as posicoes 9 e 10. Logo acedemos a
 essas posições e temos as equipas promovidas e despromovidas.

 Nota: Não é muito eficiente estarmos a percorrer 2 vezes a mesma lista ( 1 vez para a tabela e outra vez para as equipas despromovidas).
 Uma forma de resolver este problema facilmente seria com o uso de uma lista duplamente ligada, pois após a execução do primeiro varri-
 mento da lista, teriamos um ponteiro no fim da lista (atual) e outro no incio (raizPrincipal). A partir daí era muito mais fácil e efi-
 ciente obter-mos as equipas a promover e despromover.

 Outra solução para este problema seria fazer tanto a escrita das equipas promovidas/despromovidas e a tabela de posições, ao mesmo tempo,
 mas o facto de o enunciado pedir apenas 1 ficheiro de saída torna isso uma impossibilidade. Um problema com esta resolução seria o facto
 de termos código muito denso (com muitos ifs e else ifs) o que torna pior a sua leitura e talvez eficiência.
 */

void EscreveFicheiro (FILE *fichSaida, int numeroEquipas){
    struct elemFila *atual = raizPrincipal;
    int i=0;

    //Informações gerais sobre a estrutura da tabela
    fprintf(fichSaida, "CLASSIFICAÇÕES DAS EQUIPAS:\n\n");
    fprintf(fichSaida, "Organização: Lugar # Equipa # Pontos # Vitorias # Derrotas # DiferençaCestos\n\n");

    //Tabela de posições
    for( i=1 ; i<= numeroEquipas; i++){
        fprintf( fichSaida,"%2dº # %-10s # %2d # %2d # %2d # %3d\n",i, atual->Equipa.nome_equipa, atual->Equipa.pontos,
                                                                 atual->Equipa.vitorias, atual->Equipa.derrotas, atual->Equipa.diferencaCestos);
        atual = atual->proxEquipa;
    }

    //Escrever a equipa campeã
    fprintf(fichSaida,"\n\n\nEQUIPA CAMPEÃ:%s",raizPrincipal->Equipa.nome_equipa);

    //Escrever as equipas promovidas
    fprintf(fichSaida, "\n\nEQUIPAS PROMOVIDAS:\n");
    fprintf(fichSaida, "%s # %s \n\n", raizPrincipal->Equipa.nome_equipa, raizPrincipal->proxEquipa->Equipa.nome_equipa);
    atual = raizPrincipal;

    //Obter a posicao das ultimas 2 equipas
    for(i = 0; i< numeroEquipas -2; i++) atual =atual->proxEquipa;
    fprintf(fichSaida, "EQUIPAS DESPROMOVIDAS:\n");
    fprintf(fichSaida, "%s # %s", atual->Equipa.nome_equipa,atual->proxEquipa->Equipa.nome_equipa);
}