#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


struct clientTCP
{
    int sock;
    struct sockaddr_in addr;
    socklen_t len;

    ssize_t (*client_receive_tcp)(struct clientTCP *this, char *buf, size_t size);
    void (*client_send_tcp)(struct clientTCP *this, char *msg);
};

typedef struct clientTCP *ClientTCP;

ClientTCP client_create_tcp(char *addr, int port);

void clientTCP_close_and_free(struct clientTCP *this);