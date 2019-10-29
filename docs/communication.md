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
000 | Handshake | R | No | Sent out to find the device
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

A response contains two parts, a header and possibly data

### Header

Bit 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
1     | T | T | T | N | N | N | N

Note: The first bit of a response is always a 1, this is to tell if a received message is a response or a logging entry

#### T - Type response

What type of data is this response for?

Type selection | Description
----- | -----
000 | Handshake
001 | Grid contents
010 | Color values
011 | RGB LEDs
100 | Memory
101 | Unused
110 | Unused
111 | Unused

#### N - 2^N length

How many bytes of data there will be

Value | Descrition
----- | -----
0000 | Used when a write is completed to confirm to software the operation is complete
0001+ | The log 2 of how many data bytes are available. For example, sending 100, means 2^4 or 16 bytes of data will follow
