#ifndef LEVEL_H
#define LEVEL_H

#define DISPLAY_WIDTH (128)
#define DISPLAY_HEIGHT (160)

// this leaves 120x156 pixels for the level
#define LEVEL_BORDER_HORIZONTAL (4)
#define LEVEL_BORDER_VERTICAL (2)

// 10x13 rechteckige kacheln à 12x12 pixel fuer das spielfeld
#define TILE_SIZE (12)
//mapsize in kacheln
#define MAP_SIZE_HORIZONTAL ((DISPLAY_WIDTH - 2 * LEVEL_BORDER_HORIZONTAL) / TILE_SIZE)
#define MAP_SIZE_VERTICAL ((DISPLAY_HEIGHT - 2 * LEVEL_BORDER_VERTICAL) / TILE_SIZE)

typedef struct
{
    int level_id;
    char map_data[MAP_SIZE_HORIZONTAL][MAP_SIZE_VERTICAL];
} Level;

void level_load(Level *l, int id);

void display_level(const Level *l);

#endif