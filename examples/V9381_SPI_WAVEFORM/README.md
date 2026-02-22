# V9381 SPI Waveform Example

Captures V9381 waveform buffer over SPI using the `CaptureWaveform()` API for fast data acquisition.

## Features

- ✅ Uses `CaptureWaveform()` for automated capture
- ✅ SPI communication (~10x faster than UART)
- ✅ Dirty mode for CRC tolerance
- ✅ Prints samples as Python-compatible array
- ✅ Automatic overflow prevention via WAVESTORE_CNT
- ✅ Default parameters optimized for SPI speed

## Hardware Setup

### Wiring (ESP32-S3 DevKitC)

```
ESP32-S3          V9381
--------          -----
GPIO11 (MOSI) --> MOSI
GPIO13 (MISO) <-- MISO
GPIO12 (SCK)  --> SCK
GPIO10 (CS)   --> CS
3V3           --> VDD
GND           --> GND
```

## Workflow

1. Configure DSP_CTRL5 (channel, trigger, length)
2. Call `CaptureWaveform()` to capture samples
3. Print samples as hex pairs: `[LOWER,UPPER]`
4. Use `tools/plot_v9360_waveform.py` for offline analysis

## Sample Output

```
V9381 SPI Waveform Capture
SPI Ready. SYS_INTSTS: 0x00000000
System Version: 0x01020304
Configuration complete. Starting waveform capture...

Waveform Data (Python format):
wave_data = [
 1234,  5678,  9012,  3456,  7890,  1234,  5678,  9012,
 3456,  7890,  1234,  5678,  9012,  3456,  7890,  1234,
...
]

Captured 309 words (618 samples)
Copy data to Python for plotting with tools/plot_v9360_waveform.py
```

## Performance Comparison

| Interface | Capture Time | Notes |
|-----------|--------------|-------|
| **SPI (1MHz)** | ~200ms | Fast, reliable with default settings |
| **UART (19200)** | ~1200ms | Slower, requires tuning (4-word blocks) |

**SPI Advantages:**
- ✅ 6x faster waveform capture
- ✅ Uses default timeout (1000ms) and block size (16)
- ✅ No special reliability tuning needed
- ✅ Higher data throughput

## Python Analysis

Copy the printed waveform data and use the plotting tool:

```bash
python tools/plot_v9360_waveform.py
```

The tool will:
- Parse the Python array format
- Reconstruct time-domain waveform
- Plot voltage/current signal
- Perform FFT and show frequency spectrum

## Channel Configuration

Modify `ctrl5` to capture different channels:

```cpp
// Voltage A (default)
const uint32_t ctrl5 = ... | DSP_CTRL5_WAVE_U | ...;

// Current A
const uint32_t ctrl5 = ... | DSP_CTRL5_WAVE_I | ...;

// Voltage B
const uint32_t ctrl5 = ... | DSP_CTRL5_WAVE_UB | ...;

// Current B
const uint32_t ctrl5 = ... | DSP_CTRL5_WAVE_IB | ...;
```

## Sample Length Options

Adjust wave length in DSP_CTRL5:

```cpp
// 512 samples (default, Wave_Len=0)
((0 << DSP_CTRL5_WAVE_LEN_Pos) & DSP_CTRL5_WAVE_LEN_Msk)

// 1024 samples (Wave_Len=1)
((1 << DSP_CTRL5_WAVE_LEN_Pos) & DSP_CTRL5_WAVE_LEN_Msk)

// 2048 samples (Wave_Len=2)
((2 << DSP_CTRL5_WAVE_LEN_Pos) & DSP_CTRL5_WAVE_LEN_Msk)
```

**Note:** V9381 buffer stores 309 words (618 samples), so lengths >618 will wrap or truncate.

## Trigger Modes

### Manual Trigger (Default)

```cpp
DSP_CTRL5_WAVEMEM_MODE_MANUAL_SINGLE
```

Captures immediately when configured.

### Auto Trigger

```cpp
DSP_CTRL5_WAVEMEM_MODE_AUTO
```

Triggers on zero-crossing or threshold conditions (requires DSP_CTRL5 threshold config).

## For FFT Analysis

For on-board FFT processing instead of offline Python analysis, see:
- [examples/V9381_SPI_FFT](../V9381_SPI_FFT/) - SPI with ESP-DSP FFT
- [examples/V9381_UART_FFT](../V9381_UART_FFT/) - UART with ESP-DSP FFT
- [examples/V9360_UART_FFT](../V9360_UART_FFT/) - Multi-target FFT

## Comparison with UART Waveform

For V9381 UART waveform capture, see:
- [examples/V9381_UART_WAVEFORM](../V9381_UART_WAVEFORM/)

**When to use SPI:**
- Need fast capture (<250ms)
- Hardware supports SPI (V9381 only)
- Want default reliability without tuning

**When to use UART:**
- Multi-drop addressing needed
- Fewer wires preferred (2 data + 2 address vs 4 SPI)
- V9360 support required

## Troubleshooting

| Issue | Solution |
|-------|----------|
| "SPI communication failed" | Check MOSI/MISO/SCK/CS wiring |
| "Waveform capture timed out" | Verify LoadConfiguration() succeeded |
| No waveform variation | Apply test signal to V9381 inputs |
| Data shows all zeros | Check DSP_CTRL5 channel selection |

## Advanced Configuration

### Change Capture Rate

Adjust delay at end of `loop()`:

```cpp
delay(2000);  // 0.5 Hz (slower, easier to read)
delay(500);   // 2 Hz (faster updates)
delay(100);   // 10 Hz (maximum practical rate for printing)
```

### Increase SPI Speed

SPI clock defaults to 1 MHz. For faster capture:

1. Modify `V93XX_SPI.h` to expose clock parameter
2. Call `Init()` with higher clock (up to 10 MHz per datasheet)

```cpp
// Future API (not yet implemented)
v9381.Init(V93XX_SPI::WireMode::FourWire, true, 
           V93XX_SPI::ChecksumMode::Dirty,
           V93XX_SCK_PIN, V93XX_MISO_PIN, V93XX_MOSI_PIN,
           5000000);  // 5 MHz clock
```

### Use ThreeWire Mode

For hardware where CS cannot toggle:

```cpp
v9381.Init(V93XX_SPI::WireMode::ThreeWire, true, 
           V93XX_SPI::ChecksumMode::Dirty);
```

**Note:** Requires ≥400µs SCK-low time before operations.

## Notes

- SPI block reads are emulated (sequential single reads)
- Capture time includes: DSP config (~10ms) + capture (~50ms) + SPI transfer (~140ms)
- For production, consider Clean mode after verifying CRC stability
- Python array format is zero-indexed: `wave_data[0]` = first sample
