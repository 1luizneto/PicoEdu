#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdio.h>
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "inc/menu.h"

// Protótipos das funções
void setup_pwm_led(uint led, uint *slice, uint16_t level);
void joystick_read_axis(uint16_t *vrx, uint16_t *vry);
void joystick_rgb(void);
void joystick_pos(void);

#endif