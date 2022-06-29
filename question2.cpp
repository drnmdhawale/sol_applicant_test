#include <stdio.h>

// Constant definitions and HAL functions declarations
#include "main.h"

// FreeRTOS-related headers
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

static volatile int my_value = INIT_VALUE; // see instructions.txt to know what numerical value this corresponds to
static volatile TaskHandle_t my_task;
static volatile QueueHandle_t my_queue;
static volatile SemaphoreHandle_t my_semaphore;

// EXTI8 interrupt handler
extern "C" void EXTI8_IRQHandler()
{
    BaseType_t woken = pdFALSE;
    if (__HAL_GPIO_EXTI_GET_FALLING_IT(NOTIF_Pin)){
        __HAL_GPIO_EXTI_CLEAR_FALLING_IT(NOTIF_Pin);
        my_value /= 12;
        xQueueSendFromISR(my_queue, (int*)&my_value, &woken);
        my_value = 0;
    } else if (__HAL_GPIO_EXTI_GET_RISING_IT(NOTIF_Pin)){
        __HAL_GPIO_EXTI_CLEAR_RISING_IT(NOTIF_Pin);
        my_value = 41;
        vTaskNotifyGiveFromISR((TaskHandle_t)my_task, &woken);
        my_value = 42;
    }
    portYIELD_FROM_ISR(woken);
}

static void task_A(void* arg)
{
    printf("Task A start\r\n");
    for (;;){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        int event = my_value;
        printf("hello %d\r\n", event);
        xQueueSend(my_queue, &event, portMAX_DELAY);
        printf("how are you?\r\n");
        xSemaphoreTake(my_semaphore, portMAX_DELAY);
        printf("goodbye\r\n");
    }
}

static void task_B(void* arg)
{
    printf("Task B start\r\n");
    for (;;){
        int event;
        xQueueReceive(my_queue, &event, portMAX_DELAY);
        printf("got %d\r\n", event);
        switch (event){
        break; case 0:
            printf("impressive\r\n");
            vTaskSuspend(nullptr);
            printf("very nice\r\n");
            xSemaphoreGive(my_semaphore);
        break; case 1:
            printf("not impressive\r\n");
            vTaskSuspend(nullptr);
            xSemaphoreGive(my_semaphore);
            printf("not very nice\r\n");
        break; case 41:
            xSemaphoreGive(my_semaphore);
            printf("great!\r\n");
        break; case 42:
            xSemaphoreGive(my_semaphore);
            printf("the answer\r\n");
        break; default:
            event /= 12;
            vTaskNotifyGive((TaskHandle_t)my_task);
            xQueueSend(my_queue, &event, portMAX_DELAY);
            printf("not the answer\r\n");
        }
        ++my_value;
    }
}

extern void SystemClock_Config();

static void MX_GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOF_CLK_ENABLE();
    GPIO_InitStruct.Pin = NOTIF_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING; // External Interrupt Mode with Rising/Falling edge trigger detection
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    // Enable the interrupt used by the NOTIF pin
    HAL_NVIC_SetPriority(EXTI8_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI8_IRQn);
}

extern void MX_USART2_UART_Init();

int main()
{
    // Reset all peripherals, initialize the flash interface and the systick
    HAL_Init();
    // Configure the system clock
    SystemClock_Config();
    // Initialize all GPIOs
    MX_GPIO_Init();
    // Initialize the UART peripheral used by printf for printing to the user's terminal
    // Note: Printing is a blocking operation. Interrupts, DMA or FreeRTOS function calls are not used for printing.
    MX_USART2_UART_Init();
    
    printf("Ready?\r\n");
    my_queue = xQueueCreate(16, sizeof(int));
    my_semaphore = xSemaphoreCreateBinary();
    xTaskCreate(task_A, "Task A", 1024, nullptr, 6, (TaskHandle_t*)&my_task);
    xTaskCreate(task_B, "Task B", 1024, nullptr, 17, nullptr);
    printf("Start!\r\n");
    vTaskStartScheduler();
    printf("what is this I don't even\r\n");
}
