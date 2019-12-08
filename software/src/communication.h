#pragma once

#include "types.h"

#define RESPONSE_STATUS_MASK 0x40
#define RESPONSE_STATUS_OFFSET 0x05
#define RESPONSE_STATUS_SUCCESS 0x00
#define RESPONSE_STATUS_ERROR 0x01
#define RESPONSE_STATUS_UNKNOWN 0xFF

#define RESPONSE_LENGTH_MASK 0x3F
#define RESPONSE_LENGTH_OFFSET 0x00

#define STATUS_IDLE		0x00
#define STATUS_RUNNING	0x01
#define STATUS_ERROR	0xFF

#define RESPONSE_RESULT_SUCCESS	0x00
#define RESPONSE_RESULT_ERROR	0x01

/*
 * Information about the response to a command
 */
typedef struct comm_response_s {
	/*
	 * Status of the command.
	 *		0: Success
	 *		1: Error
	 */
	u8 result;

	/*
	 * Command this response was generated for
	 */
	u8 command;

	/*
	 * How many data bytes are available? If this is 0, then |data| is NULL
	 */
	u8 length;

	/*
	 * Data of the response
	 */
	u8* data;

} comm_response;

/*
 * Create a new connection to a new hardware device
 *
 * @filename Path the serial interface to the device can be found
 *
 * returns: 0 on success, -1 on error (error was printed to stderr)
 */
int comm_init(const char* filename);

/*
 * Is a response available to be read
 *
 * returns: If comm_read has data to be returned
 *		0: Data is not availabe
 *		1: Data is available
 */
u8 comm_available(void);

/*
 * Read the next response available from the respone queue
 *
 * returns: The next available response.
 */
comm_response comm_read(void);

/*
 * Close the connection made to the hardware device
 */
void comm_close();

/*
 * Send the handshake command to the connected device
 *
 * returns: 0 on success, -1 on error (printed to stderr)
 */
int comm_handshake();

/*
 * Turn an LED on
 *
 * @col: Column the LED is in
 * @row: Row the LED is in
 * @color: Color index to set
 *
 * returns: 0 on success, -1 on error (printed to stderr)
 */
int comm_led_on(u8 col, u8 row, u8 color);

/*
 * Method to run in a detached thread
 *
 * @filename - Not currently used
 *
 * returns: Does not return
 */
void* comm_task(void* filename);
