#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_SIZE (10)

typedef struct
{
    int x, y;
    int health;
    char name[16];
} Player;

void player_init(Player *p, char *name, int startX, int startY);
void player_move(Player *p, int dx, int dy);

#endif