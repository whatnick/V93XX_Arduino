# V93XX Umbrella repository

This repository is a collection of Arduino libraries implementing the Serial protocol to communicate the Vangotech [V93XX](https://www.vangotech.com/en/uploadpic/169329776853.pdf) series of ASIC for Single-phase energy monitoring.

The UART protocol is simplex with alternative send and receive cycles, the baud rate is set via auto-bauding via the command byte after the first reset cycle triggered by pulling the RX pin of the chip (TX pin of the host CPU) low for a period of 92.5 ms.