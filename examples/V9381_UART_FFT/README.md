# V9381 UART FFT Example

V9381-specific FFT example with reliability tuning for UART capture at 19200 baud.

## Features

- ✅ Uses `CaptureWaveform()` API for automated capture
- ✅ Dirty mode for CRC tolerance during capture
- ✅ Reliability tuning: 2000ms timeout, 4-word block reads
- ✅ ESP-DSP FFT2R with Hann windowing
- ✅ Automatic peak frequency detection

## Reliability Configuration

This example is tuned for V9381 UART reliability:

```cpp
CaptureWaveform(waveform_buffer, kWaveformWords, ctrl5, 2000, 4);
//                                                   ^^^^  ^
//                                                   |     |
//                                   2s timeout ----+     +---- 4-word blocks
```

**Why these values?**
- **2000ms timeout**: Allows sufficient time for 309-word capture at 19200 baud
- **4-word blocks**: Reduces UART timeout frequency vs default 16-word blocks
- **Dirty mode**: Tolerates intermittent CRC mismatches during capture

## Waveform Configuration

```cpp
const uint32_t ctrl5 = (1 << 18) |  // Manual trigger
                       (2 << 9) |   // 512 samples
                       (0 << 6);    // Channel UA (Voltage A)
```

Channel options (bits 8-6):
- `0` - UA (Voltage A)
- `1` - IA (Current A)
- `2` - UB (Voltage B)
- `3` - IB (Current B)

## Expected Output

```
V9381 UART FFT Example
Dirty mode enabled for capture robustness
FFT ready. Length: 512

Capturing waveform...
[Some RegisterBlockRead() timeouts may appear - these are tolerated]
Capture successful! 309 words
Running FFT...
Peak at bin 5 (48.8 Hz): magnitude 1234.56
```

## Comparison with V9360

For the multi-target FFT example that supports V9360/V9381/SPI, see:
- [examples/V9360_UART_FFT](../V9360_UART_FFT/)

This V9381-specific example provides optimized settings for V9381 UART.
