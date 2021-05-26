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

#include "random_walk.h"

void RandomWalk::draw() {
    int sweepDuration   = rand() % 1800 + 200; // milliseconds (ms)
    int valueChangeRate = 20; // msecs
    int changesPerSweep = sweepDuration / valueChangeRate;// 1000/20 -> 50

    int delay = rand() % 2000;
    int newDutyX = (int) rand() % (this->m_max_value - this->m_min_value) + this->m_min_value;
    int newDutyY = (int) rand() % (this->m_max_value - this->m_min_value) + this->m_min_value;
    int changeDeltaX = (int) (newDutyX - this->m_duty_x) / changesPerSweep;
    int changeDeltaY = (int) (newDutyY - this->m_duty_y) / changesPerSweep;
    ESP_LOGI(tag, "Moving to: %d, %d\n", newDutyX, newDutyY);


    // move to new pos
    for (int i=0; i<changesPerSweep; i++) {

        this->m_duty_x += changeDeltaX;
        this->m_duty_y += changeDeltaY;

        // set x pos
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, this->m_duty_x);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

        // set y pos
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, this->m_duty_y);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);

        vTaskDelay(valueChangeRate/portTICK_PERIOD_MS);
    }
    this->m_duty_x = newDutyX;
    this->m_duty_y = newDutyY;
    vTaskDelay(delay/portTICK_PERIOD_MS);
}