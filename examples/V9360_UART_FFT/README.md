# V9360/V9381 FFT Example (Multi-Target)

This example captures waveform data from V93XX ASICs and performs on-board FFT analysis using ESP-DSP.

## Features

- ✅ Multi-target support (V9360/V9381, UART/SPI)
- ✅ Uses `CaptureWaveform()` API for unified capture
- ✅ ESP-DSP FFT2R (radix-2 real FFT)
- ✅ Hann windowing for spectral leakage reduction
- ✅ Automatic peak detection and frequency identification

## Workflow

1. Configure DSP for waveform capture (channel, length, trigger)
2. Call `CaptureWaveform()` to capture samples into buffer
3. Apply Hann window to reduce spectral leakage
4. Execute FFT (bit reversal + FFT2R + magnitude calculation)
5. Find peak frequency and magnitude
6. Output results over serial

## Target Selection

Set `V93XX_FFT_TARGET` at the top of the sketch:

- `V93XX_FFT_TARGET_V9360_UART` - V9360 over UART (default)
- `V93XX_FFT_TARGET_V9381_UART` - V9381 over UART
- `V93XX_FFT_TARGET_V9381_SPI` - V9381 over SPI

Each target has optimized constants:
- `kWaveformWords` - Capture word count
- `kFftLen` - FFT length (must be power of 2)
- Timeout and block size tuned per interface

## Performance

| Target | Capture Time | FFT Time | Notes |
|--------|--------------|----------|-------|
| V9360 UART | ~1000ms | ~50ms | 19200 baud, 16-word blocks |
| V9381 UART | ~1200ms | ~50ms | 19200 baud, 4-word blocks (reliability) |
| V9381 SPI | ~200ms | ~50ms | 1MHz SPI clock |

## ESP-DSP Library

This example uses **ESP-DSP** for FFT processing:
- `dsps_fft2r_init_fc32()` - Initialize FFT tables
- `dsps_wind_hann_f32()` - Apply Hann window
- `dsps_fft2r_fc32()` - Execute radix-2 FFT
- `dsps_bit_rev_fc32()` - Bit reversal for FFT output
- `dsps_cplx2reC_fc32()` - Convert complex to magnitude

Install via Arduino Library Manager: Search "ESP-DSP"

## Expected Output

```
V9360 UART FFT Example
Initializing...
FFT ready. Length: 512

Capturing waveform...
Capture successful! 309 words
Running FFT...
Peak at bin 5 (48.8 Hz): magnitude 1234.56

Capturing waveform...
```

## Calibration

For best results, calibrate with a signal generator:
1. Apply known frequency (e.g., 50Hz) to V93XX input
2. Verify FFT peak aligns with expected bin
3. Adjust sampling rate or FFT length if needed