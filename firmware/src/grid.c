#include "grid.h"

unsigned char grid_data[64] = {};

void grid_init(void) {
	for (int i = 0; i < 64; ++i) {
		grid_data[i] = 0x00;
	}
}

void grid_read(unsigned char* data) {

}
