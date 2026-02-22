# Integration Guide

Using V93XX ChecksumMode in your Arduino project.

## 🚀 Quick Start

### 1. Include Library
```cpp
#include "V93XX_UART.h"

V93XX_UART v9381;
```

### 2. Initialize
```cpp
void setup() {
  Serial.begin(115200);
  
  // Uses default Dirty mode for debugging
  v9381.Init();
  
  // Or be explicit:
  // v9381.Init(SerialConfig::SERIAL_8O1, V93XX_UART::ChecksumMode::Dirty);
}
```

### 3. Use It
```cpp
void loop() {
  // Operations work the same way in both modes
  uint32_t value = v9381.RegisterRead(0x00);
  
  Serial.print("Register value: 0x");
  Serial.println(value, HEX);
  
  delay(1000);
}
```

---

## 🎯 Mode Selection

### Choose Dirty Mode When
- Debugging hardware issues
- Investigating CRC mismatches
- First-time setup
- Learning the protocol

```cpp
v9381.Init(SerialConfig::SERIAL_8O1, V93XX_UART::ChecksumMode::Dirty);
```

### Choose Clean Mode When
- Production deployment
- CRC validation critical
- Preventing corrupted data
- Full integration testing

```cpp
v9381.SetChecksumMode(V93XX_UART::ChecksumMode::Clean);
```

---

## 📋 Common Patterns

### Pattern 1: Debug Then Deploy
```cpp
void setup() {
  Serial.begin(115200);
  
  // Start with Dirty for initialization
  v9381.Init();
  
  // Perform initialization/checks
  delay(1000);
  
  // Switch to Clean for production
  v9381.SetChecksumMode(V93XX_UART::ChecksumMode::Clean);
  Serial.println("Ready for production");
}
```

### Pattern 2: Error Handling
```cpp
// Works the same in both modes
// Difference is: Clean aborts, Dirty continues

uint32_t value = v9381.RegisterRead(0x00);

if (value == 0 && /* some error condition */) {
  // Could be CRC error (in Clean mode) or actual 0 value
  Serial.println("Check CRC or value");
}
```

### Pattern 3: Mode Verification
```cpp
void loop() {
  v9381.SetChecksumMode(V93XX_UART::ChecksumMode::Dirty);
  
  uint32_t dirty_value = v9381.RegisterRead(0x00);
  // Serial: "CRC Valid/Mismatch" message
  
  delay(100);
  
  v9381.SetChecksumMode(V93XX_UART::ChecksumMode::Clean);
  
  uint32_t clean_value = v9381.RegisterRead(0x00);
  // Serial: "ERROR" or success, value returned
}
```

---

## 🔗 Hardware Setup (ESP32-S3 DevKitC Defaults)

These are the pin defaults used by the examples. You can remap pins in your sketch if needed.

### V9360 (UART)

```mermaid
flowchart LR
  subgraph ESP32[ESP32-S3 DevKitC]
    TX16[GPIO16 (TX)]
    RX15[GPIO15 (RX)]
    V3[3V3]
    GND1[GND]
  end
  subgraph V9360[V9360 (UART)]
    RX[UART RX]
    TX[UART TX]
    VDD[VDD (3.3V)]
    GND2[GND]
  end
  TX16 --> RX
  RX15 --> TX
  V3 --> VDD
  GND1 --> GND2
```

### V9381 (UART)

```mermaid
flowchart LR
  subgraph ESP32[ESP32-S3 DevKitC]
    TX11[GPIO11 (MOSI/TX)]
    RX13[GPIO13 (MISO/RX)]
    A0[GPIO12 (SCK/A0)]
    A1[GPIO10 (CS/A1)]
    V3[3V3]
    GND1[GND]
  end
  subgraph V9381[V9381 (UART)]
    RX[UART RX / MOSI]
    TX[UART TX / MISO]
    A0P[A0 / SCK]
    A1P[A1 / CS]
    VDD[VDD (3.3V)]
    GND2[GND]
  end
  TX11 --> RX
  RX13 --> TX
  A0 --> A0P
  A1 --> A1P
  V3 --> VDD
  GND1 --> GND2
```

### V9381 (SPI)

```mermaid
flowchart LR
  subgraph ESP32[ESP32-S3 DevKitC]
    MOSI[GPIO11 (MOSI)]
    MISO[GPIO13 (MISO)]
    SCK[GPIO12 (SCK)]
    CS[GPIO10 (CS)]
    V3[3V3]
    GND1[GND]
  end
  subgraph V9381[V9381 (SPI)]
    MOSI2[MOSI]
    MISO2[MISO]
    SCK2[SCK]
    CS2[CS]
    VDD[VDD (3.3V)]
    GND2[GND]
  end
  MOSI --> MOSI2
  MISO --> MISO2
  SCK --> SCK2
  CS --> CS2
  V3 --> VDD
  GND1 --> GND2
```

### Baud Rate
- UART: 19200 baud
- Serial Monitor: 115200 baud (for Serial.println output)

---

## 🧪 Testing Your Integration

### 1. Verify with Unit Tests
```bash
python tools/test_checksum_mode.py
# Should pass all 6 categories
```

### 2. Test on Hardware
```bash
# Upload your sketch
# Open Serial Monitor (115200 baud)
# You should see CRC messages
```

### 3. Full Validation
```bash
.\tools\run_automated_tests.ps1 -Port COM3
# Builds, uploads, monitors, captures, analyzes
```

---

## 🛠️ Advanced Usage

### Reading Multiple Registers
```cpp
uint32_t regs[4];

regs[0] = v9381.RegisterRead(0x00);  // CRC check
regs[1] = v9381.RegisterRead(0x01);  // CRC check
regs[2] = v9381.RegisterRead(0x02);  // CRC check
regs[3] = v9381.RegisterRead(0x03);  // CRC check

// In Dirty mode: all succeed, CRC shown
// In Clean mode: all succeed if CRC valid, halt if not
```

### Writing with Verification
```cpp
// Write a register
v9381.RegisterWrite(0x10, 0x12345678);

// Wait a bit
delay(100);

// Read it back
uint32_t read_back = v9381.RegisterRead(0x10);

if (read_back == 0x12345678) {
  Serial.println("Write verified");
} else {
  Serial.println("Write failed");
}
```

### Block Operations
```cpp
uint8_t buffer[16];

// Read block of registers
v9381.RegisterBlockRead(0x20, 16, buffer);

// Process data
for (int i = 0; i < 16; i++) {
  Serial.print(buffer[i], HEX);
  Serial.print(" ");
}
```

---

## 🐛 Troubleshooting

| Issue | Solution |
|-------|----------|
| No CRC messages | Check Serial Monitor baud rate (115200) |
| "CRC Mismatch" every time | Check GPIO wiring (GPIO1=TX, GPIO2=RX) |
| Device not responding | Check V9381 power and baud rate (19200) |
| Clean mode always errors | Verify wiring and baud rate settings |
| Value always 0 | Could be CRC error (Clean) or device not responding |

---

## 📊 What You Should See

### In Dirty Mode (Debug)
```
Initializing V9381 UART...
Checksum Mode set to Dirty (skip CRC validation)
CRC Valid: Expected 0x2B, Received 0x2B
Register value: 0x12345678
CRC Valid: Expected 0x31, Received 0x31
Register value: 0x87654321
```

### In Clean Mode (Production)
```
Checksum Mode set to Clean (enforce CRC validation)
CRC Valid: Expected 0x2B, Received 0x2B
Register value: 0x12345678
CRC Valid: Expected 0x31, Received 0x31
Register value: 0x87654321
```

---

## ✅ Production Checklist

Before deploying:
- [ ] Unit tests pass: `python tools/test_checksum_mode.py`
- [ ] Hardware test passes: `.\tools\run_automated_tests.ps1 -Port COM3`
- [ ] In Dirty mode first: Verify CRC messages
- [ ] Serial output looks good: No CRC mismatches
- [ ] Switch to Clean mode: `SetChecksumMode(ChecksumMode::Clean);`
- [ ] Verify operations still succeed in Clean mode
- [ ] Monitor for errors in production

---

## 📚 Related Documentation

- [GETTING_STARTED.md](GETTING_STARTED.md) - New to the library
- [UART_CHECKSUM_MODE.md](UART_CHECKSUM_MODE.md) - ChecksumMode details
- [TESTING.md](TESTING.md) - Testing workflows
- [API_REFERENCE.md](API_REFERENCE.md) - Complete API

---

Ready to integrate? Start with the [Quick Start](#-quick-start) section above!
