# V93XX Arduino Library Documentation

Complete documentation for the V93XX UART and SPI communication libraries.

## 📖 Documentation Index

### Getting Started (Start Here)
- **[GETTING_STARTED.md](GETTING_STARTED.md)** - Quick start guide for new users (5 min read)

### Testing & Automation
- **[TESTING.md](TESTING.md)** - Complete testing framework and workflows
- **[COPILOT.md](COPILOT.md)** - Using GitHub Copilot for autonomous testing

### Implementation Guides
- **[UART_CHECKSUM_MODE.md](UART_CHECKSUM_MODE.md)** - V9381 UART ChecksumMode (Clean/Dirty)
- **[INTEGRATION.md](INTEGRATION.md)** - Integrating libraries into your project
- **[API_REFERENCE.md](API_REFERENCE.md)** - Complete API documentation

### Architecture & Design
- **[ARCHITECTURE.md](ARCHITECTURE.md)** - Design patterns and alignment between UART/SPI
- **[STATUS.md](STATUS.md)** - Implementation status and feature checklist

---

## 🎯 By Use Case

**"I want to test the code"**
→ See [TESTING.md](TESTING.md)

**"I want to use Copilot for automated testing"**
→ See [COPILOT.md](COPILOT.md)

**"I want to use ChecksumMode for debugging"**
→ See [UART_CHECKSUM_MODE.md](UART_CHECKSUM_MODE.md)

**"I want to integrate into my project"**
→ See [INTEGRATION.md](INTEGRATION.md)

**"I need API documentation"**
→ See [API_REFERENCE.md](API_REFERENCE.md)

**"I'm new to this library"**
→ See [GETTING_STARTED.md](GETTING_STARTED.md)

---

## 📋 Directory Structure

```
docs/
├── README.md                           (this file)
├── GETTING_STARTED.md                  Quick start guide
├── TESTING.md                          Testing framework & workflows
├── COPILOT.md                          Copilot autonomous testing
├── UART_CHECKSUM_MODE.md               ChecksumMode implementation
├── INTEGRATION.md                      Project integration guide
├── API_REFERENCE.md                    API documentation
├── ARCHITECTURE.md                     Design patterns & alignment
└── STATUS.md                           Implementation status

examples/
├── V9381_UART_DIRTY_MODE/
│   ├── V9381_UART_DIRTY_MODE.ino       (code-specific README in folder)
│   └── README.md
├── V9381_UART/
│   ├── DATASHEET_UART.md               (code-specific UART docs)
│   └── ...
└── V9381_SPI/
    ├── DATASHEET_SPI.md                (code-specific SPI docs)
    └── ...
```

---

## ⚡ Quick Links

### For Implementation Details
- UART communication flow: See `examples/V9381_UART/DATASHEET_UART.md`
- SPI communication flow: See `examples/V9381_SPI/DATASHEET_SPI.md`
- CRC algorithm details: See [UART_CHECKSUM_MODE.md](UART_CHECKSUM_MODE.md#crc-reference)

### For Tools & Automation
- Unit tests: `tools/test_checksum_mode.py`
- Serial monitor: `tools/monitor_serial.py`
- UART capture: `tools/capture_v9381_uart.py`
- Frame analyzer: `tools/analyze_checksum_captures.py`
- Automated testing: `tools/run_automated_tests.ps1`

### For Code
- UART driver: `V93XX_UART.h` / `V93XX_UART.cpp`
- SPI driver: `V93XX_SPI.h` / `V93XX_SPI.cpp`
- Register definitions & UART timing macros: `V93XX_Registers.h` (protocol-independent)

---

## 🚀 Quick Start (2 Minutes)

### 1. Install
```bash
# Copy library to Arduino libraries folder
cp -r V93XX_Arduino ~/Arduino/libraries/
```

### 2. Use
```cpp
#include "V93XX_UART.h"

V93XX_UART v9381;

void setup() {
  Serial.begin(115200);
  v9381.Init();  // Use default Dirty mode for debugging
}

void loop() {
  uint32_t value = v9381.RegisterRead(0x00);
  Serial.println(value, HEX);
  delay(1000);
}
```

### 3. Test
```bash
# Quick hardware test
.\tools\run_automated_tests.ps1 -Port COM3 -SkipCapture
```

For more details, see [GETTING_STARTED.md](GETTING_STARTED.md).

---

## 📊 Feature Overview

| Feature | UART (V9381) | SPI (V9381) | Status |
|---------|------------|-----------|--------|
| Register Read | ✅ | ✅ | Complete |
| Register Write | ✅ | ✅ | Complete |
| Block Read | ✅ | ✅ | Complete |
| **ChecksumMode** | ✅ | ✅ | Complete |
| Dirty Mode (skip CRC) | ✅ | ✅ | Complete |
| Clean Mode (enforce CRC) | ✅ | ✅ | Complete |
| Automated Testing | ✅ | ⚠️ | UART only |
| Logic Analyzer Capture | ✅ | ✅ | Complete |
| Hardware Examples | ✅ | ✅ | Complete |

---

## 🔗 External Resources

- **V93XX ASIC Datasheet**: [Vangotech V93XX](https://www.vangotech.com/en/uploadpic/169329776853.pdf)
- **V9381 Datasheet**: [Vangotech V9381](https://www.vangotech.com/en/uploadpic/175272285131.pdf)
- **Arduino IDE**: [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
- **Arduino CLI**: [https://arduino.cc/en/software](https://arduino.cc/en/software)
- **Saleae Logic Analyzer**: [https://www.saleae.com/](https://www.saleae.com/)

---

## ✅ Implementation Checklist

- ✅ UART driver with ChecksumMode (Dirty/Clean)
- ✅ SPI driver with ChecksumMode (Dirty/Clean)
- ✅ CRC-8 algorithm (V9381 standard)
- ✅ Unit tests (6 categories, all passing)
- ✅ Serial monitoring tool
- ✅ Hardware capture tooling
- ✅ Frame analysis tool
- ✅ Automated test framework
- ✅ Complete documentation

---

## 🆘 Support

| Question | See |
|----------|-----|
| How do I get started? | [GETTING_STARTED.md](GETTING_STARTED.md) |
| How do I test the code? | [TESTING.md](TESTING.md) |
| How do I use Copilot? | [COPILOT.md](COPILOT.md) |
| What is ChecksumMode? | [UART_CHECKSUM_MODE.md](UART_CHECKSUM_MODE.md) |
| How do I integrate this? | [INTEGRATION.md](INTEGRATION.md) |
| What's the API? | [API_REFERENCE.md](API_REFERENCE.md) |
| How is it designed? | [ARCHITECTURE.md](ARCHITECTURE.md) |
| What's the status? | [STATUS.md](STATUS.md) |

---

**Last Updated**: February 22, 2026  
**Status**: ✅ Complete & Production Ready
