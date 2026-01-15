#ifndef GAME_H
#define GAME_H
#define TICK_DURATION 50

#include "player/player.h"
#include "level/level.h"

typedef struct
{
    Player player;
    Level current_level;
    int is_running;
} Game;

void game_run(Game *game);

void display_ui();

void game_init_display();

#endif