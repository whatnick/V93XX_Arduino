# V93XX Arduino Library - Development Tools

This directory contains Python tools for debugging and testing V93XX ASIC communication.

## Main Tools

### capture_v9381_uart.py
**Primary UART debugging tool** - Captures and analyzes V9381 UART communication using Saleae Logic analyzer.

**Features:**
- Captures UART traffic on both TX and RX lines
- Automatically resets ESP32 during capture to catch boot sequence
- Exports decoded frames to CSV
- Verifies request frame checksums per datasheet
- Analyzes frame structure and protocol compliance

**Usage:**
```bash
python tools/capture_v9381_uart.py
```

**Requirements:**
- Saleae Logic 2 software running with automation API enabled (port 10430)
- ESP32-S3 connected on COM7
- V9381 wired according to library examples

**Output:**
- Raw capture (.sal) file for viewing in Logic 2
- CSV exports of TX/RX data with UART decoding
- Frame-by-frame analysis with checksum verification

### capture_v9381_spi.py
Captures and analyzes V9381 SPI communication.

### analyze_spi_csv.py
Post-processing tool for analyzing SPI transaction CSV exports.

### plot_v9360_waveform.py
Visualization tool for V9360 UART waveform data.

## Configuration

Edit the constants at the top of each script to match your setup:
- `ESPTOOL_PORT`: COM port for ESP32 (default: COM7)
- `UART_BAUD`: V9381 UART baud rate (default: 19200)
- `UART_RX_CHANNEL`: Saleae channel for V9381 TX (default: 1)
- `UART_TX_CHANNEL`: Saleae channel for ESP32 TX (default: 7)

## Dependencies

Install via uv:
```bash
uv sync
```

Or manually:
```bash
pip install saleae pyserial
```

## Saleae Logic Setup

1. Download and install [Saleae Logic 2](https://www.saleae.com/downloads/)
2. Enable automation API: Preferences → Developer → Enable Socket API (port 10430)
3. Connect Logic analyzer and set up channels per your wiring

## Workflow

### UART Communication Debugging

1. **Wire the hardware:**
   - Channel 0: V9381 A1 (address pin)
   - Channel 1: V9381 TX/MISO → ESP32 RX
   - Channel 2: V9381 A0 (address pin)
   - Channel 7: ESP32 TX/MOSI → V9381 RX

2. **Compile and upload sketch:**
   ```bash
   arduino-cli compile -b esp32:esp32:esp32s3 examples/V9381_UART/V9381_UART.ino
   arduino-cli upload -b esp32:esp32:esp32s3 -p COM7 examples/V9381_UART/V9381_UART.ino
   ```

3. **Capture and analyze:**
   ```bash
   python tools/capture_v9381_uart.py
   ```

4. **Review results:**
   - Check console output for frame analysis
   - Open `.sal` file in Logic 2 for detailed inspection
   - Review CSV exports for post-processing

## Checksum Verification

The V9381 UART protocol uses a specific checksum formula (per datasheet):

**Request:** `CKSUM = 0x33 + ~(CMD1 + CMD2)`
**Response:** `CKSUM = 0x33 + ~(CMD1 + CMD2 + Data[0] + Data[1] + Data[2] + Data[3])`

The capture tool automatically verifies request checksums and notes response frames for manual verification.

## Troubleshooting

**"No Saleae devices found"**
- Ensure Logic 2 is running
- Check automation API is enabled (port 10430)
- Verify USB connection to Logic analyzer

**"Could not open COM7"**
- COM port may be in use by another program
- Check Device Manager for correct port number
- Try closing Arduino IDE or other serial terminals

**"0 bytes captured"**
- Verify UART is transmitting (check with oscilloscope)
- Confirm baud rate matches (19200 for V9381)
- Check parity setting (odd parity required)
- Verify wiring connections

**"Checksum invalid"**
- Confirm datasheet formula is implemented correctly
- Check for framing errors in capture
- Verify V9381 power supply stability

## Development Notes

- Removed duplicate pyserial-based monitoring tools (focus on Saleae for ground truth)
- Consolidated checksum verification into main capture tool
- Roadmap: Fix ESP32 debug UART output monitoring separately
