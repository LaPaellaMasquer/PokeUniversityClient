#pragma once
#include <stdio.h>

#define ERR -1 // les fonctions retournent -1 en cas d'erreurs
#define NO_ERROR 0
#define SOCKET_ERROR 1
#define BIND_ERROR 2
#define SOCKET_CLOSE_ERROR 3
#define SEND_ERROR 4


#define syserror(n) perror(msgErreur[n]), exit(n)

extern char * msgErreur[];
