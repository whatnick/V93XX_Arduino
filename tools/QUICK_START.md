# Quick Reference: V9381 UART Debugging

## One Command to Debug Everything

```bash
& .\.venv\Scripts\Activate.ps1
python tools/orchestrate_debug.py
```

This single command will:
1. ✅ Compile the V9381_UART sketch
2. ✅ Upload to ESP32-S3 on COM7
3. ✅ Monitor serial output and validate UART frames
4. ✅ Trigger Saleae Logic 2 capture (if running)
5. ✅ Correlate findings and identify issues
6. ✅ Save comprehensive debug report

**Output:** `debug_output/<timestamp>/results.json` + logs

---

## Tools at Your Fingertips

### When you need...

**Real-time frame monitoring:**
```bash
python tools/monitor_uart.py
```

**Detailed frame validation:**
```bash
python tools/uart_diagnostics.py 60
```

**Saleae automation (with Logic 2):**
```bash
python tools/logic2_automation.py --duration 5s --export capture.csv
```

**Full orchestrated debug:**
```bash
python tools/orchestrate_debug.py --duration 30
```

**Verify setup:**
```bash
python tools/verify_setup.py
```

---

## Common Issues & Fixes

### "V9381 not responding"
1. Check A0/A1 pins are tied to GND
2. Run: `python tools/monitor_uart.py`
3. Watch for RX activity on CH1 in logic analyzer

### "Checksum mismatches"
1. Run `orchestrate_debug.py`
2. Check `debug_output/<timestamp>/uart_diagnostic.jsonl`
3. Compare `checksum_expected` vs `checksum_calculated`
4. Review V93XX_Raccoon.cpp RegisterRead() logic

### "No frames captured"
1. Verify UART config: 8 bits, odd parity, 1 stop bit, 19200 baud
2. Check Serial1 pins: 11 (RX), 12 (TX) on ESP32-S3
3. Restart ESP32 and re-run diagnostics

---

## Files Created

All tools are in `tools/` directory:

| File | Purpose |
|------|---------|
| `orchestrate_debug.py` | Master controller (run this first) |
| `automated_uart_debug.py` | Arduino CLI + pySerial automation |
| `uart_diagnostics.py` | Serial frame capture & validation |
| `monitor_uart.py` | Simple byte monitor |
| `logic2_automation.py` | Saleae Logic 2 control |
| `verify_setup.py` | Environment checker |
| `DEBUG_GUIDE.md` | Comprehensive documentation |

---

## Next Steps

1. **Initialize:** `python tools/verify_setup.py` (check output)
2. **Debug:** `python tools/orchestrate_debug.py`
3. **Analyze:** Open `debug_output/<timestamp>/results.json`
4. **Fix:** Edit V93XX_Raccoon.cpp based on findings
5. **Verify:** Re-run `orchestrate_debug.py` to confirm fix

---

## Status

✅ **Setup Complete!**
- Arduino CLI connected to ESP32-S3
- COM7 available for debug output
- Logic 2 automation ready
- All Python tools installed

Ready to debug → Run: `python tools/orchestrate_debug.py`
