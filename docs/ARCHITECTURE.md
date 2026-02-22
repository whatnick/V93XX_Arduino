# Architecture & Design

Design patterns and implementation alignment between UART and SPI drivers.

## 🏗️ Design Principles

### 1. ChecksumMode Alignment
Both V93XX_Raccoon (UART) and V93XX_SPI use **identical ChecksumMode pattern**:

```cpp
// Exactly the same in both drivers
enum class ChecksumMode : uint8_t {
    Dirty = 0,   // Skip CRC, show values
    Clean = 1    // Enforce CRC
};

// Same API in both
v9381.Init(config, ChecksumMode::Dirty);
v9381.SetChecksumMode(ChecksumMode::Clean);
```

### 2. CRC Calculation
Both drivers use **same CRC-8 algorithm** per V9381 datasheet:
```
CRC = 0x33 + ~(sum of payload bytes)
```

### 3. Variable Length Support
Both drivers handle:
- Single register reads/writes
- Block reads (up to 16 registers)
- Frame-based communication

---

## 📐 UART Protocol Layer

### Baud Rate
- **19200 bps** (fixed per V9381 spec)
- Configured automatically in `Init()`

### Frame Formats

**Command Frame (Request)**:
```
[Header: 0x7D] [CMD1] [CMD2] [CRC]
```

**Response Frame**:
```
[Header: 0x7D] [Data0] [Data1] [Data2] [Data3] [CRC]
```

### CRC Scope
- **Request**: Covers [CMD1, CMD2]
- **Response**: Covers [CMD1, CMD2, Data0-3]

---

## 📊 Mode Decision Logic

```
                        ┌─ Init(mode)
                        │
                    [MODE SET]
                        │
         ┌──────────────┼──────────────┐
         │              │              │
    [Dirty]        [Clean]         [?]
         │              │
    [Skip CRC]   [Validate CRC]
         │              │
    [Show Value] ┌──────┴──────┐
         │       │             │
      [Continue] │        [Pass]
                 │       │
              [Invalid?] │
                 │       │
            [Show ERROR,│
             Return 0] │
                       │
                     [Fail]
```

---

## 🔄 Implementation Flow

### RegisterRead() Flow

```
┌─ Preparation                                    ┌─ Validation
│  • Encode CMD1, CMD2 from address              │  • Calculate expected CRC
│  • Calculate request CRC                       │  • Compare with received CRC
│  • Send request frame [0x7D CMD1 CMD2 CRC]    │  • If mismatch:
│                                                │    - Dirty: Show warning
│  ┌─ UART Communication                        │    - Clean: Show error, return 0
│  │  • ESP32 sends request                      │
│  │  • V9381 processes request                  │
│  │  • V9381 sends response frame               │
│  │  • ESP32 receives: [0x7D D0 D1 D2 D3 CRC] │
│  │                                             │
│  ├─ Parse Response                             │
│  │  • Extract 4 data bytes                     │
│  │  • Extract received CRC                     │
│  │  • Assemble uint32_t value                  │
│  │                                             │
│  └─ Return value ─────────────────────────────┘
```

### Mode-Specific Behavior

| Step | Dirty Mode | Clean Mode |
|------|-----------|-----------|
| CRC invalid | Print warning | Print ERROR |
| After CRC error | Continue, return value | Return 0 |
| Serial output | "CRC Mismatch!" | "ERROR: CRC mismatch!" |

---

## 📱 Software Stack

```
┌─────────────────────────────────┐
│  User Sketch                    │
│  • RegisterRead()               │
│  • RegisterWrite()              │
└─────────────┬───────────────────┘
              │
┌─────────────▼───────────────────┐
│ V93XX_Raccoon (PUBLIC API)      │
│ • Init(mode)                    │
│ • SetChecksumMode()            │
│ • ChecksumMode enum             │
└─────────────┬───────────────────┘
              │
┌─────────────▼───────────────────┐
│ V93XX_Raccoon PRIVATE           │
│ • calculate_crc8()             │
│ • encode_command()             │
│ • parse_response()             │
│ • Mode-based validation        │
└─────────────┬───────────────────┘
              │
┌─────────────▼───────────────────┐
│ ESP32 UART Driver               │
│ • uart_write_bytes()            │
│ • uart_read_bytes()             │
│ • Baud: 19200                   │
│ • Config: 8O1                   │
└─────────────┬───────────────────┘
              │
┌─────────────▼───────────────────┐
│ Hardware: UART Interface        │
│ • ESP32 GPIO1 (TX) → V9381 RX  │
│ • ESP32 GPIO2 (RX) ← V9381 TX  │
└─────────────────────────────────┘
```

---

## 🧪 Testing Architecture

```
┌─────────────────────────────────────────┐
│ Test Levels                             │
├─────────────────────────────────────────┤
│                                         │
│  Level 1: Unit Tests (no hardware)      │
│  • test_checksum_mode.py                │
│  • CRC algorithm validation             │
│  • Mode logic verification              │
│                                         │
│  Level 2: Integration Tests             │
│  • run_automated_tests.ps1              │
│  • Build, upload, verify                │
│  • Serial monitoring                    │
│                                         │
│  Level 3: Hardware Validation           │
│  • capture_v9381_uart.py                │
│  • analyze_checksum_captures.py         │
│  • Frame-by-frame CRC verification      │
│                                         │
└─────────────────────────────────────────┘
```

---

## 📈 Compatibility Matrix

| Feature | UART | SPI | Status |
|---------|------|-----|--------|
| ChecksumMode enum | ✅ | ✅ | Aligned |
| CRC algorithm | ✅ | ✅ | Identical |
| API pattern | ✅ | ✅ | Aligned |
| Unit tests | ✅ | ✅ | Common |
| Hardware tests | ✅ | ⚠️ | UART only |
| Datasheets | ✅ | ✅ | Separate |

---

## 🎯 Design Decisions

### Why Dirty Mode by Default?
- Easier for new users to debug
- Shows actual vs expected CRC
- Helps identify wiring issues
- Can be changed to Clean for production

### Why ChecksumMode as Enum?
- Type-safe (vs boolean)
- Extensible (add modes later)
- Self-documenting (Dirty vs Clean clear)
- Matches SPI implementation pattern

### Why Both Register Methods?
- Single: Simple, common case
- Block: Efficient for multiple registers
- Both use same CRC logic
- Mode applies to both equally

---

## 🔗 Related Files

| File | Purpose |
|------|---------|
| `V93XX_Raccoon.h` | Public API & ChecksumMode enum |
| `V93XX_Raccoon.cpp` | Implementation & CRC logic |
| `V93XX_SPI.h` | SPI driver (for comparison) |
| `V93XX_SPI.cpp` | SPI implementation |
| `examples/V9381_UART_DIRTY_MODE/` | Complete example |
| `tools/test_checksum_mode.py` | Algorithm validation |

---

**Status**: ✅ Stable & Production Ready
