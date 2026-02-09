# V9381 UART Example (Shared Bus)

This example demonstrates UART communication with the V9381 energy monitoring ASIC using the same wiring as SPI on ESP32-S3. The pins are configured to allow switching between UART and SPI modes without rewiring.

## Hardware Setup

### Shared UART/SPI Pin Configuration
The V9381 supports dual-mode operation where UART and SPI share the same physical connections, with A0/A1 pins serving different purposes depending on the active mode:

| Pin Purpose | UART Mode | SPI Mode | V9381 Pin |
|---|---|---|---|
| TX Data | TX (GPIO 13) | MISO | TX/MISO |
| RX Data | RX (GPIO 11) | MOSI | RX/MOSI |
| Address Bit 0 | A0 (GPIO 12) | SCK | A0/SCK |
| Address Bit 1 | A1 (GPIO 10) | CS | A1/CS |

### Default ESP32-S3 Pin Configuration
```
ESP32-S3            V9381
--------            -----
GPIO11 (MOSI)   --> RX/MOSI
GPIO13 (MISO)   <-- TX/MISO
GPIO12 (SCK)    --> A0/SPCK  (UART address bit 0)
GPIO10 (CS)     --> A1/SPCSN (UART address bit 1)
GND             --- GND
3V3             --- VDD
```

For non-ESP32 boards, adjust pin definitions in the sketch accordingly.

## Serial Configuration

- **Baud Rate:** 19200 bps (configurable)
- **Data Bits:** 8
- **Parity:** Odd (8O1)
- **Stop Bits:** 1
- **Auto-baud:** Enabled by default after RX reset

## Features Demonstrated

1. **UART Address Configuration** - Setting A0/A1 pins for device addressing
2. **Initialization** - UART communication setup with hardware reset
3. **Configuration Loading** - Writing control and calibration registers
4. **Block Read** - Efficiently reading multiple registers
5. **Register Access** - Reading individual registers
6. **Mode Switching** - Preparing pins for SPI mode transition

## Multi-Drop Addressing

The V9381 supports addressing on a shared UART bus via A0 and A1 pins:

| A1 | A0 | Address |
|----|----|---------|
| 0  | 0  | 0x00    |
| 0  | 1  | 0x01    |
| 1  | 0  | 0x02    |
| 1  | 1  | 0x03    |

Multiple V9381 chips can share the same RX/TX lines with different addresses set via hardware pins.

## Switching to SPI

To switch from UART to SPI in the same sketch:

```cpp
// In UART mode, release A0/A1 pins
ReleaseUartAddressPinsForSpi();

// Then initialize SPI (see V9381_SPI example)
// v9381_spi.Init(...);
```

The `ReleaseUartAddressPinsForSpi()` function sets A0/A1 pins to INPUT mode, allowing the SPI bus to control SCK/CS.

## Output Format

Measurements are displayed in hexadecimal format. To convert to physical units, refer to the [V9381 datasheet](https://www.vangotech.com/en/uploadpic/175272285131.pdf) for scaling factors and conversion formulas.

See [DATASHEET_UART.md](DATASHEET_UART.md) for detailed UART protocol information from the datasheet.

## Customization

### Change Device Address
```cpp
const int V93XX_DEVICE_ADDRESS = 0x01;  // Change address as needed
ConfigureUartAddressPins(V93XX_DEVICE_ADDRESS);
```

### Adjust Pin Assignments
Edit the pin definitions for different boards:
```cpp
const int V93XX_UART_TX_PIN = 11;  // Change as needed
const int V93XX_UART_RX_PIN = 13;  // Change as needed
const int V93XX_ADDR0_PIN = 12;    // Change as needed
const int V93XX_ADDR1_PIN = 10;    // Change as needed
```

### Modify Block Read Registers
Edit the `ConfigureBlockRead()` call in `setup()` to read different registers according to your application needs.
