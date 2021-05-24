/*
Code is adapted from 

https://github.com/nkolban/esp32-snippets/blob/master/hardware/servos/servoSweep.c

*/

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


/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
//#define BLINK_GPIO CONFIG_BLINK_GPIO

static uint8_t s_led_state = 0;

static char tag[] = "catbot_mk2";

static const char* TAG = "catbot_mk2";

int colorb = 0;
int colorr = 0;
int direction = 1;


static led_strip_t *pStrip_a;



static void configure_led(void)
{
    ESP_LOGI(TAG, "CATBOT MK2");
    /* LED strip initialization with the GPIO and pixels number*/
    pStrip_a = led_strip_init(CONFIG_BLINK_LED_RMT_CHANNEL, 1, 1);
    /* Set all LED off to clear all pixels */
    pStrip_a->clear(pStrip_a, 50);
}



void sweepLED_task(void *ignore) {
    /* Configure the peripheral according to the LED type */
    configure_led();

    while (1) {
        //ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        //blink_led();
        /* Toggle the LED state */
        //s_led_state = !s_led_state;

        pStrip_a->set_pixel(pStrip_a, 0, colorr, 0, colorb);
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

void getNextCorner(int *newDutyX, int *newDutyY, int maxValue, int minValue){
    static int index = 0;
    switch (index) {
        case 0:
            *newDutyX = minValue;
            *newDutyY = minValue;
            break;
        case 1:
            *newDutyX = minValue;
            *newDutyY = maxValue;
            break;
        case 2:
            *newDutyX = maxValue;
            *newDutyY = maxValue;
            break;
         case 3:
            *newDutyX = maxValue;
            *newDutyY = minValue;
            break;
    }
    index ++;
    if (index > 3){
        index = 0;
    }
}

void square(int *dutyX, int *dutyY, int maxValue, int minValue){
    static int idx=0;

    int sweepDuration   = 1000; // milliseconds (ms)
    int valueChangeRate = 20; // msecs
    int changesPerSweep = sweepDuration / valueChangeRate;// 1000/20 -> 50

    int delay = 500;
    int newDutyX = (int) rand() % (maxValue-minValue) + minValue;
    int newDutyY = (int) rand() % (maxValue-minValue) + minValue;
    getNextCorner(&newDutyX, &newDutyY, minValue, maxValue);
    int changeDeltaX = (int) (newDutyX-*dutyX) / changesPerSweep;
    int changeDeltaY = (int) (newDutyY-*dutyY) / changesPerSweep;
    ESP_LOGI(tag, "newdutyX=%d, newdutyY=%d\n", newDutyX, newDutyY);




    // move to new pos
    for (int i=0; i<changesPerSweep; i++) {

        *dutyX += changeDeltaX;
        *dutyY += changeDeltaY;

        // set x pos
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, *dutyX);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        // set y pos
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, *dutyY);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);

        vTaskDelay(valueChangeRate/portTICK_PERIOD_MS);
        ESP_LOGI(tag, "dutyX=%d, dutyY=%d\n", *dutyX, *dutyY);
    }
    *dutyX = newDutyX;
    *dutyY = newDutyY;
    vTaskDelay(delay/portTICK_PERIOD_MS);
}

void moveToRandomPos(int *dutyX, int *dutyY, int maxValue, int minValue){

    int sweepDuration   = rand() % 1800 + 200; // milliseconds (ms)
    int valueChangeRate = 20; // msecs
    int changesPerSweep = sweepDuration / valueChangeRate;// 1000/20 -> 50

    int delay = rand() % 2000;
    int newDutyX = (int) rand() % (maxValue-minValue) + minValue;
    int newDutyY = (int) rand() % (maxValue-minValue) + minValue;
    int changeDeltaX = (int) (newDutyX-*dutyX) / changesPerSweep;
    int changeDeltaY = (int) (newDutyY-*dutyY) / changesPerSweep;
    ESP_LOGI(tag, "newdutyX=%d, newdutyY=%d\n", newDutyX, newDutyY);


    // move to new pos
    for (int i=0; i<changesPerSweep; i++) {

        *dutyX += changeDeltaX;
        *dutyY += changeDeltaY;

        // set x pos
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, *dutyX);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        // set y pos
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, *dutyY);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);

        vTaskDelay(valueChangeRate/portTICK_PERIOD_MS);
        ESP_LOGI(tag, "dutyX=%d, dutyY=%d\n", *dutyX, *dutyY);
    }
    *dutyX = newDutyX;
    *dutyY = newDutyY;
    vTaskDelay(delay/portTICK_PERIOD_MS);
}

void sweepServo_task(void *ignore) {
	int bitSize         = 15;
	int minValue        = 1200;  // micro seconds (uS)
	int maxValue        = 1700; // micro seconds (uS)
	int dutyX           = 1500; //(1<<bitSize) * minValue / 20000 ;//1638
	int dutyY           = 1500; //(1<<bitSize) * minValue / 20000 ;//1638
	int direction       = 1; // 1 = up, -1 = down


	time_t t;

   /* Intializes random number generator */
    srand((unsigned) time(&t));

    ESP_LOGD(tag, ">> task_servo1");
	ledc_timer_config_t timer_conf1;
	timer_conf1.bit_num    = LEDC_TIMER_14_BIT;
	timer_conf1.freq_hz    = 50;
	timer_conf1.speed_mode = LEDC_LOW_SPEED_MODE;
	timer_conf1.timer_num  = LEDC_TIMER_0;
	ledc_timer_config(&timer_conf1);

	ledc_channel_config_t ledc_conf1;
	ledc_conf1.channel    = LEDC_CHANNEL_0;
	ledc_conf1.duty       = dutyX;
	ledc_conf1.gpio_num   = 9;
	ledc_conf1.intr_type  = LEDC_INTR_DISABLE;
	ledc_conf1.speed_mode = LEDC_LOW_SPEED_MODE;
	ledc_conf1.timer_sel  = LEDC_TIMER_0;
	ledc_channel_config(&ledc_conf1);

	ledc_timer_config_t timer_conf2;
	timer_conf2.bit_num    = LEDC_TIMER_14_BIT;
	timer_conf2.freq_hz    = 50;
	timer_conf2.speed_mode = LEDC_LOW_SPEED_MODE;
	timer_conf2.timer_num  = LEDC_TIMER_1;
	ledc_timer_config(&timer_conf2);

	ledc_channel_config_t ledc_conf2;
	ledc_conf2.channel    = LEDC_CHANNEL_1;
	ledc_conf2.duty       = dutyY;
	ledc_conf2.gpio_num   = 7;
	ledc_conf2.intr_type  = LEDC_INTR_DISABLE;
	ledc_conf2.speed_mode = LEDC_LOW_SPEED_MODE;
	ledc_conf2.timer_sel  = LEDC_TIMER_1;
	ledc_channel_config(&ledc_conf2);
//
////	// set initial x pos
	ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, dutyX);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    // set initial y pos
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, dutyY);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);


	while(1) {
	    moveToRandomPos(&dutyX, &dutyY, maxValue,  minValue);
	    //square(&dutyX, &dutyY, maxValue,  minValue);
	    //vTaskDelay(1000/portTICK_PERIOD_MS);
	} // End loop forever

	vTaskDelete(NULL);
}

void app_main()
{	
        esp_log_level_set("catbot_mk2", ESP_LOG_DEBUG);

        xTaskCreate(&sweepServo_task,"sweepServo_task",2048,NULL,5,NULL);
        xTaskCreate(&sweepLED_task,"sweepLED_task",2048,NULL,5,NULL);

        printf("sweep tasks  started\n");
}
