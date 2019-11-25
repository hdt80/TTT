#pragma once

#include <stdint.h>

#define u8 uint8_t

void led_init(void);

void led_set_color(unsigned char index, unsigned char value);

void led_on(u8 row, u8 col, u8 r, u8 g, u8 b);

void led_off(u8 col, u8 row);

void led_clear(void);

void led_set_brightness(u8 value);

void led_update(void);

// 64 RGB LEDs with 24 bit color
extern unsigned char led_data[64 * 3];

// 8 different colors
extern unsigned char led_colors[8];

extern u8 led_brightness;

extern volatile uint8_t* led_port;
