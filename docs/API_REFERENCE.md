# API Reference

Complete API documentation for V93XX_Raccoon (UART) driver with ChecksumMode.

## 📖 Quick Reference

### Initialization
```cpp
v9381.Init();  // Default: Dirty mode, 19200 baud
v9381.Init(config);  // Custom config
v9381.Init(config, mode);  // Custom config + mode
```

### Mode Control
```cpp
v9381.SetChecksumMode(ChecksumMode::Dirty);  // Skip CRC, show values
v9381.SetChecksumMode(ChecksumMode::Clean);  // Enforce CRC
```

### Operations
```cpp
uint32_t value = v9381.RegisterRead(address);
v9381.RegisterWrite(address, value);
v9381.RegisterBlockRead(address, count, buffer);
```

---

## 🔧 Complete API

### Class: V93XX_Raccoon

#### Type: ChecksumMode
```cpp
enum class ChecksumMode : uint8_t {
    Dirty = 0,   // Skip CRC validation
    Clean = 1    // Enforce CRC validation
};
```

#### Type: SerialConfig
```cpp
enum class SerialConfig : uint32_t {
    SERIAL_8O1 = /* esp-idf config */
};
```

---

### Method: Init()

**Initialize the UART driver**

```cpp
void Init(SerialConfig config = SERIAL_8O1, 
          ChecksumMode checksum_mode = ChecksumMode::Dirty);
```

**Parameters**:
- `config` - Serial configuration (default: 8 bits, odd parity, 1 stop)
- `checksum_mode` - CRC validation mode (default: Dirty for debugging)

**Behavior**:
- Configures UART pins (GPIO1=TX, GPIO2=RX)
- Sets baud rate to 19200
- Stores ChecksumMode for runtime use

**Example**:
```cpp
// Default (Dirty mode)
v9381.Init();

// Explicit Dirty mode
v9381.Init(SerialConfig::SERIAL_8O1, V93XX_Raccoon::ChecksumMode::Dirty);

// Explicit Clean mode
v9381.Init(SerialConfig::SERIAL_8O1, V93XX_Raccoon::ChecksumMode::Clean);
```

---

### Method: SetChecksumMode()

**Change CRC validation mode at runtime**

```cpp
void SetChecksumMode(ChecksumMode mode);
```

**Parameters**:
- `mode` - New ChecksumMode (Dirty or Clean)

**Behavior**:
- Switches mode immediately
- Prints message to Serial: "Checksum Mode set to [mode]"

**Example**:
```cpp
// Start in Dirty mode
v9381.Init();

// Later: Switch to Clean mode
v9381.SetChecksumMode(V93XX_Raccoon::ChecksumMode::Clean);
```

---

### Method: RegisterRead()

**Read a 32-bit register from V9381**

```cpp
uint32_t RegisterRead(uint8_t address);
```

**Parameters**:
- `address` - Register address (0x00-0x7F, 7-bit)

**Returns**:
- 32-bit register value (little-endian)
- 0 if CRC error in Clean mode

**Behavior**:
- **Dirty mode**: Sends value + CRC message, continues
- **Clean mode**: Checks CRC, returns value or prints ERROR

**Serial Output**:
```
// Dirty mode - always succeeds
CRC Valid: Expected 0x2B, Received 0x2B
CRC Mismatch! Expected 0x31, Received 0x15

// Clean mode - aborts on error
CRC Valid: Expected 0x2B, Received 0x2B
ERROR: CRC mismatch! Expected 0x31, Received 0x15
```

**Example**:
```cpp
// Simple read
uint32_t value = v9381.RegisterRead(0x00);

// Multiple reads
for (int addr = 0; addr < 4; addr++) {
    uint32_t val = v9381.RegisterRead(addr);
}
```

---

### Method: RegisterWrite()

**Write a 32-bit value to a register**

```cpp
void RegisterWrite(uint8_t address, uint32_t value);
```

**Parameters**:
- `address` - Register address (0x00-0x7F, 7-bit)  
- `value` - 32-bit value to write (little-endian)

**Behavior**:
- **Dirty mode**: Sends request, validates response CRC, continues
- **Clean mode**: Sends request, validates response CRC, aborts on error

**Serial Output**:
```
CRC Valid: Expected 0x2B, Received 0x2B
```

**Example**:
```cpp
// Write scalar value
v9381.RegisterWrite(0x10, 0x12345678);

// Write multiple registers
v9381.RegisterWrite(0x10, 0x00000001);
v9381.RegisterWrite(0x11, 0x00000002);
```

---

### Method: RegisterBlockRead()

**Read multiple consecutive registers (up to 16)**

```cpp
void RegisterBlockRead(uint8_t address, uint8_t count, uint8_t* buffer);
```

**Parameters**:
- `address` - Starting register address
- `count` - Number of registers to read (max 16)
- `buffer` - Pointer to buffer to store data (must be ≥count bytes)

**Behavior**:
- Reads multiple registers in sequence
- Each read includes CRC validation
- Stores results in buffer (little-endian)

**Example**:
```cpp
uint8_t data[16];

// Read 8 registers starting at 0x20
v9381.RegisterBlockRead(0x20, 8, data);

// Process data
for (int i = 0; i < 8; i++) {
    Serial.print(data[i], HEX);
    Serial.print(" ");
}
```

---

## 🎯 Mode Behavior Matrix

| Operation | Dirty Mode | Clean Mode |
|-----------|-----------|-----------|
| **Valid CRC** | Continues, shows CRC | Continues, shows CRC |
| **Invalid CRC (Read)** | Continues, shows mismatch | Prints ERROR, returns 0 |
| **Invalid CRC (Write)** | Continues, shows mismatch | Prints ERROR, aborts |
| **Serial Output** | "CRC Valid/Mismatch" | "CRC Valid" or "ERROR" |
| **Use Case** | Debugging | Production |

---

## 📊 CRC Frame Structure

### Request Frame
```
[Marker] [CMD1] [CMD2] [CRC]
  0x7D    [op]   [addr] [crc]

CRC = CRC8(CMD1, CMD2)
```

### Response Frame
```
[Marker] [D0] [D1] [D2] [D3] [CRC]
  0x7D   [  ] [  ] [  ] [  ] [crc]

CRC = CRC8(CMD1, CMD2, D0, D1, D2, D3)
```

---

## 🔗 Related Documentation

- [UART_CHECKSUM_MODE.md](UART_CHECKSUM_MODE.md) - Mode details and CRC algorithm
- [INTEGRATION.md](INTEGRATION.md) - How to use in projects
- [TESTING.md](TESTING.md) - Testing and validation

---

**Status**: ✅ Complete documentation
