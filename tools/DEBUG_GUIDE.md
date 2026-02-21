# V9381 UART Debugging Toolkit

Unified automation framework for comprehensive V9381 UART debugging on ESP32-S3, combining **Arduino CLI**, **Saleae Logic 2**, and **pySerial** into a single orchestrated workflow.

## Quick Start

```bash
# Full debug sequence (compile, upload, monitor, capture, analyze)
python tools/orchestrate_debug.py

# Extended monitoring (60 seconds)
python tools/orchestrate_debug.py --duration 60

# Skip upload if sketch already running
python tools/orchestrate_debug.py --no-upload

# Custom sketch
python tools/orchestrate_debug.py --sketch examples/V9360_UART/V9360_UART.ino
```

## Tool Stack

### 1. **Arduino CLI** — Compilation & Upload
- Compiles V9381_UART.ino sketch
- Uploads binary to ESP32-S3 on COM7
- Validates board and programmer setup

### 2. **pySerial** — Serial Diagnostics
- Monitors ESP32 debug output (19200 baud)
- Captures UART frame transactions
- Validates checksums in real-time
- Reports frame decode errors and timeouts
- Logs raw bytes + decoded frames to JSONL

### 3. **Saleae Logic 2** — Protocol Capture
- Records bus signals (CH0/1/2/7) at 24MHz
- Decodes UART with 8O1 (odd parity) protocol
- Exports detailed frame data to CSV
- Enables side-by-side comparison with pySerial

### 4. **Correlator** — Cross-Reference Analysis
- Compares Saleae decode vs. pySerial capture
- Identifies frame loss or corruption
- Pinpoints checksum mismatches
- Generates actionable debugging recommendations

## Individual Tools

### Orchestrator (Recommended)
**Master script that automates everything:**

```bash
python tools/orchestrate_debug.py [options]
```

Workflow:
1. Compile sketch (arduino-cli)
2. Upload to ESP32 (arduino-cli)
3. Monitor serial output (pySerial diagnostics)
4. Trigger Saleae capture (parallel)
5. Correlate and analyze results
6. Save comprehensive report

Output: `debug_output/<timestamp>/` directory with all logs and analysis

---

### Automated Debug (pySerial + Arduino CLI)
**Detailed serial diagnostics without Logic 2:**

```bash
python tools/automated_uart_debug.py [sketch] [duration] [--no-upload]
```

Example:
```bash
python tools/automated_uart_debug.py examples/V9381_UART/V9381_UART.ino 30
```

Features:
- Real-time frame decode with checksum validation
- Checksum pass/fail reporting
- Serial output correlation
- JSON log output for analysis

Output: `uart_debug.jsonl`

---

### UART Diagnostics (pySerial Only)
**Serial monitoring with frame validation:**

```bash
python tools/uart_diagnostics.py [duration_seconds]
```

Example:
```bash
python tools/uart_diagnostics.py 60  # Monitor for 60 seconds
```

Features:
- Real-time hex + ASCII dump
- Frame extraction and checksum validation
- Supports 5-byte register read/write frames
- JSON log with detailed decode

Output: `uart_monitor.jsonl`

---

### Basic UART Monitor (Simple pySerial)
**Lightweight byte capture:**

```bash
python tools/monitor_uart.py
```

Features:
- Raw byte capture
- Simple frame detection
- Minimal logging

---

### Logic 2 Automation
**Saleae capture control and export:**

```bash
python tools/logic2_automation.py [options]
```

Options:
```
--duration 5s              Capture duration (default: 5s)
--channels 0,1,2,7        Channels to capture (default: A1/RX/A0/TX)
--sample-rate 24MHz        Sample rate (default: 24MHz)
--export capture.csv       Export decoded data to CSV
--correlate uart_debug.jsonl  Cross-reference with pySerial log
```

Example:
```bash
# Full capture with export and correlation
python tools/logic2_automation.py \
  --duration 10s \
  --export capture.csv \
  --correlate uart_debug.jsonl
```

Prerequisites:
- Saleae Logic 2 running and open
- Automation enabled (Settings → Developer → Enable Automation Socket)

---

## Setup & Prerequisites

### Install Dependencies

```bash
# Using uv (already configured)
uv sync

# Or manually
python -m pip install pyserial logic2-automation
```

### Verify Tools

```bash
# Arduino CLI
arduino-cli --version

# python/pip
python --version
pyserial-list  # Verify COM ports

# Saleae Logic 2
# Open Logic 2 application and enable automation:
# Settings → Developer → Enable Automation Socket
```

### Hardware Setup

- **ESP32-S3**: Connected to COM7 at 115200 baud (for debug output)
- **V9381 ASIC**: UART on ESP32 pins 11 (RX) / 12 (TX) at 19200 baud (8O1)
- **Logic Analyzer**: Saleae device connected
  - Channel 0: A1 address pin
  - Channel 1: RX/MISO (V9381 response)
  - Channel 2: A0 address pin
  - Channel 7: TX/MOSI (command to V9381)

---

## Troubleshooting

### "Error: Could not connect to Logic 2"
→ Ensure Logic 2 is running and automation is enabled (Settings → Developer)

### "arduino-cli not found"
→ Ensure Arduino CLI is in PATH or installed: https://arduino.github.io/arduino-cli/latest/

### "Port COM7 not found"
→ Check Arduino IDE Tools → Port or use `pyserial-list`

### No frames captured
→ V9381 may not be responding
   - Check A0/A1 address pins are tied to GND
   - Verify UART configuration (8O1 = 8 bits, Odd parity, 1 stop bit)
   - Ensure 19200 baud is set

### Frames with checksum errors
→ Driver validation failing
   - Compare checksum_expected vs checksum_calculated in uart_debug.jsonl
   - Review V93XX_Raccoon.cpp RegisterRead() logic
   - Check byte order or frame format assumptions

---

## Output Files

### Orchestrator Output (`debug_output/<timestamp>/`)
```
results.json              Full execution report
uart_diagnostic.jsonl     pySerial frame capture + validation
capture.csv              Saleae Logic 2 export (if available)
```

### Log Format Examples

**uart_diagnostic.jsonl:**
```json
{"time": 0.123, "type": "text", "content": "Initializing V9381..."}
{"time": 1.456, "type": "frame", "content": "7D 00 40 00 3D", "decoded": {"marker": "0x7D", "addr": "0x00", "cmd": "0x40", "value": "0x003D", "checksum_expected": "0x3D", "checksum_calculated": "0x3D", "match": true}}
```

**capture.csv** (Saleae):
```
Time [s],Channel,Value
0.000000,0,0
0.000042,7,1
0.000084,7,1
...
```

---

## Workflow Examples

### Example 1: Quick Verification
Check if ASIC is responding:

```bash
python tools/monitor_uart.py
```
Then manually trigger RegisterRead() on the ESP32 and watch for response frames.

### Example 2: Full Debug with Logic Analyzer
Comprehensive debugging with all three tools:

```bash
python tools/orchestrate_debug.py --duration 30
```

Outputs:
- Compiled binary
- Frame captures with checksums
- Saleae recording (if Logic 2 available)
- Correlation analysis
- Recommendations

### Example 3: Driver Validation Testing
Isolate checksum mismatches:

```bash
python tools/automated_uart_debug.py examples/V9381_UART/V9381_UART.ino 30
```

Look for `✗` markers in output indicating checksum failures. Parse `uart_debug.jsonl` to see expected vs. calculated checksums.

### Example 4: Timing Analysis
Compare Saleae decode vs. driver receive timing:

```bash
python tools/logic2_automation.py \
  --duration 5s \
  --export capture.csv \
  --correlate uart_debug.jsonl
```

Generates discrepancy report showing:
- Frames captured by Saleae vs. pySerial
- Dropped bytes or timeout issues
- Checksum validation differences

---

## Debugging Guide

### Issue: V9381 Not Responding
**Symptoms:** Saleae shows TX on CH7 but no RX on CH1

**Diagnosis:**
1. Verify UART wiring (pins 11/12 on ESP32)
2. Check A0/A1 address pins are tied to GND (address 0x00)
3. Run minimal RegisterRead() loop and capture Logic 2 screenshots
4. Ensure 19200 baud rate

**Tools:**
```bash
python tools/monitor_uart.py  # Check for any RX data
```

### Issue: Checksum Mismatches
**Symptoms:** Saleae shows valid UART frames but `uart_debug.jsonl` shows checksum failures

**Diagnosis:**
1. Compare `checksum_expected` vs. `checksum_calculated` in logs
2. Review V93XX_Raccoon.cpp `calculate_checksum()` function
3. Verify byte order (most-significant byte first)
4. Check parity bit handling (odd vs. even)

**Tools:**
```bash
python tools/automated_uart_debug.py --no-upload  # Re-run diagnostics
# Check first few frames in uart_debug.jsonl
```

### Issue: Frame Loss or Corruption
**Symptoms:** Fewer frames in pySerial than in Saleae, or corrupted bytes

**Diagnosis:**
1. Run orchestrator with correlation:
   ```bash
   python tools/orchestrate_debug.py --duration 60
   ```
2. Check `results.json` for frame count discrepancies
3. Review RX buffering in V93XX_Raccoon.cpp
4. Verify WaitForRx() timeout is sufficient

**Tools:**
```bash
python tools/logic2_automation.py --correlate uart_debug.jsonl
```

---

## Next Steps

1. **Run orchestrator** to identify the root cause
2. **Review logs** in `debug_output/<timestamp>/`
3. **Implement fix** in V93XX_Raccoon.cpp based on findings
4. **Re-run** orchestrator to verify fix
5. **Commit changes** and push to feat/v9381_uart

---

For questions or issues, refer to UART frame captures and logs in the output directory.
