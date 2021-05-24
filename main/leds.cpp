#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/ledc.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "led_strip.h"


int colorb = 0;
int colorr = 0;
int direction = 1;


static led_strip_t *pStrip_a;

static char tag[] = "catbot_leds";
static const char* TAG = "catbot_leds";


static void configure_led(void)
{
    ESP_LOGI(TAG, "CATBOT MK2");
    /* LED strip initialization with the GPIO and pixels number*/
    pStrip_a = led_strip_init(0, 8, 1);
    /* Set all LED off to clear all pixels */
    pStrip_a->clear(pStrip_a, 50);
}


// main freeRTOS entry point
void leds_task(void *ignore) {
    /* Configure the peripheral according to the LED type */
    configure_led();

    ESP_LOGI(TAG, "Starting the LED Sweep");
    while (1) {
        //blink_led();
        /* Toggle the LED state */
        //s_led_state = !s_led_state;

        pStrip_a->set_pixel(pStrip_a, 0, colorb, 0, 0);
        pStrip_a->refresh(pStrip_a, 100);

        if (colorb <= 0){
            //ESP_LOGI(TAG, "going up!");
            direction = 2;
            vTaskDelay(500 / portTICK_PERIOD_MS);

        } else if (colorb >= 100) {
            //ESP_LOGI(TAG, "going down!");
            direction = -2;
        }
        colorb += direction;
        div_t divres =  div(colorb, 5);
        colorr = (int) divres.quot;
        /* Refresh the strip to send data */
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);

}