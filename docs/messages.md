# Messaging

Documentation on how the device and software talk to each other.

## Needs:
- Handshake (S-\>H)
	- Find which port the device is on, instead of selecting it
	- Version?
- Ask for data (S-\>H)
	- Contents of all grid squares
- Set options (S-\>H)
	- Color values
- Set RGB LEDs (S-\>H)
	- Which one
	- Which color value to use (0-7)
- Logging (H-\>S)
	- Must be able to tell difference between response and logging data

## Software commands

A command to the hardware that expects a response.

### Format

Header | Address | Data
----- | ----- | -----
Single byte | Optional, up to 8 bytes | Used for writes, 1 byte

A single message contains at most three parts.

- Header: Indicates how the meaning of the rest of the bytes.
- Address: If the header's M bit is not set, there will be 0 address bytes. If set, contains the address to read or write to.
- Data: A single byte only present when this is a write message.

### Header

Bit 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
----- | - | - | - | - | - | - | -
R/W   |	T | T | T | M | A | A | A

#### R/W - Read write bit

Read write bit | Description
----- | -----
0 | Read
1 | Write

#### T - Type selection

What type of data will this command be performed on?

Type selection | Description | Read/Write | Addressed? | Notes
----- | ----- | ----- | ----- | -----
000 | Handshake | W | No | Sent out to find the device
001 | Grid contents | R | No | Read all 64 grid squares, left to right, top bottom
010 | Color values | W | No | Only 8 color values available (0-7)
011 | RGB LEDs | W | Yes | 4 MSBs are the row, 4 LSBs are the column
100 | Memory | R | Yes | Memory address to read from
101 | Unused | | |
110 | Unused | | |
111 | Unused | | |

Read/Write: What operation can be used on this type?

Addressed? Does this type have the M bit set?

#### M - Mode selection

Mode selection | Description
----- | -----
0 | No address bytes will be send after data
1 | Use address bits as length, next n bytes will be addresses

#### A - Address length/index

Address bits are used depending on the mode selection bit. When M is 0, A will contain an offset, and no address bytes will be present. When M is 1, A contains the number of address bytes that will follow the header.

## Hardware responses

Responses are sent by the hardware after a command has been received.

### Format

A response contains single byte header, followed by at least 1 data byte

### Header

Bit 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
----- | - | - | - | - | - | - | -
1     | R | R | 0 | 0 | N | N | N

Note: The first bit of a response is always a 1, this is to tell if a received message is a response or a logging entry

#### R - Command result

What was the status of the last command?

Type selection | Description | Contains
----- | ----- | -----
00 | Success | The command was successfully completed. The first data byte contains the command sent, followed by (2^N) - 1 more data bytes.
01 | Warning | The command was accepted, but not completed. N will always be 1, with the first data byte containing the command, and the second containing a warning ID.
10 | Error | The command was rejected. N will always be 1, with the first data byte containing the command, and the second containing an error ID
11 | Unused | N will always be 0, with the first data byte containing the command sent

#### N - 2^N length

How many bytes of data there will be.

Value | Descrition
----- | -----
000 | 1 data byte follows this header
001 | 2 data bytes follow this header
010 | 4 data bytes follow this header
011 | 8 data bytes follow this header
100 | 16 data bytes follow this header
101 | 32 data bytes follow this header
110 | 64 data bytes follow this header
111 | 128 data bytes follow this header

## Hardware logging entry

An ASCII logging message to interact with the device. Since ASCII uses only 7 bits, the MSB of a logging entry will always be 0, allowing to distinguish between a response and entry.

# Examples

## Software commands

### Handshake

Sender | Message | Meaning
----- | ----- | -----
S | 0x8F | Handshake, are you a TTT device?
D | 0x80 0x8F | Handshake, yes, I will accept commands from you now

### Reading grid contents

Sender | Message | Meaning
----- | ----- | -----
S | 0x10 | Send me the grid contents
D | 0x85 0x10 0x5F ... 0x00 | Grid contents

### Setting RGB LED to a color

Sender | Message | Meaning
----- | ----- | -----
S | 0xB9 0x13 0x02 | Set the RGB at column 2 row 4 to be color value 2 (purple)
D | 0x80 0xB9 | RGB set

### Setting a color value

Sender | Message | Meaning
----- | ----- | -----
S | 0xA2 0xE2 | Set color value 2 to purple
D | 0x80 0xA2 | I got your set color value command
