#include <stdio.h>

#include <avr/interrupt.h>

#include "message_types.h"
#include "uart.h"
#include "led.h"
#include "grid.h"

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
 * Control bytes of a received message
 */
unsigned char msg_readwrite = MSG_READWRITE_UNKNOWN;
unsigned char msg_type = MSG_TYPE_UNKNOWN;
unsigned char msg_mode = MSG_MODE_UNKNOWN;
unsigned char msg_address = MSG_ADDRESS_UNKNOWN;

int main(void) {
	// Create FILE stream for logging messages and response messages
	FILE uart_out = FDEV_SETUP_STREAM(uart_send, 0, _FDEV_SETUP_WRITE);

	// Initialize modules
	uart_init();
	led_init();
	grid_init();

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

	while (1) {
		/*
		for (u8 a = 0; a < 255; ++a) {
			for (u8 b = 0; b < 255; ++b) {
				for (u8 c = 0; c < 5; ++c) {

				}
			}
		}

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

ISR(USART_RX_vect) {
	unsigned char msg_byte = UDR0;
	//printf("0x%02x\r\n", msg_byte);

	if (msg_started == 0) {
		msg_started = 1;
		msg_index = 0;
		msg_length = 1;

		msg_readwrite = (msg_byte & MSG_READWRITE_MASK) >> MSG_READWRITE_OFFSET;
		msg_type = (msg_byte & MSG_TYPE_MASK) >> MSG_TYPE_OFFSET;
		msg_mode = (msg_byte & MSG_MODE_MASK) >> MSG_MODE_OFFSET;
		msg_address = (msg_byte & MSG_ADDRESS_MASK) >> MSG_ADDRESS_OFFSET;

		// Expect a data byte
		if (msg_readwrite == 0x01) {
			++msg_length;
		}

		// Expect a address byte
		if (msg_mode == 0x01) {
			++msg_length;
		}

		/*
		printf("RW: 0x%02x\r\n", msg_readwrite);
		printf("T : 0x%02x\r\n", msg_type);
		printf("M : 0x%02x\r\n", msg_mode);
		printf("A : 0x%02x\r\n", msg_address);
		*/
	}

	msg_buffer[msg_index++] = msg_byte;
	--msg_length;

	if (msg_length == 0) {
		msg_started = 0;

		//printf("done\r\n");

		switch (msg_type) {
			case MSG_TYPE_HANDSHAKE:
				//printf("Handshaking\r\n");
				fputc(0x80, stdout);
				fputc(0x00, stdout);
				break;

			case MSG_TYPE_GRID:
				//printf("Grid contents\r\n");
				break;

			case MSG_TYPE_COLOR:
				//printf("Setting color\r\n");
				break;

			case MSG_TYPE_RGB:
				//printf("Setting RGB LED: %d\r\n", msg_index);

				// Must be 3 chars, command, addr and data
				if (msg_index != 3) {
					fputc(0x80 | 0x40, stdout);
					fputc(msg_buffer[0], stdout);
				} else {
					u8 col = (msg_buffer[1] & 0x70) >> 4;
					u8 row = msg_buffer[1] & 0x07;
					u8 color = msg_buffer[2];

					/*
					printf("C: %d\r\n", col);
					printf("R: %d\r\n", row);
					printf("I: %d\r\n", color);
					*/

					led_clear();
					led_on_value(col, row, color);
					led_update();

					fputc(0x80, stdout);
					fputc(msg_buffer[0], stdout);
				}

				break;

			case MSG_TYPE_MEMORY:
				printf("Reading memory address\r\n");
				break;

			default:
				printf("Unknown message type: 0x%02x\r\n", msg_type);
				msg_type = MSG_TYPE_UNKNOWN;

				// Set error flag
				fputc(0x80 | 0x40, stdout);
				fputc(msg_byte, stdout);

				break;
		}
	}
}
