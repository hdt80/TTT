# Communication

Documentation on how the device and software talk to each other

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

## Bytes

Header | Address | Data
----- | ----- | -----
Single byte | Optional, up to 8 bytes | Used for writes, 1 byte

### Header

Bit 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
----- | - | - | - | - | - | - | -
R/W   |	T | T | T | M | A | A | A

#### R/W

Read write bit | Description
----- | -----
0 | Read
1 | Write

#### T

Type selection | Description | Read/Write | Addressed? | Notes
----- | ----- | ----- | ----- | -----
000 | Handshake | R | No | Sent out to find the device
001 | Grid contents | R | No | Read all 64 grid squares, left to right, top bottom
010 | Color values | RW | No | Only 8 color values available (0-7)
011 | RGB LEDs | RW | Yes | 4 MSBs are the row, 4 LSBs are the column
100 | Memory | R | Yes | Memory address to read from
101 | Unused | | |
110 | Unused | | |
111 | Unused | | |

#### M

Mode selection bit | Description
----- | -----
0 | No address bytes will be send after data
1 | Use address bits as length, next n bytes will be addresses

#### A

Address bits are used depending on the mode selection bit. When



