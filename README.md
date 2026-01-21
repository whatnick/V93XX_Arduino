# V93XX Umbrella repository

This repository is a collection of Arduino libraries implementing communication protocols for the Vangotech [V93XX](https://www.vangotech.com/en/uploadpic/169329776853.pdf) series of ASIC for Single-phase energy monitoring.

## Supported Communication Protocols

### UART (V9360)
The UART protocol is simplex with alternative send and receive cycles. The baud rate is set via auto-bauding via the command byte after the first reset cycle triggered by pulling the RX pin of the chip (TX pin of the host CPU) low for a period of 92.5 ms.

**Library:** `V93XX_Raccoon`

### SPI (V9381)
The SPI protocol provides a synchronous full-duplex interface for communicating with the [V9381](https://www.vangotech.com/en/uploadpic/175272285131.pdf) energy monitoring ASIC. Supports standard SPI modes with configurable clock speeds up to several MHz.

**Library:** `V93XX_SPI`

## Examples

- **V9360_UART** - Basic UART communication example for V9360
- **V9360_UART_WAVEFORM** - Advanced UART example with waveform analysis
- **V9381_SPI** - SPI communication example for V9381

## Hardware Requirements

### V9360 (UART)
- TX pin (Arduino) → RX pin (V9360)
- RX pin (Arduino) → TX pin (V9360)
- Hardware serial port with 8O1 configuration

### V9381 (SPI)
- MOSI, MISO, SCK - Standard SPI pins
- CS - Chip Select pin (configurable)
- Supports SPI Mode 0, up to 1-10MHz clock