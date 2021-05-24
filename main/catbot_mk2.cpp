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
#include "sdkconfig.h"
#include "esp_log.h"

// project files
#include "leds.h"
#include "laser_servos.h"



extern "C" void app_main()
{	
        esp_log_level_set("catbot_mk2", ESP_LOG_DEBUG);

        xTaskCreate(&laser_servos_task,"laser_servos_task",2048,NULL,5,NULL);
        xTaskCreate(&leds_task,"leds_task",2048,NULL,5,NULL);

        printf("sweep tasks  started\n");
}
