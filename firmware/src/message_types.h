#pragma once

// Mask to get the READWRITE bit of a message header
#define MSG_READWRITE_MASK 0x80
#define MSG_READWRITE_OFFSET 0x07
#define MSG_READWRITE_READ 0x00
#define MSG_READWRITE_WRITE 0x80
#define MSG_READWRITE_UNKNOWN 0xFF

// Mask to get the message type of a header
#define MSG_TYPE_MASK 0x70
#define MSG_TYPE_OFFSET 0x4
#define MSG_TYPE_HANDSHAKE 0x00
#define MSG_TYPE_GRID 0x01
#define MSG_TYPE_COLOR 0x02
#define MSG_TYPE_RGB 0x03
#define MSG_TYPE_MEMORY 0x04
#define MSG_TYPE_UNKNOWN 0xFF

// Mask to get the mode bit of a header
#define MSG_MODE_MASK 0x08
#define MSG_MODE_OFFSET 0x03
#define MSG_MODE_INDEX 0x00
#define MSG_MODE_ADDRESS 0x08
#define MSG_MODE_UNKNOWN 0xFF

// Mask to get the address of a header
#define MSG_ADDRESS_MASK 0x07
#define MSG_ADDRESS_OFFSET 0x00
#define MSG_ADDRESS_UNKNOWN 0xFF
