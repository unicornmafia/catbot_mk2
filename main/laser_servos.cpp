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



static char tag[] = "catbot_laser_servos";
static const char* TAG = "catbot_laser_servos";


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
    ESP_LOGI(tag, "Moving to: %d, %d\n", newDutyX, newDutyY);


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
    }
    *dutyX = newDutyX;
    *dutyY = newDutyY;
    vTaskDelay(delay/portTICK_PERIOD_MS);
}


void laser_servos_task(void *ignore) {
	int minValue        = 1200;  // micro seconds (uS)
	int maxValue        = 1700; // micro seconds (uS)
	int dutyX           = 1500; //(1<<bitSize) * minValue / 20000 ;//1638
	int dutyY           = 1500; //(1<<bitSize) * minValue / 20000 ;//1638


	time_t t;

   /* Intializes random number generator */
    srand((unsigned) time(&t));

    ESP_LOGD(tag, ">> task_servo1");
	ledc_timer_config_t timer_conf1;
	timer_conf1.duty_resolution = LEDC_TIMER_14_BIT;
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


	ledc_channel_config_t ledc_conf2;
	ledc_conf2.channel    = LEDC_CHANNEL_1;
	ledc_conf2.duty       = dutyY;
	ledc_conf2.gpio_num   = 7;
	ledc_conf2.intr_type  = LEDC_INTR_DISABLE;
	ledc_conf2.speed_mode = LEDC_LOW_SPEED_MODE;
	ledc_conf2.timer_sel  = LEDC_TIMER_0;
	ledc_channel_config(&ledc_conf2);

    // set initial x, y pos
	ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, dutyX);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, dutyY);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);


	while(1) {
	    moveToRandomPos(&dutyX, &dutyY, maxValue,  minValue);
	    //square(&dutyX, &dutyY, maxValue,  minValue);
	    //vTaskDelay(1000/portTICK_PERIOD_MS);
	} // End loop forever

	vTaskDelete(NULL);
}