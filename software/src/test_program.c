#include <stdio.h>
#include <unistd.h>

#include "communication.h"

int main(int argc, char** argv) {
	printf("TTT test program\n");

	const char* filename = "/dev/ttyUSB0";

	u8 res = comm_init(filename);
	if (res != 0) {
		fprintf(stderr, "Failed to init to '%s'\n", filename);
		return -1;
	}

	while (1) {
		comm_handshake();
		sleep(10);
	}

	return 0;
}
