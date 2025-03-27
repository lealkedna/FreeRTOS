#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

const uint led_pin_blue = 12;  
const uint buttonA = 5;        

QueueHandle_t xButtonQueue;    
QueueHandle_t xLedQueue;       


typedef struct {
    bool button_state;
} ButtonEvent;

typedef struct {
    bool led_state;
} LedEvent;


void vTask1(void *pvParameters) {
    bool last_current_state = false;
    ButtonEvent event;
    
    for(;;) {
        
        bool current_state = !gpio_get(buttonA);
        
        if (last_current_state != current_state) {
            event.button_state = current_state;
            
           
            if (xQueueSend(xButtonQueue, &event, portMAX_DELAY)) {
                printf("Button State: %d sent to queue\n", current_state);
            }
            
            last_current_state = current_state;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


void vTask2(void *pvParameters) {
    ButtonEvent received_event;
    LedEvent led_event;
    
    for(;;) {
       
        if (xQueueReceive(xButtonQueue, &received_event, portMAX_DELAY)) {
            printf("Button Pressed! State: %d\n", received_event.button_state);
            
            
            led_event.led_state = received_event.button_state;
            
            
            if (xQueueSend(xLedQueue, &led_event, portMAX_DELAY)) {
                printf("LED command sent to queue\n");
            }
        }
    }
}


void vTask3(void *pvParameters) {
    LedEvent led_event;
    
    for(;;) {
        
        if (xQueueReceive(xLedQueue, &led_event, portMAX_DELAY)) {
            gpio_put(led_pin_blue, led_event.led_state);
            
            if (led_event.led_state) {
                printf("LED ligado\n");
            } else {
                printf("LED desligado\n");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

int setup(void) {
    stdio_init_all();
    gpio_init(buttonA);
    gpio_set_dir(buttonA, GPIO_IN);
    gpio_pull_up(buttonA); 
    gpio_init(led_pin_blue);
    gpio_set_dir(led_pin_blue, GPIO_OUT);
    
    
    xButtonQueue = xQueueCreate(5, sizeof(ButtonEvent));
    xLedQueue = xQueueCreate(5, sizeof(LedEvent));
    
    if (xButtonQueue == NULL || xLedQueue == NULL) {
        printf("Erro ao criar as filas!\n");
        return -1;
    }
    return 0;
}

int main() {
    if (setup() != 0) {
        printf("Falha na inicialização\n");
        return -1;
    }
    
    xTaskCreate(vTask1, "Button Read Task", 128, NULL, 1, NULL);
    xTaskCreate(vTask2, "Button Process Task", 128, NULL, 2, NULL);
    xTaskCreate(vTask3, "LED Control Task", 128, NULL, 3, NULL);

    
    vTaskStartScheduler();


    while(1);
    return 0;
}