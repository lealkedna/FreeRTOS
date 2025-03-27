#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

const uint led_pin_blue = 12;  // Pino do LED
const uint buttonA = 5;        // Pino do Botão A

// Filas para comunicação entre tarefas
QueueHandle_t xButtonQueue;    // Fila entre Tarefa 1 e Tarefa 2
QueueHandle_t xLedQueue;       // Fila entre Tarefa 2 e Tarefa 3

// Estruturas para enviar dados através das filas
typedef struct {
    bool button_state;
} ButtonEvent;

typedef struct {
    bool led_state;
} LedEvent;

// Tarefa 1: Leitura do Botão
void vTask1(void *pvParameters) {
    bool last_current_state = false;
    ButtonEvent event;
    
    for(;;) {
        // Lê o estado do botão
        bool current_state = !gpio_get(buttonA);
        
        if (last_current_state != current_state) {
            event.button_state = current_state;
            
            // Envia o estado do botão para a fila
            if (xQueueSend(xButtonQueue, &event, portMAX_DELAY)) {
                printf("Button State: %d sent to queue\n", current_state);
            }
            
            last_current_state = current_state;
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Tarefa 2: Processamento do Estado do Botão
void vTask2(void *pvParameters) {
    ButtonEvent received_event;
    LedEvent led_event;
    
    for(;;) {
        // Recebe o evento da fila de botões
        if (xQueueReceive(xButtonQueue, &received_event, portMAX_DELAY)) {
            printf("Button Pressed! State: %d\n", received_event.button_state);
            
            // Prepara evento para o LED
            led_event.led_state = received_event.button_state;
            
            // Envia o comando para a fila do LED
            if (xQueueSend(xLedQueue, &led_event, portMAX_DELAY)) {
                printf("LED command sent to queue\n");
            }
        }
    }
}

// Tarefa 3: Controle do LED
void vTask3(void *pvParameters) {
    LedEvent led_event;
    
    for(;;) {
        // Recebe o evento da fila de LED
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
    
    // Cria as filas
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

    // Iniciar o agendador do FreeRTOS
    vTaskStartScheduler();

    // O código não deve chegar aqui
    while(1);
    return 0;
}