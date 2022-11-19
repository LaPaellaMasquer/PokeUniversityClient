#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "clientTCP.h"
#include "error.h"

void client_send_tcp(struct clientTCP *this, char *msg){
    if(send(this->sock, msg, strlen(msg), 0) == ERR){
        syserror(SEND_ERROR);
    }
}

ssize_t client_receive_tcp(struct clientTCP *this, char *buf, size_t size){
    if(!buf){
        return 0;
    }

    return recv(this->sock, buf, size, 0);
}

ClientTCP client_create_tcp(char *addr, int port){
    int sock;
    ClientTCP res = (struct clientTCP *) malloc(sizeof(struct clientTCP));

    sock = socket( AF_INET, SOCK_STREAM, 0);
    if(sock == ERR){
        syserror(SOCKET_ERROR);
    }

    res->sock = sock;
    res->addr.sin_family = AF_INET;
    res->addr.sin_port = htons((uint16_t) port);

    if (!inet_aton(addr, &res->addr.sin_addr) == ERR)
    {
        clientTCP_close_and_free(res);
        syserror(SOCKET_ERROR);
    }

    res->len = sizeof(struct sockaddr_in);

    res->client_send_tcp = &client_send_tcp;
    res->client_receive_tcp = &client_receive_tcp;

    return res;
}

void clientTCP_close_and_free(struct clientTCP *this){
    if(close(this->sock) == ERR){
        syserror(SOCKET_CLOSE_ERROR);
    }

    free(this);
}