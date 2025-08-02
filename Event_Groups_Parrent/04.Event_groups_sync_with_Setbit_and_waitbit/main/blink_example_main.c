#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define BIT_TASK_A   (1 << 0)
#define BIT_TASK_B   (1 << 1)
#define BIT_TASK_C   (1 << 2)
#define ALL_BITS     (BIT_TASK_A | BIT_TASK_B | BIT_TASK_C)

EventGroupHandle_t xEventGroup;

void TaskA(void *pvParameters) {

    for (;;)
    {
        printf("Task A: waiting for 100ms before setting bit...\n");
        vTaskDelay(pdMS_TO_TICKS(100)); // Giả lập đến sớm

        printf("Task A: set bit and wait\n");
        xEventGroupSetBits(xEventGroup, BIT_TASK_A);

        EventBits_t bits = xEventGroupWaitBits(
            xEventGroup,
            ALL_BITS,
            pdTRUE, // Clear bits on exit
            pdTRUE, // Wait for ALL bits
            portMAX_DELAY
        );

        printf("Task A: resumed with bits = 0x%lX\n", bits);
    }
    // vTaskDelete(NULL);
}

void TaskB(void *pvParameters) {

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(200)); // Giả lập đến sớm

        printf("Task B: set bit and wait\n");
        xEventGroupSetBits(xEventGroup, BIT_TASK_B);

        EventBits_t bits = xEventGroupWaitBits(
            xEventGroup,
            ALL_BITS,
            pdTRUE,
            pdTRUE,
            portMAX_DELAY
        );

        printf("Task B: resumed with bits = 0x%lX\n", bits);
    }
    // vTaskDelete(NULL);
}

void TaskC(void *pvParameters) {

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(500)); // Giả lập đến trễ hơn Task A & B

        printf("Task C: set bit only\n");
        
        xEventGroupSetBits(xEventGroup, BIT_TASK_C);

        printf("Task C: now wait for ALL bits\n");
        EventBits_t bits = xEventGroupWaitBits(
            xEventGroup,
            ALL_BITS,
            pdTRUE,
            pdTRUE,
            pdMS_TO_TICKS(portMAX_DELAY) // Timeout để minh họa lỗi
        );

        if ((bits & ALL_BITS) == ALL_BITS) {
            printf("Task C: synchronized successfully!\n");
        } else {
            printf("Task C: sync FAILED! bits = 0x%lX\n", bits);
        }
    }

    // vTaskDelete(NULL);
}

void app_main() {
    xEventGroup = xEventGroupCreate();

    xTaskCreate(TaskA, "TaskA", 2048, NULL, 3, NULL);
    xTaskCreate(TaskB, "TaskB", 2048, NULL, 3, NULL);
    xTaskCreate(TaskC, "TaskC", 2048, NULL, 3, NULL);
}
