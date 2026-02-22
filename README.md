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

**Library:** `V93XX_UART`

### SPI
The SPI protocol provides a synchronous full-duplex interface for communicating with the [V9381](https://www.vangotech.com/en/uploadpic/175272285131.pdf) energy monitoring ASIC. Supports standard SPI modes with configurable clock speeds up to 10 MHz, with FourWire and ThreeWire modes available.

**Library:** `V93XX_SPI`

## Examples

- **V9360_UART** - Basic UART communication example for V9360
- **V9360_UART_WAVEFORM** - Advanced UART example with waveform buffer capture and analysis
- **V9381_UART** - UART communication example for V9381 (shared bus with SPI pin configuration)
- **V9381_SPI** - SPI communication example for V9381

## Hardware Requirements

Wiring diagrams below use the ESP32-S3 DevKitC defaults from the examples. You can remap pins in your sketch if needed.

### V9360 (UART)

```mermaid
flowchart LR
	subgraph ESP32[ESP32-S3 DevKitC]
		TX16[GPIO16 (TX)]
		RX15[GPIO15 (RX)]
		V3[3V3]
		GND1[GND]
	end
	subgraph V9360[V9360 (UART)]
		RX[UART RX]
		TX[UART TX]
		VDD[VDD (3.3V)]
		GND2[GND]
	end
	TX16 --> RX
	RX15 --> TX
	V3 --> VDD
	GND1 --> GND2
```

- Hardware serial port with 8O1 configuration (8 bits, odd parity, 1 stop bit)
- Supported baud rates: 1200 to 19200 bps (auto-baud supported)

### V9381 (UART)

```mermaid
flowchart LR
	subgraph ESP32[ESP32-S3 DevKitC]
		TX11[GPIO11 (MOSI/TX)]
		RX13[GPIO13 (MISO/RX)]
		A0[GPIO12 (SCK/A0)]
		A1[GPIO10 (CS/A1)]
		V3[3V3]
		GND1[GND]
	end
	subgraph V9381[V9381 (UART)]
		RX[UART RX / MOSI]
		TX[UART TX / MISO]
		A0P[A0 / SCK]
		A1P[A1 / CS]
		VDD[VDD (3.3V)]
		GND2[GND]
	end
	TX11 --> RX
	RX13 --> TX
	A0 --> A0P
	A1 --> A1P
	V3 --> VDD
	GND1 --> GND2
```

- Hardware serial port with 8O1 configuration
- Supported baud rates: 1200 to 19200 bps (auto-baud supported)
- Supports multi-drop addressing (up to 4 devices on shared bus)

### V9381 (SPI)

```mermaid
flowchart LR
	subgraph ESP32[ESP32-S3 DevKitC]
		MOSI[GPIO11 (MOSI)]
		MISO[GPIO13 (MISO)]
		SCK[GPIO12 (SCK)]
		CS[GPIO10 (CS)]
		V3[3V3]
		GND1[GND]
	end
	subgraph V9381[V9381 (SPI)]
		MOSI2[MOSI]
		MISO2[MISO]
		SCK2[SCK]
		CS2[CS]
		VDD[VDD (3.3V)]
		GND2[GND]
	end
	MOSI --> MOSI2
	MISO --> MISO2
	SCK --> SCK2
	CS --> CS2
	V3 --> VDD
	GND1 --> GND2
```

- Supports SPI Modes 0, FourWire and ThreeWire modes available
- Clock speeds: 400 kHz to 10 MHz (default 1 MHz)
- 3.3V supply required on ESP32-S3