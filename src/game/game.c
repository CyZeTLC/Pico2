#include "include/game/game.h"
#include "hal/displays/st7735.h"
#include "include/game/level/level.h"
#include "hal/controls/joystick.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>

static int time = 0;

void game_init_display()
{
    // backlight pin
    gpio_init(2);
    gpio_set_dir(2, GPIO_OUT);
    gpio_put(2, 1);

    // 40 MHz SPI clock
    spi_init(spi0, 40 * 1000 * 1000);

    // Set SPI pin functions
    gpio_set_function(16, GPIO_FUNC_SPI);
    gpio_set_function(18, GPIO_FUNC_SPI);
    gpio_set_function(19, GPIO_FUNC_SPI);

    // Initialize ST7735 display
    st7735_init(spi0, 6, 17, 3, 0, false);
}

void display_ui()
{
    char time_str[10];
    sprintf(time_str, "Ticks: %d", time);
    st7735_draw_string(10, 10, time_str, st7735_rgb(255, 255, 255), st7735_rgb(0, 0, 0));
}

bool wall_at(int x, int y)
{
    uint16_t color;

    if (color == st7735_rgb(255, 255, 255))
    {
        return true; // Wall present
    }
    return false; // No wall
}

void move_player(Player *player, float x_delta, float y_delta)
{
    int next_x = player->x - (int)(x_delta * 5);
    int next_y = player->y + (int)(y_delta * 50);

    if (next_x < 0 || next_x > 127 || next_y < 0 || next_y > 159)
    {
        return; // Out of bounds
    }

    if (wall_at(next_x, next_y))
    {
        return; // Collision with wall
    }

    st7735_fill_rect(player->x, player->y, 10, 10, st7735_rgb(0, 0, 0));
    player->x = next_x;
    player->y = next_y;
    st7735_fill_rect(player->x, player->y, 10, 10, st7735_rgb(0, 255, 0));
}

void drawWall(int x, int y, int w, int h)
{
    st7735_fill_rect(x, y, w, h, st7735_rgb(255, 255, 255));
}

void draw_random_walls()
{
    for (int i = 0; i < 10; i++)
    {
        int x = rand() % 118;
        int y = rand() % 128 + 20;
        int w = (rand() % 20) + 5;
        int h = (rand() % 20) + 5;
        drawWall(x, y, w, h);
    }
}

void game_run(Game *game)
{
    game_init_display();
    st7735_begin();

    joystick_init_simple_center();
    joystick_event_t event;

    Level *level1 = malloc(sizeof(Level));
    level_load(&game->current_level, 1);
    player_init(&game->player, "Hero", 60, 60);

    printf("Spiel gestartet in Level %d\n", game->current_level.level_id);

    st7735_fill_screen(st7735_rgb(0, 0, 0));

    drawWall(0, 0, 128, 5);   // Top wall
    drawWall(0, 155, 128, 5); // Bottom wall
    drawWall(0, 0, 5, 160);   // Left wall
    drawWall(123, 0, 5, 160); // Right wall

    draw_random_walls();

    while (true)
    {
        display_ui();
        display_level(&game->current_level);

        joystick_read(&event);

        if (event.button_pressed)
        {
            st7735_fill_rect(game->player.x, game->player.y, 10, 10, st7735_rgb(0, 0, 0));
            game->player.x = 60;
            game->player.y = 60;
        }
        else
        {
            move_player(&game->player, event.x_norm, event.y_norm);
        }

        char location_str[10];
        sprintf(location_str, "X: %.3f Y: %.3f", game->player.x / 1.0f, game->player.y / 1.0f);
        st7735_draw_string(10, 20, location_str, st7735_rgb(255, 255, 255), st7735_rgb(0, 0, 0));

        time++;
        sleep_ms(TICK_DURATION);
    }
}