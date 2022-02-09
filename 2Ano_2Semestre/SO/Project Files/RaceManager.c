//Edgar Filipe Ferreira Duarte 2019216077
//Pedro Guilherme da Cruz Ferreira 2018277677

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>


#include "RaceSimulator.h"
#include "MultipleProcessActions.h"
#include "TeamManager.h"


struct config_fich_struct *inf_fich;
struct team *team_list;
struct semaphoreStruct *semaphore_list;

//Ids of other processes
struct ids *ids_proc;

//Ids of all of the teams and pipes of all the teams
int *pids;
int *pipes;

//Number of teams that have stopped
int number_of_teams_stopped;

//Number of teams stopped in this iteration of pausing (this is needed so that if a team where all of its cars give up terminates there isn't any unwanted bugs)
int number_added_this_iteration;

// == 0 if race hasn't paused == 1 if race is currently paused
int race_has_paused;

// == 0 if race hasn't started yet == 1 if race is currently on
int start = 0;

//Function used in case a team has all of its cars giving up
void skip(int signum){

    number_of_teams_stopped++;

    //means all the teams have finished. Sends a SIGINT to the racesimulator.
    if(race_has_paused == 0 && number_of_teams_stopped == inf_fich->number_of_teams){
        writeLog("EVERY SINGLE CAR GAVE UP", semaphore_list->logMutex, inf_fich->fp);
        kill(getppid(), SIGINT);
    }
}

//Fucntion that pauses the race. When all the teams have reported that all their cars are paused it waits for a message saying "START RACE!"
//to resume the race. Also accepts SIGINT
void pauseRace(int signum){

	//ignores subsequent SIGUSR1 while race is paused
    signal(SIGUSR1, SIG_IGN);

	//means that the race has started. If the race has not started then the signal is ignored
    if(start == 1){

        race_has_paused =1;
        writeLog("SIMULATOR PAUSING", semaphore_list->logMutex, inf_fich->fp);

		//Tells teams that the race has paused
        for(int i = 0; i < inf_fich->number_of_teams; i++){
            kill(pids[i], SIGUSR1);
        }

		//Tells the breakdownManager that the teams have ended
        kill(ids_proc->pid_breakdown, SIGUSR1);

        start = 0;

        number_added_this_iteration = 0;

        //Waits for all the teams to stop their execution
        while(number_of_teams_stopped != inf_fich->number_of_teams){
            pause();
            number_added_this_iteration++;
        }
		readStatistics(inf_fich, team_list, semaphore_list);
        writeLog("ALL CARS PAUSED", semaphore_list->logMutex, inf_fich->fp);
        printf("IF YOU WANT TO RESTART RACE TYPE: START RACE!\n");
    }

}

//Signals all the teams that the race has ended
void endRaceManager(int signum){

	//Signals all the teams that the race has ended
    for(int i = 0; i < inf_fich->number_of_teams; i++){
        kill(pids[i], SIGUSR2);
    }
    //Frees resources
    free(pids);

	//Waits for all the teams to end their execution
    for(int i = 0; i < inf_fich->number_of_teams; i++){
        wait(NULL);
    }

    #ifdef DEBUG
    printf("RaceManeger is out.\n");
    #endif
	//Termiantes its execution
    exit(0);
}

//Ends the race. Signalsthe RaceSimulator and BreakDown simulator the race has ended
void endRace(){
    kill(ids_proc->pid_breakdown, SIGUSR2);
    kill(getppid(), SIGUSR2);
    endRaceManager(0);
}

//Returns a channel that is avaiable for a new pipe
int getFreeChannel(int n, int pipes[n]){
    for(int i=0;i<n;i++){
        if(pipes[i]==-1){
            return i;
        }
    }
    return -1;

}

//Main function of the RaceManager. The RaceManager creates the teams and the cars of a race via commands this process receives from the named pipe.

void Race_Manager(struct config_fich_struct *inf_fichP, struct team *team_listP,  struct semaphoreStruct *semaphore_listP, struct ids *idsP){

    char log[550];

    sigset_t mask, new_mask;
    ids_proc = idsP;

    //Ignore all unwanted signals
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);

	//creates mask with wanted signals
    sigemptyset(&new_mask);
    sigaddset(&new_mask,SIGUSR1);
    sigaddset(&new_mask,SIGUSR2);
    sigaddset(&new_mask,SIGALRM);
    sigprocmask(SIG_UNBLOCK,&new_mask, NULL);

    signal(SIGUSR1, pauseRace);
    signal(SIGUSR2, endRaceManager);
    signal(SIGALRM, skip);

	//number of teams that have stopped in case of a pause
    number_of_teams_stopped = 0;

    #ifdef DEBUG
    printf("Race_Manager created (%ld)\n",(long)getpid());
    #endif

    inf_fich = inf_fichP;
    team_list = team_listP;
    semaphore_list = semaphore_listP;

	//allocates enough space for a pipe for each team
    pipes=malloc(sizeof(*pipes)*(inf_fich->number_of_teams+1));
    for(int i=0;i<inf_fich->number_of_teams+1;i++){
        pipes[i]=-1;
    }

	//Alocates enough space to save all of the teams pids
    pids = malloc(sizeof(int) * inf_fich->number_of_teams);

	//Variables
	//Indicates the index of the next team in the shared memory
    int index_team = 0;
    //==0 is the first start of the race == 1 is not the first start of the race (has received a pause)
    int firstStart = 0;
    //says next position in pipes array
    int i =0;

    #ifdef DEBUG
    printf("Opening named pipe for reading\n");
    #endif

    //Opens named pipe
    int fd;
    if ((fd= open(PIPE_NAME, O_RDONLY)) < 0) {
        perror("Cannot open pipe for reading: ");
        exit(0);
    }

    //Puts named pipe in first position
    pipes[0]=fd;
    #ifdef DEBUG
    printf("Named pipe open for reading\n");
    #endif

    char teamName[50];
    char received[512];
    int n=1;

	//Keeps looking for information from both the named pipe and the unamed pipes
    while(1){
        fd_set read_set;
        FD_ZERO(&read_set);
        for(int channel=0;channel<inf_fich->number_of_teams+1;channel++){
            FD_SET(pipes[channel], &read_set);
        }
        if (select(pipes[n-1]+1, &read_set, NULL, NULL, NULL) > 0 ) {

        //sees if there is new information in the named pipe
        if(FD_ISSET(pipes[0], &read_set)){
            sigprocmask(SIG_BLOCK,&new_mask, NULL);
            read(pipes[0],received,sizeof(received));
            #ifdef DEBUG
            printf("[RaceManager] (NP) Received: %s\n",received);
            #endif

			//Receives the command start race. If allowed the race will now start
            if(strcmp(received,"START RACE!")==0){
                //Sees if the race is currently running == 0 means it is not
                if(start == 0){
					//If it is the first time the race is starting == 0 means it is the first time
                    if(firstStart == 0){

                        int n_equipas=0;
                        //Sees if there are enough teams to start the race
                        for(int i=0; i<inf_fich->number_of_teams; i++){
                            if(strcmp(team_list[i].team_name,"")!=0){
                                n_equipas++;
                            }
                        }
                        //Not enough teams to start the race
                        if(n_equipas!=inf_fich->number_of_teams){
                            writeLog("CANNOT START, NOT ENOUGH TEAMS",semaphore_list->logMutex,inf_fich->fp);
                        }
                        //starts the race
                        else{
                            //Notifies all of the other processes that the race is starting
                            for(int i = 0; i<inf_fich->number_of_teams; i++){
                                kill(pids[i], SIGTERM);

                            }
                            kill(getppid(), SIGTERM);
                            kill(idsP->pid_breakdown, SIGTERM);
                            start = 1;
                            firstStart = 1;

                        }
                    }
                    //means that there was a pause and the race is going to restart
                    else{
                        //Notifies the TeamManagers and the BreakdownManager of the restart of the race
                        for(int i = 0; i<inf_fich->number_of_teams; i++){
                            kill(pids[i], SIGTERM);

                        }
                        kill(idsP->pid_breakdown, SIGTERM);

                        writeLog("RACE UNPAUSED",semaphore_list->logMutex,inf_fich->fp);
                        start = 1;
                        race_has_paused =0;
                        number_of_teams_stopped -= number_added_this_iteration;

                        //Sets SIGUSR1 from SIG_IGN to again be able to receive pauses
                        signal(SIGUSR1, pauseRace);
                    }
                }
                //means that the race was already running
                else if(start == 1){
                    sprintf(log,"%s => Rejected, race already started!",received);
                    writeLog(log,semaphore_list->logMutex,inf_fich->fp);
                }
            }
            // Check if a ADDCAR was received
            else{
                struct car *newCar = (struct car *) malloc(sizeof(struct car));

                int validation = sscanf(received,"ADDCAR TEAM: %[^,], CAR: %d, SPEED: %d, CONSUMPTION: %f, RELIABILITY: %d",teamName,&(newCar->car_number),&(newCar->speed),&(newCar->consumption),&(newCar->reliability));
				//if this test failed it means that the car does not have correct values
                if( validation != 5 ||
                    newCar->car_number < 0 ||
                    newCar->speed <= 0 ||
                    newCar->consumption < 0 ||
                    newCar->consumption > inf_fich->fuel_capacity ||
                    newCar->reliability < 0 ){

                        sprintf(log,"WRONG COMMAND => %s",received);
                        writeLog(log,semaphore_list->logMutex,inf_fich->fp);
                    }

                    else{
                    //if race has already started can not receive a new addcar
                        if(start == 1){ //Race already started
                            sprintf(log,"%s => Rejected, race already started!",received);
                            writeLog(log, semaphore_list->logMutex, inf_fich->fp);
                        }
                        //adds car to the race. If the car is from a team that has not yet been identified then it creates a new team (if there is enough space)
                        else{

							//cre
                            newCar->number_of_laps = 0;
                            newCar->amount_breakdown = 0;
                            newCar->times_refill = 0;
                            newCar->has_breakdown = 0;
                            strcpy(newCar->current_state, "CORRIDA");

							//writes new car to the shared memory
                            int teamCreated = writingNewCarInSharedMem(team_list, newCar, inf_fich, teamName, semaphore_list);

							//Means that the team was not yet created. Creates a new team
                            if(teamCreated ==1){

                                #ifdef DEBUG
                                printf("Getting channel for new team.\n");
                                #endif

								//Creates a pipe for the team
                                int c=getFreeChannel(inf_fich->number_of_teams+1,pipes);
                                int channel[2];
                                pipe(channel);
                                n++;
                                pipes[c]=channel[0];

								//Creates the team
                                if((pids[i] =fork())==0){


                                    Team_Manager(inf_fich, team_list, semaphore_list,channel,index_team,idsP);

                                }
                                i++;
                                index_team++;
                                //close(channel[1]);

                            }
                        }
                    }
                }
                sigprocmask(SIG_UNBLOCK,&new_mask, NULL);

            }
            //Sees if any of the unamed pipes has information in them
            for(int channel=1;channel<(inf_fich->number_of_teams+1);channel++){
                if(FD_ISSET(pipes[channel], &read_set)){
                    sigprocmask(SIG_BLOCK,&new_mask, NULL);

                    struct message data;

					//Receives the information
                    read(pipes[channel],&data,sizeof(struct message));

                    #ifdef DEBUG
                    printf("[RaceManager] (%d) Received %d, %d ,%s.\n",channel,data.car_index, data.team_index,data.message);
                    #endif

					//If the message says "TERMINADO" means that the race will now end
                    if(strcmp(data.message,"TERMINADO") == 0){
                        signal(SIGUSR1, SIG_IGN);
                        sigprocmask(SIG_UNBLOCK,&new_mask, NULL);
                        endRace();

                    }
                    sigprocmask(SIG_UNBLOCK,&new_mask, NULL);
                }
            }
            //}


        } //select
    } //while


}
