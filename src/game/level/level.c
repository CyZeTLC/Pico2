#include "include/game/level/level.h"
#include "include/game/player/player.h"
#include "hal/displays/st7735.h"
#include <stdio.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdlib.h>

char level_wall_at_pixel_pos(Level *l, size_t x, size_t y)
{
    // Check collision considering player size
    // Check the four corners of the player bounding box
    size_t offsets[4][2] = {
        {0, 0},                            // top-left
        {PLAYER_SIZE - 1, 0},              // top-right
        {0, PLAYER_SIZE - 1},              // bottom-left
        {PLAYER_SIZE - 1, PLAYER_SIZE - 1} // bottom-right
    };

    for (int i = 0; i < 4; i++)
    {
        size_t check_x = x + offsets[i][0];
        size_t check_y = y + offsets[i][1];

        // Bounds check
        if (check_x < LEVEL_BORDER_HORIZONTAL || check_y < LEVEL_BORDER_VERTICAL)
            return 'W';

        size_t tile_x = (check_x - LEVEL_BORDER_HORIZONTAL) / TILE_SIZE;
        size_t tile_y = (check_y - LEVEL_BORDER_VERTICAL) / TILE_SIZE;

        // Bounds check for tile indices
        if (tile_x >= MAP_SIZE_HORIZONTAL || tile_y >= MAP_SIZE_VERTICAL)
            return 'W';

        //return 'w' for collision with wall and 'e' for collision with end
        char tile = l->map_data[tile_x][tile_y];
        if (tile == 'W' || tile == 'E')
		return tile;
    }

    return ' ';
}

// converts linear index to x coordinate
#define continuous_to_x(idx) (idx % MAP_SIZE_HORIZONTAL)

// converts linear index to y coordinate
#define continuous_to_y(idx) (idx / MAP_SIZE_HORIZONTAL)

// converts xy-coordinates to linear index
#define xy_to_continuous(x, y) (y * MAP_SIZE_HORIZONTAL + x)

static void try_adding_wall(Level *l, uint16_t *wall_list, size_t *wall_list_size, size_t x_neighbour, size_t y_neighbour)
{
    // check wether coordinate is wall
    if (l->map_data[x_neighbour][y_neighbour] != 'W')
        return;

    uint16_t idx_neighbour = xy_to_continuous(x_neighbour, y_neighbour);

    // only continue if coordinate is not already in wall_list
    for (size_t i = 0; i < *wall_list_size; i++)
    {
        if (wall_list[i] == idx_neighbour)
            return;
    }

    wall_list[*wall_list_size] = idx_neighbour;
    *wall_list_size = *wall_list_size + 1;
}

// add neighbouring cells to wall_list - if they are walls
static void add_unvisited_neighbour_cells(Level *l, uint16_t *wall_list, size_t *wall_list_size, size_t x_coordinate, size_t y_coordinate)
{
    if (x_coordinate > 1)
        try_adding_wall(l, wall_list, wall_list_size, x_coordinate - 2, y_coordinate);
    if (y_coordinate > 1)
        try_adding_wall(l, wall_list, wall_list_size, x_coordinate, y_coordinate - 2);
    if (x_coordinate < MAP_SIZE_HORIZONTAL - 2)
        try_adding_wall(l, wall_list, wall_list_size, x_coordinate + 2, y_coordinate);
    if (y_coordinate < MAP_SIZE_VERTICAL - 2)
        try_adding_wall(l, wall_list, wall_list_size, x_coordinate, y_coordinate + 2);
}

static void expand_in_random_direction(Level *l, uint16_t *wall_list, size_t *wall_list_size, size_t x_coordinate, size_t y_coordinate)
{
    // we're adjacent to the mace.
    // there might be more than one way to attach to it.

    // count possible directions to go
    size_t c = 0;
    // track possible directions for attaching
    char dirs[4];

    if (x_coordinate > 1 && l->map_data[x_coordinate - 2][y_coordinate] != 'W')
        dirs[c++] = 'l';
    if (y_coordinate > 1 && l->map_data[x_coordinate][y_coordinate - 2] != 'W')
        dirs[c++] = 'u';
    if (x_coordinate < MAP_SIZE_HORIZONTAL - 2 && l->map_data[x_coordinate + 2][y_coordinate] != 'W')
        dirs[c++] = 'r';
    if (y_coordinate < MAP_SIZE_VERTICAL - 2 && l->map_data[x_coordinate][y_coordinate + 2] != 'W')
        dirs[c++] = 'd';

    if (c == 0)
    {
        // this shouldn't happen
        printf("got 0 dirs for %02zu/%02zu", x_coordinate, y_coordinate);
        return;
    }

    char dir = dirs[rand() % c];

    // add the cell itself to the maze
    l->map_data[x_coordinate][y_coordinate] = ' ';

    add_unvisited_neighbour_cells(l, wall_list, wall_list_size, x_coordinate, y_coordinate);

    // connect the cell by removing the wall between it and the existing maze
    switch (dir)
    {
    case 'l':
        l->map_data[x_coordinate - 1][y_coordinate] = ' ';
        break;
    case 'u':
        l->map_data[x_coordinate][y_coordinate - 1] = ' ';
        break;
    case 'r':
        l->map_data[x_coordinate + 1][y_coordinate] = ' ';
        break;
    case 'd':
        l->map_data[x_coordinate][y_coordinate + 1] = ' ';
        break;
    default:
        return;
    }
}

void level_load(Level *l, int id, size_t *pixel_start_mid_x, size_t *pixel_start_mid_y)
{
    l->level_id = id;

    // prim's algorithm is concerned with connecting all cells.
    // every other tile is a cell. all cells become part of the maze.
    // each time a cell is added (still surrounded by walls),
    // a random neighbouring wall is removed to connect it.

    // Start with a grid full of walls
    for (size_t x = 0; x < MAP_SIZE_HORIZONTAL; x++)
    {
        for (size_t y = 0; y < MAP_SIZE_VERTICAL; y++)
        {
            l->map_data[x][y] = 'W';
        }
    }

    // wall_list contains all cells adjacent to the maze.
    // contains tile indeces, numbered line-wise, left-to-right, downwards (0-129).
    uint16_t wall_list[MAP_SIZE_HORIZONTAL * MAP_SIZE_VERTICAL];

    // Pick a cell, mark it as part of the maze.
    size_t start_x = (rand() % (MAP_SIZE_HORIZONTAL / 2)) * 2;
    size_t start_y = (rand() % (MAP_SIZE_VERTICAL / 2)) * 2;
    l->map_data[start_x][start_y] = ' ';

    size_t wall_list_size = 1;
    wall_list[0] = xy_to_continuous(start_x, start_y);

    // Add the walls of the cell to the wall list.
    add_unvisited_neighbour_cells(l, wall_list, &wall_list_size, start_x, start_y);

    // we want to mark the last touched cell as the end after the loop.
    // use these outside-loop variables for that.
    size_t cell_x, cell_y;

    printf("wall_list_size: %zu\n", wall_list_size);
    // While there are walls in the list
    while (wall_list_size > 0)
    {
        // Pick a random wall and remove
        size_t rand_idx = rand() % wall_list_size;
        uint16_t w = wall_list[rand_idx];

        if (rand_idx != wall_list_size - 1)
            wall_list[rand_idx] = wall_list[wall_list_size - 1];
        wall_list_size--;

        cell_x = continuous_to_x(w);
        cell_y = continuous_to_y(w);

        expand_in_random_direction(l, wall_list, &wall_list_size, cell_x, cell_y);
    }

    *pixel_start_mid_x = LEVEL_BORDER_HORIZONTAL + start_x * TILE_SIZE + TILE_SIZE / 2;
    *pixel_start_mid_y = LEVEL_BORDER_VERTICAL + start_y * TILE_SIZE + TILE_SIZE / 2;

    l->map_data[cell_x][cell_y] = 'E';
}

static void drawWall(int x, int y, int width, int height)
{
    st7735_fill_rect(x, y, width, height, st7735_rgb(0, 0, 0));
}

// uncomment code for printing maze on stdout
// #define LEVEL_DISPLAY_STDOUT

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
            char current_tile = l->map_data[x][y];
	    int pixel_x = LEVEL_BORDER_HORIZONTAL + x * TILE_SIZE;
	    int pixel_y = LEVEL_BORDER_VERTICAL + y * TILE_SIZE;

            switch (current_tile)
            {
            case 'E':
                st7735_fill_rect(pixel_x, pixel_y, TILE_SIZE, TILE_SIZE, st7735_rgb(0, 255, 0));
                break;

            case 'W': // Wand
                drawWall(pixel_x,
                         pixel_y,
                         TILE_SIZE,
                         TILE_SIZE);
                break;

            default:
                st7735_fill_rect(pixel_x, pixel_y, TILE_SIZE, TILE_SIZE, st7735_rgb(255, 255, 255));
                break;
            }
        }
    }
}