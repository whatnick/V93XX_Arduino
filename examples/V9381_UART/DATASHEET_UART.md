# V93XX / V9381 UART protocol mechanics (from datasheet)

This note captures the practical, wire-level UART behavior needed to implement register access.

Source PDF: https://www.vangotech.com/en/uploadpic/175272285131.pdf

## Overview

- Default interface after RSTN, RX reset, or global software reset is UART.
- UART is asynchronous, half-duplex, and LSB-first.
- One byte is 11 bits: 1 start (low), 8 data, 1 odd parity, 1 stop (high).
- Multi-drop supported: TX is high-impedance when idle; up to 4 devices share a bus via A1/A0.
- Supported baud rate: 1200 to 19200 bps.
- Auto-baud adapts from the first frame header; re-enable by setting `UARTAUTOEN` (SYS_MISC bit0).

## UART errors (UART_ERR)

The UART error flag sets when any of these conditions happen:
- Timeout: time between any two bytes exceeds 20 ms.
- Parity bit error.
- Checksum bit error.
- Checksum byte error.

## Timing parameters

- $t_{RB}$: receive 1 byte time, $t_{RB} = 11 / \text{baudrate}$.
- $t_{RF}$: max time between bytes while receiving a command frame, $t_{RF} = 16 / \text{baudrate}$.
  - Example at 4800 bps: $t_{RF} \approx 3.33$ ms.
- $t_{RTD}$: delay from RX command end to TX response start, 0 to 20 ms.
  - For broadcast, no response; leave at least 2 ms between frames.
- $t_{TB}$: transmit 1 byte time, $t_{TB} = 11 / \text{baudrate}$.
- $t_{TBD}$: delay between bytes in a response frame, 0 to 20 ms.
- $t_{TRD}$: delay from response end to next command start; >= 2 ms recommended.
- RX/TX rise/fall time: ~300 ns.

## Common framing

- `HEADER` byte is `0x7D`.
- `CMD1` encodes device address and length.
- `CMD2` encodes the start address.
- Checksum uses 8-bit arithmetic with inversion.

Checksum form used by UART commands and responses:

$$
\mathrm{CKSUM} = 0x33 + \sim(\text{sum of bytes})
$$

## Broadcast write

- No response is sent (avoid bus conflicts).
- A1/A0 are don’t care.
- Supports writing 1 to 16 consecutive 32-bit registers.

Command frame:

- Byte 0: `HEADER = 0x7D`
- Byte 1: `CMD1`
  - Bits [7:4] = N (0 to 15) for 1..16 words
  - Bits [3:2] = don’t care
  - Bits [1:0] = `0b00`
- Byte 2: `CMD2` = start address D
- Data bytes: for each word, 4 bytes little-endian (Byte0 is bits [7:0])
- Checksum after each word group as defined below

Checksum after word 0:

$$
\mathrm{CKSUM}_0 = 0x33 + \sim(CMD1 + CMD2 + D0_0 + D0_1 + D0_2 + D0_3)
$$

Checksum after word N (cumulative over words 0..N):

$$
\mathrm{CKSUM}_N = 0x33 + \sim(CMD1 + CMD2 + \sum_{k=0}^{N}\sum_{b=0}^{3} Dk_b)
$$

## Read operation

- Device address A1/A0 must match.
- Supports reading 1 to 16 consecutive 32-bit registers.
- Response includes data and checksum.

Command frame:

- Byte 0: `HEADER = 0x7D`
- Byte 1: `CMD1` = [A1 A0] + `01` + length N
  - Bits [7:4] = N (0 to 15) for 1..16 words
  - Bits [3:2] = A1 A0
  - Bits [1:0] = `0b01`
- Byte 2: `CMD2` = start address D
- Byte 3: `CKSUM = 0x33 + ~(CMD1 + CMD2)`

Response frame:

- Data bytes for each word (little-endian)
- Final checksum:

$$
\mathrm{CKSUM} = 0x33 + \sim(CMD1 + CMD2 + \sum_{k=0}^{N}\sum_{b=0}^{3} Dk_b)
$$

If N = 0 (read 1 word), response is 5 bytes (4 data + checksum).

## Write operation

- Device address A1/A0 must match.
- Supports writing 1 to 16 consecutive 32-bit registers.
- Response is a single checksum byte used to validate the write.

Command frame:

- Byte 0: `HEADER = 0x7D`
- Byte 1: `CMD1` = [A1 A0] + `10` + length N
  - Bits [7:4] = N (0 to 15) for 1..16 words
  - Bits [3:2] = A1 A0
  - Bits [1:0] = `0b10`
- Byte 2: `CMD2` = start address D
- Data bytes (little-endian for each word)
- Checksum after each word group, same as broadcast write

Response frame:

- One checksum byte (`CKSUM`) from the device
- If `CKSUM` equals the last command checksum (`CKSUM_N`), the write succeeded

## Block read (address mapping)

- Allows reading non-consecutive registers using an address map.
- Up to 16 addresses mapped via `SYS_BLK0_ADDR`..`SYS_BLK3_ADDR`.
- Read length and start index are encoded in `CMD1` and `CMD2`.

Command frame:

- Byte 0: `HEADER = 0x7D`
- Byte 1: `CMD1` = [A1 A0] + `11` + length N
  - Bits [7:4] = N (0 to 15) for 1..16 words
  - Bits [3:2] = A1 A0
  - Bits [1:0] = `0b11`
- Byte 2: `CMD2` = start address index within the 16-entry map
- Byte 3: `CKSUM = 0x33 + ~(CMD1 + CMD2)`

Response frame:

- Data bytes for each mapped address (little-endian)
- Final checksum uses the same formula as standard read

If the read extends past the end of the map, it wraps to the start.
