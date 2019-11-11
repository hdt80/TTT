#include "uart.h"

#include <avr/io.h>
#include <stdio.h>
#include <util/setbaud.h>

void uart_init(void) {
	UBRR0H = UBRRH_VALUE; // 9600 Baud by default
	UBRR0L = UBRRL_VALUE;

	UCSR0A &= (
		~_BV(U2X0)		// DS.160 - Ensure not running at double speed
	);

	UCSR0B |= (
		_BV(RXCIE0)		// DS.160 - Enable receive interrupt
		| _BV(RXEN0)	// DS.160 - Enable Rx pin
		| _BV(TXEN0)	// DS.160 - Enable Tx pin
	);

	UCSR0C |= (
		_BV(UCSZ01)		// DS.162 - Use 8bit data words
		| _BV(UCSZ00)	// ^^
	);
}

int uart_send(char c, FILE* stream) {
	loop_until_bit_is_set(UCSR0A, UDRE0);

	UDR0 = c;

	return 0;
}
