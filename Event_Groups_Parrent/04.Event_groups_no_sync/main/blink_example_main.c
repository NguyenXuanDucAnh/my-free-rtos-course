#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define BIT_TEMP   (1 << 0)
#define BIT_HUMI   (1 << 1)
#define BIT_LIGHT  (1 << 2)
#define ALL_BITS   (BIT_TEMP | BIT_HUMI | BIT_LIGHT)

EventGroupHandle_t xSyncGroup;

void TaskTemp(void *pvParameters)
{
    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("Temp done, set bit...\n");

        xEventGroupSetBits(xSyncGroup, BIT_TEMP);
    }
}

void TaskHumi(void *pvParameters)
{
    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
        printf("Humi done, set bit...\n");

        xEventGroupSetBits(xSyncGroup, BIT_HUMI);
    }
}

void TaskLight(void *pvParameters)
{
    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(3000));
        printf("Light done, set bit...\n");

        xEventGroupSetBits(xSyncGroup, BIT_LIGHT);
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
