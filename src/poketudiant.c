#include <stdio.h>
#include <string.h>

#include "poketudiant.h"

/**
 * get the equivalent emoji of a poketudiant variety
 */
char * get_pokemoji(t_poketudiant poketudiant)
{
    if(!strncmp(poketudiant.variety, "Parlfor", 7))
    {
        return "😃";
    }
    else if(!strncmp(poketudiant.variety, "Ismar", 5))
    {
        return "😄";
    }
    else if(!strncmp(poketudiant.variety, "Rigolamor", 9))
    {
        return "😂";
    }
    else if(!strncmp(poketudiant.variety, "Procrastino", 11))
    {
        return "🥱";
    }
    else if(!strncmp(poketudiant.variety, "Couchtar", 8))
    {
        return "🥳";
    }
    else if(!strncmp(poketudiant.variety, "Nuidebou", 8))
    {
        return "😈";
    }
    else if(!strncmp(poketudiant.variety, "Alabourre", 9))
    {
        return "😱";
    }
    else if(!strncmp(poketudiant.variety, "Buchafon", 8))
    {
        return "👨‍💻";
    }
    else if(!strncmp(poketudiant.variety, "Belmention", 10))
    {
        return "🧑‍⚖️";
    }
    else if(!strncmp(poketudiant.variety, "Promomajor", 10))
    {
        return "👨‍🎓";
    }
    else if(!strncmp(poketudiant.variety, "Enseignant-dresseur", 19))
    {
        return "👨‍🏫";
    }
    else
    {
        return " ";
    }
}

/** 
 * print poketudiant stats
 */
void print_poketudiant(t_poketudiant poketudiant)
{
    printf(" │ %s Variety: %s Type: %s\n", get_pokemoji(poketudiant), poketudiant.variety, poketudiant.type);
    printf(" │ ❤️  %d/%d\n", poketudiant.pv, poketudiant.maxpv);
    printf(" │ ⚔️  %d 🛡️  %d\n", poketudiant.att, poketudiant.def);
    printf(" │ lvl: %d exp: %d/%d\n", poketudiant.lvl, poketudiant.currxp, poketudiant.xpnextlvl);
    printf(" │ Attack 1: %s type: %s\n", poketudiant.attack1, poketudiant.attack1type);
    printf(" │ Attack 2: %s type: %s\n", poketudiant.attack2, poketudiant.attack2type);
}

/**
 * poketudiant description to poketudiant
 */
t_poketudiant to_poketudiant(char *desc)
{
    t_poketudiant poketudiant;

    sscanf(desc,"%s %s %d %d %d %d %d %d %d %s %s %s %s\n", 
                poketudiant.variety, poketudiant.type, &poketudiant.lvl, &poketudiant.currxp, &poketudiant.xpnextlvl, 
                &poketudiant.pv, &poketudiant.maxpv, &poketudiant.att, &poketudiant.def, poketudiant.attack1, poketudiant.attack1type, 
                poketudiant.attack2, poketudiant.attack2type);
    return poketudiant;
}
