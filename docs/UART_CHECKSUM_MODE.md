# V9381 UART ChecksumMode Implementation

Detailed guide to the ChecksumMode feature for V9381 UART communication.

## 🎯 Overview

ChecksumMode provides two operational modes for UART CRC validation:

- **Dirty Mode** (default): Skip CRC validation, continue operation, show CRC values for debugging
- **Clean Mode** (production): Enforce strict CRC validation, abort on mismatch

Both modes are aligned with the V93XX_SPI implementation for consistency.

---

## 🔧 API Reference

### Initialization

```cpp
// Default: Dirty mode
v9381.Init();

// Explicit Dirty mode
v9381.Init(SerialConfig::SERIAL_8O1, V93XX_Raccoon::ChecksumMode::Dirty);

// Explicit Clean mode
v9381.Init(SerialConfig::SERIAL_8O1, V93XX_Raccoon::ChecksumMode::Clean);
```

### Runtime Mode Switching

```cpp
// Switch to Clean mode
v9381.SetChecksumMode(V93XX_Raccoon::ChecksumMode::Clean);

// Switch back to Dirty
v9381.SetChecksumMode(V93XX_Raccoon::ChecksumMode::Dirty);
```

### Register Operations (Unchanged)

```cpp
// These work identically in both modes
uint32_t value = v9381.RegisterRead(0x00);
v9381.RegisterWrite(0x10, 0x12345678);
v9381.RegisterBlockRead(0x20, 16, buffer);
```

---

## 🖥️ Serial Output

### In Dirty Mode

```
CRC Valid: Expected 0x2B, Received 0x2B
CRC Valid: Expected 0x31, Received 0x31
CRC Mismatch! Expected 0x2C, Received 0xFF
```

→ Operation continues regardless of CRC status

### In Clean Mode

```
CRC Valid: Expected 0x2B, Received 0x2B
CRC Valid: Expected 0x31, Received 0x31
ERROR: CRC mismatch! Expected 0x2C, Received 0xFF
```

→ Operation aborts on CRC error

---

## 💡 Usage Guide

### For Debugging (Dirty Mode)

When you suspect UART issues, use Dirty mode to see CRC values:

```cpp
void setup() {
  Serial.begin(115200);
  v9381.Init();  // Default Dirty mode
  
  Serial.println("Dirty mode: CRC shown for debugging");
}

void loop() {
  uint32_t value = v9381.RegisterRead(0x00);
  // Serial shows: "CRC Mismatch! Expected 0x31, Received 0x15"
  // But operation STILL COMPLETED
  
  delay(1000);
}
```

**Serial Monitor Output**:
```
Initializing V9381 UART...
Checksum Mode set to Dirty (skip CRC validation)
CRC Valid: Expected 0x2B, Received 0x2B
CRC Mismatch! Expected 0x31, Received 0x15
Register value: 0x12345678
```

### For Production (Clean Mode)

When deploying to production, switch to Clean mode:

```cpp
void setup() {
  Serial.begin(115200);
  v9381.Init();  // Start with Dirty for initialization
  
  delay(1000);
  
  // Switch to Clean mode
  v9381.SetChecksumMode(V93XX_Raccoon::ChecksumMode::Clean);
  Serial.println("Switched to Clean mode - strict validation");
}

void loop() {
  uint32_t value = v9381.RegisterRead(0x00);
  
  // If CRC valid: operation succeeds
  // If CRC invalid: operation fails, returns 0
  
  delay(1000);
}
```

---

## 📊 Mode Comparison

| Aspect | Dirty | Clean |
|--------|-------|-------|
| **CRC Validation** | Skipped | Enforced |
| **Invalid CRC** | Shows warning, continues | Shows error, aborts |
| **Use Case** | Debugging hardware issues | Production deployment |
| **Default** | ✅ Yes | No |
| **Serial Output** | "CRC Mismatch! Expected X, Received Y" | "ERROR: CRC mismatch!" |

---

## 🔍 CRC Algorithm

### Formula
```
1. Calculate sum of all payload bytes
2. Take bitwise NOT (~) of sum
3. Add 0x33
4. Result is the CRC byte
```

### C Implementation
```cpp
uint8_t calculate_crc8_v9381(const uint8_t* data, size_t len) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < len; i++) {
        checksum += data[i];
    }
    checksum &= 0xFF;
    uint8_t crc = (0x33 + (~checksum & 0xFF)) & 0xFF;
    return crc;
}
```

### Python Reference (for testing)
```python
def calculate_crc8_v9381(payload_bytes: list[int]) -> int:
    checksum = sum(payload_bytes) & 0xFF
    crc = (0x33 + (~checksum & 0xFF)) & 0xFF
    return crc
```

### Test Cases
```
Payload        CRC    Calculation
[0x04, 0x02]   0x2C   sum=0x06, ~=0xF9, +0x33=0x2C
[0x01, 0x00]   0x31   sum=0x01, ~=0xFE, +0x33=0x31
[0xFF]*4       0x36   sum=0xFC, ~=0x03, +0x33=0x36
[0x00]*4       0x32   sum=0x00, ~=0xFF, +0x33=0x32
```

---

## 🧪 Frame Formats

### Request Frame (4 bytes)
```
[Marker] [CMD1] [CMD2] [CRC]
[0x7D]   [     ] [     ] [   ]

CRC covers: CMD1, CMD2
```

Example: Read from address 0x00
```
0x7D 0x01 0x00 0x2B
  ↑   ↑↑   ↑↑   ↑↑
  |   ||   ||   |└─ CRC = 0x2B
  |   ||   ||   
  |   ||   |└─ Address = 0x00
  |   ||   
  |   |└─ Operation = READ (0x01)
  |   
  |   └─ CMD1 (includes read bit)
  └── Frame header
```

### Response Frame (6 bytes)
```
[Marker] [Data0] [Data1] [Data2] [Data3] [CRC]
[0x7D]   [     ] [     ] [     ] [     ] [   ]

CRC covers: CMD1, CMD2 (from request), Data0-3
```

Example: Response with value 0x12345678
```
0x7D 0x78 0x56 0x34 0x12 0x2C
  ↑   ↑↑   ↑↑   ↑↑   ↑↑   ↑↑
  |   └─────────────────────┴─ Data (little-endian)
  |
  └─ Frame header
```

---

## 🐛 Debugging Workflow

### Problem: CRC Mismatches Detected

**Dirty Mode Output**:
```
CRC Mismatch! Expected 0x2B, Received 0xAA
CRC Mismatch! Expected 0x31, Received 0x15
CRC Mismatch! Expected 0x2C, Received 0xFF
```

**Diagnosis Steps**:

1. **Check Wiring**
   ```
   ESP32-S3          V9381
   GPIO1 (TX) ---> UART RX
   GPIO2 (RX) <--- UART TX
   GND        ---> GND
   ```

2. **Verify Baud Rate** (should be 19200)
   ```cpp
   // In example sketch, verify:
   v9381.Init(SerialConfig::SERIAL_8O1);  // 19200 baud default
   ```

3. **Check V9381 Power**
   - Verify V9381 is powered
   - LED should indicate power
   - Measure voltage on VCC pin

4. **Capture with Logic Analyzer**
   ```bash
   python tools/capture_v9381_uart.py
   python tools/analyze_checksum_captures.py
   ```
   Compare expected vs received byte patterns

5. **Check Signal Integrity**
   - Look for noise on scope
   - Verify cable length not excessive
   - Try different GPIO pins
   - Try shorter cable

### Problem: Clean Mode Shows ERROR

**Expected Behavior** (correct):
```
ERROR: CRC mismatch! Expected 0x2B, Received 0xAA
[Operation aborts, returns 0]
```

**Verify**:
```cpp
v9381.SetChecksumMode(V93XX_Raccoon::ChecksumMode::Clean);

uint32_t value = v9381.RegisterRead(0x00);
// If CRC invalid above:
// - ERROR printed to Serial
// - value = 0
// - No register update
```

**This is correct design** - preventing corrupted data from being used.

---

## ✅ Implementation Status

- ✅ ChecksumMode enum (`Dirty` = 0, `Clean` = 1)
- ✅ Default initialization (Dirty mode)
- ✅ Runtime mode switching
- ✅ CRC-8 validation in all three register methods
- ✅ Serial output showing CRC values
- ✅ Mode-specific error handling
- ✅ Alignment with V93XX_SPI implementation
- ✅ Comprehensive testing (6 test categories)
- ✅ Hardware capture support
- ✅ Frame-by-frame analysis tools

---

## 📦 Files

| File | Purpose |
|------|---------|
| `V93XX_Raccoon.h` | ChecksumMode enum & API declarations |
| `V93XX_Raccoon.cpp` | Mode-aware CRC implementation |
| `examples/V9381_UART_DIRTY_MODE/` | Working example demonstrating both modes |
| `tools/test_checksum_mode.py` | Unit tests validating algorithm |
| `tools/analyze_checksum_captures.py` | Frame analyzer with CRC validation |

---

## 🔗 Related Documentation

- [GETTING_STARTED.md](GETTING_STARTED.md) - Quick start
- [TESTING.md](TESTING.md) - Testing workflows
- [INTEGRATION.md](INTEGRATION.md) - Project integration
- [ARCHITECTURE.md](ARCHITECTURE.md) - Design patterns
- Protocol details: See `examples/V9381_UART/DATASHEET_UART.md`

---

**Status**: ✅ Complete & Production Ready
