# V93XX Datasheet Documentation Index

**Document Version:** V4.9 (2025-01-22)  
**Original Source:** Vango Technologies, Inc. - V93XX_D1_Datasheet  
**Supported Devices:** V9381, V9360, V9340

## Overview

The V93XX is a single-phase metering chip supporting total-wave and fundamental-wave measurements across various power grid modes. This documentation has been extracted and reorganized from the official datasheet for optimal navigation by coding agents.

### Key Specifications
- **Main Power:** 3.3V (2.6V - 3.6V operating range)
- **Reference Voltage:** 1.21V (typical, 10 ppm/°C)
- **Typical Power Consumption:** ~2.6 mA
- **Operating Temperature:** -40°C to +105°C
- **Communication:** UART (1200-19200 bps) and SPI (800 kHz)

---

## Documentation Structure

### 1. **Hardware & Power Supply**
   - [Power Supply & Voltage Regulation](V93XX_POWER.md)
   - [Clock Generation & Management](V93XX_CLOCK.md)
   - [Reset Circuitry](V93XX_RESET.md)
   - [Bandgap Circuit](V93XX_BANDGAP.md)

### 2. **Communication Interfaces**
   - [UART Protocol & Operation](V93XX_UART.md)
   - [SPI Interface & Configuration](V93XX_SPI.md)

### 3. **Register Reference**
   - [Analog Control Registers (0x00-0x01)](V93XX_REGISTERS_ANALOG.md)
   - [System Control & Status (0x70-0x7F)](V93XX_REGISTERS_SYSTEM.md)
   - [Metering Control (0x02-0x07, 0x80)](V93XX_REGISTERS_METERING.md)
   - [Metering Data (0x08-0x24, 0x21)](V93XX_REGISTERS_DATA.md)
   - [Calibration Registers (0x25-0x38)](V93XX_REGISTERS_CALIBRATION.md)
   - [Energy Accumulator (0x39-0x54, 0x3E, 0x42, 0x45, etc.)](V93XX_REGISTERS_ENERGY.md)
   - [Threshold & Detection (0x55-0x60, 0x94-0x95)](V93XX_REGISTERS_THRESHOLDS.md)

### 4. **Metering & DSP**
   - [DSP Overview & Features](V93XX_DSP.md)
   - [RMS Calculation](V93XX_RMS.md)
   - [Power Calculation (Active, Reactive, Apparent)](V93XX_POWER_CALC.md)
   - [Frequency & Phase Measurement](V93XX_FREQUENCY_PHASE.md)
   - [Power-Creep Detection](V93XX_POWER_CREEP.md)

### 5. **Data Acquisition & Storage**
   - [Analog Inputs & ADC Configuration](V93XX_ANALOG_INPUT.md)
   - [Waveform Capture & Buffer](V93XX_WAVEFORM.md)
   - [Energy Accumulation](V93XX_ENERGY.md)

### 6. **Signal Monitoring**
   - [Zero-Crossing Detection](V93XX_ZERO_CROSSING.md)
   - [Voltage Swell/Dip Detection](V93XX_SWELL_DIP.md)
   - [Over/Under Voltage & Current](V93XX_OVERVOLTAGE.md)
   - [Signal IO Ports Configuration](V93XX_IO_PORTS.md)

### 7. **Pinouts & Physical**
   - [Pin Descriptions (V9381, V9360, V9340)](V93XX_PINS.md)
   - [Package & Dimensions](V93XX_PACKAGING.md)
   - [Reflow Soldering & Assembly](V93XX_ASSEMBLY.md)

### 8. **Quick Reference**
   - [Register Quick Map (All Addresses)](V93XX_REGISTER_MAP.md)
   - [Typical Parameter Values](V93XX_PARAMETERS.md)
   - [Common Usage Patterns](V93XX_COMMON_PATTERNS.md)

---

## Device Selection Guide

| Device | Packages | Key Features |
|--------|----------|--------------|
| **V9381** | SSOP24 | Full-featured, 24-pin, all I/O options |
| **V9360** | SOP16  | Mid-range, 16-pin, reduced I/O |
| **V9340** | SOP8   | Minimal, 8-pin, core functionality |

---

## Quick Navigation by Task

### Setting Up Communication
1. Check [UART Protocol](V93XX_UART.md) or [SPI Interface](V93XX_SPI.md)
2. Reference [Reset Circuitry](V93XX_RESET.md) for initialization
3. Verify baud rate in [UART Protocol](V93XX_UART.md#communication-timing)

### Configuring Metering
1. Start with [DSP Overview](V93XX_DSP.md)
2. Enable ADC channels: [Power Supply](V93XX_POWER.md)
3. Configure calibration: [Calibration Registers](V93XX_REGISTERS_CALIBRATION.md)
4. Set up measurement: [Metering Control](V93XX_REGISTERS_METERING.md)

### Reading Measurements
1. Check [RMS Calculation](V93XX_RMS.md)
2. Access data: [Metering Data Registers](V93XX_REGISTERS_DATA.md)
3. Understand formats: [Register Data Format](V93XX_REGISTERS_DATA.md)

### Implementing Energy Metering
1. Configure: [Energy Accumulation](V93XX_ENERGY.md)
2. Thresholds: [Threshold Registers](V93XX_REGISTERS_THRESHOLDS.md)
3. CF Output: [Energy Registers](V93XX_REGISTERS_ENERGY.md)

### Event Detection
1. Voltage/Current: [Over/Under Detection](V93XX_OVERVOLTAGE.md)
2. Swell/Dip: [Swell/Dip Detection](V93XX_SWELL_DIP.md)
3. Zero-Crossing: [Zero-Crossing Detection](V93XX_ZERO_CROSSING.md)

---

## Important Notes for Developers

### Register Access
- All registers use 32-bit format (2's complement for signed values)
- Configuration registers require self-checking via checksum
- Some registers have different behavior based on DSP mode

### Clock & Timing
- Default: 6.5536 MHz system clock (can be configured)
- ADC clock: selectable via ADCKSEL (1x, 2x, 1/4x, 1/2x)
- Frame timing: max 20ms between bytes in UART

### Checksum Implementation
- UART/SPI: Invert sum + 0x33
- Configuration checksum: Sum registers 0x0-0x7, 0x25-0x3A, 0x55-0x60, 0x80 must equal 0xFFFFFFFF
- See [Checksum Mode Documentation](UART_CHECKSUM_MODE.md) for advanced usage

---

## Revision History

| Date | Version | Key Changes |
|------|---------|------------|
| 2025-01-22 | V4.9 | Current - Reorganized for agent accessibility |
| 2024-10-28 | V4.7 | Electrical characteristic parameter updates |
| 2023-03-01 | V3.8 | V9340T renamed to V9340 |
| 2023-08-18 | V4.0 | Full D1 version chip support |

---

## Related Documentation

- **Arduino Library:** See README.md
- **Integration Guide:** See INTEGRATION.md
- **Testing Guide:** See TESTING.md
- **Checksum Mode Details:** See UART_CHECKSUM_MODE.md

---

## Support & Resources

For detailed technical support:
- Reference specific register addresses in [Register Map](V93XX_REGISTER_MAP.md)
- Check parameter constraints in [Parameters](V93XX_PARAMETERS.md)
- Review common patterns in [Usage Patterns](V93XX_COMMON_PATTERNS.md)

Last Updated: 2025-02-22
