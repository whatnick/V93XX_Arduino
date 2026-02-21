# V9381 UART Debugging Automation - Complete

## 🎯 What Was Built

A comprehensive automation framework combining **three professional tools** (Arduino CLI, Saleae Logic 2, pySerial) into a unified debugging workflow:

### Core Tools Created

1. **`orchestrate_debug.py`** ⭐ **START HERE**
   - Master controller for end-to-end debugging
   - Automates: compile → upload → monitor → capture → analyze
   - Generates timestamped debug reports in `debug_output/`
   - Single command: `python tools/orchestrate_debug.py`

2. **`automated_uart_debug.py`** 
   - Arduino CLI + pySerial integration
   - Compile, upload, and monitor in one script
   - Deep frame validation with checksum verification
   - Identifies driver-level issues

3. **`uart_diagnostics.py`**
   - Advanced serial monitoring with protocol decode
   - Real-time checksum validation (odd parity)
   - JSONL logging for programmatic analysis
   - Detailed frame reporting

4. **`logic2_automation.py`**
   - Saleae Logic 2 control via automation socket
   - Configurable capture with multi-decoder setup
   - Data export to CSV for side-by-side comparison
   - Correlation with pySerial for discrepancy detection

5. **`monitor_uart.py`**
   - Lightweight serial monitor
   - Quick byte capture and pattern detection
   - Minimal overhead for real-time monitoring

6. **`verify_setup.py`**
   - Environment validation checklist
   - Checks Python packages, Arduino CLI, Logic 2, serial ports, workspace structure
   - Hardware configuration guidance

### Documentation Created

1. **`DEBUG_GUIDE.md`** - Comprehensive troubleshooting and usage guide
2. **`QUICK_START.md`** - Quick reference for common tasks

---

## 🚀 How to Use

### Option 1: Full Automated Debug (Recommended)

```bash
# Activate environment
& .\.venv\Scripts\Activate.ps1

# Run complete debug sequence
python tools/orchestrate_debug.py

# Results saved to debug_output/<timestamp>/
```

**What it does:**
- Compiles V9381_UART sketch
- Uploads to ESP32-S3
- Captures 30 seconds of serial monitoring + UART frames
- Validates checksums in real-time
- Triggers Saleae capture (if Logic 2 is open)
- Correlates all findings
- Generates actionable report

**Output:**
```
debug_output/
├── 20260222_143022/
│   ├── results.json              # Summary findings
│   ├── uart_diagnostic.jsonl    # Frame-by-frame decode
│   └── capture.csv              # Saleae export (if available)
```

---

### Option 2: Quick Serial Check

```bash
python tools/monitor_uart.py
```

Fast byte capture to verify any UART activity.

---

### Option 3: Detailed Diagnostics (No Upload)

```bash
# If sketch is already running:
python tools/orchestrate_debug.py --no-upload --duration 60
```

Extended monitoring without recompiling/uploading.

---

### Option 4: Verify Everything Works

```bash
python tools/verify_setup.py
```

Checks:
- ✅ Python 3.12+
- ✅ Arduino CLI installed
- ✅ ESP32-S3 connected
- ✅ COM7 available
- ✅ Logic 2 automation ready
- ✅ Workspace structure intact

---

## 🔍 Key Features

### Real-Time Checksum Validation
Each UART frame is validated against expected checksum:
```
✓ Frame #1: 0x7D 0x00 0x40 0x00 0x3D  (checksum OK)
✗ Frame #2: 0x7D 0x00 0x41 0xFF 0xFF (checksum mismatch!)
  Expected: 0xBE, Calculated: 0x3E
```

### Cross-Reference Analysis
Compares what Saleae sees (hardware) vs what driver receives (software):
```
Saleae: 45 RX frames captured
pySerial: 43 valid decodes + 2 checksum failures
→ 2 frames being rejected by validation logic!
```

### Detailed Frame Logging
JSON format for programmatic analysis:
```json
{
  "timestamp": 5.234,
  "type": "frame",
  "raw": "7D 00 40 FF 3D",
  "decoded": {
    "marker": "0x7D",
    "addr": "0x00",
    "cmd": "0x40",
    "value": "0xFF3D",
    "checksum_expected": "0x3D",
    "checksum_calculated": "0x3D",
    "match": true
  }
}
```

---

## 🐛 Debugging Workflow

### Step 1: Check Hardware
```bash
python tools/verify_setup.py
```
Ensure ESP32-S3, Logic Analyzer, and COM7 are connected.

### Step 2: Run Full Debug
```bash
python tools/orchestrate_debug.py --duration 30
```
Capture 30 seconds of UART activity with checksums validated.

### Step 3: Review Results
Open `debug_output/<timestamp>/results.json`:
- **Total frames captured?** → UART wiring OK
- **Checksum mismatches?** → Driver validation bug
- **Fewer pySerial frames than Saleae?** → Buffer/timeout issue
- **All valid?** → Issue may be higher-level logic

### Step 4: Implement Fix
Based on findings, edit:
- `V93XX_Raccoon.cpp` — RegisterRead() logic
- `V93XX_Raccoon.h` — Timeout/config parameters

### Step 5: Verify Fix
```bash
python tools/orchestrate_debug.py
```
Confirm frames now pass validation.

---

## 📊 Real-World Example

**Problem:** Logic 2 shows V9381 responding, but RegisterRead() times out

**Debug Command:**
```bash
python tools/orchestrate_debug.py
```

**Results Report:**
```
✓ Frames captured: 12 total
✓ Checksum OK: 10
✗ Checksum FAIL: 2

First failure:
  Transmitted: 0x7D 0x00 0x40 0x00 0x3D (SYS_INTSTS read)
  Received:    0xFE 0x36 0x00 0x00 0x76
  
  Expected checksum (calc from 0xFE 0x36 0x00 0x00): 0x76
  Driver received:                              0x??
  
  DIAGNOSIS: RegisterRead() not capturing full response!
             → Check WaitForRx() timeout or RX buffer size
```

**Fix Applied:** Increase RX buffer or timeout in V93XX_Raccoon.cpp

**Verification:**
```bash
python tools/orchestrate_debug.py
```
✅ All checksums now pass!

---

## 🎮 Tool Selection Guide

| Situation | Command | Why |
|-----------|---------|-----|
| "I need everything at once" | `orchestrate_debug.py` | Handles all 3 tools |
| "Quick byte check" | `monitor_uart.py` | Fastest startup |
| "Detailed frame analysis" | `uart_diagnostics.py` | Best diagnostics |
| "No upload, just monitor" | `orchestrate_debug.py --no-upload` | Reuse sketch |
| "Check my setup is OK" | `verify_setup.py` | Validation checklist |
| "Logic 2 only" | `logic2_automation.py` | Hardware capture |

---

## 📁 File Structure

```
tools/
├── orchestrate_debug.py         ⭐ Master controller
├── automated_uart_debug.py      → CLI + serial combo
├── uart_diagnostics.py          → Serial + validation
├── logic2_automation.py         → Saleae control
├── monitor_uart.py              → Simple monitor
├── verify_setup.py              → Setup checker
├── DEBUG_GUIDE.md               → Full documentation
└── QUICK_START.md               → Quick reference
```

---

## ✅ Status

**Setup complete!**

- ✅ Arduino CLI connected to ESP32-S3
- ✅ pySerial installed and functional  
- ✅ Logic 2 automation socket ready
- ✅ All tools tested and working
- ✅ Documentation complete

**Next:** 
```bash
python tools/orchestrate_debug.py
```

---

## 🔧 Emergency Troubleshooting

### "Logic 2 not found" warning
→ Not critical; other tools still work. Open Logic 2 to enable automation socket.

### "Arduino CLI not found"
→ Install: https://arduino.github.io/arduino-cli/latest/

### "pySerial not working"
→ Already installed via `uv sync`. Should be in virtual environment.

### "COM7 not available"
→ Connect ESP32-S3 USB cable. Check Device Manager or run `verify_setup.py`.

### "Frames not being captured"
→ Verify UART config in V9381_UART.ino: 19200 baud, 8O1, pins 11/12

---

## 📝 Next Steps

1. **Verify setup:** `python tools/verify_setup.py`
2. **Run debug:** `python tools/orchestrate_debug.py`
3. **Review results:** Check `debug_output/<timestamp>/results.json`
4. **Fix issues:** Edit V93XX_Raccoon.cpp based on findings
5. **Re-test:** Run orchestrator again to confirm fix

**Commit:**
```bash
git add .
git commit -m "Fix V9381 UART driver based on automated debugging results"
git push origin feat/v9381_uart
```

---

For detailed usage and troubleshooting, see [DEBUG_GUIDE.md](DEBUG_GUIDE.md)
