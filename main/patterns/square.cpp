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

#include "square.h"

void Square::getNextCorner(int &newDutyX, int &newDutyY){
    static int index = 0;
    switch (index) {
        case 0:
            newDutyX = this->m_min_value;
            newDutyY = this->m_min_value;
            break;
        case 1:
            newDutyX = this->m_min_value;
            newDutyY = this->m_max_value;
            break;
        case 2:
            newDutyX = this->m_max_value;
            newDutyY = this->m_max_value;
            break;
         case 3:
            newDutyX = this->m_max_value;
            newDutyY = this->m_min_value;
            break;
    }
    index ++;
    if (index > 3){
        index = 0;
    }
}

void Square::draw(){

    int sweepDuration   = 1000; // milliseconds (ms)
    int valueChangeRate = 20; // msecs
    int changesPerSweep = sweepDuration / valueChangeRate;// 1000/20 -> 50

    int delay = 500;
    int newDutyX = this->m_duty_x;
    int newDutyY = this->m_duty_y;
    getNextCorner(newDutyX, newDutyY);
    int changeDeltaX = (int) (newDutyX - this->m_duty_x) / changesPerSweep;
    int changeDeltaY = (int) (newDutyY - this->m_duty_y) / changesPerSweep;
    ESP_LOGI(tag, "newdutyX=%d, newdutyY=%d\n", newDutyX, newDutyY);

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
        ESP_LOGI(tag, "dutyX=%d, dutyY=%d\n", this->m_duty_x, this->m_duty_y);
    }
    this->m_duty_x = newDutyX;
    this->m_duty_y = newDutyY;
    vTaskDelay(delay/portTICK_PERIOD_MS);
}