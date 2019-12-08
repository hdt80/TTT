#include <stdio.h>
#include <unistd.h>

#include "logger.h"
#include "communication.h"

int main(int argc, char** argv) {
	infof("TTT test program\n");

	const char* filename = "/dev/ttyUSB0";

	u8 res = comm_init(filename);
	if (res != 0) {
		errorf("Failed to init to '%s'\n", filename);
		return -1;
	}

	comm_handshake();

	unsigned char col = 0;
	unsigned char row = 0;
	unsigned char color = 0;

	while (1) {
		comm_led_on(col, row, color);

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

		//sleep(1);
	}

	return 0;
}
