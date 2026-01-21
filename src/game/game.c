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

static void start_level(Game *game, int level_id, int *player_start_x, int *player_start_y)
{
    size_t start_mid_x, start_mid_y;
    level_load(&game->current_level, 1, &start_mid_x, &start_mid_y);

    *player_start_x = start_mid_x - PLAYER_SIZE / 2;
    *player_start_y = start_mid_y - PLAYER_SIZE / 2;

    display_level(&game->current_level);
}

void game_run(Game *game)
{
    game_init_display();
    st7735_begin();

    st7735_fill_screen(st7735_rgb(0, 0, 0));

    joystick_init_simple_center();
    joystick_event_t event;

    int player_start_x, player_start_y;
    start_level(game, 1, &player_start_x, &player_start_y);

    player_init(&game->player, "Hero", player_start_x, player_start_y);

    printf("Spiel gestartet in Level %d\n", game->current_level.level_id);

    while (true)
    {
        // display_ui();

        joystick_read(&event);

        if (event.button_pressed)
        {
            st7735_fill_rect(game->player.x, game->player.y, PLAYER_SIZE, PLAYER_SIZE, st7735_rgb(255, 255, 255));
            game->player.x = player_start_x;
            game->player.y = player_start_y;
        }
        else
        {
            player_move(&game->current_level, &game->player, event.x_norm, event.y_norm);
        }

        char location_str[10];
        sprintf(location_str, "X: %.3f Y: %.3f", game->player.x / 1.0f, game->player.y / 1.0f);
        // st7735_draw_string(10, 20, location_str, st7735_rgb(255, 255, 255), st7735_rgb(0, 0, 0));

        time++;
        sleep_ms(TICK_DURATION);
    }
}