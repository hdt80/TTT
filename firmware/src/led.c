#include "led.h"

#include <avr/io.h>

unsigned char led_data[64 * 3] = {};

unsigned char led_colors[8] = {};

volatile uint8_t* led_port = 0;

u8 led_brightness = 0;

void led_init(void) {
	for (int i = 0; i < 64 * 3; ++i) {
		led_data[i] = 0x00;
	}

	led_data[0] = 0x00;
	led_data[1] = 0x20;
	led_data[2] = 0x00;

	led_data[3] = 0x00;
	led_data[4] = 0x00;
	led_data[5] = 0x20;

	led_colors[0] = 0x00;
	led_colors[1] = 0x00;
	led_colors[2] = 0x00;
	led_colors[3] = 0x00;
	led_colors[4] = 0x00;
	led_colors[5] = 0x00;
	led_colors[6] = 0x00;
	led_colors[7] = 0x00;

	// Set D6 to output, which is what PORT the data is on
	DDRD = (1 << 6);

	led_port = &PORTD;
}

void led_set_color(unsigned char index, unsigned char value) {
	led_colors[index] = value;
}

void led_on(u8 col, u8 row, u8 r, u8 g, u8 b) {
	u8* colors = &led_data[(col * 8 + row) * 3];

	if (led_brightness > 0) {
		colors[0] = (g * led_brightness) >> 8;
		colors[1] = (r * led_brightness) >> 8;
		colors[2] = (b * led_brightness) >> 8;
	} else {
		colors[0] = g;
		colors[1] = r;
		colors[2] = b;
	}
}

void led_off(u8 col, u8 row) {
	u8* color_data = &led_data[(col * 8 + row) * 3];

	color_data[0] = 0x00;
	color_data[1] = 0x00;
	color_data[2] = 0x00;
}

void led_clear(void) {
	for (u8 i = 0; i < 64 * 3; ++i) {
		led_data[i] = 0;
	}
}

void led_set_brightness(u8 value) {
	led_brightness = value;
}

void led_update(void) {

	// This code is a modified version of the Adafruit NeoPixel library.
	// See the licenses folder for more information, including a copy
	// of the license

	volatile uint16_t
		i   = 64 * 3; // Loop counter

	volatile uint8_t
		*ptr = led_data,// Pointer to next byte
		b   = *ptr++,   // Current byte value
		hi,             // PORT w/output bit set high
		lo,             // PORT w/output bit set low
		next,
		bit,
		pinMask = 64;

    hi   = *led_port |  pinMask;
    lo   = *led_port & ~pinMask;
    next = lo;
    bit  = 8;

    asm volatile(
     "head20:"                   "\n\t" // Clk  Pseudocode    (T =  0)
      "st   %a[port],  %[hi]"    "\n\t" // 2    PORT = hi     (T =  2)
      "sbrc %[byte],  7"         "\n\t" // 1-2  if(b & 128)
       "mov  %[next], %[hi]"     "\n\t" // 0-1   next = hi    (T =  4)
      "dec  %[bit]"              "\n\t" // 1    bit--         (T =  5)
      "st   %a[port],  %[next]"  "\n\t" // 2    PORT = next   (T =  7)
      "mov  %[next] ,  %[lo]"    "\n\t" // 1    next = lo     (T =  8)
      "breq nextbyte20"          "\n\t" // 1-2  if(bit == 0) (from dec above)
      "rol  %[byte]"             "\n\t" // 1    b <<= 1       (T = 10)
      "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 12)
      "nop"                      "\n\t" // 1    nop           (T = 13)
      "st   %a[port],  %[lo]"    "\n\t" // 2    PORT = lo     (T = 15)
      "nop"                      "\n\t" // 1    nop           (T = 16)
      "rjmp .+0"                 "\n\t" // 2    nop nop       (T = 18)
      "rjmp head20"              "\n\t" // 2    -> head20 (next bit out)
     "nextbyte20:"               "\n\t" //                    (T = 10)
      "ldi  %[bit]  ,  8"        "\n\t" // 1    bit = 8       (T = 11)
      "ld   %[byte] ,  %a[ptr]+" "\n\t" // 2    b = *ptr++    (T = 13)
      "st   %a[port], %[lo]"     "\n\t" // 2    PORT = lo     (T = 15)
      "nop"                      "\n\t" // 1    nop           (T = 16)
      "sbiw %[count], 1"         "\n\t" // 2    i--           (T = 18)
       "brne head20"             "\n"   // 2    if(i != 0) -> (next byte)
      : [port]  "+e" (led_port),
        [byte]  "+r" (b),
        [bit]   "+r" (bit),
        [next]  "+r" (next),
        [count] "+w" (i)
      : [ptr]    "e" (ptr),
        [hi]     "r" (hi),
        [lo]     "r" (lo)
	);

}
