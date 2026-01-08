#ifndef LEVEL_H
#define LEVEL_H
#define MAP_SIZE 100

typedef struct
{
    int level_id;
    char map_data[MAP_SIZE][MAP_SIZE];
} Level;

void level_load(Level *l, int id);

void display_level(const Level *l);

#endif