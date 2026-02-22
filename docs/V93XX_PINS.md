# V93XX Pin Descriptions & Configuration

Complete pin reference for all V93XX package variants.

## V9381 (SSOP24) Pin Description

Most feature-rich version with all I/O pins.

```
      Pin 1
        ↓
   A1/SPCSN ┌──────────────────────────┐ RX/MOSI
     P0 ────┤ 24                    19  ├─── P1
     P1 ────┤ 23                    20  ├─── TX/MISO
     P2 ────┤ 22                    21  ├─── A0/SPCK
     P3 ────┤ 21                    22  ├─── P2
     P5 ────┤ 20                    23  ├─── P3
     P6 ────┤ 16                    24  ├─── P6
    RSTN ───┤ 15                    25  ├─── P5
   X32KIN ──┤ 14                    13  ├─── Pad
      VSS ──┤ 13                    12  ├─── CTI
     IBN ───┤ 12                    11  ├─── DVCC
     IBP ───┤ 11                    10  ├─── VREF
     IAN ───┤ 10                     9  ├─── VDD
     IAP ───┤  9                     8  ├─── VSS**
      UN ───┤  8                     7  ├─── VSS*
      UP ───┤  7                     6  ├─── IBP
     CTI ───┤  6                     5  ├─── IAN
    DVCC ───┤  5                     4  ├─── IBN
    VREF ───┤  4                     3  ├─── NC
     VDD ───┤  3                     2  ├─── RSTN
     VSS ───┤  2                     1  ├─── X32KIN
     NC ────┤  1
           └──────────────────────────┘
```

### Pin List - V9381 (SSOP24)

| Pin | Name | Type | Description |
|-----|------|------|-------------|
| 1 | NC | - | Not connected |
| 2 | VSS | GND | Ground |
| 3 | VDD | PWR | 3.3V power supply (0.1µF cap to GND) |
| 4 | VREF | I/O | On-chip reference (1µF cap to GND) |
| 5 | DVCC | PWR | Digital supply output (0.1µF cap to GND) |
| 6 | CTI | INPUT | Input clock, 6.5536 MHz |
| 7 | UP | INPUT | Voltage channel positive input |
| 8 | UN | INPUT | Voltage channel negative input |
| 9 | IAP | INPUT | Current channel A positive |
| 10 | IAN | INPUT | Current channel A negative |
| 11 | IBP | INPUT | Current channel B positive |
| 12 | IBN | INPUT | Current channel B negative |
| 13 | VSS | GND | Ground |
| 14 | X32KIN | INPUT | 32kHz clock input (optional) |
| 15 | RSTN | INPUT | Reset (active low, 2ms min low) |
| 16 | P6 | OUTPUT | Signal output (CF, interrupt, etc.) |
| 17 | P5 | OUTPUT | Signal output (CF, interrupt, etc.) |
| 18 | P3 | OUTPUT | Signal output (CF, interrupt, etc.) |
| 19 | P2 | OUTPUT | Signal output (CF, interrupt, etc.) |
| 20 | TX/MISO | OUTPUT | UART TX / SPI MISO |
| 21 | RX/MOSI | INPUT | UART RX / SPI MOSI (pull-up ~50kΩ) |
| 22 | A1/SPCSN | INPUT | UART addr bit 1 / SPI CS (pull-up ~50kΩ) |
| 23 | A0/SPCK | INPUT | UART addr bit 0 / SPI CLK (pull-up ~50kΩ) |
| 24 | P1 | OUTPUT | Signal output (CF, interrupt, etc.) |
| 25 | P0 | OUTPUT | Signal output (CF, interrupt, etc.) |

---

## V9360 (SOP16) Pin Description

Mid-range version with reduced I/O.

```
      Pin 1
        ↓
    ┌──────────────────┐
    │ NC            15 │ CTI
    │ IAN           14 │ TX
    │ IBN           13 │ RX
    │ IBP           12 │ P3
    │ VSS           11 │ P2
    │ DVCC          10 │ RCL (?)
    │ VREF           9 │ VDD
    │ VDD            8 │ UP
    │ UP             7 │ UN
    └──────────────────┘
```

### Pin List - V9360 (SOP16)

| Pin | Name | Type | Description |
|-----|------|------|-------------|
| 1 | IAN | INPUT | Current channel A negative |
| 2 | IBN | INPUT | Current channel B negative |
| 3 | IBP | INPUT | Current channel B positive |
| 4 | VSS | GND | Ground |
| 5 | DVCC | PWR | Digital supply output |
| 6 | VREF | I/O | Reference (1µF cap to GND) |
| 7 | UN | INPUT | Voltage channel negative |
| 8 | UP | INPUT | Voltage channel positive |
| 9 | VDD | PWR | 3.3V supply (0.1µF cap to GND) |
| 10 | (alternate PIN 10) | - | May vary |
| 11 | P2 | OUTPUT | Signal output |
| 12 | P3 | OUTPUT | Signal output |
| 13 | RX | INPUT | UART RX (pull-up ~50kΩ) |
| 14 | TX | OUTPUT | UART TX |
| 15 | CTI | INPUT | Input clock, 6.5536 MHz |
| 16 | NC | - | Not connected |

---

## V9340 (SOP8) Pin Description

Minimal version - essential pins only.

```
      Pin 1
        ↓
    ┌──────────────┐
    │              │
    │ Pin...  Pin │
    │              │
    └──────────────┘
```

### Pin List - V9340 (SOP8)

| Pin | Name | Type | Description |
|-----|------|------|-------------|
| 1 | IAN | INPUT | Current channel A negative |
| 2 | IAP | INPUT | Current channel A positive |
| 3 | UP | INPUT | Voltage channel positive |
| 4 | VDD | PWR | 3.3V power (0.1µF cap to GND) |
| 5 | VSS | GND | Ground |
| 6 | /REF | I/O | Reference (1µF cap to GND) |
| 7 | DVCC | PWR | Digital supply output |
| 8 | TX/RX | I/O | UART bi-directional |

---

## Pull-Up Resistors

Built-in pull-up resistances on input pins:

| Pin | Built-in Pull-Up | When Required |External Resistor |
|-----|------------------|---------------|------------------|
| RX/MOSI | ~50kΩ | Standard | Usually not needed |
| A1/SPCSN | ~50kΩ | Standard | Usually not needed |
| A0/SPCK | ~50kΩ | Standard | Usually not needed |
| RSTN | ~50kΩ | Standard | Pull-up for long wire |
| TX/MISO | High-Z (open drain) | Output only | Pull-up for multi-device |
| P0-P6 | High-Z (open drain) | Output only | Pull-up for interrupt use |

---

## Power Supply Pins

### V9381/V9360
- **VDD (Pin 3, 9):** 3.3V main analog power
  - Bypass: 0.1µF capacitor to VSS (required)
  - Location: Close to pin (within 5mm)
  
- **DVCC (Pin 5, 10):** Digital supply output from on-chip LDO
  - Bypass: 0.1µF capacitor to VSS (recommended)
  - Output current: 35mA max
  
- **VREF (Pin 4, 6):** On-chip bandgap reference (1.21V typ)
  - Bypass: 1µF capacitor to VSS (required)
  - Do NOT drive externally

- **VSS (Pin 2, 10, 13):** Ground (return path)
  - Multiple pins for current distribution
  - All must be connected to system ground

### V9340
- Similar but minimal: VDD, VSS, DVCC, /REF (reference inverted)

---

## Analog Input Pins

### Voltage Channel
- **UP (Pin 7/8):** Positive input
- **UN (Pin 8/7):** Negative input (or GND reference)
- Supports both PT (transformer) and resistive divider

### Current Channel A
- **IAP (Pin 9):** Positive input
- **IAN (Pin 10):** Negative input
- Supports: CT, shunt, Hall cell, TMR, Roche coil

### Current Channel B
- **IBP (Pin 11):** Positive input
- **IBN (Pin 12):** Negative input
- Same input types as Channel A

### Input Ranges
- **Maximum:** ±200mV peak (ADC input)
- **Full Scale:** ±1.1V (after PGA)
- **Measurement Range:** 5000:1 dynamic range

---

## Clock Input

### CTI Pin (Pin 1, 15, 6)
- **Frequency:** 6.5536 MHz ±20%
- **Drive:** CMOS logic level or AC-coupled square wave
- **Timing:** Frequency divider set by DSP_MODE (0x02[7:4])

### X32KIN Pin (Pin 14 - V9381 only)
- **Frequency:** 32.768 kHz for low-speed energy accumulator
- **Optional:** If not used, leave floating
- **Clock Selection:** EGY_CLK_SEL bit in DSP_CTRL1 (0x03[23])

---

## Reset Pin (RSTN)

### Control
- **Logic:** Active LOW (normally high)
- **Minimum Hold:** 2ms low to force reset
- **High to Reset Completion:** 900µs + 1.25ms (RAM self-check) = ~2.15ms

### Reset Types
1. **External (RSTN pin):** Hardware reset
2. **RX Reset:** 128ms low on RX/MOSI pin
3. **Software Reset:** Write 0x4572BEAF to register 0x6C

### After Reset
- All registers return to default values
- UART is default communication mode
- RAM self-check takes ~1.25ms
- Ready for communication after 2.15ms total

---

## Output Pins (P0-P6 for V9381)

### Functions (Configurable)
- **CF Output:** Pulse (energy counter feedback)
- **Interrupt Output:** Event signals
- **Zero-Crossing:** Square wave output
- **Waveform DMA:** SPI clock/data/select
- **Energy Upload:** UART data signal
- **Clock Output:** RCH or RCL oscillator output

### Configuration
Register: **SYS_IOCFGX0 (0x7D)** for P0-P3  
Register: **SYS_IOCFGX1 (0x7E)** for P4-P6

**Default State:** High-Z (open-drain output)  
**Configuration:** Each pin maps to interrupt type (1st, 2nd, 3rd, or 4th)

### Interrupt Types Mappable
- **1st Type:** Zero-crossing, energy accumulator overflow
- **2nd Type:** Waveform refresh, RMS update, power update
- **3rd Type:** Over/under voltage/current, swell/dip
- **4th Type:** Errors (SPI, UART, checksum, reference, etc.)

---

## SPI Mode Pins

When configured for SPI operation:

| Pin | UART Function | SPI Function |
|-----|---------------|--------------|
| RX/MOSI | UART RX input | SPI MOSI (master-out) |
| TX/MISO | UART TX output | SPI MISO (slave-in) |
| A1/SPCSN | Address bit 1 | Chip Select (CS, active low) |
| A0/SPCK | Address bit 0 | Serial Clock (CLK input) |

### SPI Clock Rates
- **Register Access:** Max 1/4 system clock
- **RAM Access:** Max 1/16 system clock
- **Typical:** 400-800 kHz at 6.5536 MHz

---

## Typical Connections

### Minimal Configuration (V9381)
```
Microcontroller
      │
      ├────→ CTI (6.5536 MHz clock)
      ├─↔─ RX/MOSI (UART)
      ├─↔─ TX/MISO (UART)
      ├────→ RSTN (pull-up 10kΩ, drive low to reset)
      └────→ A0/SPCK, A1/SPCSN (address/SPI select)

Power
      ├────→ VDD (3.3V, 0.1µF BSC cap)
      ├────→ DVCC ← (from on-chip LDO, 0.1µF cap)
      └────→ VSS/GND

Analog
      ├────→ UP, UN (voltage inputs)
      ├────→ IAP, IAN (current A inputs)
      ├────→ IBP, IBN (current B inputs)
      └────→ VREF (1µF cap to GND, do NOT drive)

Optional
      ├────→ X32KIN (32.768 kHz if using low-speed accumulator)
      └────→ P0-P6 (events, CF, DMA - pull-ups if used)
```

---

## Design Considerations

### 1. **Decoupling Capacitors** (CRITICAL)
- **VDD:** 0.1µF ceramic near pin (within 5mm)
- **DVCC:** 0.1µF ceramic near pin
- **VREF:** 1µF film/ceramic near pin
- **All:** Use short, thick traces

### 2. **Analog Input Filtering**
- Add RC low-pass filters to prevent aliasing
- Typical: R=10kΩ, C=100nF (-3dB at 159 Hz)
- May increase input impedance significantly

### 3. **Clock Source**
- Use stable 6.5536 MHz oscillator
- If using internal RC: ±20% tolerance
- Can trim frequency with RCHTRIM (ANA_CTRL1[29:24])

### 4. **PCB Layout**
- Keep V93XX away from high-current circuits
- Separate analog and digital grounds if possible
- Thick GND traces connecting all VSS pins
- Shield RX/TX/CTI lines if long runs

### 5. **Isolation (if needed)**
- Optocoupler on RX/MOSI for isolation
- CT/PT isolation via 1:1 transformer
- VREF remains isolated (never drive externally)

---

## Device Selection by Pin Availability

| Feature | V9381 | V9360 | V9340 |
|---------|-------|-------|-------|
| IO Pins (P0-P6) | 7 | 2 | 0 |
| Dual Current (A+B) | Yes | Yes | Yes (IAN + mux) |
| X32KIN (32kHz) | Yes | No | No |
| SPI Mode | Yes | Yes | No (UART only) |
| Full Interrupt Config | Yes | Limited | No |
| Package | SSOP24 | SOP16 | SOP8 |

---

Last Updated: 2025-02-22  
V4.9 Datasheet Compatible
