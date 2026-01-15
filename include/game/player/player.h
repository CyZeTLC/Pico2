#ifndef PLAYER_H
#define PLAYER_H

#include "include/game/level/level.h"

#define PLAYER_SIZE (10)

typedef struct
{
    int x, y;
    int health;
    char name[16];
} Player;

void player_init(Player *p, char *name, int startX, int startY);
void player_move(Level *level, Player *player, float x_delta, float y_delta);

#endif