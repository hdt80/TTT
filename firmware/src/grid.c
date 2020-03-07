#include "grid.h"

unsigned char grid_data[64] = {};

void grid_init(void) {
	for (int i = 0; i < 64; ++i) {
		grid_data[i] = 0; //i;
	}

	// Setup port 8 and 7 to be "columns"
	// Port 0 and 1 as rows
}

void grid_read(void) {

}
