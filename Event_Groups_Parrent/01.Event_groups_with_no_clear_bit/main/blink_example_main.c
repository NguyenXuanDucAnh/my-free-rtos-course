/*
Tình huống giả định:

Task A mô phỏng cảm biến nhiệt độ gửi tín hiệu đã có dữ liệu.

Task B mô phỏng cảm biến độ ẩm gửi tín hiệu đã sẵn sàng.

Task C chờ cả hai cảm biến đã sẵn sàng rồi mới xử lý dữ liệu.


*/


#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

// Định nghĩa bit đại diện cho từng sự kiện
#define BIT_TEMP_READY    (1 << 8)  // Bit 8 - dữ liệu nhiệt độ sẵn sàng
#define BIT_HUMI_READY    (1 << 9)  // Bit 9 - dữ liệu độ ẩm sẵn sàng

// Tạo biến event group toàn cục
EventGroupHandle_t sensor_event_group;

void sensor_task_temp(void *param)
{
    while (1)
    {
        // Giả lập việc đọc xong nhiệt độ (VD: sau 2s)
        vTaskDelay(pdMS_TO_TICKS(12000));
        printf("Temperature ready\n");

        // Set bit nhiệt độ đã sẵn sàng
        xEventGroupSetBits(sensor_event_group, BIT_TEMP_READY);
    }
}

void sensor_task_humi(void *param)
{
    while (1)
    {
        // Giả lập việc đọc xong độ ẩm (VD: sau 3s)
        vTaskDelay(pdMS_TO_TICKS(10000));
        printf("Humidity ready\n");

        // Set bit độ ẩm đã sẵn sàng
        xEventGroupSetBits(sensor_event_group, BIT_HUMI_READY);
    }
}

void data_processing_task(void *param)
{
    while (1)
    {
        printf("Waiting for both sensors...\n");

        // Chờ cả hai bit (nhiệt độ và độ ẩm) được set
        EventBits_t bits = xEventGroupWaitBits(
            sensor_event_group,               // Event group đang chờ
            BIT_TEMP_READY | BIT_HUMI_READY,  // Các bit cần chờ
            pdFALSE,   // Không Xóa các bit sau khi nhận để xem tình huống gì sẽ xảy ra
            pdTRUE,   // Chờ TẤT CẢ các bit được set
            portMAX_DELAY);  // Không timeout, chờ vô thời hạn

        if ((bits & (BIT_TEMP_READY | BIT_HUMI_READY)) ==
            (BIT_TEMP_READY | BIT_HUMI_READY))
        {
            printf("====> Both sensors are ready, processing data... %lx\n", bits);
            // Xử lý dữ liệu ở đây
        }


        vTaskDelay(pdMS_TO_TICKS(1000)); // delay một chút để tránh printf ra quá nhanh và tránh waitchdog timer
    }
}

void app_main()
{
    // Tạo Event Group
    sensor_event_group = xEventGroupCreate();

    // Tạo các task cảm biến và xử lý
    xTaskCreate(sensor_task_temp, "Temp Sensor", 2048, NULL, 5, NULL);
    xTaskCreate(sensor_task_humi, "Humi Sensor", 2048, NULL, 5, NULL);
    xTaskCreate(data_processing_task, "Data Processor", 4096, NULL, 6, NULL);
}
