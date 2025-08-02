#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include <stdlib.h>

#define BIT_TEMP   (1 << 0)
#define BIT_HUMI   (1 << 1)
#define BIT_LIGHT  (1 << 2)
#define ALL_BITS   (BIT_TEMP | BIT_HUMI | BIT_LIGHT)

EventGroupHandle_t xSyncGroup;

float temperature = 0.0;
float humidity = 0.0; 
float light = 0.0;

float get_random_float(float min, float max) {
    return min + (rand() / (float)(RAND_MAX / (max - min)));
}

void TaskTemp(void *pvParameters)
{
    int max_collection_temp = 5;
    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("get temperature data \n");
        for (int i=0; i<max_collection_temp; i++) {
            temperature += get_random_float(20.0, 30.0);
        }

        temperature /= max_collection_temp; // Average temperature
        printf("Temp: %f, set bit...\n", temperature);

        xEventGroupSync(xSyncGroup, BIT_TEMP, ALL_BITS, portMAX_DELAY);
    }
}

void TaskHumi(void *pvParameters)
{
    int max_collection_humi = 5;
    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
        printf("get humi data \n");
        for (int i=0; i< max_collection_humi; i++) {
            humidity += get_random_float(0.00, 100.0);
        }

        humidity /= max_collection_humi; // Average temperature
        printf("humidity: %f, set bit...\n", humidity);

        xEventGroupSync(xSyncGroup, BIT_HUMI, ALL_BITS, portMAX_DELAY);
    }
}

void TaskLight(void *pvParameters)
{
    int max_collection_light = 5;
    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(3000));

        printf("get light data \n");
        for (int i=0; i< max_collection_light; i++) {
            light += get_random_float(0.00, 100.0);
        }

        light /= max_collection_light; // Average temperature
        printf("light: %f, set bit...\n", light);


        xEventGroupSync(xSyncGroup, BIT_LIGHT, ALL_BITS, portMAX_DELAY);
    }
}

void data_processing_task(void *param)
{
    while (1)
    {
        printf("Waiting for all sensors...\n\n");

        EventBits_t bits = xEventGroupWaitBits(
            xSyncGroup,         // Event group
            ALL_BITS,           // Bit cần chờ
            pdTRUE,             // Clear bits sau khi nhận (xóa để lần sau còn chờ tiếp)
            pdTRUE,             // Chờ đủ tất cả bit
            portMAX_DELAY);     // Không timeout

        if((bits & ALL_BITS) == ALL_BITS)
        {
            printf("[Processing] All 3 sensors ready -> Process data now!\n\n");
        }
    }
}

void app_main()
{
    // Tạo Event Group
    xSyncGroup = xEventGroupCreate();

    // Tạo các task cảm biến và xử lý
    xTaskCreate(TaskTemp, "TaskTemp", 2048, NULL, 5, NULL);
    xTaskCreate(TaskLight, "TaskLight", 2048, NULL, 5, NULL);
    xTaskCreate(TaskHumi, "TaskHumi", 2048, NULL, 5, NULL);
    xTaskCreate(data_processing_task, "DataProcessor", 4096, NULL, 6, NULL);
}
