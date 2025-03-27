Este projeto implementa um sistema embarcado utilizando FreeRTOS na Raspberry Pi Pico para controlar o estado de um LED com base na leitura de um botão. O sistema é composto por três tarefas principais:

Leitura do Botão: Monitora o estado do botão a cada 100ms e envia o estado para uma fila.

Processamento do Botão: Processa o estado do botão e decide se aciona a tarefa de controle do LED.

Controle do LED: Controla o LED de acordo com o estado do botão (aceso ou apagado).

A comunicação entre as tarefas é feita por meio de filas, permitindo a troca de informações de forma eficiente. O código foi desenvolvido para a plataforma Raspberry Pi Pico e utiliza o FreeRTOS para gerenciar a execução das tarefas.


