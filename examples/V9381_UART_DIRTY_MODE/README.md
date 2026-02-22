# V9381 UART Checksum Mode - CRC Debugging

## Overview

The V9381 UART driver supports two checksum modes for debugging:

1. **Clean Mode** - Enforces CRC validation (default for production)
2. **Dirty Mode** - Skips CRC validation and shows expected vs received CRC (debugging)

This mode aligns with the SPI implementation and helps verify if CRC calculation is implemented correctly.

## Use Cases

- Testing / debugging CRC calculation on your V9381 device
- Identifying communication issues related to checksum validation
- Comparing expected vs actual CRC to diagnose firmware issues
- Working around temporary CRC issues during development

## How to Use

### Enable Dirty Mode (Skip CRC Validation)

```cpp
v9381.SetChecksumMode(V93XX_Raccoon::ChecksumMode::Dirty);
```

### Enable Clean Mode (Enforce CRC Validation)

```cpp
v9381.SetChecksumMode(V93XX_Raccoon::ChecksumMode::Clean);
```

### Set Mode During Initialization

```cpp
// Initialize with Dirty mode (for debugging)
v9381.Init(SerialConfig::SERIAL_8O1, V93XX_Raccoon::ChecksumMode::Dirty);

// Initialize with Clean mode (default)
v9381.Init(SerialConfig::SERIAL_8O1, V93XX_Raccoon::ChecksumMode::Clean);

// Initialize with default (Clean mode)
v9381.Init(SerialConfig::SERIAL_8O1);
```

## CRC Output Examples

### Normal Reception (CRC Valid)
```
RegisterRead(0x02): marker=0x7D data=[0x01 0x00 0x00 0x00] CRC expected=0xAB received=0xAB ✓
```

### CRC Mismatch with Clean Mode
```
RegisterRead(0x02): marker=0x7D data=[0x01 0x00 0x00 0x00] CRC expected=0xAB received=0xCD ✗ - ERROR: CRC mismatch! (Clean mode)
```

### CRC Mismatch with Dirty Mode
```
RegisterRead(0x02): marker=0x7D data=[0x01 0x00 0x00 0x00] CRC expected=0xAB received=0xCD ✗ - WARNING: CRC mismatch! (Dirty mode - returning data)
```

## CRC Calculation

The V9381 uses the following CRC-8 formula:
```
CRC = 0x33 + ~(sum of payload bytes)
```

To verify CRC calculation:

1. Capture the "expected CRC" from debug output
2. Calculate CRC from the raw bytes sent
3. Compare with "received CRC" from device response

If they match → Your calculation matches the device  
If they don't match → Device's CRC algorithm may be different

## Typical Workflow

1. Start with `SetChecksumMode(ChecksumMode::Dirty)` or initialize with Dirty mode
2. Run your register read/write operations
3. Examine the CRC output in serial monitor
4. If all CRCs match, switch to `SetChecksumMode(ChecksumMode::Clean)` for normal operation
5. If CRCs don't match, investigate your CRC calculation

## Notes

- **Checksum mode only affects UART operations** (RegisterRead, RegisterWrite, RegisterBlockRead)
- Always use `ChecksumMode::Clean` in production code (or rely on default)
- Dirty mode is helpful during development and debugging
- Default mode is Clean - enforces CRC validation
- This implementation aligns with the SPI interface for consistency
