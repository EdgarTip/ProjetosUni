#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

#define PIPE_NAME "pipe"

int main(){
    int named_pipe_fd;
    if ((named_pipe_fd = open(PIPE_NAME, O_WRONLY)) < 0) {
        perror("Cannot open pipe for writing: ");
        exit(0);
    }

    char readed[500];
    char toSend[512];
    char c;
    while(1){
        fflush(stdin);

        memset(readed, '\0', sizeof(readed));
        memset(toSend, '\0', sizeof(toSend));

        if((c=getchar())!='\n'){
            toSend[0]=c;
            scanf("%[^\n]%*c", readed);
            strcat(toSend,readed);

            #ifdef DEBUG
            printf("[RaceSimulator] (NP) Sending: %s\n",toSend);
            #endif

            write(named_pipe_fd, toSend, sizeof(toSend));
        }
    }
}
