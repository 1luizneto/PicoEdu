#include "inc/joystick.h"

const int LED_B = 12;                   
const int LED_R = 11;                    
const float DIVIDER_PWM = 16.0;          
const uint16_t PERIOD = 4096;            
uint16_t led_b_level, led_r_level = 100; 
uint slice_led_b, slice_led_r;           

/**
 * @brief Configura o PWM para um LED específico.
 * 
 * Esta função configura o pino de um LED como saída PWM, define o divisor de clock,
 * o período do PWM e o nível inicial de brilho. Também habilita o PWM no slice correspondente.
 * 
 * @param led Pino do LED a ser configurado.
 * @param slice Ponteiro para armazenar o slice do PWM associado ao LED.
 * @param level Nível inicial de brilho do LED (0-4095).
 */
void setup_pwm_led(uint led, uint *slice, uint16_t level)
{
  gpio_set_function(led, GPIO_FUNC_PWM); // Configura o pino do LED como saída PWM
  *slice = pwm_gpio_to_slice_num(led);   // Obtém o slice do PWM associado ao pino do LED
  pwm_set_clkdiv(*slice, DIVIDER_PWM);   // Define o divisor de clock do PWM
  pwm_set_wrap(*slice, PERIOD);          // Configura o valor máximo do contador (período do PWM)
  pwm_set_gpio_level(led, level);        // Define o nível inicial do PWM para o LED
  pwm_set_enabled(*slice, true);         // Habilita o PWM no slice correspondente ao LED
}

/**
 * @brief Configuração inicial dos LEDs e do joystick.
 * 
 * Esta função chama a configuração do PWM para os LEDs azul e vermelho.
 */
void setup()
{                                 
  setup_pwm_led(LED_B, &slice_led_b, led_b_level); // Configura o PWM para o LED azul
  setup_pwm_led(LED_R, &slice_led_r, led_r_level); // Configura o PWM para o LED verde
}

/**
 * @brief Lê os valores dos eixos X e Y do joystick.
 * 
 * Esta função realiza a leitura dos valores analógicos dos eixos X e Y do joystick
 * usando o conversor analógico-digital (ADC).
 * 
 * @param vrx_value Ponteiro para armazenar o valor do eixo X (0-4095).
 * @param vry_value Ponteiro para armazenar o valor do eixo Y (0-4095).
 */
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value)
{
  // Leitura do valor do eixo X do joystick
  adc_select_input(0); // Seleciona o canal ADC para o eixo X
  sleep_us(2);                     // Pequeno delay para estabilidade
  *vrx_value = adc_read();         // Lê o valor do eixo X (0-4095)

  // Leitura do valor do eixo Y do joystick
  adc_select_input(1); // Seleciona o canal ADC para o eixo Y
  sleep_us(2);                     // Pequeno delay para estabilidade
  *vry_value = adc_read();         // Lê o valor do eixo Y (0-4095)
}

/**
 * @brief Função principal para controlar os LEDs com o joystick.
 * 
 * Esta função ajusta o brilho dos LEDs azul e vermelho com base nos valores dos eixos
 * X e Y do joystick. Também verifica se o botão no pino 22 foi pressionado para retornar
 * ao menu principal.
 */
void joystick_rgb()
{
  uint16_t vrx_value, vry_value, sw_value; // Variáveis para armazenar os valores do joystick (eixos X e Y) e botão
  setup();                                 // Chama a função de configuração
  // Loop principal
  while (1)
  {
    joystick_read_axis(&vrx_value, &vry_value); // Lê os valores dos eixos do joystick
    // Ajusta os níveis PWM dos LEDs de acordo com os valores do joystick
    pwm_set_gpio_level(LED_B, vrx_value); // Ajusta o brilho do LED azul com o valor do eixo X
    pwm_set_gpio_level(LED_R, vry_value); // Ajusta o brilho do LED verde com o valor do eixo Y

    // Pequeno delay antes da próxima leitura
    sleep_ms(100); // Espera 100 ms antes de repetir o ciclo

    if(gpio_get(22) == 0) 
    {
        DEBOUNCE;
        
        pwm_set_enabled(slice_led_b, false);
        pwm_set_enabled(slice_led_r, false);

        //configura o pino LED_B como saída digital e o coloca em nível baixo
        gpio_set_function(LED_B, GPIO_FUNC_SIO);
        gpio_set_dir(LED_B, GPIO_OUT);
        gpio_put(LED_B, 0);
    
        //configura o pino LED_R como saída digital e o coloca em nível baixo
        gpio_set_function(LED_R, GPIO_FUNC_SIO);
        gpio_set_dir(LED_R, GPIO_OUT);
        gpio_put(LED_R, 0);
        
        joystick_home();
        //home(1);
        break;
    }
  }
}

/**
 * @brief Exibe a posição do joystick no display OLED.
 * 
 * Esta função lê os valores dos eixos X e Y do joystick e os exibe em retângulos
 * no display OLED. Também verifica se o botão no pino 22 foi pressionado para retornar
 * ao menu principal.
 */
void joystick_pos(void) 
{
    uint16_t vrx_value, vry_value;  // Variáveis para os eixos
    char buffer[32];                // Buffer para armazenar a string
    
    // Layout:
    const uint8_t headerY = 2;      // Ajustado para descer um pouco (evita corte na borda superior)
    const uint8_t headerX = 25;     // Ajuste para centralizar o header ("JOYSTICK POS")
    char *headerText = "JOYSTICK POS";

    // Coordenadas dos retângulos para os eixos:
    // Para o eixo X: retângulo de (10,16) até (10+50, 16+30)
    const uint8_t rectX_x = 10;
    const uint8_t rectX_y = 16;
    const uint8_t rectWidth = 50;
    const uint8_t rectHeight = 30;
    // Para o eixo Y: retângulo de (68,16) até (68+50, 16+30)
    const uint8_t rectY_x = 68;
    const uint8_t rectY_y = 16;

    while (1)
    {
        // Lê os valores dos eixos do joystick
        joystick_read_axis(&vrx_value, &vry_value);

        // Preenche o display com fundo branco
        ssd1306_Fill(White);

        // Exibe o cabeçalho
        ssd1306_SetCursor(headerX, headerY);
        ssd1306_WriteString(headerText, Font_7x10, Black);

        // Desenha os retângulos para os valores X e Y
        // Note que a função ssd1306_DrawRectangle() recebe os pontos superior esquerdo e inferior direito.
        ssd1306_DrawRectangle(rectX_x, rectX_y, rectX_x + rectWidth, rectX_y + rectHeight, Black);
        ssd1306_DrawRectangle(rectY_x, rectY_y, rectY_x + rectWidth, rectY_y + rectHeight, Black);

        // Exibe o valor do eixo X centralizado no retângulo:
        sprintf(buffer, "X: %u", vrx_value);
        // Calcula a largura do texto (assumindo que Font_7x10.width e Font_7x10.height estão definidos)
        uint8_t textWidth = strlen(buffer) * Font_7x10.width;
        uint8_t textX = rectX_x + (rectWidth - textWidth) / 2;
        uint8_t textY = rectX_y + (rectHeight - Font_7x10.height) / 2;
        ssd1306_SetCursor(textX + 1, textY);
        ssd1306_WriteString(buffer, Font_7x10, Black);

        // Exibe o valor do eixo Y centralizado no retângulo:
        sprintf(buffer, "Y: %u", vry_value);
        textWidth = strlen(buffer) * Font_7x10.width;
        textX = rectY_x + (rectWidth - textWidth) / 2;
        textY = rectY_y + (rectHeight - Font_7x10.height) / 2;
        ssd1306_SetCursor(textX + 1, textY);
        ssd1306_WriteString(buffer, Font_7x10, Black);

        // Atualiza o display para aplicar as alterações
        ssd1306_UpdateScreen();

        // Verifica se o pino 22 foi pressionado para sair da função
        if(gpio_get(22) == 0) 
        {
            DEBOUNCE;
            joystick_home();
            break;
        }

        POLLING_TIME;
    }
}


