#ifndef PLAYER_H
#define PLAYER_H
#define MOVE_SPEED 5

#include "include/game/level/level.h"

#define PLAYER_SIZE (7)

typedef struct
{
    int x, y;
    int health;
    char name[16];
} Player;

void player_init(Player *p, char *name, int startX, int startY);
bool player_move(Level *level, Player *player, float x_delta, float y_delta);

#endif