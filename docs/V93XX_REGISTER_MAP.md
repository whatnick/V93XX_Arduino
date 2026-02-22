# V93XX Register Complete Map

Complete register address reference for all V93XX variants (V9381, V9360, V9340).

## Register Address Quick Reference

| Address | Register Name | Type | Purpose |
|---------|--------------|------|---------|
| 0x00 | ANA_CTRL0 | R/W | Analog Control 0 |
| 0x01 | ANA_CTRL1 | R/W | Analog Control 1 - ADC Gain, RC Tuning |
| 0x02 | DSP_CTRL0 | R/W | Metering Control 0 - DSP Mode, Enable Channels |
| 0x03 | DSP_CTRL1 | R/W | Metering Control 1 - Energy, CF, Power-Creep |
| 0x04 | DSP_CTRL2 | R/W | Metering Control 2 - Energy Accum 1-4 Config |
| 0x05 | DSP_CTRL3 | R/W | Metering Control 3 - Energy Accum 5-8 Config |
| 0x06 | DSP_CTRL4 | R/W | Metering Control 4 - Fast Detection Config |
| 0x07 | DSP_CTRL5 | R/W | Metering Control 5 - Waveform & DMA |
| 0x08-0x0D | DSP_DAT_PA/QA/SA/PB/QB/SB | R | Instantaneous Power (A/B Channels) |
| 0x0E-0x10 | DSP_DAT_RMS0U/IA/IB | R | Instantaneous RMS |
| 0x11-0x12 | DSP_DAT_CH1/CH2 | R | Fundamental Wave Channel Data |
| 0x13-0x18 | DSP_DAT_PA1/QA1/SA1/PB1/QB1/SB1 | R | Average Power (A/B Channels) |
| 0x19-0x1B | DSP_DAT_RMS1U/IA/IB | R | Average RMS |
| 0x1C-0x1D | DSP_DAT_CH1_AVG/CH2_AVG | R | Average Fundamental Wave |
| 0x1E-0x20 | DSP_DAT_RMSU_AVG/RMSIA_AVG/RMSIB_AVG | R | 10/12-Cycle Average RMS |
| 0x21 | DSP_DAT_FRQ | R | Line Frequency |
| 0x22-0x24 | DSP_DAT_DCU/DCIA/DCIB | R | DC Component |
| 0x25 | DSP_CFG_CALI_PA | R/W | Active Power A - Gain Calibration |
| 0x26 | DSP_CFG_DC_PA | R/W | Active Power A - Offset Calibration |
| 0x27 | DSP_CFG_CALI_QA | R/W | Reactive Power A - Gain Calibration |
| 0x28 | DSP_CFG_DC_QA | R/W | Reactive Power A - Offset Calibration |
| 0x29 | DSP_CFG_CALI_PB | R/W | Active Power B - Gain Calibration |
| 0x2A | DSP_CFG_DC_PB | R/W | Active Power B - Offset Calibration |
| 0x2B | DSP_CFG_CALI_QB | R/W | Reactive Power B - Gain Calibration |
| 0x2C | DSP_CFG_DC_QB | R/W | Reactive Power B - Offset Calibration |
| 0x2D | DSP_CFG_CALI_RMSU | R/W | Voltage RMS - Gain Calibration |
| 0x2E | DSP_CFG_RMS_DCU | R/W | Voltage RMS - Offset Calibration |
| 0x2F | DSP_CFG_CALI_RMSIA | R/W | Current A RMS - Gain Calibration |
| 0x30 | DSP_CFG_RMS_DCIA | R/W | Current A RMS - Offset Calibration |
| 0x31 | DSP_CFG_CALI_RMSIB | R/W | Current B RMS - Gain Calibration |
| 0x32 | DSP_CFG_RMS_DCIB | R/W | Current B RMS - Offset Calibration |
| 0x33 | DSP_CFG_PHC | R/W | Phase Calibration |
| 0x34 | DSP_CFG_DCU | R/W | Voltage DC Bias |
| 0x35 | DSP_CFG_DCIA | R/W | Current A DC Bias |
| 0x36 | DSP_CFG_DCIB | R/W | Current B DC Bias |
| 0x37 | DSP_CFG_BPF | R/W | Bandpass Filter Coefficient |
| 0x38 | DSP_CFG_CKSUM | R/W | Configuration Checksum |
| 0x39 | EGY_PROCTH | R/W | Energy Anti-Creep Threshold |
| 0x3A | EGY_PWRTH | R/W | Energy Accumulation Threshold |
| 0x3B-0x3E | EGY_CONST1/OUT1L/OUT1H/CFCNT1 | R/W | Energy Accumulator 1 |
| 0x3F-0x42 | EGY_CONST2/OUT2L/OUT2H/CFCNT2 | R/W | Energy Accumulator 2 |
| 0x43-0x45 | EGY_CONST3/OUT3/CFCNT3 | R/W | Energy Accumulator 3 |
| 0x46-0x48 | EGY_CONST4/OUT4/CFCNT4 | R/W | Energy Accumulator 4 |
| 0x49-0x4B | EGY_CONST5/OUT5/CFCNT5 | R/W | Energy Accumulator 5 |
| 0x4C-0x4E | EGY_CONST6/OUT6/CFCNT6 | R/W | Energy Accumulator 6 |
| 0x4F-0x51 | EGY_CONST7/OUT7/CFCNT7 | R/W | Energy Accumulator 7 |
| 0x52-0x54 | EGY_CONST8/OUT8/CFCNT8 | R/W | Energy Accumulator 8 |
| 0x55 | DSP_OV_THL | R/W | Power-Creep Lower Threshold |
| 0x56 | DSP_OV_THH | R/W | Power-Creep Upper Threshold |
| 0x57 | DSP_SWELL_THL | R/W | Voltage Swell Lower Threshold |
| 0x58 | DSP_SWELL_THH | R/W | Voltage Swell Upper Threshold |
| 0x59 | DSP_DIP_THL | R/W | Voltage Dip Lower Threshold |
| 0x5A | DSP_DIP_THH | R/W | Voltage Dip Upper Threshold |
| 0x5B | FD_OVTH | R/W | Fast Detection Over-Voltage (30-bit) |
| 0x5C | FD_LVTH | R/W | Fast Detection Under-Voltage (30-bit) |
| 0x5D | FD_IA_OCTH | R/W | Fast Detection Channel A Over-Current (30-bit) |
| 0x5E | FD_IA_LCTH | R/W | Fast Detection Channel A Under-Current (30-bit) |
| 0x5F | FD_IB_OCTH | R/W | Fast Detection Channel B Over-Current (30-bit) |
| 0x60 | FD_IB_LCTH | R/W | Fast Detection Channel B Under-Current (30-bit) |
| 0x61 | DSP_PHS_STT | R/W | Phase Measurement Control |
| 0x62 | DSP_PHS_U | R | Voltage Phase Value |
| 0x63 | DSP_PHS_UN | R | Voltage Phase Waveform (Before ZC) |
| 0x64 | DSP_PHS_UP | R | Voltage Phase Waveform (After ZC) |
| 0x65 | DSP_PHS_I | R | Current Phase Value |
| 0x66 | DSP_PHS_IN | R | Current Phase Waveform (Before ZC) |
| 0x67 | DSP_PHS_IP | R | Current Phase Waveform (After ZC) |
| 0x69 | DAT_WAVE | R | Waveform Data |
| 0x6A | DAT_SWELL_CNT | R/C | Voltage Swell Time Counter |
| 0x6B | DAT_DIP_CNT | R/C | Voltage Dip Time Counter |
| 0x6C | SYS_SFTRST | W | Software Reset |
| 0x70 | SYS_BAUDCNT1 | R | UART Baud Rate Count (TX 1st Bit) |
| 0x71 | SYS_BAUDCNT8 | R | UART Baud Rate Count (RX 8 Bits) |
| 0x72 | SYS_INTSTS | R/C | System Interrupt Status |
| 0x73 | SYS_INTEN | R/W | System Interrupt Enable |
| 0x74 | SYS_STS | R | System Status |
| 0x75 | SYS_MISC | R/W | System Control / Miscellaneous |
| 0x79-0x7C | SYS_BLK0_ADDR to SYS_BLK3_ADDR | R/W | Block Read Address Mapping |
| 0x7D | SYS_IOCFGX0 | R/W | IO Config - P0, P1, P2, P3 |
| 0x7E | SYS_IOCFGX1 | R/W | IO Config - P4, P5, P6 |
| 0x7F | SYS_VERSION | R | Hardware Version |
| **0x80-0x8E** | **Fundamental & DSP_CTRL6** | **R/W** | **Extended Metering & Fundamental Calibration** |
| 0x80 | DSP_CTRL6 | R/W | Metering Control 6 - Advanced Features |
| 0x81 | FUND_CALI_PA | R/W | Fundamental Active Power A - Gain |
| 0x82 | FUND_DC_PA | R/W | Fundamental Active Power A - Offset |
| 0x83 | FUND_CALI_QA | R/W | Fundamental Reactive Power A - Gain |
| 0x84 | FUND_DC_QA | R/W | Fundamental Reactive Power A - Offset |
| 0x85 | FUND_CALI_PB | R/W | Fundamental Active Power B - Gain |
| 0x86 | FUND_DC_PB | R/W | Fundamental Active Power B - Offset |
| 0x87 | FUND_CALI_QB | R/W | Fundamental Reactive Power B - Gain |
| 0x88 | FUND_DC_QB | R/W | Fundamental Reactive Power B - Offset |
| 0x89 | FUND_CALI_RMSU | R/W | Fundamental Voltage RMS - Gain |
| 0x8A | FUND_RMS_DCU | R/W | Fundamental Voltage RMS - Offset |
| 0x8B | FUND_CALI_RMSIA | R/W | Fundamental Current A RMS - Gain |
| 0x8C | FUND_RMS_DCIA | R/W | Fundamental Current A RMS - Offset |
| 0x8D | FUND_CALI_RMSIB | R/W | Fundamental Current B RMS - Gain |
| 0x8E | FUND_RMS_DCIB | R/W | Fundamental Current B RMS - Offset |
| 0x8F | DIP_SWELL_CTRL | R/W | Voltage Dip/Swell Control |
| 0x90 | SWELL_REG_MAX_CNT | R/C | Voltage Swell Max Value Time Counter |
| 0x91 | DIP_REG_MIN_CNT | R/C | Voltage Dip Min Value Time Counter |
| 0x92 | SWELL_REG_MAX | R | Maximum Voltage Swell Value |
| 0x93 | DIP_REG_MIN | R | Minimum Voltage Dip Value |
| 0x94 | ZERO_TH_U | R/W | Voltage Zero-Crossing Detection Threshold |
| 0x95 | ZERO_TH_I | R/W | Current Zero-Crossing Detection Threshold |

---

## Register Categories

### **Analog & Configuration (0x00-0x01)**
- Analog gain, ADC configuration, RC trimming
- Bandgap temperature coefficient fine-tuning

### **Metering Control (0x02-0x07, 0x80)**
- DSP mode selection, channel enable
- Energy accumulator configuration
- Waveform capture & DMA parameters

### **Data Registers (0x08-0x24, 0x21)**
- RMS values (instantaneous, average, 10/12-cycle)
- Power values (active, reactive, apparent)
- Frequency and DC bias measurements

### **Calibration (0x25-0x38)**
- Power calibration (gain & offset)
- RMS calibration
- Phase correction
- Fundamental wave calibration
- Configuration checksum

### **Energy Accumulators (0x39-0x54)**
- 8 independent energy accumulators
- Thresholds for anti-creep detection
- Pulse counters (CF output)

### **Thresholds (0x55-0x60, 0x94-0x95)**
- Power-creep detection thresholds
- Over/under voltage & current detection
- Voltage swell/dip thresholds
- Zero-crossing detection thresholds

### **Phase & Waveform (0x61-0x6B, 0x69)**
- Phase measurement control & data
- Waveform buffer control
- Swell/dip time counters

### **System Control (0x6C, 0x70-0x7F)**
- Software reset
- UART timing information
- Interrupt status & enable
- System status & configuration
- Block read address mapping
- IO port configuration
- Hardware version

### **Extended Metering (0x80-0x95)**
- Advanced DSP features
- Fundamental wave independent calibration
- Roche coil & frequency measurement
- Dip/swell event control

---

## Special Addresses

### SPI High-Byte Access
When accessing addresses 0x80 and above via SPI:
1. Write 0x4A985B67 to address 0x7F (SPI high-byte mode)
2. All addresses automatically add 128 offset
3. Write 0x76B589A4 to 0x7F to cancel offset (return to normal mode)

### UART Block Read
Configure address mapping via SYS_BLK0-3_ADDR (0x79-0x7C) to read up to 16 non-consecutive addresses efficiently.

---

## Register Access Rules

### Reset Behavior
- **RSTN pin reset:** All registers return to default
- **RX reset:** Resets UART communication
- **Software reset (0x6C):** Full system reset
- **Soft reset via 0x38:** Specific register reset via checksum

### Special Considerations
- Registers 0x00-0x07: Participate in configuration checksum
- Registers 0x08-0x1D: Read-only data (automatically updated)
- Registers 0x25-0x3A: Configuration registers (require checksum)
- Registers 0x55-0x60: Threshold registers (require checksum)
- Registers 0x80+: High-byte registers (SPI high-byte mode required)

---

Last Updated: 2025-02-22V4.9
