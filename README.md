# freeRTOSC2U1
Tarefa do Capítulo 2 da Unidade 1

### FreeRTOS para linux tutorial baseado no https://github.com/IsacBM/FreeRTOS-Configurar?tab=readme-ov-file


Uso 

1. Baixar FreeRTOS. Link: https://github.com/FreeRTOS/FreeRTOS/releases/download/202212.01/FreeRTOSv202212.01.zip
2. Descompactem o arquivo Zip.
3. mova o arquivo descompactado para a /home/user – user é o nome do usuário 
4. Entre na pasta \FreeRTOSv202212.01, depois na pasta \FreeRTOS e logo em seguida na pasta \Source
5. Com o botão direto entre no terminal nesse caminho 
6. digite pwd e copie o caminho 

#### Agora para criar a variável de ambiente : 
1. digite nano ~/.bashrc
2. no final adicione :  export FREERTOS_KERNEL_PATH="caminho copiado"
4. Pressione Ctrl + O para salvar.
   Pressione Enter 
   Pressione Ctrl + X para sair.
5. verifique se deu certo : echo $FREERTOS_KERNEL_PATH
6. inicie a variável source ~/.bashrc
7. abra o vscode com code .
8. crie o um projeto blink pela extensão do pi pico 

9. Passos 7, 8, e 9 no github https://github.com/IsacBM/FreeRTOS-Configurar?tab=readme-ov-file

