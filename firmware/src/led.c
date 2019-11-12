#include "led.h"

unsigned char led_data[64] = {};

unsigned char led_colors[8] = {};

void led_init(void) {
	for (int i = 0; i < 64; ++i) {
		led_data[i] = 0x00;
	}

	led_colors[0] = 0x00;
	led_colors[1] = 0x00;
	led_colors[2] = 0x00;
	led_colors[3] = 0x00;
	led_colors[4] = 0x00;
	led_colors[5] = 0x00;
	led_colors[6] = 0x00;
	led_colors[7] = 0x00;
}

void led_set_color(unsigned char index, unsigned char value) {
	led_colors[index] = value;
}

void led_on(unsigned char col, unsigned char row, unsigned char color_index) {
	unsigned char color = led_colors[color_index];

	led_data[col * 8 + row] = color;
}

void led_off(unsigned char col, unsigned char row) {
	led_data[col * 8 + row] = 0x00;
}
