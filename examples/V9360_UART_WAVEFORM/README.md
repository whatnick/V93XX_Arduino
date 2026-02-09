# Waveform Buffer Example

This example shows how to make us of the Waveform buffer feature of the V93XX. 
1. Configuring Waveform buffer
2. Initiating an manual capture
3. Downloading the data

With the code uploaded and running, a waveform capture is run repetitively, and the results are output over the serial console to enable further experimentation and analysis.

A test signal was applied to the UP/UN inputs of the V93XX.

![Wavegen](img/wave_gen_001.png)

Using a small python script, and the captured data we can reconstruct the waveform

![Wavegen](img/V93XX_buffer.png)

See the Python helper script in tools/plot_v9360_waveform.py for plotting and FFT analysis of captured data.

## Performance

Capturing and exporting the waveform buffer over a 19200bps UART link isn't particularly fast.
The Configuration and internal capture takes about 150ms, and downloading the entire buffer 795ms.
This is even making use of the V95XX's bulk read features.

![Wavegen](img/V93XX_saleae.png)