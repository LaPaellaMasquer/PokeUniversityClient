#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#define SIZE 500

void *thmsg(void *nothing){
    char buf[SIZE], *msg;
    int fd, size;

    if((fd = open("OUT_Tchat.fifo", O_WRONLY)) == -1){
        perror("can't opent Team.fifo please launch the client before");
        exit(1);
    }

    for(;;){
        fgets(buf, SIZE, stdin);
        msg = (char *) malloc(13+strlen(buf)+1* sizeof(char));
        strcpy(msg, "send message ");
        strcat(msg, buf);
        write(fd, msg, strlen(msg));
        free(msg);
    }
}

int main(int argc, char const *argv[])
{
    char buffer[SIZE], name[50], msg[200];
    int fd;
    pthread_t threadMsg;

    if((fd = open("IN_Tchat.fifo", O_RDONLY)) == -1){
        perror("can't opent Tchat.fifo please launch the client before");
        exit(1);
    }

    pthread_create(&threadMsg, NULL, thmsg, NULL);

    printf("Tchat\n");
    while(read(fd, buffer, SIZE))
    {
        if(!strncmp(buffer, "exit", 4)){
            break;
        }
        sscanf(buffer, "rival message %s %[^\n]", name, msg);
        msg[strlen(msg)] = '\0';
        printf("%s say's: %s\n", name, msg);
    }

    close(fd);

    return 0;
}
