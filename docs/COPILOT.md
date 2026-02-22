# GitHub Copilot Autonomous Testing Guide

Use GitHub Copilot to automatically build, test, and validate V9381 UART ChecksumMode.

## 🤖 One-Command Test

```bash
.\tools\run_automated_tests.ps1 -Port COM3
```

**Result**: Complete automated test in ~80 seconds

---

## 💬 Using with Copilot Chat

### Option 1: Direct Command
```
@copilot / Run the complete V9381 UART hardware test workflow:
.\tools\run_automated_tests.ps1 -Port COM3
Reference .github/copilot-instructions.md if any phase fails.
```

### Option 2: Multi-Step Task
```
@copilot I need to:
1. Run unit tests for V9381 UART ChecksumMode
2. Compile with Arduino CLI for ESP32-S3
3. Upload to device on COM3
4. Verify initialization via serial monitor
5. Test with Saleae Logic Analyzer (optional)
6. Analyze CRC in captured frames (optional)

Use .github/copilot-instructions.md to guide each phase.
```

### Option 3: Debugging Help
```
@copilot I'm getting CRC mismatches in my UART capture.
Use .github/copilot-instructions.md "Error Handling & Recovery" section
to diagnose and help me resolve this.
```

---

## 📋 What Copilot Does

When you run the command, Copilot will:

1. ✅ **Unit Tests** (~2s)
   - Verify CRC algorithm
   - Test Clean/Dirty mode behavior
   - Validate frame formats

2. ✅ **Build** (~10s)
   - Compile V9381_UART_DIRTY_MODE.ino
   - Generate .bin file

3. ✅ **Upload** (~15s)
   - Flash binary to ESP32-S3
   - Device restarts

4. ✅ **Serial Verify** (~10s)
   - Monitor serial output
   - Verify initialization
   - Check for errors

5. ✅ **Capture** (~30s, optional)
   - Record UART traffic
   - Save as CSV files
   - Export from Logic Analyzer

6. ✅ **Analyze** (~5s, optional)
   - Parse captured frames
   - Calculate expected CRC
   - Compare with received
   - Predict mode behavior

7. ✅ **Report** (~1s)
   - Generate JSON with results
   - Show success/failure status

---

## 🎯 Interpreting Output

### Success Output
```
✅ Unit tests passed
✅ Compilation successful
✅ Upload successful
✅ Device initialized with ChecksumMode
✅ CRC validation PASSED

OVERALL: PASSED
```

### Error Output
```
❌ Unit tests failed
ERROR: CRC Calculation test 1 failed
Expected: 0x2C, Got: 0xAA

ABORT: Check CRC algorithm in V93XX_UART.cpp
```

### What Each Phase Shows
| Phase | Success | Fail |
|-------|---------|------|
| **Unit Tests** | ✅ PASS | ❌ FAIL (abort) |
| **Build** | ✅ PASS | ❌ FAIL (abort) |
| **Upload** | ✅ PASS | ❌ FAIL (retry) |
| **Serial** | ✅ PASS | ⚠️ WARN (check device) |
| **Capture** | ✅ PASS | ⚠️ SKIP (optional) |
| **Analysis** | ✅ PASS | ⚠️ SKIP (optional) |

---

## 🔍 Copilot Instructions Reference

Complete specifications are in `.github/copilot-instructions.md`:

**Section** | **Purpose**
---|---
**Phases 1-8** | Step-by-step commands for all phases
**Error Handling & Recovery** | What to do when phases fail
**Closed-Loop Agent Workflow** | Pseudocode for agent implementation
**Validation Checklist** | Success criteria before marking PASSED

---

## 🛠️ Configuration Options

### Choose Serial Port
```bash
# Auto-detects COM3
.\tools\run_automated_tests.ps1 -Port COM3

# Different port
.\tools\run_automated_tests.ps1 -Port COM4

# List available ports first
arduino-cli board list
```

### Skip Phases for Speed
```bash
# Skip build (reuse previous build)
.\tools\run_automated_tests.ps1 -Port COM3 -SkipBuild

# Skip capture (no Logic Analyzer)
.\tools\run_automated_tests.ps1 -Port COM3 -SkipCapture

# Both (fastest - just verify existing build works)
.\tools\run_automated_tests.ps1 -Port COM3 -SkipBuild -SkipCapture

# Only generate report from existing data
.\tools\run_automated_tests.ps1 -ReportOnly
```

---

## 📊 Expected Results

| Scenario | Time | Result |
|----------|------|--------|
| All phases | 80s | ✅ PASSED with full analysis |
| No capture | 40s | ✅ PASSED (skip analyzer) |
| No build | 30s | ✅ PASSED (reuse binary) |
| Unit tests only | 2s | ✅ PASSED (no hardware) |

---

## ❓ Common Issues & Solutions

### Arduino CLI Not Found
**Symptom**: "Arduino CLI not found in PATH"

**Solution**:
```bash
# Install from https://arduino.cc/en/software
# Or via chocolatey:
choco install arduino-cli
```

### Device Not Detected
**Symptom**: "Error: Board not found" during upload

**Solution**:
```bash
# List available boards
arduino-cli board list

# Use detected port - usually COM3 or COM4
.\tools\run_automated_tests.ps1 -Port COM4
```

### Serial Not Responding
**Symptom**: No output during Phase 4 serial monitor

**Solution**:
1. Check USB cable
2. Press RST button on ESP32
3. Try different USB port on computer
4. Verify baud rate is 115200 in sketch

### CRC Mismatches
**Symptom**: "CRC Mismatch! Expected 0x2B, Received 0xAA"

**Solution**:
- **Dirty mode**: Normal - shows CRC discrepancy for debugging
- **Check**: UART wiring (GPIO1→RX, GPIO2←TX)
- **Use**: Logic Analyzer to capture actual frames

---

## 🎓 Copilot Chat Examples

### Example 1: Quick Test
```
You: Test the V9381 UART code on COM3

Copilot: I'll run the complete test suite.
[Runs: .\tools\run_automated_tests.ps1 -Port COM3 -SkipCapture]

✅ OVERALL: PASSED
- All unit tests pass
- Build successful
- Upload complete
- Serial verification: Device initialized in Dirty mode
```

### Example 2: Debugging CRC Issue
```
You: Why am I getting CRC mismatches in the captured data?

Copilot: Looking at the analysis results...
Frame #8: Expected 0x2B but received 0xAA

This could indicate:
1. Signal noise on UART lines
2. Baud rate synchronization issue
3. Device returning wrong data

Recommendations from .github/copilot-instructions.md Error Handling:
- Check UART wiring
- Verify baud rate (should be 19200)
- Try different GPIO pins
```

### Example 3: Custom Port
```
You: Test on COM4 instead of COM3

Copilot: I'll run the tests on COM4.
[Runs: .\tools\run_automated_tests.ps1 -Port COM4]

✅ PASSED
```

---

## 📖 Full Documentation

For complete specifications, see [.github/copilot-instructions.md](../.github/copilot-instructions.md).

For testing details, see [TESTING.md](TESTING.md).

---

✅ Ready? Run: `.\tools\run_automated_tests.ps1 -Port COM3`
