#include <stdio.h>

#include <avr/interrupt.h>

#include "uart.h"
#include "message_types.h"

/**
 * How many bytes from an incoming message have been received
 */
unsigned char msg_length = 0;

/**
 * Control bytes of a received message
 */
unsigned char msg_readwrite = MSG_READWRITE_UNKNOWN;
unsigned char msg_type = MSG_TYPE_UNKNOWN;
unsigned char msg_mode = MSG_MODE_UNKNOWN;
unsigned char msg_address = MSG_ADDRESS_UNKNOWN;

int main(void) {
	// Create FILE stream for logging messages and response messages
	FILE uart_out = FDEV_SETUP_STREAM(uart_send, 0, _FDEV_SETUP_WRITE);

	// Initalize the UART driver
	uart_init();

	// Redirect stdout and stdin to our UART streams
	stdout = &uart_out;

	sei(); // Enable interrupt handling

	printf("firmware\r\n");

	while (1) {
		//printf("Hi\r\n");
	}

	return -1;
}

ISR(USART_RX_vect) {
	unsigned char msg_byte = UDR0;
	printf("0x%2x\r\n", msg_byte);

	if (msg_length == 0) {
		msg_readwrite = (msg_byte & MSG_READWRITE_MASK) >> MSG_READWRITE_OFFSET;
		msg_type = (msg_byte & MSG_TYPE_MASK) >> MSG_TYPE_OFFSET;
		msg_mode = (msg_byte & MSG_MODE_MASK) >> MSG_MODE_OFFSET;
		msg_address = (msg_byte & MSG_ADDRESS_MASK) >> MSG_ADDRESS_OFFSET;

		printf("RW: 0x%02x\r\n", msg_readwrite);
		printf("T : 0x%02x\r\n", msg_type);
		printf("M : 0x%02x\r\n", msg_mode);
		printf("A : 0x%02x\r\n", msg_address);

		switch (msg_type) {
			case MSG_TYPE_HANDSHAKE:
				printf("Handshaking\r\n");
				fputc(0x80, stdout);
				fputc(0x80, stdout);
				break;

			case MSG_TYPE_GRID:
				printf("Grid contents\r\n");
				break;

			case MSG_TYPE_COLOR:
				printf("Setting color\r\n");
				break;

			case MSG_TYPE_RGB:
				printf("Setting RGB LED\r\n");
				break;

			case MSG_TYPE_MEMORY:
				printf("Reading memory address\r\n");
				break;

			default:
				printf("Unknown message type: 0x%02x\r\n", msg_type);
				msg_type = MSG_TYPE_UNKNOWN;
				break;
		}
	} else {

	}
}
