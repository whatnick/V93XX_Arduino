# V93XX Common Usage Patterns & Initialization

Quick reference for standard V93XX configuration sequences.

---

## 1. Basic Initialization Sequence

```python
# Step 1: Hardware Reset (if available)
RSTN_pin = LOW
delay(2ms)  # Minimum 2ms
RSTN_pin = HIGH
delay(3ms)  # Wait for RAM self-check (1.25ms) + margin

# Step 2: Initialize UART Communication
uart_init(19200, bits=8, parity='ODD', stop_bits=1)
A0_pin = 0  # Device address select
A1_pin = 0  # This V93XX at address 0x00

# Step 3: Verify Communication
IF NOT read_register(0x7F):  # Read SYS_VERSION
    ERROR("Communication failed")

# Step 4: Enable ADC Channels
write_register(0x02, 0x00000007)  # DSP_CTRL0: Enable U, IA, IB

# Step 5: Wait for Stabilization
delay(100ms)

# Step 6: Read First Measurement
value = read_register(0x0E)  # DSP_DAT_RMS0U (voltage RMS)
```

---

## 2. RMS Measurement Configuration

### Enable RMS Measurement Only (Minimal Mode)
```python
def enable_rms_measurement():
    """Configure for RMS data collection (no power)"""
    
    # DSP_CTRL0 (0x02): Set DSP mode, enable all channels
    dsp_ctrl0 = (
        0 << 31 |      # CURDAT_RATE: 0 = 20ms update
        0b0000 << 7 |  # DSP_MODE: 0 = 128 samples @ 6.5536MHz
        0 << 2 |       # ADCUEN: Enable voltage channel
        0 << 1 |       # ADCIBEN: Enable current B
        0 << 0         # ADCIAEN: Enable current A
    ) | 0x00000007  # OR with 0x07 to enable all
    write_register(0x02, dsp_ctrl0)
    
    # DSP_CTRL1 (0x03): Disable power-creep, energy
    write_register(0x03, 0x00000000)
    
    # DSP_CTRL5 (0x07): Disable waveform storage/DMA
    write_register(0x07, 0x00000000)
    
    # DSP_CTRL6 (0x80): Default settings
    write_register(0x80, 0x00000000)
    
    delay(200ms)  # Wait for measurements to stabilize
```

### Read RMS Data
```python
def read_rms_values():
    """Retrieve RMS measurements"""
    voltage = read_register(0x0E)  # DSP_DAT_RMS0U - Inst. voltage RMS
    current_a = read_register(0x0F)  # DSP_DAT_RMS0IA - Inst. current A RMS
    current_b = read_register(0x10)  # DSP_DAT_RMS0IB - Inst. current B RMS
    
    # Values are 32-bit 2's complement
    # Interpret as signed: convert if bit 31 is set
    return {
        'voltage_rms': voltage,
        'current_a_rms': current_a,
        'current_b_rms': current_b
    }
```

---

## 3. Active Power Measurement

### Full Power Configuration
```python
def enable_active_power():
    """Configure for active/reactive/apparent power"""
    
    # DSP_CTRL0: Enable channels, set mode
    write_register(0x02, (
        0 << 31 |      # CURDAT_RATE: Normal (20ms)
        0b0001 << 7 |  # DSP_MODE: 64 samples/cycle
        0 << 2 |       # Channel enable bits (already set)
        1 << 1 |       # ADCIBEN: Enabled
        1 << 0         # ADCIAEN: Enabled
    ) | 0x00000004)    # Enable voltage only (or'ed with previous)
    
    # DSP_CTRL1: Enable high-speed accumulators if using energy
    write_register(0x03, (
        0 << 6 |       # CALCEN1: Enable energy accum 1
        0 << 7         # CALCEN2: Enable energy accum 2
    ))
    
    # Frequency measurement
    write_register(0x02, read_register(0x02) & ~(0x1 << 27))  # DC_METER_MODE = 0
    
    delay(250ms)  # Stabilization
```

### Read Power Data
```python
def read_power_values():
    """Get active, reactive, apparent power"""
    
    pa_inst = read_register(0x08)  # DSP_DAT_PA - Inst. active power A
    qa_inst = read_register(0x09)  # DSP_DAT_QA - Inst. reactive power A
    sa_inst = read_register(0x0A)  # DSP_DAT_SA - Inst. apparent power A
    
    pa_avg = read_register(0x13)   # DSP_DAT_PA1 - Avg. active power A
    
    return {
        'power_active_inst': pa_inst,
        'power_reactive_inst': qa_inst,
        'power_apparent_inst': sa_inst,
        'power_active_avg': pa_avg
    }
```

---

## 4. Energy Accumulator Setup

### Basic Energy Metering
```python
def setup_energy_accumulator():
    """Configure high-speed energy accumulator for kWh counting"""
    
    # Enable accumulator 1 (high-speed)
    write_register(0x03, (
        0 << 6 |   # CALCEN1: Enable
        0 << 8     # CF1_SEL: Use accumulator 1
    ) | read_register(0x03))
    
    # Set accumulation constant (for pulse scaling)
    # CKSUM mode typically pre-configured
    write_register(0x3B, 0x00000001)  # EGY_CONST1: Constant
    
    # Set threshold for CF output (pulse width)
    write_register(0x3A, 0x00000064)  # EGY_PWRTH: Threshold
    
    # Set anti-creep threshold
    write_register(0x39, 0x00000001)  # EGY_PROCTH: Anti-creep
    
    # Enable energy accumulator (DSP_CTRL1[6])
    dsp_ctrl1 = read_register(0x03)
    dsp_ctrl1 |= (1 << 6)  # CALCEN1
    write_register(0x03, dsp_ctrl1)
    
    delay(200ms)
```

### Read Accumulated Energy
```python
def read_energy():
    """Get energy accumulator value"""
    
    energy_low = read_register(0x3C)   # EGY_OUT1L - Low 32-bit
    energy_high = read_register(0x3D)  # EGY_OUT1H - High part (14-bit)
    
    # Reconstruct 46-bit value
    energy_46bit = (energy_high << 32) | energy_low
    
    # Scale: accumulated value = energy_46bit >> 14
    energy_kwh = (energy_46bit >> 14) * accumulation_constant
    
    return energy_kwh
```

---

## 5. Waveform Capture

### Buffer-Based Capture (Internal RAM)
```python
def capture_waveform_buffered():
    """Capture waveform to internal RAM buffer"""
    
    # DSP_CTRL5: Configure waveform storage
    dsp_ctrl5 = (
        0 << 31 |      # WAVE_ADDR_CLR: Read address
        0b00 << 29 |   # WAVE_MDSP_MODE: Manual storage
        0 << 28 |      # WAVE_MEM_EN: Manual trigger
        1 << 10 |      # WAVE_U_SEL: Enable voltage
        1 << 9 |       # WAVE_IA_SEL: Enable current A
        0 << 8 |       # WAVE_IB_SEL: Disable current B
        0b0000 << 16   # WAVE_LENGTH: 1 cycle
    )
    write_register(0x07, dsp_ctrl5)
    
    # Wait for capture
    delay(100ms)
    
    # Check completion
    int_status = read_register(0x72)  # SYS_INTSTS
    if int_status & (1 << 7):  # WAVE_STORE flag
        return True  # Waveform captured
    return False
```

### Read Waveform Data
```python
def read_waveform():
    """Extract waveform samples from buffer"""
    
    waveform = []
    for i in range(128):  # 128 samples per cycle
        sample = read_register(0x69)  # DAT_WAVE (repeatable read)
        
        # Parse format
        voltage = extract_bits(sample, 8, 32)
        channel = extract_bits(sample, 4, 8)
        
        waveform.append({
            'index': i,
            'value': voltage,
            'channel': channel
        })
    
    return waveform
```

---

## 6. Zero-Crossing Detection

### Enable Zero-Crossing Interrupt
```python
def enable_zero_crossing_interrupt():
    """Setup voltage zero-crossing detection"""
    
    # DSP_CTRL1: Configure zero-crossing source
    dsp_ctrl1 = read_register(0x03)
    dsp_ctrl1 &= ~(0b11 << 18)  # Clear SIGN_SEL
    dsp_ctrl1 |= 0b10 << 18     # SIGN_SEL: Both directions
    write_register(0x03, dsp_ctrl1)
    
    # SYS_INTEN: Enable zero-crossing interrupt
    int_enable = read_register(0x73)
    int_enable |= (1 << 9)  # USIGN: Enable voltage ZC
    write_register(0x73, int_enable)
    
    # SYS_IOCFGX0: Map P0 to voltage zero-crossing
    write_register(0x7D, (
        0b00000010 << 0  # P0CFG: Voltage zero-crossing
    ))
```

### Handle Zero-Crossing Event
```python
def handle_zero_crossing():
    """Called when zero-crossing interrupt fires"""
    
    # Read phase values for interpolation
    phase_u = read_register(0x62)  # DSP_PHS_U
    phase_sample_before = read_register(0x63)  # DSP_PHS_UN
    phase_sample_after = read_register(0x64)  # DSP_PHS_UP
    
    # Interpolate exact zero-crossing time
    # zero_cross = -sample_before / (sample_after - sample_before)
    
    # Clear interrupt flag
    write_register(0x72, (1 << 9))  # Clear USIGN flag in SYS_INTSTS
```

---

## 7. Calibration Setup

### DC Bias Calibration
```python
def calibrate_dc_bias():
    """Remove DC offset from measurements"""
    
    # Measure DC component with no AC signal
    delay(500ms)  # Wait for stabilization
    
    dc_voltage = read_register(0x22)  # DSP_DAT_DCU
    dc_current_a = read_register(0x23)  # DSP_DAT_DCIA
    dc_current_b = read_register(0x24)  # DSP_DAT_DCIB
    
    # Write negative values as calibration offset
    write_register(0x34, -dc_voltage)   # DSP_CFG_DCU
    write_register(0x35, -dc_current_a) # DSP_CFG_DCIA
    write_register(0x36, -dc_current_b) # DSP_CFG_DCIB
    
    # Verify DC component returns to ~0
    delay(200ms)
    
    dc_check = read_register(0x22)
    return abs(dc_check) < 100  # Success if near zero
```

### Gain Calibration
```python
def calibrate_gain(reference_value, measured_register):
    """Adjust gain to match reference"""
    
    # Read measured value
    measured = read_register(measured_register)
    
    # Calculate calibration multiplier
    # gain = reference / measured (in fixed-point)
    
    gain_calibration = fixed_point_divide(reference_value, measured)
    
    # Apply to appropriate register
    # Typically: 0x2D (RMSU), 0x2F (RMSIA), 0x31 (RMSIB)
    write_register(0x2D, gain_calibration)
    
    delay(100ms)
    
    # Verify
    remeasured = read_register(measured_register)
    return abs(remeasured - reference_value) < tolerance
```

---

## 8. Power-Creep Detection

### Configure Power-Creep Thresholds
```python
def setup_power_creep():
    """Prevent measurement errors at no-load conditions"""
    
    # Set lower threshold (startup power)
    write_register(0x55, 0x00001000)  # DSP_OV_THL
    
    # Set upper threshold (power reference)
    write_register(0x56, 0x00100000)  # DSP_OV_THH
    
    # Enable power-creep detection
    write_register(0x03, read_register(0x03) | (1 << 1))  # PWR_CRP_EN
    
    # For energy accumulator
    write_register(0x39, 0x00000001)  # EGY_PROCTH
    write_register(0x3A, 0x00000064)  # EGY_PWRTH
```

### Check Power-Creep Status
```python
def check_power_creep():
    """Determine if in power-creep condition"""
    
    status = read_register(0x74)  # SYS_STS
    
    pa_creep = bool(status & (1 << 12))   # PACREEP
    qa_creep = bool(status & (1 << 13))   # QACREEP
    sa_creep = bool(status & (1 << 14))   # SACREEP
    
    return {
        'active_power_creep': pa_creep,
        'reactive_power_creep': qa_creep,
        'apparent_power_creep': sa_creep
    }
```

---

## 9. Configuration Checksum

### Calculate & Apply Configuration Checksum
```python
def apply_configuration_checksum():
    """Required after modifying calibration/config registers"""
    
    # Read all configuration registers that participate in checksum
    config_registers = [
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,  # Analog & Metering
        0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C,  # Power calibration
        0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32,              # RMS calibration
        0x33, 0x34, 0x35, 0x36, 0x37,                     # Phase & bandpass
        0x39, 0x3A,                                        # Energy thresholds
        0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,              # Power & voltage thresholds
        0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60,              # Fast detection
        0x80                                               # DSP_CTRL6
    ]
    
    # Sum all registers
    checksum_sum = 0
    for addr in config_registers:
        value = read_register(addr)
        checksum_sum += value
    
    # Calculate checksum: 0xFFFFFFFF - sum
    checksum = (0xFFFFFFFF - checksum_sum) & 0xFFFFFFFF
    
    # Write to checksum register
    write_register(0x38, checksum)  # DSP_CFG_CKSUM
    
    # Verify
    int_status = read_register(0x72)
    checksum_error = bool(int_status & (1 << 14))
    
    return not checksum_error  # True if checksum valid
```

---

## 10. Error Handling

### Check System Status
```python
def check_system_health():
    """Monitor for configuration and hardware errors"""
    
    int_status = read_register(0x72)  # SYS_INTSTS
    sys_status = read_register(0x74)  # SYS_STS
    
    errors = {
        'uart_error': bool(int_status & (1 << 16)),
        'spi_error': bool(int_status & (1 << 19)),
        'checksum_error': bool(int_status & (1 << 14)),
        'reference_error': bool(int_status & (1 << 12)),
        'bist_error': bool(int_status & (1 << 11)),
        'power_down': bool(int_status & (1 << 1)),
        'ram_init_ok': bool(sys_status & (1 << 4)),
        'checksum_ok': not bool(sys_status & 1),
    }
    
    for error, state in errors.items():
        if state:
            print(f"⚠️  {error}: {state}")
    
    return errors
```

### Clear Interrupt Flags
```python
def clear_all_interrupts():
    """Reset all interrupt flags"""
    
    # Write 1 to clear flags in SYS_INTSTS (0x72)
    flags_to_clear = 0xFFFFFFFF
    write_register(0x72, flags_to_clear)
```

---

## 11. Multi-Device Setup (Broadcasting)

### Configuration via Broadcast
```python
def broadcast_configuration():
    """Write same config to all V93XX on bus"""
    
    # Prepare broadcast frame
    # Address A1=X, A0=X (all chips)
    
    # Enable channels (all V93XX)
    broadcast_write(
        address=0x02,  # DSP_CTRL0
        length=1,      # 1 register
        data=0x00000007  # Enable all channels
    )
    
    delay(3ms)  # Wait for all devices to process
    
    # Verify each device individually
    for device_addr in [0x00, 0x01, 0x02, 0x03]:
        set_device_address(device_addr)
        value = read_register(0x02)
        assert value == 0x00000007, f"Device {device_addr} not configured"
```

---

## 12. Common Measurements Table

| Measurement | Register(s) | Format | Update Rate |
|------------|------------|--------|------------|
| Voltage RMS | 0x0E (inst), 0x19 (avg) | 32-bit 2's compl | 10-40ms |
| Current A RMS | 0x0F (inst), 0x1A (avg) | 32-bit 2's compl | 10-40ms |
| Active Power | 0x08 (inst), 0x13 (avg) | 32-bit 2's compl | 20-80ms |
| Reactive Power | 0x09 (inst), 0x14 (avg) | 32-bit 2's compl | 20-80ms |
| Line Frequency | 0x21 | 16-bit unsigned | 20-1280ms |
| Phase | 0x62 (voltage), 0x65 (current) | 16-bit unsigned | On-demand |
| Energy | 0x3C/0x3D (L/H) | 46-bit combined | Continuous |

---

## Typical Register Configuration Values

```python
# Minimal RMS-only operation
DSP_CTRL0 = 0x00000007   # Enable U, IA, IB; DSP mode 0
DSP_CTRL1 = 0x00000000   # Disable energy & CF
DSP_CTRL5 = 0x00000000   # No waveform
DSP_CTRL6 = 0x00000000   # Standard DSP

# Full metering (power + energy)
DSP_CTRL0 = 0x00000007   # Enable channels
DSP_CTRL1 = 0x000000C0   # Enable accumulators 1 & 2
DSP_CTRL5 = 0x00000000   # No waveform
DSP_CTRL6 = 0x00000000   # Standard DSP

# With waveform capture
DSP_CTRL5 = 0x00000600   # Enable U & IA waveform, 1 cycle
DSP_CTRL6 = 0x00000000   # Standard DSP
```

---

Last Updated: 2025-02-22  
V4.9 Datasheet Compatible
