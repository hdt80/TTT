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

Header | Data
----- | -----
Single byte | Optional, used in a per command basis

A single message contains at most three parts.

- Header: Indicates how the meaning of the rest of the bytes.
- Data: All the bytes coming after the header. How the data is used depends on the command being sent.

### Header

Bit 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
----- | - | - | - | - | - | - | -
R/W   |	T | T | T | A | A | A | A

#### R/W - Read write bit

Read write bit | Description
----- | -----
0 | Read
1 | Write

#### T - Type selection

What type of data will this command be performed on?

Type selection | Description | Read/Write | Description
----- | ----- | ----- | -----
000 | Handshake | W | Sent out to find the device
001 | Grid contents | R | Read all 64 grid squares, left to right, top bottom
010 | Color values | W | Only 8 color values available (0-7)
011 | RGB LEDs | W | 4 MSBs are the row, 4 LSBs are the column
100 | Memory | R | Memory address to read from
101 | Set LEDs | W | See commands.md for more details
110 | Unused | | |
111 | Unused | | |

Read/Write: What operation can be used on this type?

#### A - Command arguments

Command arguments are implemented on a per command basis.

## Hardware responses

Responses are sent by the hardware after a command has been received.

### Format

A response contains single byte header, followed by the command byte, then optionally data bytes. The command byte is the command that generated this response, allowing the software to ensure this response it expects.

### Header

Bit 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
----- | - | - | - | - | - | - | -
1     | R | D | 0 | 0 | N | N | N

Note: The first bit of a response is always a 1, this is to tell if a received message is a response or a logging entry

#### R - Command result

What was the status of the last command?

Type selection | Description | Meaning 
----- | ----- | -----
0 | Success | The command was successfully completed.
1 | Error | The command was not completed. 

#### D - Data available

Will data bytes follow the command byte?

Value | Description
----- | -----
0 | No data follows
1 | Data follows

#### N - Data length

If data is available (D == 1), 2^N data bytes follow the command byte

Value | Descrition
----- | -----
000 | 1 byte follows the command byte
001 | 2 data bytes follow the command byte
... | ...
111 | 128 data bytes follow the command byte

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
