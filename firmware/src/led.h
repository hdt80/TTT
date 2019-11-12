#pragma once

void led_init(void);

void led_set_color(unsigned char index, unsigned char value);

void led_on(unsigned char col, unsigned char row, unsigned char color_index);

void led_off(unsigned char col, unsigned char row);

extern unsigned char led_data[64];

extern unsigned char led_colors[8];
