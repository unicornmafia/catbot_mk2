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

#include "pattern.h"

Pattern::Pattern() {

	time_t t;

   /* Intializes random number generator */
    srand((unsigned) time(&t));

    ESP_LOGD(tag, ">> task_servo1");
	ledc_timer_config_t timer_conf1;
	timer_conf1.duty_resolution = LEDC_TIMER_14_BIT;
	timer_conf1.freq_hz    = 50;
	timer_conf1.speed_mode = LEDC_LOW_SPEED_MODE;
	timer_conf1.timer_num  = LEDC_TIMER_0;
	ESP_LOGD(tag, "timer config");

	ledc_timer_config(&timer_conf1);

	ledc_channel_config_t ledc_conf1;
	ledc_conf1.channel    = LEDC_CHANNEL_0;
	ledc_conf1.duty       = this->m_duty_x;
	ledc_conf1.gpio_num   = 9;
	ledc_conf1.intr_type  = LEDC_INTR_DISABLE;
	ledc_conf1.speed_mode = LEDC_LOW_SPEED_MODE;
	ledc_conf1.timer_sel  = LEDC_TIMER_0;
	ESP_LOGD(tag, "channel config1");

	ledc_channel_config(&ledc_conf1);


	ledc_channel_config_t ledc_conf2;
	ledc_conf2.channel    = LEDC_CHANNEL_1;
	ledc_conf2.duty       = this->m_duty_y;
	ledc_conf2.gpio_num   = 7;
	ledc_conf2.intr_type  = LEDC_INTR_DISABLE;
	ledc_conf2.speed_mode = LEDC_LOW_SPEED_MODE;
	ledc_conf2.timer_sel  = LEDC_TIMER_0;
	ESP_LOGD(tag, "channel config2");

	ledc_channel_config(&ledc_conf2);

    ESP_LOGD(tag, "Setting initial X,Y to: %d, %d\n", this->m_duty_x, this->m_duty_y);

    // set initial x, y pos
	ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, this->m_duty_x);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, this->m_duty_y);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
}
