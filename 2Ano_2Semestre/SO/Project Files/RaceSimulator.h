//Edgar Filipe Ferreira Duarte 2019216077
//Pedro Guilherme da Cruz Ferreira 2018277677
#define SIZE 50
#define DEBUG
#define PIPE_NAME "pipe"
#define MAX 550

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


struct config_fich_struct{
    int time_units_per_second;
    int lap_distance;
    int number_of_laps;
    int number_of_teams;
    int number_of_cars;
    int T_Avaria;
    int T_Box_Min;
    int T_Box_Max;
    int fuel_capacity;
    FILE *fp;
};

struct ids{
    pid_t pid_breakdown;
    int msg_queue_id;
};

struct message{
    int team_index;
    int car_index;
    char message[MAX];
};

struct  messageQ{
    long mtype;
    int response;
};

// has_breakdown : 0 == no breakdown , 1 == breakdown
// current_state : CORRIDA, SEGURANCA, DESISTENCIA, BOX, TERMINADO
struct car{
    int car_number;
    int speed;
    float consumption;
    int reliability;
    int number_of_laps;
    int amount_breakdown;
    int times_refill;
    int has_breakdown;
    char current_state[SIZE];
};



struct team{
    char team_name[SIZE];
    //LIVRE, OCUPADO, RESERVADO
    char box_state[SIZE];
    int number_of_cars;
    struct car *cars;
};


struct semaphoreStruct{
    sem_t *logMutex;
    sem_t *writingMutex;
    sem_t *readingMutex;
};
