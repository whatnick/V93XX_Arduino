# V9381 SPI Example

This example demonstrates how to use the V9381 energy monitoring ASIC with SPI communication.

## Hardware Setup

### Connections
- **CS** (Chip Select) - GPIO 5 (configurable)
- **MOSI** - Standard SPI MOSI pin
- **MISO** - Standard SPI MISO pin  
- **SCK** - Standard SPI Clock pin
- **VCC** - 3.3V or 5V (depending on your board)
- **GND** - Ground

### SPI Configuration
- **Mode:** SPI_MODE0 (CPOL=0, CPHA=0)
- **Clock Speed:** 1 MHz (configurable up to 10 MHz)
- **Bit Order:** MSB First

## Features Demonstrated

1. **Initialization** - Setting up the SPI interface
2. **Configuration Loading** - Writing control and calibration registers
3. **Block Read** - Efficiently reading multiple registers
4. **Individual Register Access** - Reading single registers
5. **Continuous Monitoring** - Periodic measurement updates

## Register Values

The example configures the V9381 with standard initialization values and reads:

- Active Power (Channels A & B)
- Reactive Power (Channels A & B)
- Apparent Power (Channels A & B)
- Voltage RMS
- Current RMS (Channels A & B)
- Grid Frequency

## Output Format

Measurements are displayed in hexadecimal format. To convert to physical units, refer to the [V9381 datasheet](https://www.vangotech.com/en/uploadpic/175272285131.pdf) for scaling factors and conversion formulas.

## Customization

### Change SPI Clock Speed
```cpp
V93XX_SPI v9381(V93XX_CS_PIN, SPI, 10000000); // 10 MHz
```

### Use Different SPI Bus
```cpp
V93XX_SPI v9381(V93XX_CS_PIN, SPI1, 1000000); // Use SPI1 instead of SPI
```

### Modify Block Read Registers
Edit the `ConfigureBlockRead()` call in `setup()` to read different registers according to your application needs.
