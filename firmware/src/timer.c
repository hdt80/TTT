#include "timer.h"

#include <avr/io.h>

void timer_init(void) {
	TCCR0B |= ( // DS.87
		_BV(CS02)	// /1024 prescale
		| _BV(CS00)
	);

	// DS.88
	TIMSK0 |= (1 << TOIE0); // Enable clock interrupt on overflow

	TCNT0 = 0; // Reset timer
}
