# FFT on-board ESP-32 Example

This example builds on the Waveform buffer example by performing on-board ESP-32 FFT.
1. Perform prepare steps and download data.
2. Perform FFT on fixed window of downloaded data.
3. Send FFT results, i.e. harmonics and amplitude of peaks over serial.

With the code uploaded and running, waveform buffer is passed through FFT process repeatedly and the console lists the harmonics of 50Hz/60Hz (including the main) detected and their amplitudes.

Ideally this function should be calibrated using a signal generator and passed through the V93xx.

## Performance

The V9360 will perform less well than the V9381 over SPI. Waveform buffer capture and download takes about 1000ms
using the V93xx bulk read features. FFT takes about XXms. We should benchmark different FFT processes.

1. [KissFFT](https://github.com/stolk/little_kiss_fft)
2. [ESP32-IDF FFT](https://github.com/espressif/esp-dsp/tree/master/examples/fft)
3. [Arduino FFT](https://github.com/kosme/arduinoFFT) - Latest maintained