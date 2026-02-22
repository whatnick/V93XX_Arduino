# V9381 UART Waveform Example

This example captures the V9381 waveform buffer over UART using `CaptureWaveform()` and prints samples for offline analysis.

## Notes

- UART runs in Dirty checksum mode to avoid CRC blocking during capture.
- Samples are printed as 16-bit pairs (lower, upper) from `DAT_WAVE`.
