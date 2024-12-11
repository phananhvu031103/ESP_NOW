#ifndef __WS2812B_H
#define __WS2812B_H
#include "stdint.h"

void ws2812_init(int tx_pin, int number);
void ws2812_set_rgb(int index, uint8_t r, uint8_t g, uint8_t b);
void ws2812_turn_off_all(void);
void ws2812_set_rgb_to_all(uint8_t r, uint8_t g, uint8_t b);
#endif