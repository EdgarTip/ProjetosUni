//Edgar Filipe Ferreira Duarte 2019216077
//Pedro Guilherme da Cruz Ferreira 2018277677


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/msg.h>



#include "RaceSimulator.h"
#include "MultipleProcessActions.h"



struct config_fich_struct *inf_fich;
struct team *team_list;
struct semaphoreStruct *semaphore_list;

//Thread identifier array. Size will be cast after race has started.
pthread_t *cars;

/*Variable conditions. Will be used for when the race is interrupted and the cars are waiting for the race to restart,
for when the cars are waiting for all the cars of their team to end the race */
pthread_mutex_t mutex_interruption = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t 	interruption_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t  mutex_stop = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t 	stop_cond= PTHREAD_COND_INITIALIZER;


//Identifiers of the time manager
pthread_t sleep_thread;
int sleep_thread_id;

//Semaphores that will garantee a safe and sincronized environment for the program to execute
//Semaphore responsible for signaling the TeamManager when a car is going to enter the box state
sem_t *car_in_box;
//Semphore responsible for signaling the TeamManager when a car makes a new reservation
sem_t *reservation;
//Semaphore that signals that garantees that only 1 car can check the box state. Is a semaphore due to it having some signaling functionalities
sem_t *update_waiting;
//Semaphore that makes sure that 2 cars do not update a global variable at the same time
sem_t *counter_mutex;
//Semaphore responsible for signaling a thread when it can exit the box state and start racing again
sem_t *wait_box;
//Semaphore responsible for signaling the TeamManager that all the cars have paused the race.
sem_t *interrupt_race_mutex;
//Semaphore used to signal a new iteration for the cars
sem_t *iteration_mutex;

/*Index of the car/team in the shared memory. This will be used so that their respective indexes can be accessed without the use of semaphores/mutexs
for incriesed performance*/
int team_index;
int car_index;

//Channel of the unamed pipe that the cars will use to communicate to the RaceManager
int *channel;

//If == 0 race has not started if == 1 race has started
int start_teams = 0;
//Number of cars in the team. Is defined so we don't check shared memory has often
int number_of_cars;
//If == 0 box means that the box is not reserved if == 1 means box is reserved
int is_reserved = 0;
//If == 0 means race is not ending if == 1 means race is ending
int terminating = 0;
//IF == 0 means race is not pausing if == 1 means race is pausing
int interrupting = 0;
//Tells the TeamManager how many cars have ended/paused their execution
int amount_terminated = 0;
//Variable for condicion variable that is used to tell the threads if they can or cannot end their execution
int out = 0;
//Variable that indicates if this is the first start of the race or not (in case of a pause)
int firstStart = 0;
//Sees if the main process was or not in the box when a signal is called
int is_in_box = 0;
//In case of an interruption, if the car was in the box, skips box elements
int had_interruption = 0;
//Variable that indicates to the time manager how many cars are waiting for a new loop iteration
int amount_waiting = 0;
//masks of the signals that will be used/ignored
sigset_t mask, new_mask;
struct ids *idsT;

//Ends the team

//Cleans all of the resources used
void cleanT(){
    sem_close(car_in_box);
    sem_close(reservation);
    sem_close(update_waiting);
    sem_close(counter_mutex);
    sem_close(wait_box);
    sem_close(interrupt_race_mutex);
    sem_close(iteration_mutex);
    pthread_cond_destroy(&interruption_cond);
    pthread_mutex_destroy(&mutex_interruption);
    pthread_mutex_destroy(&mutex_stop);
    pthread_cond_destroy(&stop_cond);
    sem_unlink("BOX_MUTEX");
    sem_unlink("RESERVATION_MUTEX");
    sem_unlink("UPDATE_MUTEX");
    sem_unlink("END COUNTER");
    sem_unlink("WAIT BOX");
    sem_unlink("INTERRUPT MUTEX");
    sem_unlink("ITERATION MUTEX");

}


/*This fuction is used in case of a pause occuring. The TeamManager will wait for all the cars to pause
and after that will the TeamManager will notify the RaceManager about the pause of all its cars. Then it wait
for an answer (can be SIGINT or SIGTERM) to kill/restart race. If race is restarted then it tells its cars
that they can now move again.*/
void pauseRaceTeams(int signum){

    sigprocmask(SIG_BLOCK,&new_mask, NULL);

    interrupting = 1;

    had_interruption = 1;

    if(is_in_box == 1){
        sem_post(wait_box);
    }

    //Makes sure it waits for all of the cars
    sem_wait(interrupt_race_mutex);

    //Sends a signal to the race manager that this team has stopped all cars!
    kill(getppid(), SIGALRM);

    sigprocmask(SIG_UNBLOCK,&new_mask, NULL);

    //waits for a signal
    pause();

    interrupting = 0;
    //Tell cars they can start moving again
    pthread_cond_broadcast(&interruption_cond);
}

/*This function is for when the team manager prepares to die. It waits for all the cars to
end their lap and, when they all do, the team manager cleans all of the resources it uses
and exits. This function can be called by SIGINT or when a car in the team ends the race.*/
void teamEnd(int signum){

    sigprocmask(SIG_BLOCK,&new_mask, NULL);


    //This code exists in case a SIGINT is called after a  SIGUSR1
    if(start_teams == 0){
        for(int i =0 ; i < team_list[team_index].number_of_cars; i++){
            pthread_cancel(cars[i]);
        }
        exit(0);
    }

    //Indicates the race is terminating
    terminating = 1;

    interrupting = 0;
    if(is_in_box == 1){
        sem_post(wait_box);
    }
    pthread_cond_broadcast(&interruption_cond);

    //So that we reduce the number of times we call this data from the shared memory
    int number_cars = team_list[team_index].number_of_cars;

    //While the number of cars that have finished their race is not the same as the number of total cars in the team keep waiting
    while(amount_terminated != number_cars){

        sleep(1/inf_fich->time_units_per_second);
    }

    //means all of the cars have left so now we can let free all of the cars that where waiting
    out = 1;
    pthread_cond_broadcast(&stop_cond);

    //Reclaims all of the threads
    for(int j=0; j<number_of_cars; j++){
        pthread_join(cars[j], NULL);
    }
    pthread_join(sleep_thread,NULL);

    //cleans resources
    cleanT();

    #ifdef DEBUG
    printf("Team %ld out\n",(long)getpid());
    #endif

    exit(0);

}


//Function only called in case that all of the cars in a team gave up. The TeamManager will send a signal to the RaceManager to indicate its death and end its execution seing that it does
//Not need to continue alive.
void everyoneGaveUp(){
    #ifdef DEBUG
    printf("EVERYONE IS DEAD!\n");
    #endif

    //Cancels all of the threads
    for(int i =0 ; i < team_list[team_index].number_of_cars; i++){
        pthread_cancel(cars[i]);
    }
    pthread_cancel(sleep_thread);

    //Sends signal to RaceManager
    kill(getppid(), SIGALRM);

    exit(0);


}

//Function where cars wait for the race to end
void waitForEnd(){

    pthread_mutex_lock(&mutex_stop);

    while(out == 0){
        pthread_cond_wait(&stop_cond,&mutex_stop);
    }

    pthread_mutex_unlock(&mutex_stop);

    pthread_exit(NULL);
}

//Function where a thread indicates it has  given up from a race. If all of the cars in the team have given up then
//the thread will be resent to the everyoneGaveUp() function. Else it will wait for the end of the race
void gaveUp(){

    sem_wait(counter_mutex);
    amount_terminated++;

    if(amount_terminated == team_list[team_index].number_of_cars && terminating == 0){
        kill(getpid(),SIGALRM);
    }
    sem_post(counter_mutex);
    waitForEnd();


}
//This funcion is where a thread indicates it has ended its race. The thread will be redirected to the function waitForEnd().
void anouncingEnd(){

    sem_wait(counter_mutex);
    amount_terminated++;
    sem_post(counter_mutex);


    waitForEnd();
}




//Function that simulates the cars racing
void racing(int arrayNumber){

    //Ignore all signals (so threads do not have unexpected behaviour)
    sigprocmask(SIG_BLOCK,&new_mask, NULL);

    //Variables to comunicate with the log file and unnamed pipe
    char log[MAX];
    char update[MAX];

    //Amount of fuel and distance in lap
    float current_fuel = inf_fich->fuel_capacity;
    float distance_in_lap = 0;

    //Structs used to receive data from the message queue
    struct message data;
    struct messageQ msg;
    msg.response = 0;

    //escrever para o gestor de equipas que o carro começou em corrida com o estado "CORRIDA"
    data.car_index = arrayNumber;
    data.team_index = team_index;

    //runs until a car has finished the race or the race has paused or has been called off (SIGINT)
    while(1){

        //Sees if there is a new message from the message queue
        if(terminating != 1  && interrupting != 1 && team_list[team_index].cars[arrayNumber].has_breakdown != 1){
            msgrcv(idsT->msg_queue_id, &msg, sizeof(msg)-sizeof(long), team_index*inf_fich->number_of_cars+arrayNumber+1, IPC_NOWAIT);
        }

        /*If there was a message then creates a breakdown to the car. IF the box was not yet reserved, the car reserves it. The car enters
        security mode*/

        if(msg.response == 1 && terminating != 1 && interrupting !=1){
            team_list[team_index].cars[arrayNumber].amount_breakdown++;
            #ifdef DEBUG
            printf("Breakdown Received (%02d) => Security Mode\n",team_list[team_index].cars[arrayNumber].car_number);
            #endif

            team_list[team_index].cars[arrayNumber].has_breakdown = 1;
            strcpy(team_list[team_index].cars[arrayNumber].current_state, "SEGURANCA");

            //Writes new state to terminal and log file
            sprintf(log,"CAR %02d IS NOW IN STATE: SEGURANCA",team_list[team_index].cars[arrayNumber].car_number);
            writeLog(log, semaphore_list->logMutex, inf_fich->fp);

            //Makes sure 2 threads do not write at the same time to the box

            sem_wait(update_waiting);


            //Updates the state of the box to RESERVED if it is not yet
            if(strcmp(team_list[team_index].box_state, "LIVRE") == 0){
                is_reserved = 1;
                sem_post(reservation);
                #ifdef DEBUG
                printf("Team %s made a reservation\n",team_list[team_index].team_name);
                #endif


            }
            //Means that the box is already  RESERVED or it is currently BLOCKED
            else{
                sem_post(update_waiting);
            }
            msg.response = 0;
        }

        //Racing normally. Updates distance and fuel of the car at normal levels
        if (strcmp(team_list[team_index].cars[arrayNumber].current_state,"CORRIDA") == 0) {
            current_fuel -= team_list[team_index].cars[arrayNumber].consumption;
            distance_in_lap += team_list[team_index].cars[arrayNumber].speed;

        }

        //The car needs to go to the box, and due to that it is in security mode. The car moves slower but uses less fuel
        else if (strcmp(team_list[team_index].cars[arrayNumber].current_state,"SEGURANCA") == 0) {

            current_fuel -= 0.4 * team_list[team_index].cars[arrayNumber].consumption;
            distance_in_lap += 0.3 * team_list[team_index].cars[arrayNumber].speed;
        }

        //The car has no more fuel. This means that the car is giving up on the race
        if(current_fuel <= 0){

            strcpy(team_list[team_index].cars[arrayNumber].current_state,"DESISTENCIA");

            sprintf(log,"CAR %02d IS NOW IN STATE: DESISTENCIA",team_list[team_index].cars[arrayNumber].car_number);
            writeLog(log, semaphore_list->logMutex, inf_fich->fp);

            //gives up
            gaveUp();

        }


        //If the car does not have enough fuel for the next 2 laps, the car enters security mode. If the box is not yet reserved /occupied, the car will
        //reserve the box
        if(interrupting != 1 && terminating != 1 && current_fuel < 2 * ((inf_fich->lap_distance / team_list[team_index].cars[arrayNumber].speed)*team_list[team_index].cars[arrayNumber].consumption )){



            if(strcmp(team_list[team_index].cars[arrayNumber].current_state,"SEGURANCA") != 0){
                strcpy(team_list[team_index].cars[arrayNumber].current_state, "SEGURANCA");
                sprintf(log,"CAR %02d IS NOW IN STATE: SEGURANCA",team_list[team_index].cars[arrayNumber].car_number);
                writeLog(log, semaphore_list->logMutex, inf_fich->fp);

            }

            sem_wait(update_waiting);

            //Reserve the box if needed
            if(strcmp(team_list[team_index].box_state, "LIVRE") == 0){
                is_reserved = 1;

                sem_post(reservation);
                #ifdef DEBUG
                printf("Team %s made a reservation\n",team_list[team_index].team_name);
                #endif

            }

            //Means the box was already reserved
            else{
                sem_post(update_waiting);
            }

        }


        //Passes a lap. First sees if the race has ended / is pausing. If not then sees if the car
        //has ended the race. If not, checks if the car has a breakdown or need a refill and if it does then it goes to the box, if it has
        //permission to do so. If not, the car continues the race.
        if(distance_in_lap >= inf_fich->lap_distance){
            //Increments the nubmer of lap counter
            team_list[team_index].cars[arrayNumber].number_of_laps++;
            #ifdef DEBUG
            printf("(%02d) %d laps\n", team_list[team_index].cars[arrayNumber].car_number, team_list[team_index].cars[arrayNumber].number_of_laps);
            #endif


            //race forced to terminate!
            if(terminating == 1){
                strcpy(team_list[team_index].cars[arrayNumber].current_state,"TERMINADO");
                sprintf(log,"CAR %02d IS NOW IN STATE: TERMINADO",team_list[team_index].cars[arrayNumber].car_number);
                writeLog(log, semaphore_list->logMutex, inf_fich->fp);
                break;
            }

            //the race has been interrupted. Race will start again if a RACE START is detected via the named pipe
            else if(interrupting == 1){

                //the car indicates it has stopped
                sem_wait(counter_mutex);
                amount_terminated++;
                sem_post(counter_mutex);

                #ifdef DEBUG
                printf("(%02d) Interrupted.\n", team_list[team_index].cars[arrayNumber].car_number);
                #endif

                //all of the cars are now on stand by
                if(amount_terminated == team_list[team_index].number_of_cars){
                    start_teams =  0;
                    sem_post(interrupt_race_mutex);
                }

                pthread_mutex_lock(&mutex_interruption);

                //Wait until the interruptiong is over
                while (interrupting == 1) {

                    pthread_cond_wait(&interruption_cond, &mutex_interruption);
                }

                pthread_mutex_unlock(&mutex_interruption);

                //if the race has terminated
                if(terminating == 1){

                    strcpy(team_list[team_index].cars[arrayNumber].current_state,"TERMINADO");
                    sprintf(log,"CAR %02d IS NOW IN STATE: TERMINADO",team_list[team_index].cars[arrayNumber].car_number);
                    writeLog(log, semaphore_list->logMutex, inf_fich->fp);
                    break;
                }

                //Indicates that it has restarted the race
                sem_wait(counter_mutex);
                amount_terminated--;
                sem_post(counter_mutex);

            }


            //If the race has no interruptions/ has not ended
            else{


                //Resets the distance in the lap
                distance_in_lap = distance_in_lap - inf_fich->lap_distance;

                //Checks if the car that is in securtiy mode needs to go to the box and if it is allowed it goes to the box
                if((strcmp(team_list[team_index].cars[arrayNumber].current_state, "SEGURANCA") == 0) && (strcmp(team_list[team_index].box_state, "RESERVADO") == 0) && team_list[team_index].cars[arrayNumber].number_of_laps < inf_fich->number_of_laps && terminating != 1 && interrupting != 1){


                    car_index = arrayNumber;

                    //Notify team manager that a car has entered the box
                    sem_post(car_in_box);

                    //New car state
                    strcpy(team_list[team_index].cars[arrayNumber].current_state, "BOX");

                    sprintf(log,"CAR %02d IS NOW IN STATE: BOX",team_list[team_index].cars[arrayNumber].car_number);
                    writeLog(log, semaphore_list->logMutex, inf_fich->fp);

                    //Waits for the everything to get sorted in the car
                    current_fuel = inf_fich->fuel_capacity;

                    sem_wait(wait_box);


                    //if the race has terminated while he was in the box, the thread will exit
                    if(terminating == 1){
                        strcpy(team_list[team_index].cars[arrayNumber].current_state,"TERMINADO");
                        sprintf(log,"CAR %02d IS NOW IN STATE: TERMINADO",team_list[team_index].cars[arrayNumber].car_number);
                        writeLog(log, semaphore_list->logMutex, inf_fich->fp);
                        break;
                    }

                    //if the race has interrupted while the car was in the box. Same logic has the else if(interrupting == 1) that is in line 398
                    else if(interrupting == 1){

                        sem_wait(counter_mutex);
                        amount_terminated++;

                        if(amount_terminated == team_list[team_index].number_of_cars){

                            start_teams =  0;
                            sem_post(interrupt_race_mutex);
                        }

                        sem_post(counter_mutex);
                        #ifdef DEBUG
                        printf("(%02d) Interrupted.\n", team_list[team_index].cars[arrayNumber].car_number);
                        #endif
                        pthread_mutex_lock(&mutex_interruption);


                        while (interrupting == 1) {

                            pthread_cond_wait(&interruption_cond, &mutex_interruption);
                        }

                        pthread_mutex_unlock(&mutex_interruption);

                        if(terminating == 1){
                            strcpy(team_list[team_index].cars[arrayNumber].current_state,"TERMINADO");
                            sprintf(log,"CAR %02d IS NOW IN STATE: TERMINADO",team_list[team_index].cars[arrayNumber].car_number);
                            writeLog(log, semaphore_list->logMutex, inf_fich->fp);
                            break;
                        }
                        sem_wait(counter_mutex);
                        amount_terminated--;
                        sem_post(counter_mutex);


                    }

                    //when it exits the box the current distance in lap is 0
                    distance_in_lap = 0;

                    //Car is now in racing state
                    strcpy(team_list[team_index].cars[arrayNumber].current_state, "CORRIDA");
                    sprintf(log,"CAR %02d IS NOW IN STATE: CORRIDA",team_list[team_index].cars[arrayNumber].car_number);
                    writeLog(log, semaphore_list->logMutex, inf_fich->fp);


                }

                //If the car is not in security mode but needs to got to the box checks if the box is not reserved/occupied and if this is true, then the car enters the box
                else if(current_fuel < 4 * ((inf_fich->lap_distance / team_list[team_index].cars[arrayNumber].speed)*team_list[team_index].cars[arrayNumber].consumption) && strcmp(team_list[team_index].box_state, "LIVRE") == 0 && team_list[team_index].cars[arrayNumber].number_of_laps < inf_fich->number_of_laps && terminating != 1 && interrupting != 1){

                    //car reserves and immediatly goes into box. is_reserved == 0 means that the car entered without reserving. This will prevent 1 sem_post of the waiting_update semaphore
                    is_reserved = 0;
                    sem_post(reservation);

                    //Index of the car that is entering the box
                    car_index = arrayNumber;

                    //Enters box.
                    sem_post(car_in_box);
                    #ifdef DEBUG
                    printf("(%s) Stopped in box.\n", team_list[team_index].team_name);
                    #endif


                    //Now the logic is the same as the one in lines 466-518
                    strcpy(team_list[team_index].cars[arrayNumber].current_state, "BOX");

                    sprintf(log,"CAR %02d IS NOW IN STATE: BOX",team_list[team_index].cars[arrayNumber].car_number);
                    writeLog(log, semaphore_list->logMutex, inf_fich->fp);

                    current_fuel = inf_fich->fuel_capacity;
                    sem_wait(wait_box);

                    if(terminating == 1){
                        strcpy(team_list[team_index].cars[arrayNumber].current_state,"TERMINADO");
                        sprintf(log,"CAR %02d IS NOW IN STATE: TERMINADO",team_list[team_index].cars[arrayNumber].car_number);
                        writeLog(log, semaphore_list->logMutex, inf_fich->fp);
                        break;
                    }

                    else if( interrupting == 1){
                        #ifdef DEBUG
                        printf("Stopped %s\n", team_list[team_index].team_name);
                        #endif
                        sem_wait(counter_mutex);
                        amount_terminated++;
                        if(amount_terminated == team_list[team_index].number_of_cars){
                            start_teams =  0;
                            sem_post(interrupt_race_mutex);
                        }

                        sem_post(counter_mutex);
                        #ifdef DEBUG
                        printf("(%02d) Interrupted.\n", team_list[team_index].cars[arrayNumber].car_number);
                        #endif
                        pthread_mutex_lock(&mutex_interruption);

                        while (interrupting == 1) {

                            pthread_cond_wait(&interruption_cond, &mutex_interruption);
                        }

                        pthread_mutex_unlock(&mutex_interruption);


                        if(terminating == 1){
                            break;
                        }
                        sem_wait(counter_mutex);
                        amount_terminated--;
                        sem_post(counter_mutex);

                    }

                    distance_in_lap = 0;
                    strcpy(team_list[team_index].cars[arrayNumber].current_state, "CORRIDA");

                    sprintf(log,"CAR %02d IS NOW IN STATE: CORRIDA",team_list[team_index].cars[arrayNumber].car_number);
                    writeLog(log, semaphore_list->logMutex, inf_fich->fp);

                }
                //Means that the car has ended the race. The race will now end for all cars in all teams
                else if(team_list[team_index].cars[arrayNumber].number_of_laps >= inf_fich->number_of_laps){
                    sem_post(update_waiting);

                    //New car state. The car is finished
                    strcpy(team_list[team_index].cars[arrayNumber].current_state, "TERMINADO");

                    sprintf(log,"CAR %02d IS NOW IN STATE: TERMINADO",team_list[team_index].cars[arrayNumber].car_number);
                    writeLog(log, semaphore_list->logMutex, inf_fich->fp);

                    //Sends a message throught the unnamed pipe to the race manager that tells it to tell all other teams that the race has ended
                    strcpy(data.message, "TERMINADO");
                    write(channel[1], &data, sizeof(struct message));

                    sprintf(log,"CAR %02d WON THE RACE",team_list[team_index].cars[arrayNumber].car_number);
                    writeLog(log, semaphore_list->logMutex, inf_fich->fp);

                    //exits while statement
                    break;
                }
                else{
                    sem_post(update_waiting);
                }
            }
        }

	//New car indicates it is waiting for a new iteration
    sem_wait(update_waiting);
    amount_waiting++;
    sem_post(update_waiting);

	int value = 0;

    sem_wait(iteration_mutex);


    }   //while
    //Waits for other cars to end race and announces own exit
    anouncingEnd();
}




//Auxiliar function. Threads will call this function when the are created.
void *carThread(void* team_number){

    int number=*((int *)team_number);

    //Starts the racing of the car
    racing(number);

    //code will never get to here, but in case it does the thread will die
    pthread_exit(0);

}

//Time manager. Will send a signal to all threads that are waiting in the iteration mutex every 1/units_of_time_per_second seconds. So they
//Do another iteration in the code.
void *sleepThread(void* sleep_thread_id){

    //Blocks all signals coming from the outside
    sigprocmask(SIG_BLOCK,&new_mask, NULL);

    //While the race has not ended runs
    while (out==0){

        usleep(1000000/inf_fich->time_units_per_second);

        sem_wait(update_waiting);
        for(int i =0; i<amount_waiting; i++ ){
            sem_post(iteration_mutex);
        }
        amount_waiting = 0;
        sem_post(update_waiting);
    }
    pthread_exit(NULL);


}


//Signals the race manager that the race has started
void raceStart(int signum){

    if(firstStart == 0 ){
        start_teams = 1;
        firstStart = 1;
    }
    else{
        start_teams = 1;
        pthread_cond_broadcast(&interruption_cond);
    }

}





//Team manager. Will create the car threads
void Team_Manager(struct config_fich_struct *inf_fichP, struct team *team_listP, struct semaphoreStruct *semaphore_listP, int channelP[2],int team_indexP, struct ids *idsP){

    //Ignore all unwanted signals!
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    //Creates a mask with the wanted signals
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGUSR1);
    sigaddset(&new_mask, SIGUSR2);
    sigaddset(&new_mask, SIGTERM);
    sigaddset(&new_mask, SIGALRM);


    sigprocmask(SIG_UNBLOCK,&new_mask, NULL);
    signal(SIGUSR2, teamEnd);
    signal(SIGUSR1, pauseRaceTeams);
    signal(SIGTERM, raceStart);
    signal(SIGALRM, everyoneGaveUp);

    //Creates semaphores
    sem_unlink("BOX_MUTEX");
    car_in_box = sem_open("BOX_MUTEX", O_CREAT|O_EXCL,0700,0);
    sem_unlink("RESERVATION_MUTEX");
    reservation = sem_open("RESERVATION_MUTEX", O_CREAT|O_EXCL,0700,0);
    sem_unlink("UPDATE_MUTEX");
    update_waiting = sem_open("UPDATE_MUTEX", O_CREAT|O_EXCL,0700,1);
    sem_unlink("END COUNTER");
    counter_mutex = sem_open("END COUNTER", O_CREAT|O_EXCL,0700,1);
    sem_unlink("WAIT BOX");
    wait_box = sem_open("WAIT BOX", O_CREAT|O_EXCL,0700,0);
    sem_unlink("INTERRUPT MUTEX");
    interrupt_race_mutex = sem_open("INTERRUPT MUTEX", O_CREAT|O_EXCL,0700,0);
    sem_unlink("ITERATION MUTEX");
    iteration_mutex = sem_open("ITERATION MUTEX", O_CREAT|O_EXCL,0700,0);
    #ifdef DEBUG
    printf("Team Manager created (%ld)\n", (long)getpid());
    #endif

    //Initializes global variables
    channel = channelP;
    inf_fich = inf_fichP;
    team_list = team_listP;
    semaphore_list = semaphore_listP;
    team_index = team_indexP;
    idsT = idsP;

    //waits for race to start
    while(start_teams == 0){
        pause();
    }

    //blocks any signals while creating threads
    sigprocmask(SIG_BLOCK,&new_mask, NULL);

    //Thread identifiers
    number_of_cars = team_list[team_index].number_of_cars;
    int workerId[number_of_cars];
    cars = malloc(sizeof(pthread_t) * number_of_cars);


    //Create the car threads
    for(int i=0; i<number_of_cars;i++){
        workerId[i] = i;
        pthread_create(&cars[i], NULL, carThread,&workerId[i]);
    }
    //Creates timemanager
    pthread_create(&sleep_thread,NULL, sleepThread, &sleep_thread_id);


    //unblocks signals
    sigprocmask(SIG_UNBLOCK,&new_mask, NULL);

    //Box. TeamManager waits for notifications of cars entering/reserving the box
    while(1){

        //Waits for a car to make a reservation
        sem_wait(reservation);

        sigprocmask(SIG_BLOCK,&new_mask, NULL);

        //Team updates the current box state and next waits for a car to enter the box
        strcpy(team_list[team_index].box_state, "RESERVADO");

        //This is to prevent 2 post update_waiting due to the fact that when there is no reservation a car with less than 4 * the amount of fuel to complete a lap can go into the box without a reservation
        if(is_reserved == 1) sem_post(update_waiting);
        sigprocmask(SIG_UNBLOCK,&new_mask, NULL);

        //Waits for a car to enter the box
        sem_wait(car_in_box);

        sigprocmask(SIG_BLOCK,&new_mask, NULL);

        had_interruption = 0;
        is_in_box = 1;

        strcpy(team_list[team_index].box_state, "OCUPADO");

        //let cars go past the point of update
        sem_post(update_waiting);
        sigprocmask(SIG_UNBLOCK,&new_mask, NULL);

        //if the car has a breakdown wait a randow time for it to be repared
        if(team_list[team_index].cars[car_index].has_breakdown == 1){

            //rand() (upper - lower + 1)) + lower;
            //Sleep for a random amount of time
            int amount_of_time = rand() % (inf_fich->T_Box_Max - inf_fich->T_Box_Min+1) + inf_fich->T_Box_Min;
            int time_passed = 0;
            #ifdef DEBUG
            printf("Car %02d will do %d time units in box.\n",team_list[team_index].cars[car_index].car_number,amount_of_time);
            #endif
            while(time_passed < amount_of_time){
					//Waits for time manager
                sem_wait(update_waiting);
                amount_waiting++;
                sem_post(update_waiting);

                sem_wait(iteration_mutex);

                time_passed++;
                if(had_interruption == 1)break;
            }

        }
        //refills car. Waits 2 time units
        for(int i = 0; i< 2; i++){
            if(had_interruption == 1)break;

            //pthread_cond_wait(&sleep_cond, &sleep_mutex);
            //printf("Recebi permissão %02d\n",team_list[team_index].cars[arrayNumber].car_number);
            //pthread_mutex_unlock(&sleep_mutex);

            sem_wait(update_waiting);
            amount_waiting++;
            sem_post(update_waiting);

            sem_wait(iteration_mutex);

        }
        team_list[team_index].cars[car_index].has_breakdown = 0;
        team_list[team_index].cars[car_index].times_refill++;
        #ifdef DEBUG
        printf("(%d) Left box.\n",  team_list[team_index].cars[car_index].car_number);
        #endif

        //Car exits box
        sigprocmask(SIG_BLOCK,&new_mask, NULL);
        is_reserved = 0;
        if(had_interruption != 1) sem_post(wait_box);


        //new box state
        sem_wait(update_waiting);

        strcpy(team_list[team_index].box_state, "LIVRE");

        sem_post(update_waiting);
        is_in_box = 0;
        sigprocmask(SIG_UNBLOCK,&new_mask, NULL);

    }

    return;
}
