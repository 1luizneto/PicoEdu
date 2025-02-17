#ifndef __NEOPIXEL_INC
#define __NEOPIXEL_INC

#include <stdlib.h>
#include "pico/stdlib.h"
#include "ws2818b.pio.h"
#include "inc/menu.h"

/**
 * @brief Estrutura que representa um pixel no formato GRB
 */
struct pixel_t {
  uint8_t G, R, B; // Ordem: verde, vermelho e azul.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Alias para clareza.

// Declaração do buffer de pixels que formam a matriz.
static npLED_t *leds;
static uint led_count;

// Variáveis para uso da máquina PIO.
static PIO np_pio;
static uint np_sm;

void npInit(uint pin, uint amount);
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);
void npClear(void);
void npWrite(void);
void Matriz_RGB(void);
void Matriz_OFF(void); 

/**
 * @brief Inicializa o controlador da matriz de LEDs
 * @param pin Pino GPIO conectado ao DIN da matriz
 * @param amount Quantidade de LEDs na matriz
 */
void npInit(uint pin, uint amount) {

  led_count = amount;
  leds = (npLED_t *)calloc(led_count, sizeof(npLED_t));

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  np_sm = pio_claim_unused_sm(np_pio, false);
  if (np_sm < 0) {
    np_pio = pio1;
    np_sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, np_sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < led_count; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

/**
 * @brief Define a cor de um LED específico
 * @param index Índice do LED (0 a led_count-1)
 * @param r Intensidade do vermelho (0-255)
 * @param g Intensidade do verde (0-255)
 * @param b Intensidade do azul (0-255)
 */
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].G = g;  // primeiro o valor do verde
  leds[index].R = r;  // depois o valor do vermelho
  leds[index].B = b;  // por fim, o valor do azul
}

/**
 * @brief Desliga todos os LEDs (limpa o buffer)
 */
void npClear() {
  for (uint i = 0; i < led_count; ++i)
    npSetLED(i, 0, 0, 0);
}


/**
 * @brief Envia os dados do buffer para a matriz de LEDs
 */
void npWrite() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < led_count; ++i) {
    pio_sm_put_blocking(np_pio, np_sm, leds[i].G);
    pio_sm_put_blocking(np_pio, np_sm, leds[i].R);
    pio_sm_put_blocking(np_pio, np_sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

/**
 * Converte uma cor do espaço HSV para RGB.
 * h, s, v variam de 0 a 255.
 */
static void hsv_to_rgb(uint8_t h, uint8_t s, uint8_t v, uint8_t *r, uint8_t *g, uint8_t *b) {
  if (s == 0) {
    // Sem saturação: cinza
    *r = v;
    *g = v;
    *b = v;
    return;
  }
  
  uint8_t region = h / 43;
  uint8_t remainder = (h - (region * 43)) * 6;

  uint8_t p = (v * (255 - s)) / 255;
  uint8_t q = (v * (255 - ((s * remainder) / 255))) / 255;
  uint8_t t = (v * (255 - ((s * (255 - remainder)) / 255))) / 255;

  switch(region) {
    case 0:
      *r = v; *g = t; *b = p;
      break;
    case 1:
      *r = q; *g = v; *b = p;
      break;
    case 2:
      *r = p; *g = v; *b = t;
      break;
    case 3:
      *r = p; *g = q; *b = v;
      break;
    case 4:
      *r = t; *g = p; *b = v;
      break;
    default:
      *r = v; *g = p; *b = q;
      break;
  }
}


/**
 * @brief Efeito de animação RGB contínuo na matriz
 */
void Matriz_RGB() {
  npInit(7, 25);  // Inicializa a matriz (aloca buffer, configura PIO, etc.)
  uint8_t hue = 0;

  while (true) {
    // Converte o valor de hue para uma cor RGB
    uint8_t r, g, b;
    hsv_to_rgb(hue, 255, 255, &r, &g, &b);

    // Atualiza todos os LEDs com a mesma cor
    for (uint i = 0; i < led_count; i++) {
      npSetLED(i, r, g, b);
    }

    npWrite();       // Envia os dados para os LEDs
    sleep_ms(50);    // Controla a velocidade da animação
    hue++;           // Incrementa o hue para o próximo ciclo

    // Verifica se o botão (no GPIO 22) foi pressionado
    if(gpio_get(22) == 0) 
    {
      sleep_ms(200);      // Aguarda estabilização do sinal do botão

      Matriz_OFF();

      // Retorna ao menu ou a outra função (por exemplo, a tela inicial)
      matriz_home();
      break;
    }
  }
}

/**
 * @brief Desliga a matriz e libera recursos
 */
void Matriz_OFF() 
{
      // Limpa a matriz
      npClear();
      npWrite();

      // Desabilita a state machine para liberar o hardware
      pio_sm_set_enabled(np_pio, np_sm, false);

      // Libera o buffer de LEDs
      free(leds);
      leds = NULL;
}

/**
 * @brief Lê a posição atual do joystick
 * @param x Ponteiro para armazenar posição X (0-4)
 * @param y Ponteiro para armazenar posição Y (0-4)
 */
void joystick(int *x, int *y) 
{
    static bool initialized = false;
    if (!initialized) 
    {
        adc_gpio_init(26); // ADC0
        adc_gpio_init(27); // ADC1
        initialized = true;
    }
    adc_select_input(0); // Lê eixo X
    uint16_t raw_x = adc_read();
    adc_select_input(1); // Lê eixo Y
    uint16_t raw_y = adc_read();

    // Mapeia o valor ADC (0-4095) para posição (0-4)
    *x = (raw_x * 5) / 4096;
    *y = (raw_y * 5) / 4096;
}

/**
 * @brief Calcula o índice linear para matriz serpentina
 * @param col Coluna (0-4)
 * @param row Linha (0-4)
 * @return Índice linear correspondente (0-24)
 */
int getIndex(int col, int row) {
    int index;
    if (row % 2 == 0) {
        // Linha par: da esquerda para a direita
        index = row * 5 + col;
    } else {
        // Linha ímpar: da direita para a esquerda
        index = row * 5 + (4 - col);
    }
    return index;
}

/**
 * @brief Modo interativo de controle de LED com joystick
 * @note Usa GPIO5 para botão de ação e GPIO22 para retorno ao menu
 */
void matriz_jogo() {
    npInit(7, 25); // Inicializa a matriz de 25 LEDs na porta 7
    int x = 2, y = 1; // Posição inicial (em uma lógica 0 a 4)
    int matriz[5][5][3] = {0};   // Matriz de cores para cada LED [linha][coluna][RGB]
    int matriz_set[5][5] = {0};    // Matriz auxiliar para marcar os LEDs "fixados"

    gpio_init(5);
    gpio_set_dir(5, GPIO_IN);
    gpio_pull_up(5);

    while (1) {
        // Atualiza a posição com base no joystick
        joystick(&x, &y);

        // Se o botão for pressionado, marca a posição como "fixada"
        if (gpio_get(5) == 0) {
            matriz_set[4 - y][4 - x] = 1;
        }

        // Limpa os LEDs que não estão fixados
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (matriz_set[i][j] == 0) {
                    matriz[i][j][0] = 0;
                    matriz[i][j][1] = 0;
                    matriz[i][j][2] = 0;
                }
            }
        }

        // Atualiza a posição atual do joystick com um LED vermelho "brilhante"
        matriz[4 - y][4 - x][0] = 200; // Vermelho
        matriz[4 - y][4 - x][1] = 0;
        matriz[4 - y][4 - x][2] = 0;

        // Desenha a matriz: converte a posição (col, linha) para índice 1D e atualiza o LED
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                int posicao = getIndex(j, i);
                npSetLED(posicao, matriz[i][j][0], matriz[i][j][1], matriz[i][j][2]);
            }
        }

        npWrite();
        sleep_ms(100); // Pequeno atraso para estabilidade

        // Se o botão estiver pressionado, encerra o modo "jogo" e retorna ao menu
        if (gpio_get(22) == 0) 
        {
            DEBOUNCE;
            Matriz_OFF();
            matriz_home();
            break;
        }
    }
}


#endif
