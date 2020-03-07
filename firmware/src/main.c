#include <stdio.h>

#include <avr/interrupt.h>

#include "message_types.h"
#include "uart.h"
#include "led.h"
#include "grid.h"
#include "timer.h"

/*
 * How many bytes from an incoming message have been received
 */
unsigned char msg_length = 0;

/*
 * Is a message currently being recieved?
 */
unsigned char msg_started = 0;

unsigned char msg_index = 0;

unsigned char msg_buffer[64] = {};

/*
 * Is the UART handler currently handling a set RGB command?
 * 0: Not handling, read normally
 * 1: Getting data byte
 * 2: Getting a color value byte
 */
unsigned char msg_set_rgb = 0;

unsigned char rgb_previous_color = 0;

struct set_rgb_values_s {
	unsigned char keep;
	unsigned char row;
	unsigned char column;
	unsigned char color;
} set_rgb_values;

/*
 * Control bytes of a received message
 */
unsigned char msg_readwrite = MSG_READWRITE_UNKNOWN;
unsigned char msg_type = MSG_TYPE_UNKNOWN;
unsigned char msg_args = 0x00;

int main(void) {
	// Create FILE stream for logging messages and response messages
	FILE uart_out = FDEV_SETUP_STREAM(uart_send, 0, _FDEV_SETUP_WRITE);

	// Initialize modules
	uart_init();
	led_init();
	grid_init();
	timer_init();

	// Redirect stdout and stdin to our UART streams
	stdout = &uart_out;

	// Clear LED board from last run
	led_clear();
	led_update();

	sei(); // Enable interrupt handling

	printf("firmware\r\n");

	u8 col = 0;
	u8 row = 0;
	u8 index = 0;

	led_set_brightness(10);

	led_set_color(0, 0xE0);
	led_set_color(1, 0x1C);
	led_set_color(2, 0x03);
	led_set_color(3, 0x11);
	led_set_color(4, 0xFF);
	led_set_color(5, 0xC3);
	led_set_color(6, 0xF2);
	led_set_color(7, 0x1F);

	//PORTB |= 0x01;
	PORTB |= 0x18; // Enable pullups

	while (1) {
		PORTB = 0x18; // Turn off all other rows

		if (row == 0) {
			DDRB = 0x01;
			PORTB |= 0x02;
			row = 1;
		} else if (row == 1) {
			DDRB = 0x02;
			PORTB |= 0x01;
			row = 0;
		} else {
			row = 0;
		}

		grid_data[row * 8 + 0] = !(PINB & 0x08);
		grid_data[row * 8 + 1] = !(PINB & 0x10);

		/*
		if (!(port & 0x08)) {
			printf("(%d, 0) connected\r\n", row);
		}
		if (!(port & 0x10)) {
			printf("(%d, 1) connected\r\n", row);
		}
		*/

		/*
		printf("Pin : 0x%02x\r\n", PINB);
		printf("Port: 0x%02x\r\n", PORTB);
		printf("DDR : 0x%02x\r\n", DDRB);
		printf("\r\n");
		*/

		/*
		for (u8 a = 0; a < 255; ++a) {
			for (u8 b = 0; b < 255; ++b) {
				for (u8 c = 0; c < 100; ++c) {

				}
			}
		}
		*/

		/*
		led_clear();
		led_on_value(col, row, index);
		//led_on_rgb(col, row, 0xFF, 0xFF, 0xFF);
		led_on_rgb(col, row,
			(cv & 0xE0),
			(cv & 0x1C) << 3,
			(cv & 0x03) << 6
		);

		if (index == 8) {
			index = 0;
		} else {
			//++index;
		}

		if (row == 7) {
			row = 0;
			++col;
		} else {
			++row;
		}

		led_update();
		if (col == 8) {
			col = 0;
			++index;
		}
		*/

		//printf("Hi\r\n");
	}

	return -1;
}

ISR(TIMER0_OVF_vect) {
	if (msg_started == 0) {
		return;
	}

	printf("msg done, have %d bytes to process\r\n", msg_index);

	for (unsigned char i = 0; i < msg_index; ++i) {
		printf("0x%02x\r\n", msg_buffer[i]);
	}

	msg_readwrite = (msg_buffer[0] & MSG_READWRITE_MASK) >> MSG_READWRITE_OFFSET;
	msg_type = (msg_buffer[0] & MSG_TYPE_MASK) >> MSG_TYPE_OFFSET;
	msg_args = (msg_buffer[0] & MSG_ARGS_MASK);

	printf("RW: 0x%02x\r\n", msg_readwrite);
	printf("T : 0x%02x\r\n", msg_type);
	printf("A : 0x%02x\r\n", msg_args);

	switch (msg_type) {
		case MSG_TYPE_HANDSHAKE:
			printf("Handshaking\r\n");
			fputc(0x80, stdout);
			fputc(0x00, stdout);
			break;

		case MSG_TYPE_GRID:
			fputc(0x80 | 0x20 | 6, stdout);
			fputc(msg_buffer[0], stdout);
			for (u8 i = 0; i < 64; ++i) {
				fputc(grid_data[i], stdout);
			}
			break;

		case MSG_TYPE_COLOR:
			break;

		case MSG_TYPE_RGB:
			if ((msg_args & 0x07) == 0) {
				if (msg_index != 3) {
					fputc(0x80 | 0x40, stdout);
					fputc(msg_buffer[0], stdout);
				} else {
					u8 col = (msg_buffer[1] & 0x70) >> 4;
					u8 row = msg_buffer[1] & 0x07;
					u8 color = msg_buffer[2];

					led_clear();
					led_on_value(col, row, color);
					if (msg_args & 0x08) {
						led_update();
					}

					fputc(0x80, stdout);
					fputc(msg_buffer[0], stdout);
				}
			} else if ((msg_args & 0x07) == 1) {
				led_clear();
				if (msg_args & 0x08) {
					led_update();
				}

				fputc(0x80, stdout);
				fputc(msg_buffer[0], stdout);
			}
			break;

		case MSG_TYPE_MEMORY:
			break;

		case MSG_TYPE_SET_RGB:
			printf("set RGB\r\n");

			unsigned char prev_color = 0;
			unsigned char msg_byte = 0;

			for (unsigned char i = 1; i < msg_index; ++i) {
				msg_byte = msg_buffer[i];

				set_rgb_values.keep = (msg_byte & SET_RGB_KEEP_MASK);
				set_rgb_values.row = (msg_byte & SET_RGB_ROW_MASK) >> SET_RGB_ROW_OFFSET;
				set_rgb_values.color = (msg_byte & SET_RGB_PREVIOUS_MASK);
				set_rgb_values.column = (msg_byte & SET_RGB_COLUMN_MASK);

				printf("A: 0x%02x\r\n", msg_byte);
				printf("K: 0x%02x\r\n", set_rgb_values.keep);
				printf("R: 0x%02x\r\n", set_rgb_values.row);
				printf("P: 0x%02x\r\n", set_rgb_values.column);
				printf("C: 0x%02x\r\n", set_rgb_values.color);

				// Is the next byte a color byte?
				if (set_rgb_values.color == 0) {
					set_rgb_values.color = msg_buffer[i + 1];
					++i; // Skip next byte
					printf("cv in next byte\r\n");
				} else {
					set_rgb_values.color = prev_color;
					printf("cv is previous\r\n");
				}

				printf("C: 0x%02x\r\n", set_rgb_values.color);

				prev_color = set_rgb_values.color;

				led_on_value(
					set_rgb_values.column,
					set_rgb_values.row,
					set_rgb_values.color
				);
			}
			printf("done processing set rgb\r\n");
			fputc(0x80, stdout);
			fputc(msg_buffer[0], stdout);
			break;

		default:
			printf("unknown msg type: 0x%02x\r\n", msg_type);
			msg_type = MSG_TYPE_UNKNOWN;

			fputc(0x80 | 0x40, stdout);
			fputc(msg_type, stdout);
			break;
	}

	msg_started = 0;
	msg_index = 0;
}

ISR(USART_RX_vect) {
	msg_buffer[msg_index++] = UDR0;
	msg_started = 1;
	TCNT0 = 0;
}
