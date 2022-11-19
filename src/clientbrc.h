#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


struct clientbrc
{
    int sock;
    struct sockaddr_in addr;
    socklen_t len;

    ssize_t (*client_receive_broadcast)(struct clientbrc *this, struct sockaddr_in *server, char *buf, size_t size);
    struct sockaddr_in ** (*client_receive_servers)(struct clientbrc *this, const char *answer, char *buffer_recv, int bufsize, int *nbservers);
    void (*client_send_broadcast)(struct clientbrc *this, char *msg);
};

typedef struct clientbrc *Clientbrc;

Clientbrc client_create_broadcast(int port);

void clientbrc_close_and_free(struct clientbrc *this);
