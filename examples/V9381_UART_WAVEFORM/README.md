# V9381 UART Waveform Example

Captures V9381 waveform buffer over UART using the `CaptureWaveform()` API.

## Features

- ✅ Uses `CaptureWaveform()` for automated capture
- ✅ Dirty mode for CRC tolerance
- ✅ Prints samples as 16-bit pairs for Python analysis
- ✅ Automatic overflow prevention via WAVESTORE_CNT

## Workflow

1. Configure DSP_CTRL5 (channel, trigger, length)
2. Call `CaptureWaveform()` to capture samples
3. Print samples as hex pairs: `[LOWER,UPPER]`
4. Use `tools/plot_v9360_waveform.py` for offline analysis

## Sample Output

```
V9381 UART Waveform Capture
Capturing waveform...
Capture successful! 309 words

Waveform Data (309 samples):
Sample 0000: [0x1234,0x5678]
Sample 0001: [0xABCD,0xEF01]
...
```

## Python Analysis

Copy the hex output and use the plotting tool:

```bash
python tools/plot_v9360_waveform.py
```

The tool will:
- Parse hex sample pairs
- Reconstruct waveform
- Plot time-domain signal
- Perform FFT and show frequency spectrum

## Configuration

```cpp
const uint32_t ctrl5 = (1 << 18) |  // Manual trigger
                       (2 << 9) |   // 512 samples
                       (0 << 6);    // Channel UA
```

Adjust channel (bits 8-6) to capture different signals:
- `0` - UA (Voltage A)
- `1` - IA (Current A)
- `2` - UB (Voltage B)
- `3` - IB (Current B)
