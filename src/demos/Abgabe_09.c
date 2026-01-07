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

// Funktion zum Ein-/Ausschalten einer einzelnen LED in einer bestimmten Farbe
void toggle_led(int i, int state, int red, int green, int blue)
{
    // alle LEDs aus
    ws2812_clear(&ws);

    // Setze die Farbe der LED an Index 'i' basierend auf 'state'
    // Wenn 'state' 1 (an) ist, dann wird 'red', 'green', 'blue' benutzt, sonst 0 (aus)
    ws2812_set_pixel_color_rgb(&ws, i, state == 1 ? red : 0, state == 1 ? green : 0, state == 1 ? blue : 0);

    // Sende die neuen Farben an den LED-Strip
    ws2812_show(&ws);
}

// Funktion beim Buttonklick
void toggle_btn(int btn_id, bool animation)
{
    // Lese den aktuellen Zustand des Buttons
    bool button_state = gpio_get(btn_id);

    // Setze standart LED-ID
    int led_id = 1;

    // Ordne btn_id die led_id zu
    if (btn_id == BUTTON_PIN_1)
    {
        led_id = 0; // Button 1 steuert LED 0
    }
    else if (btn_id == BUTTON_PIN_2)
    {
        led_id = 1; // Button 2 steuert LED 1
    }
    else if (btn_id == BUTTON_PIN_3)
    {
        led_id = 2; // Button 3 steuert LED 2
    }
    else if (btn_id == BUTTON_PIN_4)
    {
        led_id = 3; // Button 4 steuert LED 3
    }

    // Prüfe, ob der Taster gedrückt ist
    if (!button_state)
    {
        // Gebe eine Meldung auf der Konsole aus
        printf("Button %d is pressed!\n", btn_id);

        // Setze Standardfarbe auf Cyan
        int red = 0, green = 255, blue = 255;

        // Prüfe, ob eine Animation ausgeführt werden soll
        if (animation)
        {
            // Spezielle Animation für BUTTON_PIN_1 (Lauflicht)
            if (btn_id == BUTTON_PIN_1)
            {
                // Lauflicht vorwärts
                for (int i = 0; i < num_leds; i++)
                {
                    // Lösche alle LEDs
                    ws2812_clear(&ws);

                    // Schalte die aktuelle LED 'i' an
                    toggle_led(i, 1, red, green, blue);

                    // Warte 200 Millisekunden
                    sleep_ms(200);
                }

                // Lauflicht rückwärts
                for (int i = num_leds - 1; i >= 0; i--)
                {
                    // Lösche alle LEDs
                    ws2812_clear(&ws);

                    // Schalte die aktuelle LED 'i' an
                    toggle_led(i, 1, red, green, blue);

                    // Warte 200 Millisekunden
                    sleep_ms(200);
                }
            }
            // Animation für alle anderen Taster (Farbwechsel/Blinken)
            else
            {
                // Mache 15 Farbanpassungen/Blinkschritte
                for (int i = 0; i < 15; i++)
                {
                    // Passe Rot, Grün und Blau an (Farbübergang zu Rot)
                    red += 15;
                    green -= 15;
                    blue -= 15;

                    // Schalte die zugehörige LED mit der neuen Farbe an
                    toggle_led(led_id, 1, red, green, blue);

                    // Warte 100 Millisekunden
                    sleep_ms(100);

                    // Spezielles Blinken nur für jeden zweiten Button
                    if (btn_id % 2 == 0)
                    {
                        // Schalte die LED aus
                        toggle_led(led_id, 0, red, green, blue);

                        // Warte 100 Millisekunden
                        sleep_ms(100);
                    }
                }
            }
        }
        else
        {
            // Schalte die zugehörige LED an
            toggle_led(led_id, 1, red, green, blue);
        }
    }
    else
    {
        // Schalte die zugehörige LED aus
        toggle_led(led_id, 0, 0, 0, 0);
    }
}

void abgabe_09_execute(void)
{
    // Initialisiere von WS2812
    ws2812_init_auto_sm(&ws, num_leds, pin);

    // Starte ws2812
    ws2812_begin(&ws);

    /*
     * Lege für alle 4 Buttons den zugehörigen Pin fest
     */

    // Initialisiere den GPIO-Pin
    gpio_init(BUTTON_PIN_1);

    // Setze den Pin als Eingang (Input)
    gpio_set_dir(BUTTON_PIN_1, GPIO_IN);

    // Aktiviere den internen Pull-Up-Widerstand.
    // Der Pin ist standardmäßig HIGH (1).
    // Er wird LOW (0), wenn der Taster gedrückt wird (Verbindung mit GND).
    gpio_pull_up(BUTTON_PIN_1);

    // Ausgabemeldungen für den Start
    printf("Read button state (GPIO %d) started.\n", BUTTON_PIN_1);
    printf("Press the button (connected to GND).\n\n");

    // Endlosschleife zur Abfrage der Buttons
    while (true)
    {
        /*
         * Überprüfe den Zustand jedes Buttons und führe die entsprechende Aktion aus
         */
        toggle_btn(BUTTON_PIN_1, true);
        toggle_btn(BUTTON_PIN_2, true);
        toggle_btn(BUTTON_PIN_3, true);
        toggle_btn(BUTTON_PIN_4, true);

        // Kurze Pause, um CPU zu entlasten und Prellen zu vermeiden
        sleep_ms(100);
    }

    // Deinitialisiere WS2812
    ws2812_deinit(&ws);
}