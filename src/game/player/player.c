#include "include/game/player/player.h"
#include <stdio.h>

void player_init(Player *p, char *name, int startX, int startY)
{
    p->x = startX;
    p->y = startY;
    p->health = 100;
}

void player_move(Player *p, int dx, int dy)
{
    p->x += dx;
    p->y += dy;
    printf("Spieler bewegt nach: %d, %d\n", p->x, p->y);
}