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

```python
wave_data = [ ... ]

# plot wave_data
import matplotlib.pyplot as plt
plt.plot(wave_data)
plt.show()

# Futher analysis of results can be performed
import numpy as np
fft_data = np.fft.fft(wave_data)
plt.plot(fft_data)
plt.show()
```

## Performance

Capturing and exporting the waveform buffer over a 19200bps UART link isn't particularly fast.
The Configuration and internal capture takes about 150ms, and downloading the entire buffer 795ms.
This is even making use of the V95XX's bulk read features.

![Wavegen](img/V93XX_saleae.png)