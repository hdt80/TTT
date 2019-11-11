#include "communication.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <sys/inotify.h>
#include <sys/types.h>

#include <pthread.h>

/*
 * Background thread that reads the file
 */
pthread_t comm_thread;

/*
 * FILE used to communicate with the device
 */
FILE* comm_file = NULL;

/*
 * Is |comm_thread| currently running?
 * 0: |comm_thread| is not running
 * !0: |comm_thread| is running
 */
u8 comm_running = 0x00;

u8 comm_init(const char* filename) {
	if (comm_file != NULL) {
		fprintf(stderr, "communication already start, use comm_close\n");
		return 1;
	}

	printf("comm_init> Creating connection to: '%s'\n", filename);

	comm_file = fopen(filename, "r+");

	if (comm_file == NULL) {
		fprintf(stderr, "Failed to open port '%s'\n", filename);
		return 1;
	}

	// Set file to be unbufferd
	setvbuf(comm_file, NULL, _IONBF, 0);

	int res;

	pthread_attr_t attr;
	res = pthread_attr_init(&attr);
	if (res != 0) {
		fprintf(stderr, "Failed to init pthread attr: %i\n", res);
		return 1;
	}

	res = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (res != 0) {
		fprintf(stderr, "Failed to set thread to detact: %i\n", res);
		return 1;
	}

	comm_running = 1;

	res = pthread_create(&comm_thread, &attr, comm_task, (void*) filename);
	if (res != 0) {
		fprintf(stderr, "Failed to start thread: %i\n", res);
		return 1;
	}

	printf("comm_init> Connection initialized\n");

	return 0;
}

u8 comm_handshake(void) {
	if (comm_file == NULL) {
		fprintf(stderr, "comminication file not opened, use comm_init\n");
		return 1;
	}

	printf("comm_handshake> Sending handshake to device\n");

	int res = fputc(0x80, comm_file);
	fflush(comm_file);

	printf("comm_handshake> res: %i\n", res);

	return 0;
}

void* comm_task(void* filename) {
	u8 status = 0x00;
	u8 length = 0x00;
	u8 index = 0x00;

	u8* response = NULL;

	printf("comm_task> Background task started\n");

	while (comm_running != 0) {
		u8 res = fgetc(comm_file);

		if (length == 0x00 && ((res & 0x80) == 0x80)) {
			status = (res & RESPONSE_STATUS_MASK) >> RESPONSE_STATUS_OFFSET;
			length = (res & RESPONSE_LENGTH_MASK) >> RESPONSE_LENGTH_OFFSET;
			length = pow(2, length);

			if (response != NULL) {
				free(response);
			}

			response = malloc(1 + length);

			printf("comm_task> Response started\n");
			printf("\tstatus: 0x%02x\n", status);
			printf("\tlength: 0x%02x\n", length);
			printf("\t");

			index = 0x01;

			response[0] = res;

			printf("0x%02x ", res);
		} else if (length > 0x00) {
			response[index] = res;

			//printf("0x%02x (%i/%i)\n", res, index, length);
			printf("0x%02x ", res);

			if (index == length) {
				printf("\nResponse done\n");
				index = 0x01;
				status = 0x00;
				length = 0x00;
			} else {
				++index;
			}
		} else {
			printf("%c", res);
		}
	}

	if (response != NULL) {
		free(response);
	}

	return NULL;
}

void comm_stop(void) {
	comm_running = 0;
}
