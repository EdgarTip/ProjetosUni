//Edgar Filipe Ferreira Duarte 2019216077
//Pedro Guilherme da Cruz Ferreira 2018277677
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/msg.h>


#include "RaceSimulator.h"
#include "MultipleProcessActions.h"
#include "TeamManager.h"

struct config_fich_struct *inf_fich;
struct ids *msgid;
struct team *team_list;
struct semaphoreStruct *semaphore_list;

int start_breakdown = 0;    //Variable that turns 1 so that the breakdowns can be created, once the race starts

// Function called when SIGUSR2 is received

void endBreakDown(int signum){
    msgctl(msgid->msg_queue_id,IPC_RMID,NULL); //Removes the message queue
    start_breakdown =0 ;
    #ifdef DEBUG
    printf("Break Down Manager is out!\n");
    #endif
    exit(0);    //Exits the process
}

// Function called when SIGUSR1 is received
void waitSig(int signum){
    pause();    //Pause process until a new signal is received
}

// Function called when SIGTERM is received

void raceStartBreakdown(int signum){
    start_breakdown = 1;
}

// Function called everytime its needed to generate breakdowns - every (1/inf_fich->time_units_per_second * inf_fich->T_Avaria) seconds

void createBreakdowns(struct ids *idsP, sigset_t mask, sigset_t new_mask){

    char log[MAX];

    //ignores signals in the set "new_mask"
    sigprocmask(SIG_BLOCK,&new_mask, NULL);

    sem_wait(semaphore_list->writingMutex); //Protects access to shared memory

    srand((unsigned) time(NULL));   //seed random

    //Runs through every car
    for(int i = 0; i < inf_fich->number_of_teams; i++){

        for(int j = 0; j < team_list[i].number_of_cars; j++){

            int r = rand()%100; //Gets a random percentage

            //if the random number generated is greater than the reliability of the car, new breakdown is generated
            if(r >= team_list[i].cars[j].reliability && team_list[i].cars[j].has_breakdown != 1 && strcmp(team_list[i].cars[j].current_state,"TERMINADO") != 0 && strcmp(team_list[i].cars[j].current_state,"DESISTENCIA") != 0){

                sprintf(log, "NEW PROBLEM IN CAR %02d",team_list[i].cars[j].car_number);
                writeLog(log,semaphore_list->logMutex,inf_fich->fp);

                //builds message to send via message queue to the specific car (indicated by type)
                struct messageQ msg;
                msg.mtype=i*inf_fich->number_of_cars+j+1;
                msg.response = 1;

                msgsnd(idsP->msg_queue_id, &msg, sizeof(msg)-sizeof(long), IPC_NOWAIT);
            }

        }
    }

    sem_post(semaphore_list->writingMutex);

    sigprocmask(SIG_UNBLOCK,&new_mask, NULL); //activates signals in the set "new_mask"
}


void BreakDownManager(struct config_fich_struct *inf_fichP, struct team *team_listP, struct semaphoreStruct *semaphore_listP, struct ids *idsP){

    sigset_t mask, new_mask;
    //Ignore all unwanted signals!
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    //Builds signal set
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGUSR2);
    sigaddset(&new_mask, SIGTERM);
    sigaddset(&new_mask, SIGUSR1);

    //Activates signal set
    sigprocmask(SIG_UNBLOCK,&new_mask, NULL);

    //Routes signals to respective functions
    signal(SIGUSR2, endBreakDown);
    signal(SIGTERM, raceStartBreakdown);
    signal(SIGUSR1, waitSig);

    #ifdef DEBUG
    printf("Breakdown Manager created (%ld)\n",(long)getpid());
    #endif

    inf_fich = inf_fichP;
    team_list = team_listP;
    semaphore_list = semaphore_listP;
    msgid = idsP;

    pause();    //Waits for signal so it can start generating breakdowns


    //Gives time to threads to be created
   	usleep(250000);

    while(start_breakdown == 1) {
        usleep(1000000/inf_fich->time_units_per_second * inf_fich->T_Avaria);

        createBreakdowns(idsP, mask ,new_mask);

    }

}
