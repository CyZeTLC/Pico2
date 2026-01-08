#include "include/game/level/level.h"
#include "hal/displays/st7735.h"
#include <stdio.h>

void level_load(Level *l, int id)
{
    l->level_id = id;
    l->map_data[20][30] = 'S'; // Startpunkt
    l->map_data[99][99] = 'E'; // Endpunkt
}

void display_level(const Level *l)
{
    printf("Level ID: %d\n", l->level_id);

    for (int y = 0; y < MAP_SIZE; y++)
    {
        for (int x = 0; x < MAP_SIZE; x++)
        {
            char current_tile = l->map_data[y][x];

            switch (l->map_data[y][x])
            {
            case 'S':
                // st7735_fill_rect(x, y, 10, 10, st7735_rgb(0, 255, 0));
                break;

            case 'E':
                // st7735_fill_rect(x, y, 10, 10, st7735_rgb(255, 0, 0));
                break;

            case 'W': // Wand
                // draw_pixel(x, y, WHITE);
                break;

            default:
                // st7735_fill_rect(x, y, 10, 10, st7735_rgb(0, 0, 0));
                break;
            }
        }
    }
}