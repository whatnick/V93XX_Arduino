# V93XX FFT Plan (ESP32 DSP + Waveform Buffer)

This document summarizes relevant V93XX datasheet details and outlines a concrete implementation plan for FFT using ESP32 DSP support with waveform buffer capture.

## Datasheet Notes (Waveform Capture)

Source: V93XX_D1 datasheet, Section 10 "Active Waveform upload and buffer".

- The V93XX can provide raw waveform samples via:
  - DMA SPI active waveform upload (not used here; UART projects currently in this repo).
  - Internal waveform buffer, readable via register `DAT_WAVE` (0x69).
- Waveform buffer supports single-channel or dual-channel storage. If all three channels are enabled, channel IB is invalid.
- Buffer points per cycle depend on `DSP_MODE` (DSP_CTRL0 bit[7:4]) and scaling via DSP_CTRL6 bit[31].
- Completion is flagged by `WAVE_STORE` in SYS_INTSTS (0x72). After completion, read up to 309 data words per read from `DAT_WAVE` (0x69).
- Buffer data format (per datasheet):
  - Single channel: high 16 bits = sample 2n+1, low 16 bits = sample 2n.
  - Dual channel: packed per channel as described in the datasheet table.

## Goal

Capture raw waveform samples from the V93XX, convert them to a real-valued time-domain buffer, and compute FFT on the ESP32 using ESP-DSP. Display or stream the magnitude spectrum for the FFT example.

## Implementation Plan

### 1) Define Capture Settings

- Select a sampling mode that is well-defined and consistent with existing UART example code.
- Choose `DSP_MODE` to control points per cycle and sampling rate.
- Decide channel(s): IA, IB, or U (recommend U or IA for initial FFT).
- Decide buffer length in samples (power of 2) for FFT:
  - For example, 128, 256, or 512 samples depending on `DSP_MODE` and buffer capacity.

### 2) Configure V93XX Waveform Buffer

Key steps (registers referenced from datasheet):

- Configure DSP control:
  - `DSP_CTRL0` (0x02): set `DSP_MODE` for points-per-cycle.
  - `DSP_CTRL5` (0x07): enable waveform buffer, select channel(s), set trigger mode.
  - `DSP_CTRL6` (0x80): optional scaling of points-per-cycle via bit[31].
- Clear/ack any existing `WAVE_STORE` interrupt in `SYS_INTSTS` (0x72).
- Start waveform buffer capture (command trigger or event trigger). For UART, use command trigger.

### 3) Read Buffer Data via UART

- Poll `SYS_INTSTS.WAVE_STORE` until set.
- Read `DAT_WAVE` (0x69) repeatedly to collect all samples:
  - Each 32-bit word packs 2 samples (2n and 2n+1) as 16-bit signed values.
  - Convert to signed 16-bit values, then to float for DSP.
- If dual-channel is enabled, split the two channels accordingly.

### 4) Normalize and Window

- Convert raw 16-bit samples to float in the range [-1.0, 1.0] or to the expected ADC scale.
- Apply a window (Hann recommended) to reduce spectral leakage.

### 5) FFT Using ESP-DSP

- Use the ESP-DSP library (real FFT):
  - `dsps_fft2r_fc32` + `dsps_bit_rev_fc32` + `dsps_cplx2reC_fc32` (or newer real FFT APIs if enabled).
- Compute magnitude spectrum: `mag[k] = sqrt(re^2 + im^2)`.
- Convert to dB if desired: $20 \log_{10}(mag)$.

### 6) Output and Validation

- Print dominant frequency bin and magnitude to serial.
- Optional: stream FFT bins to a host plotting tool.
- Validate using a known input sine wave and compare peak location to expected frequency.

## Data Handling Details

### Sample Unpacking

Given a 32-bit `DAT_WAVE` word:

```
uint16_t lo = word & 0xFFFF;
uint16_t hi = (word >> 16) & 0xFFFF;
int16_t s0 = (int16_t)lo; // sample 2n
int16_t s1 = (int16_t)hi; // sample 2n+1
```

### Buffer Size Considerations

- The datasheet notes up to 309 data words per read sequence.
- This yields up to 618 samples for a single channel.
- Choose the largest power-of-two <= available samples for FFT.

## Proposed ESP32 Integration Steps (Code Tasks)

1. Add a waveform capture helper in the UART driver:
   - Configure DSP registers for buffer capture.
   - Poll `WAVE_STORE` and read `DAT_WAVE` into a sample array.
2. Add DSP support in the FFT example:
   - Add ESP-DSP dependency (CMake or Arduino library manager).
   - Implement `run_fft(samples, n)` with windowing and magnitude output.
3. Update example sketch to:
   - Capture samples every N seconds.
   - Run FFT and print dominant frequency/magnitude.

## Open Questions / To-Confirm

- Confirm exact `DSP_CTRL5` bit fields for waveform buffer enable, channel selection, and trigger mode as implemented in the existing driver.
- Confirm V93XX sampling rate for the selected `DSP_MODE` to map FFT bin to Hz.
- Confirm UART burst read behavior for `DAT_WAVE` (max word count per read transaction).

## Next Actions

- Inspect current UART example code for register definitions and waveform capture support.
- Implement waveform buffer capture in the UART driver (if not present).
- Add a basic FFT pipeline and validate with a known signal.
