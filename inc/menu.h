#ifndef MENU_H
#define MENU_H

/*
 * Este arquivo de cabeçalho define a interface do sistema de menus,
 * incluindo as declarações de funções para os demos de joystick, buzzer,
 * matriz de LEDs, microfone, display e wifi.
 * Também inclui as bibliotecas necessárias e diversas macros de configuração.
 */

#include <stdint.h>   // Tipos inteiros padrão
#include <stdbool.h>  // Tipo booleano (true/false)
#include <string.h>   // Funções para manipulação de strings
#include <stdio.h>    // Funções de entrada/saída padrão

#include "pico/stdlib.h"  // Biblioteca padrão do Pico SDK
#include "inc/display.h"  // Funções e definições para o display
#include "inc/fonts.h"    // Definições de fontes para o display
#include "inc/icons.h"    // Definições de ícones/bitmaps para o display
#include "hardware/adc.h" // Interface de hardware para o ADC
#include "inc/joystick.h" // Funções de interface para o joystick
#include "inc/buzzer.h"   // Funções para controle do buzzer
#include "inc/microfone.h"// Funções para o microfone
#include "inc/wifi.h"     // Funções para controle do wifi
// #include "inc/global_lock.h"  // (Opcional) Mecanismo de trava global (comentado)
// #include "src/neopixel.c"     // (Opcional) Funções para Neopixel (comentado)

/*
 * Macros definindo os índices das opções nos menus.
 * OBSERVAÇÃO: A macro MENU é redefinida várias vezes para cada contexto,
 * sempre com o valor 2, representando a opção "Menu Principal".
 */

// Menu do Joystick
#define JOYSTICK_POS 0
#define JOYSTICK_RGB 1
#define MENU 2

// Menu da Matriz (LEDs)
#define MATRIZ_1 0
#define MATRIZ_2 1
#define MENU 2

// Menu do Buzzer
#define BUZZER_1 0
#define BUZZER_2 1
#define MENU 2

// Menu do Microfone
#define MIC_1 0
#define MIC_2 1
#define MENU 2

// Menu do Display
#define DISPLAY_1 0
#define DISPLAY_2 1
#define MENU 2

// Menu do Wifi
#define WIFI_1 0
#define WIFI_2 1
#define MENU 2

/*
 * Macros de tempo:
 * - DEBOUNCE: Aguarda 200 ms, usado para debouncing do botão.
 * - POLLING_TIME: Aguarda 10 ms, usado para o delay entre as leituras.
 */
#define DEBOUNCE        sleep_ms(200)
#define POLLING_TIME    sleep_ms(10)

/*
 * Outras macros de configuração:
 * - TOTAL_OPTIONS: Número total de opções no menu principal.
 * - ADC_PIN: Pino utilizado para leitura do ADC (por exemplo, do joystick).
 * - JOYSTICK_BUTTON: Pino GPIO usado para o botão do joystick.
 * - JOYSTICK_LEFT_THRESHOLD e JOYSTICK_RIGHT_THRESHOLD: Limiares do ADC para detectar movimentos para a esquerda ou direita.
 */
#define TOTAL_OPTIONS 6
#define ADC_PIN 27
#define JOYSTICK_BUTTON 22
#define JOYSTICK_LEFT_THRESHOLD 1500
#define JOYSTICK_RIGHT_THRESHOLD 2500

/*
 * Declarações das funções utilizadas no sistema de menus.
 * Essas funções são implementadas em outros arquivos fonte e gerenciam
 * os diversos demos e a navegação do menu principal.
 */
void joystick_screen();
void joystick_home(void);
void home(uint8_t option);
void buzzer_screen(void);
void buzzer_home(void);
void demo_matriz(void);
void matriz_screen(void);
void matriz_home(void);
void mic_screen(void);
void mic_home(void);
void display_screen(void);
void display_home(void);
void wifi_screen(void);
void wifi_home(void);
float read_temperature_sensor(void);
void set_adc_channel(uint8_t channel);

#endif
