# V93XX UART Communication Protocol

Complete UART protocol reference for V93XX communication with master MCU.

## UART Overview

### Features
- **Asynchronous, half-duplex communication**
- **Baud Rate:** 1200 - 19200 bps (auto-adapting supported)
- **Data Frame:** 11-bit (1 START + 8 DATA + 1 PARITY + 1 STOP)
- **Parity:** Odd parity (ODD)
- **Bit Transmission:** LSB first, LSB byte first
- **Multi-Device:** Up to 4 V93XX on single bus via device address (A0, A1)
- **Protocol:** Half-duplex (end data → wait 1ms → receive response)

### Frame Structure

```
START  B0  B1  B2  B3  B4  B5  B6  B7  P  STOP
  0     |------------ 8-bit data -----------| 1
```

---

## Communication Timing

### Critical Timing Parameters

| Parameter | Symbol | Timing | Description |
|-----------|--------|--------|-------------|
| **1-bit duration** | t_TB | 11 / baudrate | Time to send/receive one bit |
| **1-byte receive** | t_RB | 11 / baudrate | Time for complete byte RX |
| **Byte gap (RX)** | t_RF | 16 / baudrate | Max time between RX bytes (max 20ms) |
| **RTD (Response)** | t_RTD | 0-20 ms | Delay: CMD RX → Response TX |
| **TBD (TX gap)** | t_TBD | 0-20 ms | Delay between TX bytes in response |
| **TRD (Next CMD)** | t_TRD | >2 ms | Delay: Response TX → next CMD RX |
| **RT (rise time)** | t_R | ~300 ns | RX/TX line rise time |
| **FT (fall time)** | t_F | ~300 ns | RX/TX line fall time |

### Example Timing at 9600 bps
```
t_TB  = 11 / 9600 ≈ 1.146 ms
t_RB  = 11 / 9600 ≈ 1.146 ms
t_RF  = 16 / 9600 ≈ 1.667 ms (max before timeout)
t_RTD = [0-20] ms recommended
```

---

## Communication Protocol Modes

### 1. **BROADCAST WRITE** (No Response)
Write to all V93XX on bus simultaneously. No device address matching required.

#### Frame Structure
```
[HEADER][CMD1][CMD2][DATA_0...][CKSUM_0]...[DATA_N...][CKSUM_N]
```

#### Byte Details

| Byte | Content | Format | Notes |
|------|---------|--------|-------|
| 0 | HEADER | 0x7D | Fixed broadcast header |
| 1 | CMD1 | `0 1 1 1 1 1 0 0` | Length (N) in bits [7:2], `00` type bits |
| 2 | CMD2 | Address | Starting register address |
| 3-6 | DATA 0 | 32-bit LE | Register value (little-endian) |
| 7 | CKSUM 0 | Calculated | See checksum formula below |
| ... | ... | ... | Repeat for N data values |

#### Checksum Calculation
```
CKSUM[0] = 0x33 + ~(CMD1 + CMD2 + Data0_B0 + Data0_B1 + Data0_B2 + Data0_B3)
CKSUM[N] = 0x33 + ~(CMD1 + CMD2 + Data0-N)
```

#### Example: Write 0xAABBCCDD to address 0x10
```
Byte:  0     1     2     3     4     5     6     7
Data: 0x7D  0x04  0x10  0xDD  0xCC  0xBB  0xAA  [CKSUM]

Calculation:
Sum = 0x04 + 0x10 + 0xDD + 0xCC + 0xBB + 0xAA = 0x3E4
Inv = ~0x3E4 = 0xFFFFC1B (mask to 8-bit) = 0x1B
CKSUM = 0x33 + 0x1B = 0x4E
```

---

### 2. **READ OPERATION** (V93XX Responds)
Read consecutive registers with device address matching.

#### Master → V93XX Frame
```
[HEADER][CMD1][CMD2][CKSUM]
```

| Byte | Content | Format | Notes |
|------|---------|--------|-------|
| 0 | HEADER | 0x7D | Fixed |
| 1 | CMD1 | `N A1 A0 0 1` | Length, Address bits, type `01` |
| 2 | CMD2 | Address | Starting register address |
| 3 | CKSUM | Calculated | Sum of CMD1 + CMD2 |

#### V93XX → Master Frame
```
[DATA_0...][DATA_N...][CKSUM]
```

| Bytes | Content | Format | Notes |
|-------|---------|--------|-------|
| 0-3 | DATA 0 | 32-bit LE | First register value |
| 4-7 | DATA 1 | 32-bit LE | Second register value |
| ... | ... | ... | Up to 16 registers |
| Last | CKSUM | Byte | Checksum of all data |

#### Checksum Calculation (V93XX → Master)
```
CKSUM = 0x33 + ~(CMD1 + CMD2 + AllDataBytes)
```

---

### 3. **WRITE OPERATION** (V93XX Acknowledges)
Write multiple consecutive registers with device address & response.

#### Master → V93XX Frame
```
[HEADER][CMD1][CMD2][DATA_0...][CKSUM_0]...[DATA_N...][CKSUM_N]
```

| Byte | Content | Format | Notes |
|------|---------|--------|-------|
| 0 | HEADER | 0x7D | Fixed |
| 1 | CMD1 | `N A1 A0 1 0` | Length, Address bits, type `10` |
| 2 | CMD2 | Address | Starting register address |
| 3-6 | DATA 0 | 32-bit LE | Register value |
| 7 | CKSUM 0 | Byte | Calculated per data block |

#### V93XX → Master Response
```
[CKSUM]
```

Single checksum byte for verification. Compare with master's CKSUM_N.

#### Checksum Calculation
```
CKSUM[0] = 0x33 + ~(CMD1 + CMD2 + Data0_B0 + ... + Data0_B3)
CKSUM[N] = 0x33 + ~(CMD1 + CMD2 + Data0-N)
```

---

### 4. **BLOCK READ OPERATION** (Non-Consecutive)
Read up to 16 non-consecutive registers mapped via SYS_BLK0-3_ADDR (0x79-0x7C).

#### Setup (Before Block Read)
```
Write addresses to block mapping registers:
- SYS_BLK0_ADDR (0x79): Bits [31:24]=ADDR3, [23:16]=ADDR2, [15:8]=ADDR1, [7:0]=ADDR0
- SYS_BLK1_ADDR (0x7A): Bits [31:24]=ADDR7, ..., [7:0]=ADDR4
- SYS_BLK2_ADDR (0x7B): Bits [31:24]=ADDR11, ..., [7:0]=ADDR8
- SYS_BLK3_ADDR (0x7C): Bits [31:24]=ADDR15, ..., [7:0]=ADDR12
```

#### Master → V93XX Frame
```
[HEADER][CMD1][CMD2][CKSUM]
```

| Byte | Content | Format | Notes |
|------|---------|--------|-------|
| 0 | HEADER | 0x7D | Fixed |
| 1 | CMD1 | `N A1 A0 1 1` | Length, Address bits, type `11` |
| 2 | CMD2 | Start block + index | Which mapped address to start from |
| 3 | CKSUM | Byte | 0x33 + ~(CMD1 + CMD2) |

#### V93XX → Master Frame
Same as READ operation - returns data from mapped addresses.

---

## Frame Error Detection

### Error Conditions

| Error | Condition | UART_ERR Flag | Recovery |
|-------|-----------|---------------|----------|
| **Overtime** | >20ms between bytes | SET | Abort, wait for next CMD |
| **Parity Error** | Odd parity fail | SET | Invalid frame, ignore |
| **Checksum Error** | CKSUM mismatch | SET | Write fails, no response |
| **Frame Error** | Invalid header/type | NO | Ignored, waits for valid CMD |

### Enabling Auto Baud-Rate
Set bit0 of SYS_MISC (0x75) register:
- `UARTAUTOEN = 0`: One-time auto-adjust on first frame
- `UARTAUTOEN = 1`: Continuous auto-adjust every frame

---

## Practical Examples

### Example 1: Read Single Register via UART

**Goal:** Read DSP_CTRL0 (0x02) from V9381 at address 0x00

**Master TX:**
```
Byte  0     1     2     3
Data  0x7D  0x01  0x02  0x34

Calculation:
N=0 (read 1 register) → bits [7:2] = 0x00
A1=0, A0=0 (address 0x00)
Type = 01 (read) → 0x01
CMD1 = 0b00000101 = 0x05
CMD2 = 0x02

CKSUM = 0x33 + ~(0x05 + 0x02) = 0x33 + ~0x07 = 0x33 + 0xF8 = 0x2B
```

**V93XX RX:**
- Recognizes 0x7D header
- Matches A1=0, A0=0 (addressed to this chip)
- Reads DSP_CTRL0 contents

**V93XX TX:**
```
Bytes 0-3: Register value (32-bit LE)
Byte 4: CKSUM = 0x33 + ~(0x05 + 0x02 + RegValue_Bytes)
```

---

### Example 2: Broadcast Write Gain Calibration

**Goal:** Write calibration value 0x01234567 to DSP_CFG_CALI_PA (0x25) - All V93XX

**Master TX:**
```
Byte  0     1     2     3     4     5     6     7
Data  0x7D  0x04  0x25  0x67  0x45  0x23  0x01  [CKSUM]

Calculation:
N=0 (write 1 register) → bits [7:2] = 0x00
Broadcast → bits [1:0] = 00
CMD1 = 0x00
CMD2 = 0x25

Data bytes (little-endian): 0x67, 0x45, 0x23, 0x01
Sum = 0x00 + 0x25 + 0x67 + 0x45 + 0x23 + 0x01 = 0xFB
CKSUM = 0x33 + ~0xFB = 0x33 + 0x04 = 0x37
```

**V93XX RX:**
- All V93XX receive and write to address 0x25
- No response generated (broadcast)
- Wait >2ms before sending next command

---

### Example 3: Check UART Errors

**Reading Error Flags:**
```c
// Read system interrupt status
read_register(0x72, &int_status);

// Check UART error flag
if (int_status & 0x10000) {
    // UART_ERR bit set
    // Conditions that set this:
    // - Overtime (>20ms between bytes)
    // - Parity error
    // - Checksum error on write
    
    // Clear flag
    write_register(0x72, 0x10000); // Write 1 to clear
}
```

---

## Configuration

### Baud Rate Auto-Adapting
**Register:** SYS_MISC (0x75), bit0 = UARTAUTOEN

| Mode | Behavior |
|------|----------|
| 0 | Auto-adapt on first frame only, then fixed |
| 1 | Auto-adapt every frame (supports baud rate changes) |

### Burst Mode
**Register:** SYS_MISC (0x75), bit1 = UARTBURSTEN

| Mode | Behavior |
|------|----------|
| 0 | Enable - Continuous write mode (faster) |
| 1 | Disable - Standard mode |

---

## Best Practices

1. **Between Commands:** Always allow >2ms delay (tTRD) after response
2. **Broadcast Safety:** Wait 2-3ms after broadcast write before reading
3. **Auto-Baud:** Enable on startup, then optionally disable if baud rate is fixed
4. **Device Address:** Set A0/A1 pins correctly before UART communication
5. **Checksum Validation:** Always verify response checksum on reads
6. **Error Recovery:** On timeout/error, re-send command after slight delay
7. **Interrupt Handling:** Clear interrupt flags (write 1) after servicing events

---

## Broadcast Write Timing Diagram

```
Master tx frame (ends)
                    |
                    v
                 [1ms delay]
                    |
                    v
              V93XX accepts command
           and writes to registers
              [Device 1,2,3,4]
                    |
                    v
              [Master can query status]

Important: Master must wait >2ms after
broadcast before assuming write is complete
```

---

## Device Address Configuration

| A1 Pin | A0 Pin | Device Address | Priority |
|--------|--------|------|----------|
| 0 | 0 | 0x00 | Highest (responds first) |
| 0 | 1 | 0x01 | - |
| 1 | 0 | 0x02 | - |
| 1 | 1 | 0x03 | Lowest |

**Broadcast:** A1=X, A0=X (don't care - all chips respond)

---

Last Updated: 2025-02-22  
V4.9 Datasheet Compatible
