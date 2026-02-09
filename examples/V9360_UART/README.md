# V9360 UART Example

This example demonstrates basic UART communication with the V9360 energy monitoring ASIC.

## Hardware Setup

### Connections
- **TX** (Arduino) → RX pin (V9360)
- **RX** (Arduino) → TX pin (V9360)
- **GND** → GND (V9360)
- **3.3V or 5V** → VDD (V9360)

### Serial Configuration
- **Baud Rate:** 19200 bps (configurable via hardware serial)
- **Data Bits:** 8
- **Parity:** Odd (8O1)
- **Stop Bits:** 1
- **Auto-baud:** Supported (enabled by default)

## Features Demonstrated

1. **Initialization** - Setting up UART communication with hardware reset
2. **Configuration Loading** - Writing control and calibration registers
3. **Block Read** - Efficiently reading multiple registers in sequence
4. **Individual Register Access** - Reading single registers
5. **Continuous Monitoring** - Periodic measurement updates

## Register Values

The example configures the V9360 with standard initialization values and reads:

- Active Power (Phase A: PA1, Phase B: PB1)
- Reactive Power (Phase A: QA1, Phase B: QB1)
- Apparent Power (Phase A: SA1, Phase B: SB1)
- Voltage RMS (RMSUA)
- Current RMS (Phase A: RMSIA, Phase B: RMSIB)
- Grid Frequency (FRQ)

## Output Format

Measurements are displayed in hexadecimal format. To convert to physical units, refer to the [V9360 datasheet](https://www.vangotech.com/en/uploadpic/169329776853.pdf) for scaling factors and conversion formulas.

## Customization

### Change Serial Port
Edit the pin definitions to use a different serial port:
```cpp
const int V93XX_TX_PIN = 16;  // Change as needed
const int V93XX_RX_PIN = 15;  // Change as needed
```

### Modify Block Read Registers
Edit the `ConfigureBlockRead()` call in `setup()` to read different registers according to your application needs.

### Adjust Update Rate
Modify the `sleep(1)` call in the `loop()` function to change the measurement interval.
