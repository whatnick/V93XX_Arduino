
# V93XX / V9381 SPI protocol mechanics (from datasheet)

This note captures the practical, wire-level SPI behavior needed to implement register/RAM access.

Source PDF: https://www.vangotech.com/en/uploadpic/175272285131.pdf

## Overview

- The chip supports **UART** and **SPI**. After **RSTN reset**, **RX reset**, or **global software reset**, it defaults back to **UART**.
- The SPI interface supports **standard 4‑wire** and **3‑wire** SPI.
- SPI uses **CPOL = 0** and **CPHA = 0** (SPI mode 0).

## Signals

- `SPICSN`: chip select (active low)
- `SPICLK`: SPI clock
- `SPIMOSI`: master → V93XX
- `SPIMISO`: V93XX → master (tri-stated when idle; supports multiple devices on a shared bus)

## Bit/byte ordering

- Multi-byte transfers are **little-endian by byte**: the **least-significant byte** is shifted first.
- Within each byte, bits are shifted **MSB-first** (“most significant bit … shifted out first”).
- Register/RAM payloads are **32-bit values**, transferred as 4 data bytes:
	- Data Byte 0 = bits `[7:0]`
	- Data Byte 1 = bits `[15:8]`
	- Data Byte 2 = bits `[23:16]`
	- Data Byte 3 = bits `[31:24]`

## SPI frame length (critical)

- Every **read** or **write** operation must be **exactly 48 clock edges** (6 bytes).
- If the 4‑wire SPI clock count is not 48, the device flags a SPI error condition.

## Command byte format

The first byte on the bus is `CMD`:

- `CMD[7:1]` = `ADDR[6:0]` (7-bit address)
- `CMD[0]` = R/W (0 = write, 1 = read)

Notes:

- Address `0x7F` is used for SPI interface control sequences (see initialization and high-address access).
- Some address ranges are **RAM** (slower max clock; see “Clock rate limits”).

## Checksum byte

The last byte of the 6-byte frame is `CKSUM`.

For both SPI **read** and **write**, the checksum is computed as:

$$
\mathrm{CKSUM} = 0x33 + \sim(\mathrm{CMD} + D0 + D1 + D2 + D3)
$$

Where:

- $D0..D3$ are the four data bytes (for read: the returned data; for write: the bytes you send).
- The sum and inversion are performed as **8-bit arithmetic** (mod 256).

Practical implication:

- On **read**, validate `CKSUM` to confirm the response is timely and correct.
- On **write**, the chip does not return a valid response, so the usual way to confirm is to **read back** and verify.

## Read transaction (48 clocks)

During a read, the master transmits 6 bytes while simultaneously receiving 6 bytes:

1. Byte 0 (master → chip): `CMD` with `CMD[0]=1`
	 - The byte received during this first byte is “don’t care”.
2. Bytes 1–5 (master → chip): any values (dummy bytes)
	 - These 40 clocks are used to receive the **5-byte response**:
		 - Response Byte 0: don’t care
		 - Response Byte 1: Data Byte 0
		 - Response Byte 2: Data Byte 1
		 - Response Byte 3: Data Byte 2
		 - Response Byte 4: Data Byte 3
		 - Response Byte 5: `CKSUM`

## Write transaction (48 clocks)

On write, the master sends:

1. Byte 0: `CMD` with `CMD[0]=0`
2. Bytes 1–4: Data Byte 0..3 (little-endian bytes of the 32-bit word)
3. Byte 5: `CKSUM`

The device **does not return a valid response** during SPI write; use a follow-up read to confirm.

## SPI interface initialization (required after reset)

Before any SPI access is accepted, you must initialize SPI by **writing** `0x5A7896B4` to address `0x7F` over SPI.

The datasheet shows the exact 6-byte write frame:

- `CMD` = `0xFE`
- Data bytes = `0xB4 0x96 0x78 0x5A`
- `CKSUM` = `0x18`

After initialization, it’s recommended to **read any readable register** and verify the checksum to confirm SPI is active.

After any RSTN / RX / global software reset, SPI is reset back to UART and you must **re-run initialization**.

## Accessing addresses ≥ 0x80 (high-address window)

SPI addressing is fundamentally **7-bit** (`ADDR[6:0]`), but the chip provides an **automatic +0x80 address offset mode**.

- To enable the +0x80 offset, write `0x4A985B67` to address `0x7F` via SPI.
	- After that, “all address accesses add a 128 offset to the originally given 7-bit address”.
- To cancel the offset mode and return to low addresses, write `0x76B589A4` to address `0x7F`.

The datasheet recommendation is to configure addresses below `0x80` first, then enable the offset mode for high addresses.

## 4-wire SPI mode requirements

- SPI interface initialization must be done after reset events.
- There must be a **50 μs** interval between any two read/write operations.
- Each operation must be **48 clocks**.
- `SPICSN` must return **high** after each read/write.

## 3-wire SPI mode requirements

- SPI interface initialization must be done after reset events.
- `SPICSN` is **always low**.
- Each operation must be **48 clocks**.
- Before each read/write, `SPICLK` must be held **low** for at least **400 μs**.

## Clock rate limits (from datasheet)

- When reading **registers**, max SPI clock is **1/4 of system CLK**.
- When reading **RAM**, max SPI clock is **1/16 of system CLK**.

The datasheet notes that if the MCU reads RAM/register data “not in time”, the chip may return an **incorrect checksum**.

RAM address ranges called out in the datasheet:

- `0x11–0x38`, `0x43–0x54`, `0x68`, `0x69`
- Remaining addresses are registers.

## SPI timeout rule

- During a transaction, the time between rising edges of any two `SPICLK` pulses must be **< 20 ms**, otherwise the chip considers an SPI timeout.

