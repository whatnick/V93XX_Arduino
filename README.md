# V93XX Umbrella Repository

Collection of Arduino libraries implementing communication protocols for the Vangotech [V93XX](https://www.vangotech.com/en/uploadpic/169329776853.pdf) series of ASICs for single-phase energy monitoring.

## 📚 Documentation

All documentation has been consolidated in the **[docs/](docs/)** folder for easier navigation:

- **[docs/README.md](docs/README.md)** - Documentation index and navigation hub
- **[docs/GETTING_STARTED.md](docs/GETTING_STARTED.md)** - Quick start guide (5 minutes)
- **[docs/TESTING.md](docs/TESTING.md)** - Complete testing framework
- **[docs/INTEGRATION.md](docs/INTEGRATION.md)** - Integration guide for your projects
- **[docs/API_REFERENCE.md](docs/API_REFERENCE.md)** - Complete API documentation
- **[docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)** - Design patterns and architecture
- **[docs/UART_CHECKSUM_MODE.md](docs/UART_CHECKSUM_MODE.md)** - ChecksumMode feature details
- **[docs/COPILOT.md](docs/COPILOT.md)** - GitHub Copilot integration
- **[docs/STATUS.md](docs/STATUS.md)** - Implementation status and production readiness

👉 **[Start here](docs/README.md)** if you're new to this library!

---

## Supported Communication Protocols

### UART
The UART protocol is half-duplex with alternating send and receive cycles. Supports multi-drop addressing on the same bus via A1/A0 address pins. The baud rate is set via auto-bauding, with supported rates from 1200 to 19200 bps.

- **V9360**: UART is the primary interface
- **V9381**: UART supported with configurable device addressing (A0/A1 pins)

**Library:** `V93XX_Raccoon`

### SPI
The SPI protocol provides a synchronous full-duplex interface for communicating with the [V9381](https://www.vangotech.com/en/uploadpic/175272285131.pdf) energy monitoring ASIC. Supports standard SPI modes with configurable clock speeds up to 10 MHz, with FourWire and ThreeWire modes available.

**Library:** `V93XX_SPI`

## Examples

- **V9360_UART** - Basic UART communication example for V9360
- **V9360_UART_WAVEFORM** - Advanced UART example with waveform buffer capture and analysis
- **V9381_UART** - UART communication example for V9381 (shared bus with SPI pin configuration)
- **V9381_SPI** - SPI communication example for V9381

## Hardware Requirements

### V9360 (UART)
- TX pin (Arduino) → RX pin (V9360)
- RX pin (Arduino) → TX pin (V9360)
- Hardware serial port with 8O1 configuration (8 bits, odd parity, 1 stop bit)
- Supported baud rates: 1200 to 19200 bps (auto-baud supported)

### V9381 (UART)
- TX pin (Arduino) → RX/MOSI pin (V9381)
- RX pin (Arduino) → TX/MISO pin (V9381)
- A0 pin (Arduino) → A0/SCK pin (V9381) - Device address bit 0
- A1 pin (Arduino) → A1/CS pin (V9381) - Device address bit 1
- Hardware serial port with 8O1 configuration
- Supported baud rates: 1200 to 19200 bps (auto-baud supported)
- Supports multi-drop addressing (up to 4 devices on shared bus)

### V9381 (SPI)
- MOSI → MOSI pin (V9381)
- MISO ← MISO pin (V9381)
- SCK → SCK pin (V9381)
- CS → CS pin (V9381, configurable)
- Supports SPI Modes 0, FourWire and ThreeWire modes available
- Clock speeds: 400 kHz to 10 MHz (default 1 MHz)
- 3.3V or 5V supply (depending on your board)