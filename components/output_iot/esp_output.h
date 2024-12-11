#include "driver/gpio.h"


#ifndef __ESP_OUTPUT_H
#define __ESP_OUTPUT_H

void output_create(int pin);
void output_set_level(int pin, int level);
void output_toggle(int pin);
#endif