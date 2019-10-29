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

## Messages

### Software commands

Bit 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0
------|---|---|---|---|---|---|---
M		T	T	T	T	L	L	L

#### M
Mode selection bit.|
--------|----
1 | Write
0 | Read
