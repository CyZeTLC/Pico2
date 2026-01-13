#include "include/game/level/level.h"
#include "hal/displays/st7735.h"
#include <stdio.h>

// converts linear index to x coordinate
#define continuous_to_x(idx) (idx % MAP_SIZE_HORIZONTAL)

// converts linear index to y coordinate
#define continuous_to_y(idx) (idx / MAP_SIZE_HORIZONTAL)

//converts xy-coordinates to linear index
#define xy_to_continuous(x, y) (y * MAP_SIZE_HORIZONTAL + x)

static void try_adding_wall(Level *l, uint16_t *wall_list, size_t *wall_list_size, size_t x_neighbour, size_t y_neighbour)
{
    //check wether coordinate is wall
    if (l->map_data[x_neighbour][y_neighbour] != 'W')
        return;

    uint16_t idx_neighbour = xy_to_continuous(x_neighbour, y_neighbour);

    // only continue if coordinate is not already in wall_list
    for (size_t i = 0; i < *wall_list_size; i++) {
        if (wall_list[i] == idx_neighbour)
            return;
    }

    wall_list[*wall_list_size] = idx_neighbour;
    *wall_list_size = *wall_list_size + 1;
}

static void add_neighbour_walls(Level *l, uint16_t *wall_list, size_t *wall_list_size, size_t x_coordinate, size_t y_coordinate)
{
    if (x_coordinate > 0)
        try_adding_wall(l, wall_list, wall_list_size, x_coordinate - 1, y_coordinate);
    if (y_coordinate > 0)
        try_adding_wall(l, wall_list, wall_list_size, x_coordinate, y_coordinate - 1);
    if (x_coordinate < MAP_SIZE_HORIZONTAL - 1)
        try_adding_wall(l, wall_list, wall_list_size, x_coordinate + 1, y_coordinate);
    if (y_coordinate < MAP_SIZE_VERTICAL - 1)
        try_adding_wall(l, wall_list, wall_list_size, x_coordinate, y_coordinate + 1);
}

static size_t count_wall_neighbours(Level *l, size_t x_coordinate, size_t y_coordinate)
{
    size_t c = 0;

    if (x_coordinate > 0 && l->map_data[x_coordinate - 1][y_coordinate] == 'W')
        c++;
    if (y_coordinate > 0 && l->map_data[x_coordinate][y_coordinate - 1] == 'W')
        c++;
    if (x_coordinate < MAP_SIZE_HORIZONTAL - 1
        && l->map_data[x_coordinate + 1][y_coordinate] == 'W')
        c++;
    if (y_coordinate < MAP_SIZE_VERTICAL - 1
        && l->map_data[x_coordinate][y_coordinate + 1] == 'W')
        c++;

    return c;
}

void level_load(Level *l, int id)
{
    l->level_id = id;

    // Start with a grid full of walls
    for (size_t x = 0; x < MAP_SIZE_HORIZONTAL; x++){
        for (size_t y = 0; y < MAP_SIZE_VERTICAL; y++) {
            l->map_data[x][y] = 'W';
        }
    }

    // tile index numbered line-wise, left-to-right, downwards (0-129)
    uint16_t wall_list[MAP_SIZE_HORIZONTAL * MAP_SIZE_VERTICAL];

    // Pick a cell, mark it as part of the maze.
    size_t wall_list_size = 1;
    wall_list[0] = rand() % 130;
    l->map_data[continuous_to_x(wall_list[0])][continuous_to_y(wall_list[0])] = ' ';

    // Add the walls of the cell to the wall list.
    add_neighbour_walls(l, wall_list, &wall_list_size, continuous_to_x(wall_list[0]), continuous_to_y(wall_list[0]));

    // While there are walls in the list
    while (wall_list_size > 0){
        // Pick a random wall and remove
        size_t rand_idx = rand() % wall_list_size;
        uint16_t w = wall_list[rand_idx];
        if (rand_idx != wall_list_size - 1)
            wall_list[rand_idx] = wall_list[wall_list_size - 1];
        wall_list_size--;

        size_t x = continuous_to_x(w);
        size_t y = continuous_to_y(w);

        //If only one of the cells that the wall divides is visited, then:
        if (count_wall_neighbours(l, x, y) == 1) {
            // Make the wall a passage
            l->level_id[x][y] = ' ';
            // Add the neighboring walls of the cell to the wall list.
            add_neighbour_walls(l, wall_list, &wall_list_size, x, y);
        }
    }

    // TODO add player and goal
}

static void drawWall(int x, int y, int width, int height)
{
    st7735_fill_rect(x, y, width, height, st7735_rgb(255, 255, 255));
}

// uncomment code for printing maze on stdout
//#define LEVEL_DISPLAY_STDOUT

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

#ifdef LEVEL_DISPLAY_STDOUT
    for (int x = 0; x < MAP_SIZE_HORIZONTAL + 2; x++)
        putchar('#');
    putchar('\n');
#endif

    for (int y = 0; y < MAP_SIZE_VERTICAL; y++) {

#ifdef LEVEL_DISPLAY_STDOUT
        putchar('#');
#endif

        for (int x = 0; x < MAP_SIZE_HORIZONTAL; x++) {
            char current_tile = l->map_data[x][y];

#ifdef LEVEL_DISPLAY_STDOUT
            putchar(current_tile != 'W' ? current_tile : '#');
#endif

            switch (current_tile)
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

#ifdef LEVEL_DISPLAY_STDOUT
        puts("#");
#endif
    }

#ifdef LEVEL_DISPLAY_STDOUT
    for (int x = 0; x < MAP_SIZE_HORIZONTAL + 2; x++)
        putchar('#');
    putchar('\n');
#endif
}