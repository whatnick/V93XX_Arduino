# V9381 UART FFT Example

This example captures the V9381 waveform buffer over UART and runs ESP-DSP FFT on the ESP32.

## Notes

- UART runs in Dirty checksum mode for capture robustness.
- FFT window length is set to 512; adjust to match your waveform buffer size and sampling rate.
