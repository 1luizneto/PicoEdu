#ifndef BUZZER_H
#define BUZZER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "inc/notes.h"
#include "inc/menu.h"

//#define MARIO_A_LENGTH (sizeof(marioA) / sizeof(marioA[0]))
//#define MARIO_B_LENGTH (sizeof(marioB) / sizeof(marioB[0]))

// Configurações de hardware
extern const uint16_t BUZZER_A;
extern const uint16_t BUZZER_B;
extern const float DIVISOR_CLK_PWM;

// Estados para o gerenciamento da música
typedef enum
{
    PLAY_NOTE,     // Estado para tocar uma nota
    DURATION_NOTE, // Estado para controlar a duração da nota
    PLAY_REST,     // Estado para tocar um descanso
    DURATION_REST  // Estado para controlar a duração do descanso
} state_song;

// Estrutura de controle para o Buzzer
typedef struct {
    int *melody;
    int length;
    uint16_t pin;
    uint16_t index;
    uint64_t next_event_time;
    state_song state;
} Buzzer;

// Declaração de Buzzers
extern Buzzer buzzerA;
extern Buzzer buzzerB;

// Funções de configuração e reprodução
void setup_pwm(uint pin);
void play_note_pwm(uint pin, uint16_t wrap);
void play_rest_pwm(uint pin);
void play_song(Buzzer *buzzer);
void stop_song(Buzzer *buzzer);
void asaBranca_play(void);
void mario_play(void);

#endif 
