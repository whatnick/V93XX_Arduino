# UART/SPI Register Headers and Timing Macros

## Overview

The V93XX Arduino library uses a unified, protocol-independent register header that consolidates register definitions and timing macros in a single location used by both UART and SPI implementations.

## Architecture

### Register File Structure
- **File:** `V93XX_Registers.h` (Protocol-independent)
- **Used by:** V93XX_UART.h, V93XX_SPI.h, and all user sketches
- **Rationale:** Both UART and SPI protocols use identical register addresses and definitions, making a shared header the optimal design choice

#### Include Architecture
All protocol headers include the unified register file:
- `V93XX_UART.h`: `#include "V93XX_Registers.h"`
- `V93XX_SPI.h`: `#include "V93XX_Registers.h"`
- All examples and user sketches access registers via protocol headers

#### Timing Macros
The inter-frame timing macros have been moved to `V93XX_Registers.h`:

```cpp
// Device baud rate configuration (19200 by default)
#define V93XX_UART_BAUD_RATE 19200

// Auto-calculated inter-frame delay in milliseconds
// Formula: (5 bytes * 11 bits/byte / baud_rate) + 4ms safety margin
#define V93XX_INTERFRAME_DELAY_MS ((55 * 1000) / V93XX_UART_BAUD_RATE + 4)
```

**Benefits:**
- Single source of truth for both UART and SPI timing configuration
- Auto-calculation provides optimal speed & safety for any baud rate
- Easy to adjust: change only `V93XX_UART_BAUD_RATE` to optimize for different speeds

### Example Usage

#### In `.ino` sketches:
```cpp
#include "V93XX_UART.h"  // Includes V93XX_Registers.h automatically

void setup() {
    v9381.Init(SerialConfig::SERIAL_8O1, V93XX_UART::ChecksumMode::Dirty);
}

void loop() {
    // Use the auto-calculated delay between operations
    uint32_t value = v9381.RegisterRead(SYS_VERSION);
    delay(V93XX_INTERFRAME_DELAY_MS);  // Optimal for configured baud rate
}
```

#### To change baud rate:
Edit `V93XX_Registers.h` line 10:
```cpp
#define V93XX_UART_BAUD_RATE 9600     // Changes delay to ~9.7ms
#define V93XX_UART_BAUD_RATE 115200   // Changes delay to ~4.5ms
```

### Timing Values by Baud Rate

| Baud Rate | Calculated Delay | Safety Margin | Use Case |
|-----------|------------------|---------------|----------|
| 1200 | 46ms | Very Safe | Legacy/Very slow devices |
| 9600 | 9.7ms | Very Safe | Standard/Compatible |
| 19200 | **6.9ms** | **Adequate** | **Default (Current)** |
| 38400 | 5.4ms | Safe | High-speed |
| 115200 | 4.5ms | Adequate | Maximum speed |

### Configuration and Customization

#### Using timing macros in your code:
```cpp
// Include protocol header
#include "V93XX_UART.h"  // or V93XX_SPI.h for SPI

// Timing macros from V93XX_Registers.h automatically available
uint32_t value = v9381.RegisterRead(SYS_VERSION);
delay(V93XX_INTERFRAME_DELAY_MS);  // Optimal for configured baud rate
```

#### Adjusting baud rate:
Edit `V93XX_Registers.h` line 10 to change device speed and auto-recalculate delays:
```cpp
#define V93XX_UART_BAUD_RATE 9600     // Changes delay to ~9.7ms
#define V93XX_UART_BAUD_RATE 19200    // Changes delay to ~6.9ms (default)
#define V93XX_UART_BAUD_RATE 115200   // Changes delay to ~4.5ms
```

### Verification Commands

Verify all header files are correctly configured:
```bash
# Check includes
grep -r "#include" V93XX_*.h

# Verify macro definitions
grep -n "V93XX_INTERFRAME_DELAY_MS" V93XX_Registers.h

# Compile test
arduino-cli compile --fqbn esp32:esp32:esp32s3 examples/V9381_UART_DIRTY_MODE/
```



### Architecture Benefits

1. **Clean Design:** Single unified header for protocol-independent register definitions
2. **Automatic Optimization:** Timing macros calculate optimal delays for any baud rate
3. **Simplified Configuration:** Change one macro to adapt for different speeds
4. **Clear Semantics:** Generic naming accurately reflects that registers/addresses are shared across both UART and SPI
5. **Zero Duplication:** Eliminates separate register file copies
6. **Production Ready:** Timing tested and verified in hardware with Saleae Logic Analyzer

### Implementation

Core architecture files:
- `V93XX_Registers.h` - Protocol-independent registers and timing configuration
- `V93XX_UART.h` - UART protocol implementation (includes V93XX_Registers.h)
- `V93XX_SPI.h` - SPI protocol implementation (includes V93XX_Registers.h)
- `examples/` - All sketches use shared timing macros
- `tools/` - Utilities reference unified register definitions

### Questions?

Refer to the original [UART_CHECKSUM_MODE.md](UART_CHECKSUM_MODE.md) documentation for detailed timing specifications and register reference.
