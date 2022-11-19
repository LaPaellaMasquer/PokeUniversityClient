#pragma once

typedef enum{BLACK, 
            RED, 
            GREEN, 
            BROWN,
            BLUE,
            PURPLE,
            CYAN,
            LIGHT_GRAY,
            DARK_GRAY,
            LIGHT_RED,
            LIGHT_GREEN,
            YELLOW,
            LIGHT_BLUE,
            LIGHT_PURPLE,
            LIGHT_CYAN,
            WHITE}t_color;

char * code_foreground(t_color color);
char * code_background(t_color color); 
char * color_text(t_color foregroud, t_color backgroud, char *m);
