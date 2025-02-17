#ifndef MATRIZ_H
#define MATRIZ_H

#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "inc/display.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "pico/binary_info.h"
#include "ws2818b.pio.h"

#define LED_COUNT 25
//#define ADC_UPPER_THRESHOLD_X 3100  // Limite superior do ADC no eixo X direita
//#define ADC_LOWER_THRESHOLD_X 1100  // Limite inferior do ADC no eixo X esquerda
//#define ADC_UPPER_THRESHOLD_Y 2800  // Limite superior do ADC no eixo Y cima
//#define ADC_LOWER_THRESHOLD_Y 1300  // Limite inferior do ADC no eixo Y baixo
#define BUTTON_A 6               // Pino do botão


//extern LED leds[LED_COUNT];
//extern int matriz[5][5][3];

// Funções
//uint low_pass_filter(uint new_value);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npWrite();
void npClear();
void npInit(uint pin, uint amount);
//int getIndex(int x, int y);
//void ledDeslizante();
//void joystick(int *x, int *y);
//void pwmWrite(uint8_t duty_cycle);
//void control_led_with_joystick();
void Matriz_Init();

#endif // MATRIZ_H