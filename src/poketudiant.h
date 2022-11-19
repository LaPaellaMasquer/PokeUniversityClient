#pragma once

typedef struct
{
    char variety[25];
    char type[25];
    int lvl, currxp, xpnextlvl;
    int pv, maxpv;
    int att, def;
    char attack1[25], attack1type[25];
    char attack2[25], attack2type[25];
}t_poketudiant;

char * get_pokemoji(t_poketudiant poketudiant);

void print_poketudiant(t_poketudiant poketudiant);

t_poketudiant to_poketudiant(char *desc);
