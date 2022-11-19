#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#include "clientbrc.h"
#include "error.h"

void client_send_broadcast(struct clientbrc *this, char *msg){
    if(sendto(this->sock, msg, strlen(msg), 0, (struct sockaddr*) &this->addr, this->len) == ERR){
        syserror(SEND_ERROR);
    }
}

ssize_t client_receive_broadcast(struct clientbrc *this, struct sockaddr_in *server, char *buf, size_t size){
    if(!buf){
        return 0;
    }

    return recvfrom(this->sock, buf, size, MSG_DONTWAIT, (struct sockaddr *) server, &this->len);
}

struct sockaddr_in ** client_receive_servers(struct clientbrc *this, const char *answer, char *buffer_recv, int bufsize, int *nbserv){
    time_t timer, endtimer, pointtimer;;
    struct sockaddr_in *server, **servers = (struct sockaddr_in**) malloc(sizeof(struct sockaddr_in*));
    int ret, size, nbpoint;

    server = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
    *nbserv = 0;

    size = strlen(answer);

    timer = time(NULL);
    endtimer = timer + 4;

    system("clear");
    printf("Researching pokétudiant server\n");
    pointtimer = timer + 1;
    nbpoint = 0;

    while (timer < endtimer)
    {
        if(timer >= pointtimer){
            system("clear");
            nbpoint++;
            switch (nbpoint % 4)
            {
            case 1:
                printf("Researching pokétudiant server .\n");
                break;
            
            case 2:
                printf("Researching pokétudiant server . .\n");
                break;

            default:
                printf("Researching pokétudiant server . . .\n");
                break;
            }
            pointtimer = timer + 1;
        }

        ret = this->client_receive_broadcast(this, server, buffer_recv, bufsize);
        if(ret != -1 && ret){
            if(!strncmp(buffer_recv, answer, size)){
                if(*nbserv != 0){
                    servers = (struct sockaddr_in**) realloc(servers, (*nbserv+1) * sizeof(struct sockaddr_in*));
                }
                servers[*nbserv] = server;
                *nbserv+=1;
                server = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
            }
        }
        timer = time(NULL);
    }

    free(server);
    return servers;
}

Clientbrc client_create_udp( char *addr, int port){
    int sock;
    Clientbrc res = (struct clientbrc *) malloc(sizeof(struct clientbrc));

    sock = socket( AF_INET, SOCK_DGRAM, 0);
    if(sock == ERR){
        syserror(SOCKET_ERROR);
    }

    res->sock = sock;
    res->addr.sin_family = AF_INET;
    res->addr.sin_port = htons((uint16_t) port);

    if (!inet_aton(addr, &res->addr.sin_addr) == ERR)
    {
        clientbrc_close_and_free(res);
        syserror(SOCKET_ERROR);
    }

    res->len = sizeof(struct sockaddr_in);

    res->client_receive_broadcast = &client_receive_broadcast;
    res->client_send_broadcast = &client_send_broadcast;
    res->client_receive_servers = &client_receive_servers;

    return res;
}

Clientbrc client_create_broadcast(int port){
    Clientbrc res = client_create_udp("255.255.255.255", port);
    int broadcastEnable = 1;

    if(setsockopt(res->sock, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable))){
        clientbrc_close_and_free(res);
        syserror(SOCKET_ERROR);
    }

    return res;
}

void clientbrc_close_and_free(struct clientbrc *this){
    if(close(this->sock) == ERR){
        syserror(SOCKET_CLOSE_ERROR);
    }

    free(this);
}