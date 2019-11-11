#pragma once

#include "types.h"

#define RESPONSE_STATUS_MASK 0x60
#define RESPONSE_STATUS_OFFSET 0x05
#define RESPONSE_STATUS_SUCCESS 0x00
#define RESPONSE_STATUS_WARNING 0x01
#define RESPONSE_STATUS_ERROR 0x02
#define RESPONSE_STATUS_UNKNOWN 0xFF

#define RESPONSE_LENGTH_MASK 0x07
#define RESPONSE_LENGTH_OFFSET 0x00

/*
 * Create a new connection to a new hardware device
 *
 * @filename Path the serial interface to the device can be found
 */
u8 comm_init(const char* filename);

/*
 * Send the handshake command to the connected device
 *
 * returns: 0 on success, 1 on error (printed to stderr)
 */
u8 comm_handshake();

/*
 * Close the connection made to the hardware device
 */
void comm_close();

/*
 * Method to run in a detached thread
 *
 * @filename - Not currently used
 *
 * returns: Does not return
 */
void* comm_task(void* filename);
