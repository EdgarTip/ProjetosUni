//Edgar Filipe Ferreira Duarte 2019216077
//Pedro Guilherme da Cruz Ferreira 2018277677

//TO RUN THE CODE AFTER COMPILING IT USE THE FORMAT : ./exec {configuration file name}

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/stat.h>
#include <assert.h>
#include <sys/msg.h>


#include "RaceSimulator.h"
#include "RaceManager.h"
#include "ReadConfig.h"
#include "MultipleProcessActions.h"
#include "BreakDownManager.h"
/*Struct that retains the information given by the initial file
Not a shared memory struct but the values will be given to the
processes created in this script*/
struct config_fich_struct *inf_fich;


/*Shared memory struct the will share memory about the current state of all
of the cars. Will be updated by the Race Manager
*/
struct team *team_list;

/*List of semaphores used in multiple functions*/
struct semaphoreStruct *semaphore_list;

/*Struct with the id of BreakDownManager and message queue id
Usefull to pass these idś to other functions insted of passing separated */
struct ids *idsP;

int shmid;
int named_pipe_fd;
pid_t pids[2];  //{BreakDownManager pid, RaceManager pid}
int race_started = 0 ;

//cleans active memory
void clean(){
    //Unmap shared memory from the struct
    shmdt(team_list);
    //Remove message queue
    msgctl(idsP->msg_queue_id,IPC_RMID,NULL);
    //Remove shared memory
    shmctl(shmid, IPC_RMID, NULL);
    //Close semaphores
    sem_close(semaphore_list->logMutex);
    sem_close(semaphore_list->writingMutex);
    sem_close(semaphore_list->readingMutex);
    //Unlink semaphores names
    sem_unlink("MUTEX");
    sem_unlink("WRITING_MUTEX");
    sem_unlink("READING_MUTEX");
    //Free structs
    free(inf_fich);
    free(semaphore_list);
    free(idsP);


    char commandReturn[10];
    /*Opens a pipe stream to execute the command "pidof commands" in read mode.
    It returns the pid of the process with name "command" that is the process responsible for writing commands to the named pipe */
    FILE * command = popen("pidof commands","r");   //executes command "pidof commands" in read mode.

    //Gets the return of the previous command into a string
    fgets(commandReturn,10,command);

    //Converts the string into a pid_t (unsigned long int) ; 0 if no valid conversion
    pid_t pid = strtoul(commandReturn,NULL,10);
    if(pid!=0)
        kill(pid, SIGINT);  //Sends signal to stop the commands process
    pclose(command);    //Close the pipe stream

    //Close and unlink named pipe
    close(named_pipe_fd);
    unlink(PIPE_NAME);
}


//Function called when the race ends
void endRaceSim(int signum){
    pid_t wpid;
    int status = 0;

    while ((wpid = wait(&status)) > 0); //Waits for children to die: BreakDownManager and RaceManager

    if (race_started == 1) {
        readStatistics(inf_fich, team_list, semaphore_list);    //Prints statistics
    }

    clean();    //Cleans memory before exiting
    exit(0);    //Exits the main process
}

//Function called when SIGINT is received
void sigint(int signum){
    printf("\n");
    writeLog("SIGNAL SIGINT RECEIVED", semaphore_list->logMutex, inf_fich->fp);
    writeLog("SIMULATOR CLOSING", semaphore_list->logMutex, inf_fich->fp);

    signal(SIGINT, SIG_IGN);    //Ignore new SIGINT signals

    kill(pids[0],SIGUSR2);  //Sends signal for BreakDownManager to end
    kill(pids[1],SIGUSR2);  //Sends signal for RaceManager to end

    endRaceSim(0);
}

//Function called when SIGTSTP is received
void sigtstp(int signum){
    printf("\n");
    writeLog("SIGNAL SIGTSTP RECEIVED", semaphore_list->logMutex, inf_fich->fp);
    signal(SIGTSTP, SIG_IGN);   //Ignore new SIGTSTP signals
    if(race_started == 1) readStatistics(inf_fich, team_list, semaphore_list);  //print statistics to screen
    signal(SIGTSTP, sigtstp);   //Re-activates signal

}

//Function called when SIGTERM is received
void raceStartRaceSim(int signum){
    race_started =1;
}



//Main function. Here the RaceManager and the MalfunctionManager processes will be created
int main(int argc, char* argv[]){

	if (argc!=2){
      printf("Invalid number of arguments!\nUse as: executable {name of the configurations file}\n");
      exit(1);
    }

    sigset_t mask, new_mask;

    //Ignore all unwanted signals!
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    //Create the wanted signals set
    sigemptyset(&new_mask);
    sigaddset(&new_mask,SIGINT);
    sigaddset(&new_mask,SIGTSTP);
    sigaddset(&new_mask,SIGTERM);
    sigaddset(&new_mask, SIGUSR2);

    //Activates set created
    sigprocmask(SIG_UNBLOCK,&new_mask, NULL);

    //Routes signals for their respective functions
    signal(SIGINT, sigint);
    signal(SIGTSTP, sigtstp);
    signal(SIGUSR2, endRaceSim);
    signal(SIGTERM, raceStartRaceSim);

    //Initialize named semaphores
    semaphore_list = (struct semaphoreStruct*) malloc(sizeof(struct semaphoreStruct));
    sem_unlink("MUTEX");
    semaphore_list->logMutex = sem_open("MUTEX", O_CREAT|O_EXCL,0700,1);
    sem_unlink("WRITING_MUTEX");
    semaphore_list->writingMutex = sem_open("WRITING_MUTEX", O_CREAT|O_EXCL,0700,1);
    sem_unlink("READING_MUTEX");
    semaphore_list->readingMutex = sem_open("READING_MUTEX", O_CREAT|O_EXCL,0700,1);


    //Read the configuration file
    char *file_name = argv[1];
    inf_fich=readConfigFile(file_name);

    sem_post(semaphore_list->logMutex);

    #ifdef DEBUG
    printf("Creating shared memory\n");
    #endif

    //Creates shared memory
    shmid = shmget(IPC_PRIVATE, inf_fich->number_of_teams * (sizeof(struct team*) + sizeof(struct car*) * inf_fich->number_of_cars), IPC_CREAT|0700);
    if (shmid < 1) exit(0);
    team_list = (struct team*) shmat(shmid, NULL, 0);
    if (team_list < (struct team*) 1) exit(0);

    for(int i = 0; i <= inf_fich->number_of_teams ; i++){
        team_list[i].cars = (struct car*)(team_list + inf_fich->number_of_teams + 1 + i * inf_fich->number_of_cars);
    }

    writeLog("SIMULATOR STARTING", semaphore_list->logMutex, inf_fich->fp);

    idsP = (struct ids*) malloc(sizeof(struct ids));

    //Creates the message queue
    assert( (idsP->msg_queue_id= msgget(IPC_PRIVATE, IPC_CREAT|0700)) != -1 );

    //Creates named pipe
    unlink(PIPE_NAME);
    #ifdef DEBUG
    printf("Creating named pipe.\n");
    #endif
    if ((mkfifo(PIPE_NAME, O_CREAT|O_EXCL|0600)<0) && (errno!= EEXIST)) {
        perror("Cannot create pipe: ");
        exit(0);
    }
    #ifdef DEBUG
    printf("Named pipe created.\n");
    #endif


    //Creates Breakdown Manager process
    if((pids[0] = fork()) == 0){
        idsP->pid_breakdown = getpid();

        BreakDownManager(inf_fich, team_list, semaphore_list, idsP);
    }

    idsP->pid_breakdown = pids[0];

    //Creates Race Manager process
    if( (pids[1] = fork()) == 0){
        Race_Manager(inf_fich, team_list, semaphore_list,  idsP);
    }

    while(1){
        pause();
    }



    return 0;

}
