# PR Update Notes

## Summary
- Added shared CaptureWaveform API (UART/SPI) and CamelCase naming.
- Added V9381 UART Waveform and FFT examples to match V9360 naming.
- Updated FFT flows to use Dirty UART mode and added UART inter-frame delay during capture.

## Tests
- Compiled all examples (esp32:esp32:esp32s3):
  - examples/V9360_UART/V9360_UART.ino
  - examples/V9360_UART_FFT/V9360_UART_FFT.ino
  - examples/V9360_UART_WAVEFORM/V9360_UART_WAVEFORM.ino
  - examples/V9381_SPI/V9381_SPI.ino
  - examples/V9381_UART/V9381_UART.ino
  - examples/V9381_UART_DIRTY_MODE/V9381_UART_DIRTY_MODE.ino
  - examples/V9381_UART_FFT/V9381_UART_FFT.ino
  - examples/V9381_UART_WAVEFORM/V9381_UART_WAVEFORM.ino
- Uploaded V9381 UART FFT to ESP32-S3 on COM7.

## Runtime Notes
- V9381 UART FFT output shows Dirty-mode CRC warnings as expected.
- No explicit "Waveform capture failed" or overflow messages in latest 10s capture.
- UART capture still logs intermittent RegisterBlockRead timeouts; needs follow-up tuning.
