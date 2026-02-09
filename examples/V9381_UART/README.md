# V9381 UART (shared bus)

This example uses UART mode on the same wiring as SPI so an ESP32-S3 can switch modes without rewiring. RX/MOSI and TX/MISO share pins, and A0/A1 share SCK/CS.

## Wiring (ESP32-S3 default SPI pins)

```
ESP32-S3            V9381
--------            -----
GPIO11 (MOSI)   --> RX/MOSI
GPIO13 (MISO)   <-- TX/MISO
GPIO12 (SCK)    --> A0/SPCK  (UART address bit 0)
GPIO10 (CS)     --> A1/SPCSN (UART address bit 1)
GND             --- GND
3V3             --- VDD
```

## Notes

- UART uses 8O1 framing at 19200 bps with auto-baud available.
- Set A1/A0 to the device address before UART.
- If switching to SPI in the same sketch, release A0/A1 pins (set to input) before SPI starts.
