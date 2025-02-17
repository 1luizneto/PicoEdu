#ifndef MICROFONE_H
#define MICROFONE_H

#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/dma.h"
//#include "src/neopixel.c"
#include "inc/menu.h"

// Pino e canal do microfone no ADC.
#define MIC_CHANNEL 2
#define MIC_PIN (26 + MIC_CHANNEL)

// Parâmetros e macros do ADC.
#define ADC_CLOCK_DIV 96.f
#define SAMPLES 200
#define ADC_ADJUST(x) (x * 3.3f / (1 << 12u) - 1.65f)
#define ADC_MAX 3.3f
#define ADC_STEP (3.3f/5.f)

// Pino e número de LEDs da matriz de LEDs.
#define LED_PIN 7
#define LED_COUNT 25

#define TOTAL_ROWS 5
#define TOTAL_COLS 5
#define MIDDLE_COL 2

#define LED_INDEX(row, col) (((row) * TOTAL_COLS) + (col))

#define my_abs(x) ((x < 0) ? (-x) : (x))

#define MIC_THRESHOLD 1000.0      // Limiar para considerar que há som; ajuste conforme necessário
#define HEADER_Y 1              // Posição Y para o cabeçalho
#define RECT_X 10               // Posição X do retângulo
#define RECT_Y 20               // Posição Y do retângulo
#define RECT_WIDTH (SSD1306_WIDTH - 20)
#define RECT_HEIGHT 22


// Canal e configurações do DMA
extern uint dma_channel;
extern dma_channel_config dma_cfg;

// Buffer de amostras do ADC.
extern uint16_t adc_buffer[SAMPLES];

// Declaração das funções.
void sample_mic();
float mic_power();
uint8_t get_intensity(float v);
void mic_matriz(void);
void mic_test(void);

#endif
