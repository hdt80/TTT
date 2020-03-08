#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "logger.h"
#include "communication.h"

void test_led(void);

int main(int argc, char** argv) {
	infof("TTT test program\n");

	const char* filename = "/dev/ttyUSB0";

	u8 res = comm_init(filename);
	if (res != 0) {
		errorf("Failed to init to '%s'\n", filename);
		return -1;
	}

	comm_handshake();

	//while(1);

	u8* grid_data = malloc(64);
	comm_grid_read(grid_data);

	comm_set_rgb();

	while (1) {
		comm_grid_read(grid_data);
		u8 found = 0;

		comm_led_clear(0);
		for (u8 i = 0; i < 64; ++i) {
			if (grid_data[i] != 0) {
				u8 cv = (grid_data[i] & 0xE0) >> 5;
				infof("Grid (%d, %d) is connected: 0x%x (0x%x)\n",
					(i % 8),
					(i / 8),
					grid_data[i],
					cv
				);
				comm_led_on(i % 8, i / 8, cv);
				found = 1;
			}
		}

		if (found == 0) {
			comm_led_clear(1);
		}

		//sleep(5);
	}

	//test_led();

	return 0;
}

void test_led(void) {
	unsigned char col = 0;
	unsigned char row = 0;
	unsigned char color = 0;
	u8* grid_data = malloc(64);

	while (1) {
		comm_led_on(row, col, color);

		if (row == 7) {
			row = 0;
			++col;
		} else {
			++row;
		}

		if (col == 8) {
			col = 0;
		}

		if (color == 7) {
			color = 0;
		} else {
			++color;
		}

		//comm_grid_read(grid_data);
		//sleep(5);
	}

}
