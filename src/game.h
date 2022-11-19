#pragma once

#include <stdbool.h>

extern bool isExit; 
extern bool isRunning;

void get_msg(const char *need, char *msg);

void print_title();

void prompt_server_list(struct sockaddr_in **servers, int nbserv, char *buffer_send);

char** prompt_party_list(struct clientTCP *cltTCP, int *nbparty, char *buffer_recv, char *buffer_send, int bufsize);

void launch_game(struct clientTCP *cltTCP, char *buffer_send, char *buffer_recv, int bufsize);