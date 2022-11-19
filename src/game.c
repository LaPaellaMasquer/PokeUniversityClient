#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/select.h>

#include "clientTCP.h"
#include "poketudiant.h"
#include "color.h"

// private global var for threads
bool isExit;
bool isRunning;
// player pos
int x, y, nbrow, nbcol;

/**
 * get player input
 */
void get_msg(const char *need, char *msg){
    printf("%s \n", need);
    fgets(msg, 500, stdin);
}

/**
 * print game title
 */
void print_title()
{
    printf("\n  %s\n", color_text(GREEN, BLACK,
    "                            ███╗                                                                                 "));
    printf("  %s\n", color_text(GREEN, BLACK,
    "                          ███╔═╝                                                                                 "));
    printf("  %s\n", color_text(GREEN, BLACK,
    "                          ╚══╝                                                                                   "));
    printf("  %s\n", color_text(GREEN, BLACK,
    "██████╗  ██████╗ ██╗  ██╗███████╗    ██╗   ██╗███╗   ██╗██╗██╗   ██╗███████╗██████╗ ███████╗██╗████████╗██╗   ██╗"));
    printf("  %s\n", color_text(GREEN, BLACK,
    "██╔══██╗██╔═══██╗██║ ██╔╝██╔════╝    ██║   ██║████╗  ██║██║██║   ██║██╔════╝██╔══██╗██╔════╝██║╚══██╔══╝╚██╗ ██╔╝"));
    printf("  %s\n", color_text(GREEN, BLACK,
    "██████╔╝██║   ██║█████╔╝ █████╗█████╗██║   ██║██╔██╗ ██║██║██║   ██║█████╗  ██████╔╝███████╗██║   ██║    ╚████╔╝ "));
    printf("  %s\n", color_text(GREEN, BLACK,
    "██╔═══╝ ██║   ██║██╔═██╗ ██╔══╝╚════╝██║   ██║██║╚██╗██║██║╚██╗ ██╔╝██╔══╝  ██╔══██╗╚════██║██║   ██║     ╚██╔╝  "));
    printf("  %s\n", color_text(GREEN, BLACK,
    "██║     ╚██████╔╝██║  ██╗███████╗    ╚██████╔╝██║ ╚████║██║ ╚████╔╝ ███████╗██║  ██║███████║██║   ██║      ██║   "));
    printf("  %s\n\n", color_text(GREEN, BLACK,
    "╚═╝      ╚═════╝ ╚═╝  ╚═╝╚══════╝     ╚═════╝ ╚═╝  ╚═══╝╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝╚══════╝╚═╝   ╚═╝      ╚═╝   "));
}

/**
 * show the server list and let the player select one
 */
void prompt_server_list(struct sockaddr_in **servers, int nbserv, char *buffer_send){
    int iserv =-1;
    do{
        system("clear");
        print_title();
        for (int i = 0; i < nbserv; i++)
        {
            printf("Server %d: %s\n", i, inet_ntoa(servers[i]->sin_addr));
        }

        // choose the server
        printf("\n%s Exit the game ", color_text(BLACK, LIGHT_GRAY, "[exit]"));
        printf("%s Refresh server list ", color_text(BLACK, LIGHT_GRAY, "[refresh]"));
        get_msg(strcat(color_text(BLACK, LIGHT_GRAY, "[0...]"), " Choose server"), buffer_send);
        iserv = atoi(buffer_send);
        if(!iserv && buffer_send[0] != '0')
        {
            iserv = -1;
        }
    }while(strncmp(buffer_send, "exit", 4) && strncmp(buffer_send, "refresh", 7) && (iserv < 0 || iserv >= nbserv));
}

/**
 * demand to the server and show the party list
 */
char ** prompt_party_list(struct clientTCP *cltTCP, int *nbparty, char *buffer_recv, char *buffer_send, int bufsize){
    int iparty, ibuff, nbplayer, size;
    char *temp, pname[25], **lparty;

    lparty = NULL;
    do{
        system("clear");
        print_title();
        cltTCP->client_send_tcp(cltTCP, "require game list\n");
        
        ibuff = 0;
        do
        {
            read(cltTCP->sock, &buffer_recv[ibuff], sizeof(char));
            ibuff++;
        }while (buffer_recv[ibuff-1]!='\n');
        buffer_recv[ibuff] = '\0';

        sscanf(buffer_recv, "number of games %d\n", nbparty);

        if(lparty!=NULL){
            for (int i = 0; i < *nbparty; i++)
            {
                free(lparty[i]);
            }
            
            free(lparty);
        }
        lparty = (char **) malloc(*nbparty * sizeof(char *));

        if(*nbparty != 0){
            size = cltTCP->client_receive_tcp(cltTCP, buffer_recv, bufsize);
            buffer_recv[size]='\0';

            for (int i = 0; i < *nbparty; i++)
            {
                if(!i)
                {
                    temp = strtok(buffer_recv, "\n");
                }
                else
                {
                    temp = strtok(NULL, "\n");
                }
                printf("party %d: %s\n", i, temp);
                sscanf(temp, "%d %[^\n]", &nbplayer, pname);
                lparty[i] = (char *) malloc((strlen(pname)+2)*sizeof(char));
                strcpy(lparty[i], pname);
                lparty[i][strlen(pname)] = '\n';
                lparty[i][strlen(pname)+1] = '\0';
            }

            printf("\n%s Return to servers list ", color_text(BLACK, LIGHT_GRAY, "[exit]"));
            printf("%s Refresh the party list ", color_text(BLACK, LIGHT_GRAY, "[refresh]")); // it could be everything because I've make the choice to don't stock the nbplayer
            printf("%s Create new party ", color_text(BLACK, LIGHT_GRAY, "[create]"));
            get_msg(strcat(color_text(BLACK, LIGHT_GRAY, "[0...]"), " Choose a party"), buffer_send);
            iparty = atoi(buffer_send);

            if(!iparty && buffer_send[0] != '0')
            {
                iparty = -1;
            }
        }
        else{
            printf("no parties found\n");
            printf("\n%s Return to servers list ", color_text(BLACK, LIGHT_GRAY, "[exit]"));
            printf("%s Refresh the party list ", color_text(BLACK, LIGHT_GRAY, "[refresh]"));
            get_msg(strcat(color_text(BLACK, LIGHT_GRAY, "[create]"), " Create new party"), buffer_send);
            iparty = -1;
        }
    }while (strncmp(buffer_send, "exit", 4) && strncmp(buffer_send, "create", 4) && (iparty < 0 || iparty >= *nbparty));

    return lparty;
}

/**
 * show only where the plaer is
 */
void show_player(char **map){
    bool isfound;
    int xMax, xMin, yMax, yMin;
    for (int i = 0; i < nbrow; i++)
    {
        for (int j = 0; j < nbcol; j++)
        {
            if(isfound = (map[i][j] == '0')){
                x=j;
                y=i;
                break;
            }
        }
        if(isfound){
            break;
        }
    }

    if(nbcol < 20){
        xMax = nbcol;
        xMin = 0;
    }
    else if(x < 20)
    {
        xMax = 20;
        xMin = 0;
    }
    else if(x > nbcol-20)
    {
        xMax = nbcol;
        xMin = nbcol-20;
    }
    else
    {
        xMax = x+10;
        xMin = x-10;
    }
    
    
    if(nbrow < 20){
        yMax = nbrow;
        yMin = 0;
    }
    else if(y < 20)
    {
        yMax = 20;
        yMin = 0;
    }
    else if(y > nbrow-20)
    {
        yMax = nbrow;
        yMin = nbrow-20;
    }
    else
    {
        yMax = y+10;
        yMin = y-10;
    }

    for (int i = yMin; i < yMax; i++)
    {
        // center the map
        printf("%39s", " ");
        
        for (int j = xMin; j < xMax; j++)
        {
            switch (map[i][j])
            {
            case ' ':
                printf("%s", color_text(GREEN, LIGHT_GREEN, "🌿"));
                break;
            
            case '*':
                printf("%s", color_text(GREEN, LIGHT_GREEN, "🥦"));
                break;
            
            case '0':
                printf("%s", color_text(GREEN, GREEN, "🧒"));
                break;
            
            case '+':
                printf("%s", color_text(GREEN, GREEN, "🏥"));
                break;
            
            default:
                printf("%s", color_text(GREEN, GREEN, "👱"));
                break;
            }
        }
        printf("\n");
    }
}

/**
 * update the world map and show it
 */
void update_map(const char *bufmap){
    char *temp, **map;
    
    temp = (char *) malloc((nbrow * (nbcol+1)+1) * sizeof(char));
    strcpy(temp, bufmap);
    map = (char **) malloc(nbrow * sizeof(char*));
    for (int i = 0; i < nbrow; i++)
    {
        map[i] = (char *) malloc(nbcol * sizeof(char));
        if (!i)
        {
            map[i] = strtok(temp, "\n");
        }
        else
        {
            map[i] = strtok(NULL, "\n");
        }
    }
    system("clear");
    print_title();
    show_player(map);
    
    printf("\n%s Return to server list ", color_text(BLACK, LIGHT_GRAY, "[exit]"));
    printf("%s Go up ", color_text(BLACK, LIGHT_GRAY, "[z]"));
    printf("%s Go down ", color_text(BLACK, LIGHT_GRAY, "[s]"));
    printf("%s Go left ", color_text(BLACK, LIGHT_GRAY, "[q]"));
    printf("%s Go right\n", color_text(BLACK, LIGHT_GRAY, "[d]"));

    free(temp);
    free(map);
}

/**
 * get the movement of the player and send it to the sever
 */ 
void *thsend_move(void *clt){
    ClientTCP cltTCP = (struct clientTCP *)clt;
    char buffer_send[500];

    fgets(buffer_send, 500, stdin);
    isExit = !strncmp(buffer_send, "exit", 4);

    if (!isExit)
    {
        switch (buffer_send[0])
        {
        case 'z':
            if(y-1 >= 0)
            {
                cltTCP->client_send_tcp(cltTCP, "map move up\n");
            }
            else
            {
                printf("can't go here\n");
                thsend_move(clt);
            }
            break;

        case 's':
            if(y+1 < nbrow)
            {
                cltTCP->client_send_tcp(cltTCP, "map move down\n");
            }
            else
            {
                printf("can't go here\n");
                thsend_move(clt);
            }
            break;

        case 'q':
            if(x-1 >= 0)
            {
                cltTCP->client_send_tcp(cltTCP, "map move left\n");
            }
            else
            {
                printf("can't go here\n");
                thsend_move(clt);
            }
            break;

        case 'd':
            if(x+1 < nbcol)
            {
                cltTCP->client_send_tcp(cltTCP, "map move right\n");
            }
            else
            {
                printf("can't go here\n");
                thsend_move(clt);
            }
            break;

        default:
            thsend_move(clt);
            break;
        }
    }
    
    isRunning = false;
    return NULL;
}

/**
 * get the player actions on his team by team interface and send them to the server
 */
void *thout_Team(void *clt){
    char buf[500];
    int fd, size, ibuff;
    ClientTCP cltTcp;

    fd = open("OUT_Team.fifo", O_RDONLY);
    cltTcp = (struct clientTCP *) clt;

    for (;;)
    {
        ibuff = 0;
        do
        {
            if(!(size = read(fd, &buf[ibuff], sizeof(char))))
            {
                break;
            }

            ibuff++;
        }while (buf[ibuff-1]!='\n');

        if(!size)
        {
            break;
        }

        buf[ibuff]='\0';

        cltTcp->client_send_tcp(cltTcp, buf);
    }

    close(fd);
    return NULL;
}

/**
 * thread to get all the player message give in tchat interface to send them to the server
*/
void *thout_Tchat(void *clt){
    char buf[500];
    int fd, size, ibuff;
    ClientTCP cltTcp;

    fd = open("OUT_Tchat.fifo", O_RDONLY);
    cltTcp = (struct clientTCP *) clt;

    for (;;)
    {
        ibuff = 0;
        do
        {
            if(!(size = read(fd, &buf[ibuff], sizeof(char))))
            {
                break;
            }

            ibuff++;
        }while (buf[ibuff-1]!='\n');

        if(!size)
        {
            break;
        }
        buf[ibuff]='\0';

        cltTcp->client_send_tcp(cltTcp, buf);
    }

    close(fd);
    return NULL;
}

/**
 * manage manage the communication between the server, the map update, start fight and the tchat and team interfaces in the fight
 */
void start_fight(struct clientTCP *cltTCP, char *buffer_recv, char *buffer_send, int bufsize, char *bufmap, int *nbpoke, int fifoTeam, int fifoTchat){
    bool isrival;
    int ibuff, ipoke, pokesel, nbopp, size;
    char msg[bufsize], temp;
    t_poketudiant poke, opp;
    float pokeLife, oppLife;

    bufmap = NULL;

    // get nmber of pokédutiant opponent
    printf("[combat] %s", buffer_recv);
    if(isrival = !strncmp(buffer_recv, "encounter new rival", 18))
    {
        sscanf(buffer_recv, "encounter new rival %d\n", &nbopp);
    }
    else
    {
        sscanf(buffer_recv, "encounter new wild %d\n", &nbopp);
    }

    do
    {
        // read socket line
        ibuff = 0;
        do
        {
            read(cltTCP->sock, &buffer_recv[ibuff], sizeof(char));
            ibuff++;
        }while(buffer_recv[ibuff-1]!='\n');
        buffer_recv[ibuff]='\0';

        if(!strncmp(buffer_recv, "map", 3))
        {
            // get the map to update it at the end of the match
            sscanf(buffer_recv, "map %d %d\n", &nbrow, &nbcol);
            free(bufmap);
            bufmap = (char *) malloc((nbrow * (nbcol+1)+1) * sizeof(char));
            size = read(cltTCP->sock, bufmap, nbrow * (nbcol+1));
            bufmap[size] = '\0';
        }
        else if(!strncmp(buffer_recv, "team contains", 13))
        {
            // update team interface information
            sscanf(buffer_recv, "team contains %d\n", nbpoke);
            ipoke = 0;
            do
            {
                read(cltTCP->sock, &buffer_recv[ibuff], sizeof(char));
                if(buffer_recv[ibuff] == '\n')
                {
                    ipoke++;
                }
                ibuff++;
            }while (ipoke < *nbpoke);

            buffer_recv[ibuff] = '\0';
            write(fifoTeam, buffer_recv, strlen(buffer_recv));
        }
        else if(!strncmp(buffer_recv, "rival message", 13))
        {
            // send message to tchat interface
            write(fifoTchat, buffer_recv, strlen(buffer_recv));
        }
        else if(!strncmp(buffer_recv, "encounter poketudiant player", 28))
        {
            // show the player fighter status
            system("clear");
            print_title();
            sscanf(buffer_recv, "encounter poketudiant player %s %d %f %s %s %s %s\n", 
            poke.variety, &poke.lvl, &pokeLife, poke.attack1, poke.attack1type, poke.attack2, poke.attack2type);

            printf("\n 🧒 Your Pokétudiant: ");
            for (int i = 0; i < *nbpoke; i++)
            {
                printf("🎒 ");
            }

            printf("\n │ %s %s lvl: %d  ❤️  %f\n", get_pokemoji(poke), poke.variety, poke.lvl, pokeLife);
            printf(" │ attack 1: %s type: %s\n", poke.attack1, poke.attack1type);
            printf(" │ attack 2: %s type: %s\n", poke.attack2, poke.attack2type);
        }
        else if(!strncmp(buffer_recv, "encounter poketudiant opponent", 30))
        {
            // show the opponent fighter status
            sscanf(buffer_recv, "encounter poketudiant opponent %s %d %f\n", opp.variety, &opp.lvl, &oppLife);

            if(isrival)
            {
                printf("\n 👱 Opponent Pokétudiant: ");
            }
            else
            {
                printf("\n 🥦 Wild Pokétudiant: ");
            }

            for (int i = 0; i < nbopp; i++)
            {
                printf("🎒 ");
            }
            
            printf("\n │ %s %s lvl: %d  ❤️  %f\n", get_pokemoji(opp), opp.variety, opp.lvl, oppLife);
        }
        else if(!strncmp(buffer_recv, "encounter enter action", 22))
        {
            // demand player action
            printf("\n%s Attack %s ", color_text(BLACK, LIGHT_GRAY, "[attack1]"), poke.attack1);
            printf("%s Attack %s ", color_text(BLACK, LIGHT_GRAY, "[attack2]"), poke.attack2);

            if(*nbpoke > 1)
            {
                printf("%s Switch your pokemon ", color_text(BLACK, LIGHT_GRAY, "[switch]"));
            }

            if(!isrival){
                printf("\n%s Try to catch %s %s ", color_text(BLACK, LIGHT_GRAY, "[catch]"), get_pokemoji(opp), opp.variety);
                printf("%s Try to escape the fight\n ", color_text(WHITE, LIGHT_BLUE, "[leave]"));
            }
            printf("\n");

            do
            {
                fgets(msg, bufsize, stdin);
            }while(strncmp(msg, "attack1", 7) && strncmp(msg, "attack2", 7) && (isrival || (strncmp(msg, "catch", 5) &&  strncmp(msg, "leave", 5))) && (*nbpoke == 1 || strncmp(msg, "switch", 6)));

            strcpy(buffer_send, "encounter action ");
            strncat(buffer_send, msg, strlen(msg));
            cltTCP->client_send_tcp(cltTCP, buffer_send);
        }
        else if(!strncmp(buffer_recv, "encounter enter poketudiant index", 33))
        {
            // demand poketudiant switch 
            printf("\n%s Choose the pokétudiant to switch\n", color_text(LIGHT_GRAY, BLACK, "[0...]"));
            do
            {
                fgets(msg, bufsize, stdin);
                pokesel = atoi(msg);
                if(!pokesel && msg[0] != '0'){
                    pokesel = -1;
                }
            }while(pokesel < 0 || pokesel >= *nbpoke);
            sprintf(msg, "%d\n", pokesel);

            strcpy(buffer_send, "encounter poketudiant index ");
            strcat(buffer_send, msg);
            cltTCP->client_send_tcp(cltTCP, buffer_send);
        }
        else if(!strncmp(buffer_recv, "encounter catch fail", 20))
        {
            printf("%s %s won't enter in your schoolbag\n", get_pokemoji(opp), opp.variety);
        }
        else if(!strncmp(buffer_recv, "encounter escape fail", 21))
        {
            printf("%s %s catches up with you\n", get_pokemoji(opp), opp.variety);
        }
        else if(!strncmp(buffer_recv, "encounter KO opponent", 21))
        {
            printf("\n Opponent: %s %s ☠️\n", get_pokemoji(opp), opp.variety);
        }
        else if(!strncmp(buffer_recv, "encounter KO player", 19))
        {
            printf("\n You: %s %s ☠️\n", get_pokemoji(poke), poke.variety);
        }
    }while(strncmp(buffer_recv, "encounter win", 13) && strncmp(buffer_recv, "encounter lose", 14) && strncmp(buffer_recv, "encounter escape ok", 19) && strncmp(buffer_recv, "encounter catch ok", 18));

    // victory screen
    if(!strncmp(buffer_recv, "encounter win", 13))
    {
        printf("\n🎉 You win 🎉\n");
    }
    else if(!strncmp(buffer_recv, "encounter lose", 14))
    {
        printf("\n😥 You lose 😥\n");
    }
    else if(!strncmp(buffer_recv, "encounter catch ok", 18))
    {
        printf("\n 🎉 %s %s join your team 🎉\n", get_pokemoji(opp), opp.variety);
    }
    else
    {
        printf("\n 🏃 You succesfully escape %s %s  \n", get_pokemoji(opp), opp.variety);
    }
    get_msg("press any key to continue", msg);
}

/**
 * manage the communication between the server, the map update, start fight and the tchat and team interfaces 
 */
void launch_game(struct clientTCP *cltTCP, char *buffer_send, char *buffer_recv, int bufsize){
    int ibuff, fifoTeam, fifoTchat, nbpoke, ipoke, size;
    char *bufmap;
    t_poketudiant poke;
    pid_t pidTeam, pidTchat;
    pthread_t threadMap, threadTeam, threadTchat;
    fd_set rfds;
    struct timeval tv;

    isRunning = false;
    isExit = false;

    // create communication pipe for tchat and tean
    unlink("IN_Team.fifo");
    unlink("OUT_Team.fifo");
    unlink("IN_Tchat.fifo");
    unlink("OUT_Tchat.fifo");

    if(mkfifo("IN_Team.fifo", 0777) == -1)
    {
        perror("error create Team.fifo");
        exit(1);
    }

    if(mkfifo("OUT_Team.fifo", 0777) == -1)
    {
        perror("error create Team.fifo");
        exit(1);
    }
    
    if(mkfifo("IN_Tchat.fifo", 0777) == -1)
    {
        perror("error create Tchat.fifo");
        exit(1);
    }

    if(mkfifo("OUT_Tchat.fifo", 0777) == -1)
    {
        perror("error create Tchat.fifo");
        exit(1);
    }

    // launch team interface
    if((pidTeam = system("gnome-terminal -- ./team"))==-1)
    {
        perror("couldn't launch team");
        exit(1);
    }

    // launch tchat interface
    if((pidTchat = system("gnome-terminal -- ./tchat"))==-1)
    {
        perror("couldn't launch tchat");
        exit(1);
    }

    fifoTeam = open("IN_Team.fifo", O_WRONLY);
    fifoTchat = open("IN_Tchat.fifo", O_WRONLY);

    pthread_create(&threadTeam, NULL, thout_Team, (void *) cltTCP);
    pthread_create(&threadTchat, NULL, thout_Tchat, (void *) cltTCP);

    do
    {
        // select() var initialization
        tv.tv_sec = 0;
        tv.tv_usec = 0;
        FD_ZERO(&rfds);
        FD_SET(cltTCP->sock, &rfds);

        if(select(cltTCP->sock+1, &rfds, NULL, NULL, &tv))
        {
            // read on line of the tcp socket
            ibuff = 0;
            do
            {
                read(cltTCP->sock, &buffer_recv[ibuff], sizeof(char));
                ibuff++;
            }while (buffer_recv[ibuff-1]!='\n');
            buffer_recv[ibuff]='\0';

            if(!strncmp(buffer_recv, "map", 3)){
                // get map and print it
                sscanf(buffer_recv, "map %d %d\n", &nbrow, &nbcol);
                
                bufmap = (char *) malloc((nbrow * (nbcol+1)+1) * sizeof(char));
                size = read(cltTCP->sock, bufmap, nbrow * (nbcol+1));
                bufmap[size] = '\0';
                update_map(bufmap);

                // start thsend_move to get player movement
                if(!isRunning){
                    isRunning = true;
                    pthread_create(&threadMap, NULL, thsend_move, (void *) cltTCP);
                }
            }
            else if(!strncmp(buffer_recv, "team contains", 13))
            {
                // get the player team
                sscanf(buffer_recv, "team contains %d\n", &nbpoke);
                ipoke = 0;

                do
                {
                    read(cltTCP->sock, &buffer_recv[ibuff], sizeof(char));
                    if(buffer_recv[ibuff] == '\n')
                    {
                        ipoke++;
                    }
                    ibuff++;
                }while (ipoke<nbpoke);
                buffer_recv[ibuff] = '\0';

                // send(the team to the team interface)
                write(fifoTeam, buffer_recv, strlen(buffer_recv));
            }
            else if(!strncmp(buffer_recv, "rival message", 13))
            {
                // send the message to the tchat interface
                write(fifoTchat, buffer_recv, strlen(buffer_recv));
            }
            else if(!strncmp(buffer_recv, "encounter new", 13))
            {
                // prevent the team interface that fight start
                write(fifoTeam, "fight\n", 6);

                // stop to get movement of player
                pthread_cancel(threadMap);
                isRunning = false;

                // start the fight
                start_fight(cltTCP, buffer_recv, buffer_send, bufsize, bufmap, &nbpoke, fifoTeam, fifoTchat);

                // signal the team interface that the fight stop
                write(fifoTeam, "endfight\n", 9);

                // update the map and restart player movements capture
                update_map(bufmap);
                if(!isRunning){
                    isRunning = true;
                    pthread_create(&threadMap, NULL, thsend_move, (void *) cltTCP);
                }
            }
            else if(!strncmp(buffer_recv, "encounter poketudiant xp", 24))
            {
                // stop to get movement of player
                pthread_cancel(threadMap);
                isRunning = false;

                // show poketudiant exp gain
                sscanf(buffer_recv, "encounter poketudiant xp %d %d", &ipoke, &poke.lvl);
                printf("\nPokétudiant %d gain %d experience\n", ipoke, poke.lvl);
                get_msg("press enter to continue", buffer_send);

                // update the map and restart player movements capture
                update_map(bufmap);
                if(!isRunning){
                    isRunning = true;
                    pthread_create(&threadMap, NULL, thsend_move, (void *) cltTCP);
                }
            }
            else if(!strncmp(buffer_recv, "encounter poketudiant level", 27))
            {   
                // stop to get movement of player
                pthread_cancel(threadMap);
                isRunning = false;

                // show poketudiant level up
                sscanf(buffer_recv, "encounter poketudiant level %d %d", &ipoke, &poke.lvl);
                printf("\nPokétudiant %d level to level %d\n", ipoke, poke.lvl);
                get_msg("press enter to continue", buffer_send);

                // update the map and restart player movements capture
                update_map(bufmap);
                if(!isRunning){
                    isRunning = true;
                    pthread_create(&threadMap, NULL, thsend_move, (void *) cltTCP);
                }
            }
            else if(!strncmp(buffer_recv, "encounter poketudiant evolution", 27))
            {
                // stop to get movement of player
                pthread_cancel(threadMap);
                isRunning = false;

                // show poketudiant evolve
                sscanf(buffer_recv, "encounter poketudiant evolution %d %s", &ipoke, poke.variety);
                printf("\nPokétudiant %d evolve in %s %s\n", ipoke, get_pokemoji(poke), poke.variety);
                get_msg("press enter to continue", buffer_send);

                // update the map and restart player movements capture
                update_map(bufmap);
                if(!isRunning){
                    isRunning = true;
                    pthread_create(&threadMap, NULL, thsend_move, (void *) cltTCP);
                }
            }            
        }
    }while(!isExit); // if player enter exit quit the party

    // end team interface
    write(fifoTeam, "exit", 4);
    close(fifoTeam);

    // end tchat interface
    write(fifoTchat, "exit", 4);
    close(fifoTchat);

    // delete fifo
    unlink("IN_Team.fifo");
    unlink("OUT_Team.fifo");
    unlink("IN_Tchat.fifo");
    unlink("OUT_Tchat.fifo");

    isExit = false;
    strcpy(buffer_send, "refresh");
}