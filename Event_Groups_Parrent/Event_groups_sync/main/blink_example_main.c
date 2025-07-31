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
        printf("Temp done, waiting sync...\n");

        xEventGroupSync(xSyncGroup, BIT_TEMP, ALL_BITS, portMAX_DELAY);

    }
}

void TaskHumi(void *pvParameters)
{
    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
        printf("Humi done, waiting sync...\n");

        xEventGroupSync(xSyncGroup, BIT_HUMI, ALL_BITS, portMAX_DELAY);

    }
}

void TaskLight(void *pvParameters)
{
    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(3000));
        printf("Light done, waiting sync...\n");

        xEventGroupSync(xSyncGroup, BIT_LIGHT, ALL_BITS, portMAX_DELAY);

    }
}

void data_processing_task(void *param)
{
    while (1)
    {
        printf("Waiting for both sensors...\n\n");

        // Chờ cả hai bit (nhiệt độ và độ ẩm) được set
        EventBits_t bits = xEventGroupWaitBits(
            xSyncGroup,               // Event group đang chờ
            ALL_BITS,  // Các bit cần chờ
            pdTRUE,   // Xóa các bit sau khi nhận để xem tình huống gì sẽ xảy ra
            pdTRUE,   // Chờ TẤT CẢ các bit được set
            portMAX_DELAY);  // Không timeout, chờ vô thời hạn

        if((bits & ALL_BITS) == ALL_BITS)
        {
            printf("[Processing] All 3 sensors ready -> Process data now!\n");
        }
    }
}

void app_main()
{
    // Tạo Event Group
    xSyncGroup = xEventGroupCreate();

    // Tạo các task cảm biến và xử lý
    xTaskCreate(TaskTemp, "Temp TaskTemp", 2048, NULL, 5, NULL);
    xTaskCreate(TaskLight, "Temp TaskLight", 2048, NULL, 5, NULL);
    xTaskCreate(TaskHumi, "Temp TaskHumi", 2048, NULL, 5, NULL);
    xTaskCreate(data_processing_task, "Data Processor", 4096, NULL, 6, NULL);
}
