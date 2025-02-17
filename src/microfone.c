#include "inc/microfone.h"

// Supondo que esses macros já estejam definidos em algum header:
// #define TOTAL_ROWS 5
// #define TOTAL_COLS 5
// #define MIDDLE_COL 2
// #define LED_INDEX(row, col) (((TOTAL_ROWS - 1 - (row)) * TOTAL_COLS) + (col))

uint dma_channel;
dma_channel_config dma_cfg;
uint16_t adc_buffer[SAMPLES];


/*
 * Função: mic_test
 * -----------------
 * Objetivo: Testar o microfone e exibir seu status (ligado/desligado) em um display OLED.
 *
 * Fluxo:
 *   - Configura o ADC e o DMA para coletar amostras do microfone.
 *   - Limpa o display, desenha o cabeçalho ("Teste Mic") e um retângulo para exibir o status.
 *   - Em loop, captura uma amostra, calcula a potência do sinal e, com base num limiar, exibe "microfone on" ou "microfone off".
 *   - Se o botão (pino 22) for pressionado, chama mic_home() para retornar ao menu inicial.
 */

void mic_test(void)
{
    // ***** Configuração do ADC e DMA para o microfone *****
    adc_gpio_init(MIC_PIN);
    adc_select_input(MIC_CHANNEL);

    adc_fifo_setup(
      true,   // habilita FIFO
      true,   // habilita DMA data request
      1,      // número de amostras (ou profundidade do FIFO)
      false,
      false
    );
    adc_set_clkdiv(ADC_CLOCK_DIV);

    dma_channel = dma_claim_unused_channel(true);
    dma_cfg = dma_channel_get_default_config(dma_channel);
    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&dma_cfg, false);
    channel_config_set_write_increment(&dma_cfg, true);
    channel_config_set_dreq(&dma_cfg, DREQ_ADC);
    // *******************************************************

    // Limpa o display (fundo branco)
    ssd1306_Fill(White);

    // Desenha o cabeçalho "Teste Mic" no topo
    ssd1306_SetCursor(32, HEADER_Y);
    ssd1306_WriteString("Teste Mic", Font_7x10, Black);

    ssd1306_UpdateScreen();

    while (1)
    {
        // Captura uma amostra do microfone
        sample_mic();
        float power = mic_power();
        
        // Prepara a string com preenchimento para ter tamanho fixo
        char micState[20];
        if (power > MIC_THRESHOLD)
            snprintf(micState, sizeof(micState), "%-15s", "microfone on");
        else
            snprintf(micState, sizeof(micState), "%-15s", "microfone off");

        // Atualiza a área interna do retângulo para exibir o status
        // Ajuste as coordenadas para garantir que a área seja grande o suficiente
        ssd1306_FillRectangle(RECT_X + 1, RECT_Y + 1, RECT_WIDTH - 2, RECT_HEIGHT - 2, White);
        ssd1306_SetCursor(RECT_X + 5, RECT_Y + (RECT_HEIGHT / 2) - 5);
        ssd1306_WriteString(micState, Font_7x10, Black);

        ssd1306_UpdateScreen();

        // Se o pino 22 indicar saída (por exemplo, botão pressionado), sai da função
        if (gpio_get(22) == 0)
        {
            DEBOUNCE;  // Certifique-se de que o debouncing esteja implementado
            mic_home();
            break;
        }
    }
}

/*
 * Função: mic_matriz
 * -------------------
 * Objetivo: Visualizar a intensidade do som captado pelo microfone usando uma matriz de LEDs.
 *
 * Fluxo:
 *   - Inicializa a matriz de LED e configura o ADC e o DMA para o microfone.
 *   - Em loop, captura amostras, calcula a potência e converte o valor em intensidade.
 *   - Atualiza os LEDs na coluna central, atribuindo cores conforme a intensidade (verde, amarelo ou vermelho).
 *   - Se o pino 22 (botão) for pressionado, limpa a matriz, aborta o DMA, desliga a matriz e chama mic_home().
 */
void mic_matriz() 
{
    // Configura a matriz de LED e o ADC:
    npInit(LED_PIN, LED_COUNT);
    adc_gpio_init(MIC_PIN);
    adc_select_input(MIC_CHANNEL);

    adc_fifo_setup(
      true,
      true,
      1,
      false,
      false
    );
    adc_set_clkdiv(ADC_CLOCK_DIV);

    dma_channel = dma_claim_unused_channel(true);
    dma_cfg = dma_channel_get_default_config(dma_channel);

    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&dma_cfg, false);
    channel_config_set_write_increment(&dma_cfg, true);
    channel_config_set_dreq(&dma_cfg, DREQ_ADC);

    // Amostragem inicial:
    sample_mic();

    while (1)
    {
        sample_mic();
        float avg = mic_power();
        avg = 2.f * my_abs(ADC_ADJUST(avg));  // usamos my_abs() em vez de abs()

        uint intensity = get_intensity(avg);
        // Limita a quantidade de barras ao máximo de linhas
        int barsToShow = intensity;
        if (barsToShow > TOTAL_ROWS) {
            barsToShow = TOTAL_ROWS;
        }

        npClear();
        // Desenha a barra na coluna central, preenchendo a parte inferior
        // Se barsToShow é o número de "blocos" acesos, eles serão posicionados a partir da linha
        // "TOTAL_ROWS - barsToShow" (base) até TOTAL_ROWS-1 (topo dos acesos)
        for (int i = 0; i < barsToShow; i++) {
            // Calcula a linha lógica onde o bloco deve ser desenhado:
            int logical_row = TOTAL_ROWS - barsToShow + i;
            int led = LED_INDEX(TOTAL_ROWS - 1 - logical_row, MIDDLE_COL);
            uint8_t r_val, g_val, b_val;
            // Define a cor conforme a posição:
            // - Os blocos mais baixos (i = 0 ou 1) serão verdes (baixa intensidade)
            // - Os intermediários (i = 2 ou 3) serão amarelos (média intensidade)
            // - O bloco superior (i = 4, se houver) será vermelho (alta intensidade)
            if (i < 2) { 
                // Baixa intensidade: verde
                r_val = 0; g_val = 255; b_val = 0;
            } else if (i < 4) { 
                // Média intensidade: amarelo
                r_val = 255; g_val = 255; b_val = 0;
            } else { 
                // Alta intensidade: vermelho
                r_val = 255; g_val = 0; b_val = 0;
            }
            npSetLED(led, r_val, g_val, b_val);
        }
        npWrite();

        // Se o pino 22 indicar saída (por exemplo, botão pressionado)
        if (gpio_get(22) == 0)
        {
            DEBOUNCE;
            // Desliga a matriz antes de sair, deixando-a pronta para outro uso
            npClear();
            npWrite();

            dma_channel_abort(dma_channel);
            Matriz_OFF();

            mic_home();
            break;
        }
    }
}

/*
 * Função: sample_mic
 * -------------------
 * Objetivo: Capturar um conjunto de amostras do microfone utilizando o ADC com DMA.
 *
 * Fluxo:
 *   - Limpa o FIFO do ADC e desativa temporariamente a conversão.
 *   - Configura o canal DMA para transferir SAMPLES do ADC para o buffer adc_buffer.
 *   - Inicia a conversão do ADC, aguarda a transferência via DMA e, em seguida, para o ADC.
 */
void sample_mic() 
{
    adc_fifo_drain();
    adc_run(false);

    dma_channel_configure(dma_channel, &dma_cfg,
        adc_buffer,          // buffer de escrita
        &(adc_hw->fifo),     // leitura do ADC
        SAMPLES,             // número de amostras
        true
    );

    adc_run(true);
    dma_channel_wait_for_finish_blocking(dma_channel);
    adc_run(false);
}

/*
 * Função: mic_power
 * ------------------
 * Objetivo: Calcular a potência (valor RMS) do sinal captado pelo microfone.
 *
 * Fluxo:
 *   - Itera sobre o buffer de amostras, somando os quadrados dos valores.
 *   - Calcula a média desses quadrados e retorna a raiz quadrada do resultado.
 */
float mic_power() {
    float avg = 0.f;
    for (uint i = 0; i < SAMPLES; ++i)
        avg += adc_buffer[i] * adc_buffer[i];

    avg /= SAMPLES;
    return sqrt(avg);
}

/*
 * Função: get_intensity
 * ----------------------
 * Objetivo: Converter um valor de potência do sinal em um nível de intensidade para visualização.
 *
 * Fluxo:
 *   - Inicia um contador em zero.
 *   - Em um loop, subtrai ADC_STEP/10 do valor enquanto este for positivo,
 *     incrementando o contador a cada iteração.
 *   - Retorna o contador, que representa o nível de intensidade do sinal.
 */
uint8_t get_intensity(float v) {
    uint count = 0;
    // Menor sensibilidade: requer mais som para aumentar a intensidade.
    // A cada iteração, subtrai ADC_STEP/10 (em vez de ADC_STEP/20)
    while ((v -= ADC_STEP/10) > 0.f)
        ++count;

    return count;
}
