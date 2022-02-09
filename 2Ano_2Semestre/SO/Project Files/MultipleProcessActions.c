//Edgar Filipe Ferreira Duarte 2019216077
//Pedro Guilherme da Cruz Ferreira 2018277677

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <semaphore.h>
#include <time.h>
#include "RaceSimulator.h"



//Function called everytime its needed to write a log to the file. The logs are also printed to the screen
void writeLog(char * string, sem_t *mutex,FILE *fp){
    fflush(stdout);
    char buffer[550]="";    //Buffer that holds the whole log string (time + message)

    //Get time current time into a struct
    time_t rawtime;
    struct tm timeinfo;
    time ( &rawtime );
    localtime_r ( &rawtime, &timeinfo);

    //Print the whole log into the buffer
    sprintf(buffer,"%.2d:%.2d:%.2d %s\n",timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec,string);

    //Synchronization so that only one process can print at a time
    sem_wait(mutex);

    printf("%s",buffer);    //Print log in the terminal
    fprintf(fp,"%s",buffer);    //Print log into the log file (logs.txt)
    fflush(fp);     //Flush file

    sem_post(mutex);

    fflush(stdout);

}


/*Function called when an ADDCAR command is received. It looks for space in the shared memory for the car and responds in conformity
Returns 1 when a new team is added and 0 when it is not.
This return is usefull so that the RaceManager knows if its necessary to create a new TeamManager process*/
int writingNewCarInSharedMem(struct team *team_list, struct car *new_car, struct config_fich_struct *inf_fich, char *team_name, struct semaphoreStruct *semaphore_list){

    sem_wait(semaphore_list->writingMutex);

    char carLog[200];
    for(int i=0; i<inf_fich->number_of_teams; i++){
        //A team with the given name was found
        if(strcmp(team_list[i].team_name,team_name)==0){
            for(int j = 0; j<inf_fich->number_of_cars; j++){

                //Sees if there is space for the car
                if(team_list[i].cars[j].speed == 0){

                    team_list[i].number_of_cars++;

                    team_list[i].cars[j] = *new_car;

                    sem_post(semaphore_list->writingMutex);
                    sprintf(carLog,"NEW CAR LOADED => TEAM: %s, CAR: %02d, SPEED: %d, CONSUMPTION: %.2f, RELIABILITY: %d",team_name,new_car->car_number, new_car->speed,new_car->consumption,new_car->reliability);
                    writeLog(carLog,semaphore_list->logMutex, inf_fich->fp);
                    return 0;
                }
            }
            writeLog("WARNING! THERE WAS NO SPACE IN THE SELECTED TEAM. THE CAR WILL NOT BE CREATED!", semaphore_list->logMutex, inf_fich->fp);
            sem_post(semaphore_list->writingMutex);
            return 0;
        }
        //No team with the given name exist and there is enough space to create a new team
        else if(strcmp(team_list[i].team_name,"") == 0){

            strcpy(team_list[i].team_name, team_name);
            strcpy(team_list[i].box_state, "LIVRE");
            team_list[i].cars[0] = *new_car;
            team_list[i].number_of_cars = 1;

            sem_post(semaphore_list->writingMutex);
            sprintf(carLog,"NEW CAR LOADED => TEAM: %s, CAR: %02d, SPEED: %d, CONSUMPTION: %.2f, RELIABILITY: %d",team_name,new_car->car_number, new_car->speed,new_car->consumption,new_car->reliability);
            writeLog(carLog,semaphore_list->logMutex, inf_fich->fp);
            return 1;
        }
    }
    //There is no space for a new team
    writeLog("WARNING! THERE WAS NO SPACE TO CREATE A NEW TEAM. THE CAR WILL NOT BE CREATED!",semaphore_list->logMutex, inf_fich->fp);
    sem_post(semaphore_list->writingMutex);
    return 0;
}

/*Function called by readStatistics
Gets the best 5 cars in the race ordered into the "top5Teams" array.
It orders the cars by the laps completed.
Note that the values on the ordered array represent the car position in the shared memory
*/
void getTop5Teams(struct config_fich_struct *inf_fich, struct team *team_list, int top5Teams[5][2]){

    for(int i= 0; i < inf_fich->number_of_teams; i++){

        for(int j = 0; j < team_list[i].number_of_cars; j++){
            for(int k = 0; k < 5; k++){

                //First sees if there is an empty space, then sees if its value is superior to another
                if(top5Teams[k][0]==-1){
                    top5Teams[k][0] = i;
                    top5Teams[k][1] = j;
                    break;
                }
                else if(team_list[i].cars[j].number_of_laps > team_list[top5Teams[k][0]].cars[top5Teams[k][1]].number_of_laps){
                    int temp1 = top5Teams[k][0];
                    int temp2 = top5Teams[k][1];

                    int temp3 = 0;
                    int temp4 = 0;
                    //get the index to the team and car of the current value
                    top5Teams[k][0] = i;
                    top5Teams[k][1] = j;

                    for(int l = k+1; l< 5;l++){
                        if(top5Teams[l][0] == -1){
                            top5Teams[l][0] = temp1;
                            top5Teams[l][1] = temp2;
                            break;
                        }
                        temp3 = top5Teams[l][0];
                        temp4 = top5Teams[l][1];

                        top5Teams[l][0] = temp1;
                        top5Teams[l][1] = temp2;

                        temp1 = temp3;
                        temp2 = temp4;
                    }
                    break;
                }
            }
        }
    }
}

//Gives the indexes of the last team of the team list in terms of amount of laps
void getLastTeam(struct config_fich_struct *inf_fich, struct team *team_list,  int lastTeam[1][2]){

    for(int i= 0; i < inf_fich->number_of_teams; i++){
        for(int j = 0; j < team_list[i].number_of_cars; j++){

            if(lastTeam[0][0] == -1){
                lastTeam[0][0] = i;
                lastTeam[0][1] = j;
            }
            if(team_list[i].cars[j].number_of_laps < team_list[lastTeam[0][0]].cars[lastTeam[0][1]].number_of_laps){
                lastTeam[0][0] = i;
                lastTeam[0][1] = j;
            }
        }
    }
}

//Function that counts the total breakdowns that appened during the race
int amountBreakdowns(struct config_fich_struct *inf_fich, struct team *team_list){
    int total_breakdowns = 0;
    for(int i = 0 ; i < inf_fich->number_of_teams; i++){
        for( int j = 0 ; j < team_list[i].number_of_cars; j++){
            total_breakdowns += team_list[i].cars[j].amount_breakdown;
        }
    }
    return total_breakdowns;
}
//Function that counts the number refills that appened during the race
int amountReffil(struct config_fich_struct *inf_fich, struct team *team_list){
    int total_reffils = 0;
    for(int i = 0 ; i < inf_fich->number_of_teams; i++){
        for( int j = 0 ; j < team_list[i].number_of_cars; j++){
            total_reffils += team_list[i].cars[j].times_refill;
        }
    }
    return total_reffils;
}

//Function that counts the number of cars that are in the race in the moment it is called
int amountRacing(struct config_fich_struct *inf_fich, struct team *team_list){
    int total_racing = 0;

    for(int i = 0 ; i < inf_fich->number_of_teams; i++){
        for( int j = 0 ; j < team_list[i].number_of_cars; j++){
            if(strcmp(team_list[i].cars[j].current_state, "CORRIDA") == 0 || strcmp(team_list[i].cars[j].current_state, "SEGURANCA") == 0 ){
                total_racing++;
            }
        }
    }

    return total_racing;
}

//Prints the statistics of a race (could be midway or at the end). This has priority over writing actions
void readStatistics(struct config_fich_struct *inf_fich, struct team *team_list, struct semaphoreStruct *semaphore_list){

    sem_wait(semaphore_list->writingMutex);
    //First line for team index second line for car index
    int top5Teams[5][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
    int lastTeam[1][2] = {{-1,-1}};
    getTop5Teams(inf_fich, team_list, top5Teams);
    getLastTeam(inf_fich, team_list, lastTeam);
    int total_breakdowns = amountBreakdowns(inf_fich, team_list);
    int total_reffils = amountReffil(inf_fich, team_list);
    int total_racing = amountRacing(inf_fich, team_list);
    for(int i = 0; i<5; i++){
        if(top5Teams[i][0]==-1) break;
        printf("Lugar: %d, Número Carro: %02d, Nome Equipa: %s, Número Voltas: %d, Número de Paragens na Box: %d\n", i+1, team_list[top5Teams[i][0]].cars[top5Teams[i][1]].car_number
                , team_list[top5Teams[i][0]].team_name
                , team_list[top5Teams[i][0]].cars[top5Teams[i][1]].number_of_laps
                , team_list[top5Teams[i][0]].cars[top5Teams[i][1]].amount_breakdown+team_list[top5Teams[i][0]].cars[top5Teams[i][1]].times_refill);
    }

    printf("Carro em último lugar: Número Carro: %d, Nome Equipa: %s, Número Voltas: %d, Número de Paragens na Box: %d\n", team_list[lastTeam[0][0]].cars[lastTeam[0][1]].car_number
            , team_list[lastTeam[0][0]].team_name
            , team_list[lastTeam[0][0]].cars[lastTeam[0][1]].number_of_laps
            , team_list[lastTeam[0][0]].cars[lastTeam[0][1]].amount_breakdown+ team_list[lastTeam[0][0]].cars[lastTeam[0][1]].times_refill);


    printf("Quantidade de avarias: %d\n", total_breakdowns);
    printf("Quantidade de reabastecimentos: %d\n", total_reffils);
    printf("Quantidade de carros na pista: %d\n", total_racing);

    sem_post(semaphore_list->writingMutex);



}
