# ChecksumMode Testing & Validation Workflow

This document provides a complete workflow for testing and validating the V9381 UART Checksum Mode implementation using both simulated tests and live capture analysis.

## Overview

Three tools work together to validate ChecksumMode:

1. **test_checksum_mode.py** - Unit tests validating CRC calculations and mode behavior logic
2. **capture_v9381_uart.py** - Hardware capture tool using Saleae Logic Analyzer  
3. **analyze_checksum_captures.py** - Analyzer for verifying CRC in captured data

## Quick Start

### Phase 1: Validate Logic with Unit Tests

```bash
# Run the complete test suite
python tools/test_checksum_mode.py
```

**What gets tested:**
- CRC-8 calculation algorithm (5 payloads)
- Clean mode enforcement (valid & invalid CRC)
- Dirty mode permissive behavior (valid & invalid CRC)
- UART request frame format
- UART response frame format
- Mode comparison logic

**When to use:** Before hardware testing to verify the implementation logic is sound.

**Expected output:**
```
✅ PASS - CRC Calculation
✅ PASS - Clean Mode Behavior
✅ PASS - Dirty Mode Behavior
✅ PASS - UART Request Frame
✅ PASS - UART Response Frame
✅ PASS - Mode Comparison

✅ All tests passed!
```

### Phase 2: Capture Live UART Communication

```bash
# Connect:
# 1. V9381 UART TX to GPIO (check your ESP32-S3 pinout)
# 2. V9381 UART RX to GPIO
# 3. Logic Analyzer to the same lines + GND
# 4. Connect ESP32-S3 to USB

# Run example with dirty mode (default)
# Then capture with Logic Analyzer (⏺ button or via Python API)

# Capture to memory and save as SAL + CSV
python tools/capture_v9381_uart.py
```

**What gets captured:**
- Raw UART data at 19200 baud
- Digital waveforms on TX and RX lines
- CSV export with timestamps and data bytes

**When to use:** After unit tests pass, to validate real hardware behavior.

**Expected output:**
```
Logic Analyzer capture saved to:
  tools/captures/v9381_uart_20260222_162000/
    capture.sal (raw capture)
    uart_rx.csv (decoded RX)
    uart_tx.csv (decoded TX)
    ...
```

### Phase 3: Analyze Captured Data

```bash
# Automatically finds latest capture and analyzes it
python tools/analyze_checksum_captures.py

# Or specify a specific capture directory
python tools/analyze_checksum_captures.py tools/captures/v9381_uart_20260222_162000
```

**What gets analyzed:**
- Extracts frame structures from CSV
- Calculates expected CRC for each frame
- Compares with received CRC
- Shows what Clean vs Dirty mode would do
- Generates comprehensive report

**When to use:** After capture completes, to verify real device matches CRC calculations.

**Expected output:**
```
====================================================================================================
V9381 UART CAPTURE CHECKSUM ANALYSIS
File: uart_rx.csv
====================================================================================================

✓ Analyzed 12 frames

SUMMARY:
----------------------------------------------------------------------------------------------------
  Total Frames: 12
  Valid CRCs: 12 (100%)
  Invalid CRCs: 0 (0%)
  Errors: 0

DETAILED FRAME ANALYSIS:
----------------------------------------------------------------------------------------------------

Frame #1 (REQUEST #1):
  Address: 0x00 | Operation: READ
  Bytes: 0x7D 0x01 0x00 0x2B
  CRC Analysis:
    Expected: 0x2B
    Received: 0x2B
    Status: ✅ VALID
  Mode Behavior:
    Clean Mode: CONTINUE
    Dirty Mode: CONTINUE

...

ASSESSMENT:
----------------------------------------------------------------------------------------------------
✅ All frames have valid CRCs!
   → Your V9381 device and CRC calculation are correctly implemented
   → Use Clean mode for production (enforce CRC)
====================================================================================================
```

## Detailed Workflow

### Setup (One-time)

1. **Hardware connections:**
   ```
   ESP32-S3              V9381
   GPIO_TX    ------->  UART_RX
   GPIO_RX    <--------  UART_TX
   GND        ------->  GND
   
   Logic Analyzer
   D0         ------>   UART_RX (channel 0)
   D1         ------>   UART_TX (channel 1)
   GND        ------>   GND
   ```

2. **Python environment:**
   ```bash
   cd c:\Users\tisha\OneDrive\Documents\Arduino\libraries\V93XX_Arduino
   
   # Activate venv
   .\.venv\Scripts\activate
   
   # Verify tooling available
   python -c "import saleae; print('Saleae SDK available')"
   ```

3. **Verify Example Runs:**
   ```bash
   # In Arduino IDE: Upload examples/V9381_UART_DIRTY_MODE/V9381_UART_DIRTY_MODE.ino
   # Should see Serial output like:
   #   "Initializing V9381 UART..."
   #   "Checksum Mode set to Dirty (skip CRC validation)"
   #   "Testing with Dirty Mode..."
   #   [Various register operations]
   ```

### Test Scenario 1: Verify CRC Algorithm (Unit Test)

```bash
python tools/test_checksum_mode.py
```

**What this validates:**
- Core CRC formula: `CRC = 0x33 + ~(sum of payload bytes)`
- Algorithm handles edge cases (zeros, all ones)
- Expected values for each payload type

**Success criteria:** All 6 test categories show ✅ PASS

**Common failures & fixes:**

| Failure | Cause | Fix |
|---------|-------|-----|
| "CRC Calculation FAIL" | Algorithm not implemented | Check V93XX_Raccoon.cpp CRC logic |
| "Clean Mode Behavior FAIL" | Validation logic broken | Check `checksum_mode == ChecksumMode::Clean` |
| "Dirty Mode Behavior FAIL" | Skip logic broken | Check `ChecksumMode::Dirty` handling |

### Test Scenario 2: Verify Clean Mode (Strict CRC Validation)

**Test code:**
```cpp
void TestCleanMode() {
  v9381.SetChecksumMode(V93XX_Raccoon::ChecksumMode::Clean);
  // Register read that would send valid CRC
  uint32_t value = v9381.RegisterRead(0x00);  // Should succeed
  
  // Simulate invalid CRC by modifying packet (hardware won't really allow this)
  // Clean mode SHOULD report ERROR and not return data
}
```

**Expected behavior:**
- Valid CRC: Operation succeeds, returns data
- Invalid CRC: Operation fails, prints: `ERROR: CRC mismatch!`
- No operation proceeds when CRC invalid

**Where to check:** Arduino Serial Monitor at 115200 baud

### Test Scenario 3: Verify Dirty Mode (Permissive CRC Handling)

**Test code:**
```cpp
void TestDirtyMode() {
  v9381.SetChecksumMode(V93XX_Raccoon::ChecksumMode::Dirty);
  // Any register operation
  uint32_t value = v9381.RegisterRead(0x00);
}
```

**Expected behavior:**
- Valid CRC: Operation succeeds, prints: `CRC Valid: Expected 0xXX, Received 0xXX`
- Invalid CRC: Operation still succeeds, prints: `CRC Mismatch! Expected 0xXX, Received 0xYY`
- Operation proceeds even if CRC invalid (useful for debugging)

**Where to check:** Arduino Serial Monitor at 115200 baud

### Test Scenario 4: Live Capture & Analysis

#### Step 1: Upload Example
```
1. Open examples/V9381_UART_DIRTY_MODE/V9381_UART_DIRTY_MODE.ino
2. Select ESP32-S3 board and COM port
3. Upload sketch
4. Serial Monitor should show initialization
```

#### Step 2: Capture UART Traffic
```bash
python tools/capture_v9381_uart.py
```

This will:
1. Connect to Logic Analyzer
2. Start capture on UART RX and TX channels
3. Wait for user to perform operations (press keys in Serial Monitor or call test functions)
4. Save capture as `.sal` and export as CSV

#### Step 3: Analyze Capture
```bash
python tools/analyze_checksum_captures.py
```

This will:
1. Parse CSV files
2. Extract UART frames (request & response)
3. Calculate expected CRC for each
4. Compare with captured CRC
5. Generate report showing Clean vs Dirty mode behavior

#### Step 4: Validate Results
```
✅ All frames have valid CRCs!
   → Device correctly sends CRC
   → Your CRC calculation matches device
   → Implementation is correct
```

## CRC Reference

### Algorithm
```python
def calculate_crc8_v9381(payload_bytes: list[int]) -> int:
    checksum = sum(payload_bytes) & 0xFF
    crc = (0x33 + (~checksum & 0xFF)) & 0xFF
    return crc
```

### Examples
| Payload | Sum | ~Sum | 0x33 + ~Sum | CRC |
|---------|-----|------|------------|-----|
| [0x04, 0x02] | 0x06 | 0xF9 | 0x2C | 0x2C |
| [0x01, 0x00] | 0x01 | 0xFE | 0x31 | 0x31 |
| [0xFF, 0xFF, 0xFF, 0xFF] | 0xFC | 0x03 | 0x36 | 0x36 |

### Payload Types
- **Request**: CRC = CRC8([CMD1, CMD2])
- **Response**: CRC = CRC8([CMD1, CMD2, Data0, Data1, Data2, Data3])

## Frame Formats

### Request Frame (4 bytes)
```
[Marker=0x7D] [CMD1] [CMD2] [CRC]
```

### Response Frame (6 bytes)
```
[Marker=0x7D] [Data0] [Data1] [Data2] [Data3] [CRC]
```

Note: Response CRC includes CMD1 and CMD2 from the request that triggered it.

## Troubleshooting

### Problem: CRC tests fail
**Diagnosis:**
```bash
python tools/test_checksum_mode.py
# Look for "CRC Calculation FAIL"
```

**Solutions:**
1. Check CRC formula in V93XX_Raccoon.cpp is exactly: `0x33 + ~(sum & 0xFF)) & 0xFF`
2. Verify `calculate_crc8_v9381()` is called in all three register methods
3. Compare captured CRC with expected on hardware

### Problem: Clean mode doesn't enforce CRC
**Diagnosis:**
```
Operations succeed even when CRC invalid
```

**Solutions:**
1. Check `if (this->checksum_mode == ChecksumMode::Clean)` condition
2. Verify error is printed and operation returns early
3. Test with: `v9381.SetChecksumMode(V93XX_Raccoon::ChecksumMode::Clean);`

### Problem: Capture file missing UART data
**Diagnosis:**
```bash
# Check if uart_rx.csv exists
ls tools/captures/v9381_uart_*/uart_rx.csv
# Should see files
```

**Solutions:**
1. Verify Logic Analyzer connected to correct GPIO pins
2. Check UART baud rate is 19200
3. Try manual capture in Logic Analyzer software first
4. Verify capture_v9381_uart.py finds Logic Analyzer device

### Problem: analyze_checksum_captures.py reports mismatches
**Diagnosis:**
```
❌ Found 5 frames with CRC mismatches
   → Use Dirty mode to debug
```

**Solutions:**
1. First run `test_checksum_mode.py` to verify algorithm
2. Check device firmware CRC implementation
3. Look for UART wiring issues or noise
4. Try different GPIO pins or cable routing
5. Compare expected vs received CRC in detail output

## Integration with Existing Tools

### Using with capture_v9381_uart.py

capture_v9381_uart.py now benefits from ChecksumMode:
```python
# In capture_v9381_uart.py, could add:
from analyze_checksum_captures import analyze_uart_capture

# After saving capture:
results = analyze_uart_capture(saved_csv_file)
for frame in results:
    if not frame.get('valid', True):
        print(f"CRC mismatch in frame: {frame}")
```

### Using with plot_v9360_waveform.py

The plotting tool can now add CRC status as overlay:
```python
# Future enhancement: Add CRC validation layer to plots
# Show clean vs dirty mode decision points
```

## Reference: Mode Comparison

| Aspect | Clean Mode | Dirty Mode |
|--------|-----------|-----------|
| **Purpose** | Production use | Debugging |
| **Invalid CRC** | ERROR, abort | WARNING, continue |
| **Serial Output** | Shows error | Shows warning with CRC values |
| **Ideal For** | Strict validation | Hardware debugging |
| **Default** | No (explicit set) | Yes (Init default) |

## Files Modified

- `V93XX_Raccoon.h` - Added ChecksumMode enum
- `V93XX_Raccoon.cpp` - Updated all register methods with mode awareness
- `examples/V9381_UART_DIRTY_MODE/V9381_UART_DIRTY_MODE.ino` - Example code
- `tools/test_checksum_mode.py` - Unit test suite (NEW)
- `tools/analyze_checksum_captures.py` - Capture analyzer (NEW)

## Support & Documentation

For more information, see:
- [CHECKSUM_MODE_QUICK_REFERENCE.md](../CHECKSUM_MODE_QUICK_REFERENCE.md)
- [DIRTY_MODE_IMPLEMENTATION.md](../DIRTY_MODE_IMPLEMENTATION.md)
- [ALIGNMENT_SUMMARY.md](../ALIGNMENT_SUMMARY.md)
