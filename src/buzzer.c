
#include "inc/buzzer.h"
//#include "menu.h"

// Configurações de hardware
const uint16_t BUZZER_A = 21;
const uint16_t BUZZER_B = 10;
const float DIVISOR_CLK_PWM = 16.0;


// Inicialização dos buzzers
Buzzer buzzerA = {melodyA, 182, BUZZER_A, 0, 0, PLAY_NOTE}; //Configuração do buzzer A
Buzzer buzzerB = {melody_mario, 307, BUZZER_B, 0, 0, PLAY_NOTE}; //Configuração do buzzer B

/**
 * @brief Configura o PWM para um pino específico.
 * 
 * Esta função configura o pino como saída PWM, define o divisor de clock e o valor de wrap (período).
 * Também habilita o PWM no slice correspondente ao pino.
 * 
 * @param pin Pino a ser configurado como saída PWM.
 */
void setup_pwm(uint pin) 
{
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_clkdiv(slice, DIVISOR_CLK_PWM);
    pwm_set_wrap(slice, 1000); // Wrap padrão
    pwm_set_enabled(slice, true);
}

/**
 * @brief Toca uma nota no buzzer.
 * 
 * Esta função ajusta o valor de wrap do PWM para gerar a frequência da nota desejada
 * e define o nível do PWM para tocar a nota.
 * 
 * @param pin Pino do buzzer onde a nota será tocada.
 * @param wrap Valor de wrap para gerar a frequência da nota.
 */
void play_note_pwm(uint pin, uint16_t wrap) {
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice, wrap);
    pwm_set_gpio_level(pin, wrap / 8); // Ajuste o divisor conforme necessário
}

/**
 * @brief Toca um descanso (silêncio) no buzzer.
 * 
 * Esta função define o nível do PWM como 0, silenciando o buzzer.
 * 
 * @param pin Pino do buzzer onde o descanso será tocado.
 */
void play_rest_pwm(uint pin) {
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_gpio_level(pin, 0); // Nível 0 para descanso
}


/**
 * @brief Toca uma música no buzzer.
 * 
 * Esta função controla a reprodução de uma música no buzzer, alternando entre tocar notas
 * e descansos com base na melodia e duração definidas na estrutura do buzzer.
 * 
 * @param buzzer Ponteiro para a estrutura do buzzer que contém a melodia e o estado atual.
 */
void play_song(Buzzer *buzzer) 
{
    uint64_t now = time_us_64(); // Tempo atual em microssegundos

    if (now < buzzer->next_event_time) {
        return; // Aguarde o próximo evento
    }

    int note = buzzer->melody[buzzer->index * 2];
    int duration = buzzer->melody[buzzer->index * 2 + 1];
    int dur = 900000 / duration; // Duração em microssegundos

    switch (buzzer->state) {
    case PLAY_NOTE:
        if (note == pause) {
            play_rest_pwm(buzzer->pin);
        } else {
            play_note_pwm(buzzer->pin, note);
        }
        buzzer->state = DURATION_NOTE;
        buzzer->next_event_time = now + dur;
        break;

    case DURATION_NOTE:
        play_rest_pwm(buzzer->pin);
        buzzer->state = DURATION_REST;
        buzzer->next_event_time = now + dur / 2; // Descanso menor que a nota
        break;

    case DURATION_REST:
        buzzer->index = (buzzer->index + 1) % (buzzer->length / 2);
        buzzer->state = PLAY_NOTE;
        break;
    }
}


/**
 * @brief Função principal para tocar a música "Asa Branca".
 * 
 * Esta função configura o buzzer A e toca a música "Asa Branca" em loop.
 * Verifica se o botão no pino 22 foi pressionado para retornar ao menu principal.
 */
void asaBranca_play() 
{
    setup_pwm(BUZZER_A);
    while (1) 
    {
        play_song(&buzzerA);

        if(gpio_get(22) == 0) 
        {
            DEBOUNCE;

            buzzerA.index = 0;
            buzzerA.state = PLAY_NOTE;
            
            uint slice_num = pwm_gpio_to_slice_num(BUZZER_A);
            pwm_set_enabled(slice_num, false);
            gpio_set_function(BUZZER_A, GPIO_FUNC_SIO);
            gpio_set_dir(BUZZER_A, GPIO_OUT);
            gpio_put(BUZZER_A, 0);       
            
            buzzer_home();
            break;
        }
    }
}

/**
 * @brief Função principal para tocar a música do Mario.
 * 
 * Esta função configura o buzzer B e toca a música do Mario em loop.
 * Verifica se o botão no pino 22 foi pressionado para retornar ao menu principal.
 */
void mario_play() 
{
    setup_pwm(BUZZER_B);
    while (1) 
    {
        play_song(&buzzerB);

        if(gpio_get(22) == 0) 
        {
            DEBOUNCE;

            buzzerB.index = 0;
            buzzerB.state = PLAY_NOTE;
            
            uint slice_num = pwm_gpio_to_slice_num(BUZZER_B);
            pwm_set_enabled(slice_num, false);
            gpio_set_function(BUZZER_B, GPIO_FUNC_SIO);
            gpio_set_dir(BUZZER_B, GPIO_OUT);
            gpio_put(BUZZER_B, 0);       
            
            buzzer_home();
            break;
        }
    }
}
