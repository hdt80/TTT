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
#define MSG_TYPE_SET_RGB 0x05
#define MSG_TYPE_UNKNOWN 0xFF

// Mask to get the arg bits of a header
#define MSG_ARGS_MASK 0x0F
#define MSG_ARGS_OFFSET 0x00

// Set RGB masks//offsets
#define SET_RGB_KEEP_MASK 0x80
#define SET_RGB_KEEP_OFFSET 0x07
#define SET_RGB_ROW_MASK 0x70
#define SET_RGB_ROW_OFFSET 0x04
#define SET_RGB_PREVIOUS_MASK 0x08
#define SET_RGB_PREVIOUS_OFFSET 0x03
#define SET_RGB_COLUMN_MASK 0x07
#define SET_RGB_COLUMN_OFFSET 0x00
