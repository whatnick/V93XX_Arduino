# V9381 SPI FFT Example

V9381-specific FFT example using SPI communication in Dirty mode for fast, reliable waveform capture and on-board FFT analysis.

## Features

- ✅ Uses `CaptureWaveform()` API for automated capture
- ✅ SPI communication for fast data transfer (~10x faster than UART)
- ✅ Dirty mode for CRC tolerance during capture
- ✅ ESP-DSP FFT2R with Hann windowing
- ✅ Automatic peak frequency detection
- ✅ Default reliability parameters optimized for SPI

## Why SPI Over UART?

| Feature | SPI (1MHz) | UART (19200 baud) |
|---------|------------|-------------------|
| **Capture Time** | ~200ms | ~1200ms |
| **Block Size** | 16 words (default) | 4 words (reliability) |
| **Timeout** | 1000ms (default) | 2000ms (extended) |
| **Reliability** | High | Moderate (requires tuning) |
| **Wiring** | 4 wires + power | 2 wires + 2 address + power |

**Use SPI when:**
- Fast capture is critical
- You need high data throughput
- Hardware supports SPI (V9381 only)

**Use UART when:**
- Multi-drop addressing needed (up to 4 devices)
- Fewer wires preferred
- V9360 support required

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

### SPI Configuration

This example uses **FourWire mode** with the following settings:
- **Clock**: 1 MHz (default, adjustable up to 10 MHz)
- **Mode**: SPI Mode 0 (CPOL=0, CPHA=0)
- **CS behavior**: Toggles between transactions with ≥50µs inter-frame delay
- **Checksum**: Dirty mode (tolerates CRC mismatches)

## Waveform Configuration

```cpp
const uint32_t ctrl5 = BuildWaveformCtrl5();
// Configures:
// - Manual single trigger
// - Channel UA (Voltage A)
// - 512 samples (Wave_Len=0)
```

### Channel Selection

Modify `BuildWaveformCtrl5()` to capture different channels:

```cpp
// Voltage A (default)
DSP_CTRL5_WAVE_U | ...

// Current A
DSP_CTRL5_WAVE_I | ...

// Voltage B
DSP_CTRL5_WAVE_UB | ...

// Current B
DSP_CTRL5_WAVE_IB | ...
```

## Expected Output

```
V9381 SPI FFT Example (ESP-DSP)
Using Dirty mode for capture robustness
SPI Ready. SYS_INTSTS: 0x00000000
FFT ready. Length: 512
Starting continuous waveform capture and FFT...

FFT peak: bin=5 magnitude=1234.5678
FFT peak: bin=5 magnitude=1235.1234
FFT peak: bin=5 magnitude=1232.9876
...
```

## Performance Metrics

### Capture Performance

| Metric | Value | Notes |
|--------|-------|-------|
| **Capture Time** | ~200ms | Includes config + DSP capture + SPI transfer |
| **FFT Time** | ~50ms | ESP-DSP radix-2 FFT |
| **Total Cycle** | ~250ms | 4 Hz update rate |
| **Data Rate** | ~3 KB/s | 309 words × 4 bytes @ 1 Hz |

### SPI Timing

- **Single register read**: ~500µs @ 1 MHz
- **Block read (16 words)**: ~8ms @ 1 MHz
- **Full waveform (309 words)**: ~155ms @ 1 MHz

## FFT Details

### ESP-DSP Pipeline

1. **Unpack Samples**: Convert 16-bit packed samples to float32
2. **Apply Hann Window**: Reduce spectral leakage
3. **Execute FFT**: Radix-2 real FFT (512 points)
4. **Bit Reversal**: Reorder FFT output
5. **Magnitude**: Convert complex to magnitude spectrum
6. **Peak Detection**: Find maximum magnitude bin

### Window Function

The example uses a **Hann window** to reduce spectral leakage:

```
w[n] = 0.5 * (1 - cos(2π * n / (N-1)))
```

### Frequency Resolution

```
Δf = Sampling Rate / FFT Length
```

For V9381 with ~3.2 kHz sampling rate and 512-point FFT:
```
Δf ≈ 6.25 Hz per bin
```

## Calibration

For accurate frequency measurement:

1. **Apply known test signal** (e.g., 50 Hz from signal generator)
2. **Capture and run FFT**
3. **Verify peak bin** matches expected frequency
4. **Adjust sampling rate** if needed

Example:
```
50 Hz signal should appear at bin 8 (50 / 6.25 ≈ 8)
```

## Troubleshooting

| Issue | Cause | Solution |
|-------|-------|----------|
| "SPI communication failed" | Wiring incorrect | Check MOSI/MISO/SCK/CS connections |
| "Waveform capture timed out" | DSP not configured | Verify LoadConfiguration() success |
| FFT shows noise | No input signal | Apply test signal to V9381 inputs |
| Peak bin inconsistent | Spectral leakage | Verify Hann window applied correctly |

## Comparison with UART FFT

For the V9381 UART FFT example with reliability tuning, see:
- [examples/V9381_UART_FFT](../V9381_UART_FFT/)

For the multi-target FFT example supporting V9360/V9381/SPI, see:
- [examples/V9360_UART_FFT](../V9360_UART_FFT/)

## Advanced Configuration

### Increase SPI Speed

Modify `V93XX_SPI.h` or pass custom clock in `Init()`:

```cpp
// Default: 1 MHz
v9381.Init(V93XX_SPI::WireMode::FourWire, true, 
           V93XX_SPI::ChecksumMode::Dirty);

// Faster: 5 MHz (check datasheet limits)
// Note: May require modifying library to expose clock parameter
```

### Use ThreeWire Mode

For setups where CS cannot be controlled:

```cpp
v9381.Init(V93XX_SPI::WireMode::ThreeWire, true, 
           V93XX_SPI::ChecksumMode::Dirty);
```

**Note:** ThreeWire requires ≥400µs SCK-low time before each operation.

## Dependencies

- **ESP-DSP**: Install via Arduino Library Manager
  - Search: "ESP-DSP"
  - Version: 1.0.0 or later
- **V93XX_Arduino**: This library (provides `V93XX_SPI` class)

## Notes

- SPI block reads are emulated (sequential single reads), not hardware-accelerated
- For production, consider Clean mode after verifying CRC stability
- FFT length must be power of 2 (128, 256, 512, 1024, etc.)
- Larger FFT = better frequency resolution but slower processing
