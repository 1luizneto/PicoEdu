#include <stdio.h>             // Biblioteca padrão para entrada/saída
#include "inc/menu.h"          // Cabeçalho do sistema de menu
#include "inc/wifi.h"          // Cabeçalho para funções de WiFi
#include "pico/multicore.h"    // Biblioteca para manipulação de múltiplos núcleos no Raspberry Pi Pico


/*
    Algumas considerações ao utilizar o raspeberry:
    1 - Para conectar ao wifi é necessário utilizar o PUTTY para se conectar ao seu WIFI
    2 - Para acessar o site assim como o ThingSpeak é necessário pegar o ip do RP2040 quando ele se conectar a sua rede, mas também tem uma opção no menu que mostra o ip atual
    3 - No código da Matriz jogo pra salvar a posição do cursor é necessário apertar o botão A
    4 - Se o ssid do wifi for escrito errado é necessário apertar o reset da placa, mas se for a senha vai ter uma opção para digitar denovo
*/

/*
 * Função principal do programa.
 * Inicializa o sistema, configura o display e inicia o WiFi em um núcleo separado.
 */
int main()
{
    // Inicializa todas as interfaces padrão de entrada/saída (UART, USB, etc.)
    stdio_init_all();

    // Inicializa o display SSD1306 (OLED)
    ssd1306_Init();

    // Inicia a função WIFI_Init() no segundo núcleo do Raspberry Pi Pico
    // Isso permite que as operações de WiFi sejam executadas em paralelo com o restante do programa
    multicore_launch_core1(WIFI_Init);

    // Loop principal do programa
    while(1)
    {
        // Exibe a tela inicial do menu com a opção 1 selecionada
        home(1);
    }
}
