#include "demos/Abgabe_09.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hal/leds/ws2812.h"
#include <stdio.h>

#define BUTTON_PIN_1 15
#define BUTTON_PIN_2 10
#define BUTTON_PIN_3 11
#define BUTTON_PIN_4 14

static WS2812 ws;
static const uint16_t num_leds = 4;
static const uint8_t pin = 1; // GPIO Pin for the LED strip

void toggle_led(int i, int state, int red, int green, int blue)
{
    ws2812_clear(&ws);
    ws2812_set_pixel_color_rgb(&ws, i, state == 1 ? red : 0, state == 1 ? green : 0, state == 1 ? blue : 0); // Red
    ws2812_show(&ws);
}

void toggle_btn(int btn_id, bool animation)
{
    bool button_state = gpio_get(btn_id);

    int led_id = 1;

    if (btn_id == BUTTON_PIN_1)
    {
        led_id = 0;
    }
    else if (btn_id == BUTTON_PIN_2)
    {
        led_id = 1;
    }
    else if (btn_id == BUTTON_PIN_3)
    {
        led_id = 2;
    }
    else if (btn_id == BUTTON_PIN_4)
    {
        led_id = 3;
    }

    if (!button_state)
    {
        // Zustand ist LOW (0) -> Taster ist gedrückt
        printf("Button %d is pressed!\n", btn_id);
        int red = 0, green = 255, blue = 255;

        if (animation)
        {
            if (btn_id == BUTTON_PIN_1)
            {
                for (int i = 0; i < num_leds; i++)
                {
                    ws2812_clear(&ws);
                    toggle_led(i, 1, red, green, blue);
                    sleep_ms(200);
                }

                for (int i = num_leds - 1; i >= 0; i--)
                {
                    ws2812_clear(&ws);
                    toggle_led(i, 1, red, green, blue);
                    sleep_ms(200);
                }
            }
            else
            {
                for (int i = 0; i < 15; i++)
                {
                    red += 15;
                    green -= 15;
                    blue -= 15;

                    toggle_led(led_id, 1, red, green, blue);
                    sleep_ms(100);

                    if (btn_id % 2 == 0)
                    {
                        toggle_led(led_id, 0, red, green, blue);
                        sleep_ms(100);
                    }
                }
            }
        }
        else
        {
            toggle_led(led_id, 1, red, green, blue);
        }
    }
    else
    {
        // Zustand ist HIGH (1) -> Taster ist nicht gedrückt
        toggle_led(led_id, 0, 0, 0, 0);
    }
}

void abgabe_09_execute(void)
{
    ws2812_init_auto_sm(&ws, num_leds, pin);
    ws2812_begin(&ws);

    // Initialisiere den GPIO-Pin
    gpio_init(BUTTON_PIN_1);

    // Setze den Pin als Eingang (Input)
    gpio_set_dir(BUTTON_PIN_1, GPIO_IN);

    // Aktiviere den internen Pull-Up-Widerstand.
    // Der Pin ist standardmäßig HIGH (1).
    // Er wird LOW (0), wenn der Taster gedrückt wird (Verbindung mit GND).
    gpio_pull_up(BUTTON_PIN_1);

    printf("Read button state (GPIO %d) started.\n", BUTTON_PIN_1);
    printf("Press the button (connected to GND).\n\n");

    // Endlosschleife
    while (true)
    {
        toggle_btn(BUTTON_PIN_1, true);
        toggle_btn(BUTTON_PIN_2, true);
        toggle_btn(BUTTON_PIN_3, true);
        toggle_btn(BUTTON_PIN_4, true);

        // Kurze Pause, um CPU zu entlasten und Prellen zu vermeiden
        sleep_ms(100);
    }

    ws2812_deinit(&ws);
}