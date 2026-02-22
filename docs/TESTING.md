# Testing V93XX UART ChecksumMode

Complete testing framework and workflows for validating V9381 UART implementation.

## 🎯 Testing Overview

Three levels of testing available:

### Level 1: Unit Tests (No Hardware)
```bash
python tools/test_checksum_mode.py
# Time: ~2 seconds
# Tests: CRC algorithm, mode behavior, frame formats
```
**Use when**: You want to validate logic without hardware

### Level 2: Device Tests (Hardware Required)
```bash
.\tools\run_automated_tests.ps1 -Port COM3 -SkipCapture
# Time: ~40 seconds
# Tests: Build, upload, serial initialization, mode verification
```
**Use when**: You want to verify hardware integration

### Level 3: Full Integration Tests (Hardware + Logic Analyzer)
```bash
.\tools\run_automated_tests.ps1 -Port COM3
# Time: ~80 seconds
# Tests: All of Level 2 + UART capture + CRC frame analysis
```
**Use when**: You want complete validation including captured frames

---

## 📋 Test Phases

### Phase 1: Unit Tests
**Command**: `python tools/test_checksum_mode.py`

**What gets tested**:
- CRC-8 calculation (5 test cases)
- Clean mode behavior (strict validation)
- Dirty mode behavior (permissive)
- UART request frames
- UART response frames
- Mode comparison logic

**Expected output**:
```
✅ PASS - CRC Calculation
✅ PASS - Clean Mode Behavior
✅ PASS - Dirty Mode Behavior
✅ PASS - UART Request Frame
✅ PASS - UART Response Frame
✅ PASS - Mode Comparison

✅ All tests passed!
```

### Phase 2: Build & Compile
**Command**: `arduino-cli compile --fqbn esp32:esp32:esp32s3 ...`

**What gets tested**:
- Arduino sketch compilation
- Binary artifact generation (>100KB)
- No compilation errors

**Success**: `.bin` file created in `build/` directory

### Phase 3: Upload to Device
**Command**: `arduino-cli upload --port COM3 ...`

**What gets tested**:
- Device connectivity
- Binary flashing
- Device restart

**Success**: Device restarts and runs sketch

### Phase 4: Serial Verification
**Command**: `python tools/monitor_serial.py COM3 115200 10`

**What gets tested**:
- Serial communication (115200 baud)
- Device initialization
- ChecksumMode setting
- Serial output parsing

**Expected output**:
```
[14:32:01.234] Initializing V9381 UART...
[14:32:01.567] Checksum Mode set to Dirty (skip CRC validation)
[14:32:02.123] CRC Valid: Expected 0x2B, Received 0x2B
```

### Phase 5: UART Capture
**Command**: `python tools/capture_v9381_uart.py`

**What gets tested**:
- Saleae Logic Analyzer connection
- UART data capture at 19200 baud
- CSV export of captured frames
- RX and TX channel separation

**Success**: Files created in `tools/captures/v9381_uart_YYYYMMDD_HHMMSS/`

### Phase 6: CRC Analysis
**Command**: `python tools/analyze_checksum_captures.py [capture_dir]`

**What gets tested**:
- UART frame parsing
- CRC calculation on captured frames
- Expected vs received CRC comparison
- Mode behavior prediction

**Expected output**:
```
✓ Analyzed 12 frames

Frame #1 (REQUEST #1):
  Address: 0x00 | Operation: READ
  CRC Analysis:
    Expected: 0x2B
    Received: 0x2B
    Status: ✅ VALID
  Mode Behavior:
    Clean Mode: CONTINUE
    Dirty Mode: CONTINUE
```

---

## ✅ Success Criteria

### Unit Tests Pass When
- ✅ CRC algorithm correctly calculates all 5 test cases
- ✅ Clean mode rejects invalid CRC (shows ERROR)
- ✅ Dirty mode accepts invalid CRC (shows WARNING)
- ✅ Frame formats match specification

### Device Tests Pass When
- ✅ Binary compiles without errors
- ✅ Upload succeeds
- ✅ Serial output contains "Checksum Mode set to Dirty"
- ✅ CRC messages appear (Valid or Mismatch)

### Full Integration Passes When
- ✅ All of above PLUS:
- ✅ Capture files created (uart_rx.csv, uart_tx.csv)
- ✅ CRC analysis shows valid/mismatch status
- ✅ JSON report generated

---

## 🐛 Debugging Workflow

### Scenario: CRC Mismatches in Dirty Mode
```
Serial output:
  CRC Mismatch! Expected 0x2B, Received 0xAA
  CRC Mismatch! Expected 0x31, Received 0x15
```

**Diagnosis steps**:
1. Verify UART wiring (GPIO1=TX→RX, GPIO2=RX←TX)
2. Check baud rate (should be 19200)
3. Verify V9381 device is powered
4. Use Logic Analyzer to capture and analyze with Phase 6

### Scenario: Clean Mode Shows ERROR
```
Serial output:
  ERROR: CRC mismatch!
```

**Expected behavior** (design):
- Operation aborts
- Returns 0 or previous value
- No data corruption

**Verify this works**: Check that operations don't proceed after ERROR

### Scenario: No CRC Messages at All
```
Serial output:
  (blank)
```

**Diagnosis**:
1. Device not initializing - check power
2. Wrong serial port - run `arduino-cli board list`
3. Baud rate mismatch - verify it's 115200
4. Sketch not running - check reset button

---

## 📊 CRC Reference

### Algorithm
```python
def calculate_crc8_v9381(payload_bytes: list[int]) -> int:
    checksum = sum(payload_bytes) & 0xFF
    crc = (0x33 + (~checksum & 0xFF)) & 0xFF
    return crc
```

### Test Cases (for validation)
| Payload | CRC | Notes |
|---------|-----|-------|
| [0x04, 0x02] | 0x2C | Basic case |
| [0x01, 0x00] | 0x31 | Low byte |
| [0xFF, 0xFF, 0xFF, 0xFF] | 0x36 | All ones |
| [0x00, 0x00, 0x00, 0x00] | 0x32 | All zeros |
| [0xF0, 0x02] | 0xF0 | High/low |

### Frame Types
- **Request**: CRC = CRC8([CMD1, CMD2])
- **Response**: CRC = CRC8([CMD1, CMD2, Data0, Data1, Data2, Data3])

---

## 🛠️ Tools Reference

| Tool | Purpose | Command |
|------|---------|---------|
| `test_checksum_mode.py` | Unit tests | `python tools/test_checksum_mode.py` |
| `monitor_serial.py` | Serial capture | `python tools/monitor_serial.py COM3 115200 10` |
| `capture_v9381_uart.py` | UART capture | `python tools/capture_v9381_uart.py` |
| `analyze_checksum_captures.py` | CRC analysis | `python tools/analyze_checksum_captures.py [dir]` |
| `run_automated_tests.ps1` | Full workflow | `.\tools\run_automated_tests.ps1 -Port COM3` |

---

## 📈 Expected Performance

| Test Level | Time | Hardware | Coverage |
|-----------|------|----------|----------|
| Unit tests | 2s | None | Logic verification |
| Device tests | 40s | ESP32-S3 | Build + upload + init |
| Full tests | 80s | ESP32-S3 + Analyzer | Complete validation |

---

## 🔍 Interpreting Results

### JSON Report
After each run, check: `test_report_YYYYMMDD_HHMMSS.json`

```json
{
  "overall": {
    "result": true,
    "status": "PASSED"
  },
  "serial_verification": {
    "result": true,
    "description": "Device initialized correctly"
  }
}
```

### Serial Output Files
Check: `serial_output.txt` (created during Phase 4)

### CSV Capture Files
Check: `tools/captures/v9381_uart_YYYYMMDD_HHMMSS/`
- `uart_rx.csv` - Received bytes with timestamps
- `uart_tx.csv` - Transmitted bytes with timestamps

---

## 🚀 Running Tests

### Everyone's First Test
```bash
python tools/test_checksum_mode.py
# Should pass in 2 seconds
```

### Quick Hardware Validation
```bash
.\tools\run_automated_tests.ps1 -Port COM3 -SkipCapture
# Should pass in 40 seconds
```

### Full Validation
```bash
.\tools\run_automated_tests.ps1 -Port COM3
# Should pass in 80 seconds
```

### Retest Without Rebuild
```bash
.\tools\run_automated_tests.ps1 -Port COM3 -SkipBuild
# Skips compilation, reuses previous build
```

---

For automated testing with AI agents, see [COPILOT.md](COPILOT.md).
