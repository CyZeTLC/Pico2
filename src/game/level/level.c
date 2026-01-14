#include "include/game/level/level.h"
#include "hal/displays/st7735.h"
#include <stdio.h>

void level_load(Level *l, int id)
{
    l->level_id = id;
    // TODO
    l->map_data[20][30] = 'S'; // Startpunkt
    l->map_data[99][99] = 'E'; // Endpunkt
}

static void drawWall(int x, int y, int width, int height)
{
    st7735_fill_rect(x, y, width, height, st7735_rgb(255, 255, 255));
}

void display_level(const Level *l)
{
    // Top wall
    drawWall(0, 0, DISPLAY_WIDTH, LEVEL_BORDER_VERTICAL);
    // Bottom wall
    drawWall(0, DISPLAY_HEIGHT - LEVEL_BORDER_VERTICAL, DISPLAY_WIDTH, LEVEL_BORDER_VERTICAL);
    // Left wall
    drawWall(0, 0, LEVEL_BORDER_HORIZONTAL, DISPLAY_HEIGHT);
    // Right wall
    drawWall(DISPLAY_WIDTH - LEVEL_BORDER_HORIZONTAL, 0, LEVEL_BORDER_HORIZONTAL, DISPLAY_HEIGHT);

    printf("Level ID: %d\n", l->level_id);

    for (int y = 0; y < MAP_SIZE_VERTICAL; y++)
    {
        for (int x = 0; x < MAP_SIZE_HORIZONTAL; x++)
        {
            char current_tile = l->map_data[y][x];

            switch (l->map_data[y][x])
            {
            case 'S':
                // st7735_fill_rect(x, y, TILE_SIZE, TILE_SIZE, st7735_rgb(0, 255, 0));
                break;

            case 'E':
                // st7735_fill_rect(x, y, TILE_SIZE, TILE_SIZE, st7735_rgb(255, 0, 0));
                break;

            case 'W': // Wand
                // drawWall(x, y, TILE_SIZE, TILE_SIZE);

                break;

            default:
                // st7735_fill_rect(x, y, TILE_SIZE, TILE_SIZE, st7735_rgb(0, 0, 0));
                break;
            }
        }
    }
}