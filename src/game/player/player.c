#include "include/game/player/player.h"
#include "hal/displays/st7735.h"
#include <stdio.h>

void player_init(Player *p, char *name, int startX, int startY)
{
    p->x = startX;
    p->y = startY;
    p->health = 100;
}

void player_move(Level *level, Player *player, float x_delta, float y_delta)
{
    unsigned rightmost = DISPLAY_WIDTH - LEVEL_BORDER_HORIZONTAL - PLAYER_SIZE;

    int next_x = player->x - (int)(x_delta * MOVE_SPEED);
    // outer wall
    if (next_x < LEVEL_BORDER_HORIZONTAL)
        next_x = LEVEL_BORDER_HORIZONTAL;
    else if (next_x > rightmost)
        next_x = rightmost;

    unsigned bottommost = DISPLAY_HEIGHT - LEVEL_BORDER_VERTICAL - PLAYER_SIZE;

    int next_y = player->y + (int)(y_delta * MOVE_SPEED);

    if (next_y < LEVEL_BORDER_VERTICAL)
        next_y = LEVEL_BORDER_VERTICAL;
    else if (next_y > bottommost)
        next_y = bottommost;

    if (level_wall_at_pixel_pos(level, (size_t)next_x, (size_t)next_y))
    {
        return; // Collision with wall
    }

    st7735_fill_rect(player->x, player->y, PLAYER_SIZE, PLAYER_SIZE, st7735_rgb(0, 0, 0));
    player->x = next_x;
    player->y = next_y;
    st7735_fill_rect(player->x, player->y, PLAYER_SIZE, PLAYER_SIZE, st7735_rgb(0, 255, 0));
}