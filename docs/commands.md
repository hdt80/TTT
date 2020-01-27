# Commands

Commands and how their arguments are interpreted.

## Command list
* Handshake
* Read grid
* Set LED
* Set color value
* Read memory

## Handshake

Sent to the hardware device to indicate to accept commands from this port. Will probably be reworked in the future to add feature detection.

## Read grid

Read command for all 64 RGB LEDs.

## Set LED

Different from all other commands. The address field is not used, and there is address and data bytes alternating.

The first byte after the command byte indicates the first LED to set, followed by a data byte. If the `K` field is set, this pattern will continue, but with the data byte optional, depending on the value of the `P` field.

Bit 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
----- | ----- | ----- | ----- | ----- | ----- | ----- | ----- 
K | R | R | R | P | C | C | C

Field | Name | Description
----- | ----- | ----- 
K | Keep reading | If this bit is set, another LED will be sent
R | Row | Indicates the row to set
P | Previous color | Use the previous color from the previous address byte
C | Column | Indicates the column to set

### Examples

`0xB8 0x80 0x01 0x99 0xA2 0x02 0x7F`

Byte | Binary | K | R | P | C | Meaning
---- | ---- | ---- | ---- | ---- | ---- | ---- 
0xB8 | 1011'1000 | -- | -- | -- | -- | Start command
0x80 | 1000'0000 | 1 | 000 (0) | 0 | 000 (0) | Set (0, 0) to the following byte
0x01 | 0000'0001 | -- | -- | -- | -- | Color value to set previous LED to
0x99 | 1001'1001 | 1 | 001 (1) | 1 | 001 (1) | Set (1, 1) to previous color (0x01)
0xA2 | 1010'0001 | 1 | 010 (2) | 0 | 001 (1) | Set (2, 1) to the following byte
0x02 | 0000'0010 | -- | -- | -- | -- | Color value to set previous LED to
0x7F | 0111'1111 | 0 | 111 (7) | 1 | 111 (7) | Set (7, 7) to previous color (0x02), no more bytes 
