#include "communication.h"

#include "logger.h"

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <termios.h>

#include <sys/types.h>

#include <pthread.h>

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

/*
 * Background thread that reads the file
 */
pthread_t comm_thread;

/*
 * File descriptor used to connect to the hardware device
 */
int comm_fd = -1;

/*
 * Is |comm_thread| currently running?
 *
 * 0: |comm_thread| is not running
 * !0: |comm_thread| is running
 */
u8 comm_running = 0x00;

/*
 * Status of the communication file
 */
u8 comm_status = STATUS_IDLE;

/*
 * Buffer of responses, FIFO
 */
comm_response comm_buffer[256] = {};

/*
 * How many responses are in |comm_buffer|
 */
u8 comm_buffer_length = 0;

////////////////////////////////////////////////////////////////////////////////
// Methods
////////////////////////////////////////////////////////////////////////////////

int comm_init(const char* filename) {
	if (comm_fd != -1) {
		errorf("Communication already started, use comm_close\n");
		return -1;
	}

	debugf("comm_init> Creating connection to '%s'\n", filename);

	comm_fd = open(filename, O_RDWR | O_NOCTTY | O_SYNC);
	if (comm_fd < 0) {
		errorf("Failed to open '%s': %s\n", filename, strerror(errno));
		return -1;
	}

	int res;

	res = isatty(comm_fd);
	if (res == 0) {
		warnf("comm_init> Port is not a tty: '%s'\n", filename);
	} else if (res == 1) {
		debugf("comm_init> Port is a tty: '%s'\n", filename);
	} else {
		debugf("Unknown result from isatty: %i", res);
	}

	struct termios tty;

	res = tcgetattr(comm_fd, &tty);
	if (res < 0) {
		errorf("Failed to get tty attr: %s", strerror(errno));
		return -1;
	}

	int speed = B9600; // 9600 baud

	// Modified from: https://stackoverflow.com/questions/6947413
	cfsetospeed(&tty, (speed_t) speed);
    cfsetispeed(&tty, (speed_t) speed);

    tty.c_cflag |= (CLOCAL | CREAD); // Ignore modem controls
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         // 8 bit words
    tty.c_cflag &= ~PARENB;     // No parity
    tty.c_cflag &= ~CSTOPB;     // One stop bit
    tty.c_cflag &= ~CRTSCTS;    // No flow control

	// Set input flags
    tty.c_iflag &= ~(
		IGNBRK		// Don't ignore BREAK on input
		| BRKINT	// Don't ignore BREAK
		| PARMRK	// Don't mark parity and framing errors
		| ISTRIP	// Don't strip off 8th bit (keep it)
		| INLCR		// Don't translate NL to CR on input
		| IGNCR		// Don't ignore CR  on input
		| ICRNL		// Don't translate CR to NL
		| IXON		// Disable XON / XOFF flow control
	);

	// Set local flags
    tty.c_lflag &= ~(
		ECHO		// Don't echo back characters
		| ECHONL	// Don't echo NL
		| ICANON	// Disable canonical mode
		| ISIG		// Don't generate signals
		| IEXTEN	// Disable input processing
	);

	// Set output flags
    tty.c_oflag &= ~OPOST; // Disable output processing

	// Fetch bytes as they're available
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

	// Set the tty attribs 
	res = tcsetattr(comm_fd, TCSANOW, &tty);
    if (res != 0) {
		errorf("Failed to set tty attr: %s\n", strerror(errno));
        return -1;
    }

	// Background thread setup
	pthread_attr_t attr;

	// Initalize the pthread attribs, used to set as detached
	res = pthread_attr_init(&attr);
	if (res < 0) {
		errorf("Failed to init pthread attr: %s\n", strerror(res));
		return -1;
	}

	// Set thread as detached
	res = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (res < 0) {
		errorf("Failed to set thread to detach: %s\n", strerror(res));
		return -1;
	}

	comm_running = 1;

	// Being running the background thread
	res = pthread_create(&comm_thread, &attr, comm_task, (void*) filename);
	if (res != 0) {
		errorf("Failed to start thread: %s\n", strerror(res));
		return -1;
	}

	// Give it time to setup the connection
	sleep(2);

	debugf("comm_init> Connection initialized\n");

	return 0;
}

u8 comm_busy(void) {
	return comm_status != STATUS_IDLE;
}

comm_response comm_read(void) {
	return comm_buffer[0];
}

int comm_handshake(void) {
	if (comm_fd == -1) {
		errorf("Cannot handshake, fd is not set\n");
		return -1;
	}
	if (comm_busy() == 1) {
		errorf("A command is currently being executed, cannot handshake\n");
		return -1;
	}

	debugf("comm_handshake> Sending handshake to device\n");

	comm_status = STATUS_RUNNING;

	const char msg[] = { 0x00 };
	int res = write(comm_fd, msg, 1);

	while(comm_busy() == 1);

	comm_response data = comm_read();

	if (data.command == 0x00) {
		if (data.result == RESPONSE_RESULT_SUCCESS) {
			infof("comm_handshake> Handshake success, ready for commands\n");
		} else {
			errorf("comm_handshake> Handshake failed\n");
		}
	} else {
		warnf("Failed to get command back. Got: 0x%02x\n", data.command);
	}

	return 0;
}

int comm_led_on(u8 col, u8 row, u8 color) {
	if (comm_fd == -1) {
		errorf("Cannot turn LED on, fd is not set\n");
		return -1;
	}
	if (comm_busy() == 1) {
		errorf("A command is currently being executed, cannot turn LED on\n");
		return -1;
	}

	debugf("comm_led_on> Setting (%d, %d) to %d\n", col, row, color);

	comm_status = STATUS_RUNNING;

	const unsigned char msg[] = {
		0xB9,
		(
			0x00
			| ((col & 0x07) << 4)
			| 0x00
			| ((row & 0x07))
		),
		color & 0x07
	};

	debugf("\t0x%02x 0x%02x 0x%02x\n", msg[0], msg[1], msg[2]);

	int res = write(comm_fd, msg, 3);

	while(comm_busy() == 1);

	comm_response data = comm_read();

	if (data.command == 0xB9) {
		if (data.result == RESPONSE_RESULT_SUCCESS) {
			infof("comm_led_on> LED on successful\n");
		} else {
			errorf("comm_led_on> LED on failed\n");
		}
	} else {
		warnf("Failed to get command back. Got: 0x%02x\n", data.command);
	}

	return 0;
}

int comm_grid_read(u8* result) {
	if (comm_fd == -1) {
		errorf("Cannot read grid, fd is not set\n");
		return -1;
	}
	if (comm_busy() == 1) {
		errorf("A command is currently being executed, cannot read grid\n");
		return -1;
	}

	debugf("comm_grid_read> Performing grid read\n");

	comm_status = STATUS_RUNNING;

	const unsigned char msg[] = {
		0x10
	};

	int res = write(comm_fd, msg, 1);

	while(comm_busy() == 1);

	comm_response data = comm_read();

	if (data.command == 0x10) {
		if (data.result == RESPONSE_RESULT_SUCCESS) {
			infof("comm_read_grid> Read %d entries\n", data.length);
		} else {
			errorf("comm_read_grid> Failed to read grid\n");
		}
	} else {
		warnf("Failed to get command back. Got: 0x%02x\n", data.command);
	}

	return 0;

}

typedef enum SM_COMM_s {
	IDLE,
	CMD,
	READING
} SM_COMM;

void* comm_task(void* filename) {
	u8 status = 0x00;
	u8 length = 0x00;
	u8 index = 0x00;
	u8 data_avail = 0x00;

	u8* response = NULL;

	SM_COMM state = IDLE;

	u8* buf = malloc(1);
	u8 res;

	comm_response pending_response = {
		.result = 0,
		.command = 0x00,
		.length = 0,
		.data = NULL
	};

	int s;

	infof("comm_task> Background task started\n");

	while (comm_running != 0) {
		s = read(comm_fd, buf, 1);
		if (s == 0) {
			continue;
		} else if (s < 0) {
			errorf("Failed to read from comm: %s\n", strerror(errno));
			continue;
		}

		res = *buf;

		if (state == IDLE) {
			if ((res & 0x80) == 0x80) {
				status = (res & RESPONSE_STATUS_MASK) >> RESPONSE_STATUS_OFFSET;
				data_avail = (res & RESPONSE_DATA_MASK) >> RESPONSE_DATA_OFFSET;

				if (data_avail == 1) {
					length = (res & RESPONSE_LENGTH_MASK) >> RESPONSE_LENGTH_OFFSET;
					length = pow(2, length);
				} else {
					length = 0;
				}

				if (response != NULL) {
					free(response);
				}

				// +2 for header and command byte
				response = malloc(2 + length);

				debugf("comm_task> Response started\n");
				debugf("\tstatus: 0x%02x\n", status);
				debugf("\tavail:  0x%02x\n", data_avail);
				debugf("\tlength: 0x%02x\n", length);
				debugf("\tdata: ");

				index = 0x01; // Already read the first byte

				response[0] = res;

				printf("0x%02x ", res);

				comm_status = STATUS_RUNNING;
				state = CMD;
			} else {
				printf("%c", res);
			}
		} else if (state == CMD) {
			response[index] = res;
			++index;

			if (data_avail == 1) {
				state = READING;
			} else {
				printf("\n");
				debugf("comm_task> no data: skipping READING state\n");

				comm_response res = {
					.result = status,
					.command = response[1],
					.length = 0,
					.data = NULL
				};

				comm_buffer[comm_buffer_length] = res;

				comm_status = STATUS_IDLE;
				state = IDLE;
			}
		} else if (state == READING) {
			response[index] = res;

			printf("0x%02x ", res);

			if (index >= length) {
				printf("\n");
				debugf("comm_task> Response done\n");

				u8* res_data = NULL;
				if (length > 0) {
					debugf("\tAllocating %d bytes for data\n", length);
					res_data = malloc(length);

					for (u8 i = 0; i < length; ++i) {
						// Ignore header and command bytes
						res_data[i] = response[i + 2];
					}
				}

				comm_response res = {
					.result = status,
					.command = response[1],
					.length = length,
					.data = res_data
				};

				comm_buffer[comm_buffer_length] = res;
				//++comm_buffer_length;

				index = 0x01;
				status = 0x00;
				length = 0x00;

				comm_status = STATUS_IDLE;
				state = IDLE;
			} else {
				++index;
			}
		} else {
			printf("hello? %c", res);
		}
	}

	if (response != NULL) {
		free(response);
	}

	free(buf);

	return NULL;
}

void comm_stop(void) {
	close(comm_fd);
	comm_fd = -1;

	comm_running = 0;
}
