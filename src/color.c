#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "color.h"

/**
 * get the color of foreground color 
 */
char * code_foreground(t_color color){
    char *res;
    if (color < DARK_GRAY){
       res  = (char *) malloc(2*sizeof(char));
       sprintf(res,"%d", color+30);
    }
    else{
        res  = (char *) malloc(4*sizeof(char));
        sprintf(res,"1;%d", (color%8)+30);
    }
    return res;
    free(res);
}

/**
 * get the color of background color 
 */
char * code_background(t_color color){
    char *res;
    if (color < DARK_GRAY){
       res  = (char *) malloc(2*sizeof(char));
       sprintf(res,"%d", color+40);
    }
    else{
        res  = (char *) malloc(4*sizeof(char));
        sprintf(res,"1;%d", (color%8)+40);
    }
    return res;
    free(res);
}

/**
 * color the txt m
 */
char * color_text(t_color foregroud, t_color backgroud, char *m){
   int size = strlen(m) + (foregroud < DARK_GRAY?2:4) + (backgroud < DARK_GRAY?2:4)+10;
   char *res = (char *) malloc(size*sizeof(char));
   sprintf(res,"\e[%s;%sm%s\e[0m", code_foreground(foregroud),code_background(backgroud),m);
   return res;
}
