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
#include "square.h"




Square square_pattern;
RandomWalk random_walk_pattern;

void laser_servos_task(void *ignore) {

    esp_log_level_set("*", ESP_LOG_DEBUG);     // enable INFO logs from DHCP client
    ESP_LOGI(tag, "Starting the laser servos");
    ESP_LOGD(tag, "Debugging turned ON");

    while(1) {
	    //moveToRandomPos(&dutyX, &dutyY, maxValue,  minValue);
	    random_walk_pattern.draw();
	} // End loop forever

	vTaskDelete(NULL);
}