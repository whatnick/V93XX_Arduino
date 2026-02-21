# ✅ V9381 UART Debugging Automation - Complete Implementation

## 🎯 Mission Accomplished

You now have a **professional-grade debugging framework** combining Arduino CLI, Saleae Logic 2, and pySerial into a unified orchestrated workflow.

---

## 📦 What's Included

### Master Tools (3 Main Scripts)

1. **`orchestrate_debug.py`** ⭐ **PRIMARY TOOL**
   - One-command automated debugging
   - Compiles → Uploads → Monitors → Captures → Analyzes
   - Real-time UART frame validation
   - Cross-references hardware vs software captures
   - Generates timestamped reports

2. **`automated_uart_debug.py`** (Alternative)
   - Arduino CLI + pySerial integration
   - Frame-by-frame diagnostics
   - Checksum validation reporting

3. **`logic2_automation.py`** (Optional)
   - Saleae Logic 2 control
   - Protocol decoder setup
   - Data export and correlation

### Supporting Tools

- **`uart_diagnostics.py`** - Serial monitoring with advanced frame decode
- **`monitor_uart.py`** - Simple byte capture for quick checks
- **`verify_setup.py`** - Environment validation and prerequisites checker

### Quick Launch Scripts

- **`run_debug.ps1`** (PowerShell) - Parameter-based execution
- **`run_debug.bat`** (Batch) - Simple command execution

### Documentation

- **`QUICK_START.md`** - Fast reference guide
- **`DEBUG_GUIDE.md`** - Comprehensive troubleshooting
- **`AUTOMATION_SUMMARY.md`** - Complete feature breakdown

---

## 🚀 How to Run (Three Options)

### Option 1: PowerShell (Recommended)
```powershell
.\run_debug.ps1
```
Parameters:
```powershell
.\run_debug.ps1 -Duration 60      # Extended monitoring (60 sec)
.\run_debug.ps1 -NoUpload         # Skip upload
.\run_debug.ps1 -Duration 60 -NoUpload  # Both options
```

### Option 2: Command Prompt
```batch
run_debug.bat
```

### Option 3: Direct Execution
```bash
& .\.venv\Scripts\Activate.ps1
python tools\orchestrate_debug.py
```

---

## 📊 What It Does

### Step 1: Compile ✅
- Uses Arduino CLI to compile V9381_UART sketch
- Validates board configuration (esp32:esp32:esp32s3)
- Generates binary for upload

### Step 2: Upload ✅
- Uploads compiled binary to ESP32-S3 on COM7
- Verifies upload success
- Waits for device restart

### Step 3: Monitor Serial ✅
- Captures ESP32 debug output (115200 baud)
- Extracts UART frames (5-byte protocol)
- Validates checksum for each frame
  - Expected vs. Calculated
  - Shows `✓ PASS` or `✗ FAIL`
- Logs to JSON for analysis

### Step 4: Capture Hardware ✅
- Triggers Saleae Logic 2 (if available)
- Records channels 0/1/2/7 (A1/RX/A0/TX)
- Exports decoded UART data

### Step 5: Correlate & Analyze ✅
- Compares hardware capture vs software decode
- Reports frame discrepancies
- Identifies root causes
- Generates actionable recommendations

---

## 📈 Example Output

**When Running:**
```
[ARDUINO CLI] Compiling V9381_UART.ino...
[ARDUINO CLI] Compile ✓ SUCCESS

[ARDUINO CLI] Uploading to COM7...
[ARDUINO CLI] Upload ✓ SUCCESS

[PYSERIAL] Monitoring COM7 at 115200 baud...
[0.123s] Initializing V9381...
[1.456s] ✓ Frame: 7D 00 40 00 3D
[1.789s] ✓ Frame: 7D 00 41 FF BE
[2.012s] ✗ Frame: 7D 00 42 AB CD
  Expected checksum: 0xCD, Calculated: 0x45

[ANALYZER] Total frames: 12
[ANALYZER] Valid: 11, Invalid: 1
[ANALYZE] Check V93XX_Raccoon.cpp RegisterRead() validation
```

**Results Saved to:**
```
debug_output/20260222_143022/
├── results.json              (Summary)
├── uart_diagnostic.jsonl    (Frame data)
└── capture.csv              (Saleae export)
```

---

## 🔍 Key Features

### ✅ Real-Time Checksum Validation
```
✓ Frame #1: Checksum OK
✗ Frame #2: Expected 0xBE, Got 0x3E → Mismatch detected!
```

### ✅ Cross-Reference Analysis  
```
Hardware (Saleae): 45 RX frames
Software (pySerial): 43 valid + 2 failed
→ Driver rejecting valid frames!
```

### ✅ Machine-Readable Logs
```json
{
  "timestamp": 5.234,
  "decoded": {
    "marker": "0x7D",
    "checksum_expected": "0x3D",
    "checksum_calculated": "0x3D",
    "match": true
  }
}
```

### ✅ Actionable Diagnostics
- Identifies timeout issues
- Spots checksum calculation errors
- Detects frame loss
- Finds parity mismatches

---

## 🛠️ Troubleshooting Integration

### Problem: V9381 Not Responding
```bash
python tools/monitor_uart.py
```
Watch for any RX activity on COM7

### Problem: Checksum Mismatches
```bash
.\run_debug.ps1 -Duration 30
```
Open `debug_output/latest/uart_diagnostic.jsonl`
→ Compare `checksum_expected` vs `checksum_calculated`

### Problem: Frame Loss
```bash
.\run_debug.ps1 -Duration 60 -NoUpload
```
Run extended monitoring without reupload
→ Check timing in results.json

---

## 📋 Verification Checklist

Before running orchestrator:

```bash
python tools/verify_setup.py
```

Checks:
- ✓ Python 3.12+
- ✓ Arduino CLI installed
- ✓ pySerial available
- ✓ ESP32-S3 on COM7
- ✓ Logic 2 socket ready (optional)
- ✓ Workspace structure
- ✓ Driver files present

---

## 💾 All New Files Summary

| File | Purpose | Type |
|------|---------|------|
| `tools/orchestrate_debug.py` | Master controller | Python |
| `tools/automated_uart_debug.py` | CLI + Serial combo | Python |
| `tools/uart_diagnostics.py` | Serial diagnostics | Python |
| `tools/monitor_uart.py` | Serial monitor | Python |
| `tools/logic2_automation.py` | Saleae control | Python |
| `tools/verify_setup.py` | Setup checker | Python |
| `run_debug.ps1` | PowerShell launcher | Script |
| `run_debug.bat` | Batch launcher | Script |
| `tools/QUICK_START.md` | Quick reference | Documentation |
| `tools/DEBUG_GUIDE.md` | Full guide | Documentation |
| `tools/AUTOMATION_SUMMARY.md` | Feature breakdown | Documentation |

---

## 🎮 Workflow by Use Case

### "Quick Check"
```bash
python tools/monitor_uart.py
```
→ 1-2 minutes

### "See What's Happening"
```bash
.\run_debug.ps1 -Duration 30
```
→ 3-5 minutes (compile + upload + monitor)

### "Full Diagnostic"
```bash
.\run_debug.ps1 -Duration 60
```
→ 5-8 minutes (everything + extended monitoring)

### "Verify Fix"
```bash
.\run_debug.ps1 -NoUpload -Duration 30
```
→ 2-3 minutes (skip compile/upload, just monitor)

---

## 📈 Performance Impact

- **Compilation:** ~30 seconds (Arduino CLI)
- **Upload:** ~10 seconds (to COM7)
- **Monitoring:** User-configurable (default 30s)
- **Total:** ~2-5 minutes for full cycle

---

## 🔐 Data & Privacy

All tools:
- ✅ Run locally (no cloud)
- ✅ Log to local files only
- ✅ No data transmission
- ✅ Clear output directory structure

---

## 📞 Support & Next Steps

### If Everything Works ✅
```bash
python tools/orchestrate_debug.py
```
Check `debug_output/<timestamp>/results.json` for findings

### If Issues Found
1. Review frame details in `uart_diagnostic.jsonl`
2. Edit `V93XX_Raccoon.cpp` based on diagnostics
3. Re-run orchestrator to verify fix

### If Tool Fails
1. Run: `python tools/verify_setup.py` (check environment)
2. Check: Hardware connections (ESP32, Logic Analyzer)
3. Review: SyntaxErrors in Python output

---

## 🎓 Educational Value

This toolkit demonstrates:
- ✅ Multi-tool automation orchestration
- ✅ Real-time protocol analysis
- ✅ Hardware/software correlation
- ✅ Automated issue detection
- ✅ JSON-based data logging
- ✅ Python subprocess management
- ✅ UART protocol validation

---

## ✨ Ready to Debug!

You can now:

1. **Start here:**
   ```powershell
   .\run_debug.ps1
   ```

2. **Or verify everything first:**
   ```bash
   python tools/verify_setup.py
   ```

3. **Or deep-dive specific tool:**
   ```bash
   python tools/uart_diagnostics.py 60
   ```

---

## 📝 Commits Made

- ✅ Added all Python automation tools
- ✅ Added documentation (QUICK_START, DEBUG_GUIDE, AUTOMATION_SUMMARY)
- ✅ Added execution scripts (run_debug.ps1, run_debug.bat)
- ✅ Updated pyproject.toml with dependencies
- ✅ Pushed to feat/v9381_uart branch

**Branch:** `feat/v9381_uart`
**Pull Request:** #11 (V9381 UART Support)

---

🎉 **You're all set to debug the V9381 UART driver!**

Start with: `.\run_debug.ps1`
