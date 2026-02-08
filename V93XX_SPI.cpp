
#include "V93XX_SPI.h"

const uint8_t ControlAddresses[] = {
    DSP_ANA0,   // Analog Control 0
    DSP_ANA1,   // Analog Control 1
    DSP_CTRL0,  // Digital Control 0
    DSP_CTRL1,  // Digital Control 1
    DSP_CTRL2,  // Digital Control 2
    DSP_CTRL3,  // Digital Control 3
    DSP_CTRL4,  // Digital Control 4
    DSP_CTRL5,  // Digital Control 5
};

const uint8_t CalibrationAddresses[] = {
    DSP_CFG_CALI_PA,    // Active power A ratio difference correction
    DSP_CFG_DC_PA,      //  Active power A small signal correction
    DSP_CFG_CALI_QA,    //	Reactive power A ratio difference correction
    DSP_CFG_DC_QA,      //	Reactive power A small signal correction
    DSP_CFG_CALI_PB,    //	Active power B ratio difference correction
    DSP_CFG_DC_PB,      //	Active power B small signal correction
    DSP_CFG_CALI_QB,    //	Reactive power B ratio difference correction
    DSP_CFG_DC_QB,      //	Reactive power B small signal correction
    DSP_CFG_CALI_RMSUA, //	Voltage RMS ratio correction
    DSP_CFG_RMS_DCUA,   //	Voltage RMS small signal correction
    DSP_CFG_CALI_RMSIA, //	Current A effective value ratio correction
    DSP_CFG_RMS_DCIA,   //	Current A RMS small signal correction
    DSP_CFG_CALI_RMSIB, //	Current B effective value ratio difference correction
    DSP_CFG_RMS_DCIB,   //	Current B RMS small signal correction
    DSP_CFG_PHC,        //	Angular error correction register
    DSP_CFG_DCUA,       //	Voltage channel DC correction value
    DSP_CFG_DCIA,       //	Current A channel DC correction value
    DSP_CFG_DCIB,       //	Current B channel DC correction value
    DSP_CFG_BPF,        //	Bandpass filter coefficients
    DSP_CFG_CKSUM,      //	Checksum Configuration Registers
    EGY_PROCTH,         //	Energy register anti-submarine threshold (less than it will not accumulate)
    EGY_PWRTH,          //	Energy register accumulation threshold. Since the energy register is 46 bits
};

V93XX_SPI::V93XX_SPI(int cs_pin, SPIClass& spi_bus, uint32_t spi_freq) 
    : spi_bus(spi_bus),
      cs_pin(cs_pin),
      spi_freq(spi_freq),
      spi_settings(spi_freq, MSBFIRST, SPI_MODE0)
{
}

void V93XX_SPI::Init(WireMode wire_mode, bool initialize_interface) {
    Init(wire_mode, initialize_interface, -1, -1, -1);
}

void V93XX_SPI::Init(WireMode wire_mode, bool initialize_interface, int8_t sck_pin, int8_t miso_pin, int8_t mosi_pin) {
    this->wire_mode = wire_mode;

    pinMode(this->cs_pin, OUTPUT);
    digitalWrite(this->cs_pin, HIGH);

    if (sck_pin >= 0 || miso_pin >= 0 || mosi_pin >= 0) {
        // ESP32/ESP32-S3 Arduino core supports pin-mapped begin().
        this->spi_bus.begin(sck_pin, miso_pin, mosi_pin, this->cs_pin);
    } else {
        this->spi_bus.begin();
    }

    delay(10);

    if (this->wire_mode == WireMode::ThreeWire) {
        // Datasheet: SPICSN always low in 3-wire mode.
        digitalWrite(this->cs_pin, LOW);
        // Datasheet: clock must be low for >=400us before each operation.
        // In SPI mode 0, SCK idles low; we still ensure a pre-op delay.
        delayMicroseconds(400);
    }

    this->last_op_end_us = micros();

    if (initialize_interface) {
        (void)InitializeInterface();
    }
}

inline void V93XX_SPI::BeginTransaction() {
    EnforceInterOpTiming();

    if (this->wire_mode == WireMode::ThreeWire) {
        // Datasheet: SPICLK must be low for >=400us before each operation.
        // In SPI mode 0, SCK idles low between transactions; we still enforce a pre-op wait.
        delayMicroseconds(400);
    }

    this->spi_bus.beginTransaction(this->spi_settings);

    if (this->wire_mode == WireMode::FourWire) {
        digitalWrite(this->cs_pin, LOW);
        delayMicroseconds(1);
    } else {
        // 3-wire: CS is always low; ensure required SCK-low time before operation.
        digitalWrite(this->cs_pin, LOW);
    }
}

inline void V93XX_SPI::EndTransaction() {
    this->spi_bus.endTransaction();

    if (this->wire_mode == WireMode::FourWire) {
        delayMicroseconds(1);
        digitalWrite(this->cs_pin, HIGH);
    } else {
        // 3-wire: keep CS asserted.
        digitalWrite(this->cs_pin, LOW);
    }

    this->last_op_end_us = micros();
}

uint8_t V93XX_SPI::CalculateCRC8(const uint8_t* data, size_t length) {
    // Datasheet "checksum": 0x33 + ~sum (8-bit arithmetic)
    uint8_t checksum = 0;
    for(size_t i = 0; i < length; i++){
        checksum += data[i];
    }
    return 0x33 + ~checksum;
}

uint8_t V93XX_SPI::BuildCmdByte(uint8_t address7, bool is_read) const {
    // CMD = (ADDR[6:0] << 1) | R/W
    // Example from datasheet: write to 0x7F => CMD = 0xFE
    return (uint8_t)(((address7 & 0x7F) << 1) | (is_read ? 1 : 0));
}

void V93XX_SPI::EnforceInterOpTiming() {
    if (this->wire_mode == WireMode::FourWire) {
        // Datasheet: must be >=50us between any two operations.
        // Use end-of-last-operation timestamp.
        uint32_t now = micros();
        uint32_t elapsed = now - this->last_op_end_us;
        if (elapsed < 50) {
            delayMicroseconds((uint16_t)(50 - elapsed));
        }
    }
}

void V93XX_SPI::ApplyAddressOffsetModeIfNeeded(uint8_t address) {
    // Datasheet: to access >=0x80, write magic to 0x7F to enable +0x80 offset.
    // For convenience, automatically enable offset when a high address is used.
    bool needs_high = (address & 0x80) != 0;
    if (needs_high && !this->high_address_offset_enabled) {
        SetHighAddressOffsetEnabled(true);
    } else if (!needs_high && this->high_address_offset_enabled) {
        SetHighAddressOffsetEnabled(false);
    }
}

bool V93XX_SPI::InitializeInterface() {
    // Write 0x5A7896B4 to address 0x7F.
    // CMD for write to 0x7F => 0xFE.
    uint8_t frame[6];
    frame[0] = BuildCmdByte(0x7F, false);
    frame[1] = 0xB4;
    frame[2] = 0x96;
    frame[3] = 0x78;
    frame[4] = 0x5A;
    frame[5] = CalculateCRC8(frame, 5);

    BeginTransaction();
    for (size_t i = 0; i < sizeof(frame); i++) {
        (void)this->spi_bus.transfer(frame[i]);
    }
    EndTransaction();

    // Datasheet suggests reading any readable register and checking checksum.
    uint32_t dummy;
    return RegisterReadChecked(SYS_VERSION, dummy);
}

void V93XX_SPI::SetHighAddressOffsetEnabled(bool enabled) {
    // Write magic values to 0x7F to enable/disable +0x80 offset mode.
    // Enable:  0x4A985B67
    // Disable: 0x76B589A4
    uint32_t magic = enabled ? 0x4A985B67UL : 0x76B589A4UL;

    uint8_t frame[6];
    frame[0] = BuildCmdByte(0x7F, false);
    frame[1] = (uint8_t)(magic & 0xFF);
    frame[2] = (uint8_t)((magic >> 8) & 0xFF);
    frame[3] = (uint8_t)((magic >> 16) & 0xFF);
    frame[4] = (uint8_t)((magic >> 24) & 0xFF);
    frame[5] = CalculateCRC8(frame, 5);

    BeginTransaction();
    for (size_t i = 0; i < sizeof(frame); i++) {
        (void)this->spi_bus.transfer(frame[i]);
    }
    EndTransaction();

    this->high_address_offset_enabled = enabled;
}

void V93XX_SPI::RegisterWrite(uint8_t address, uint32_t data) {
    (void)RegisterWriteChecked(address, data);
}

bool V93XX_SPI::RegisterWriteChecked(uint8_t address, uint32_t data) {
    ApplyAddressOffsetModeIfNeeded(address);

    uint8_t addr7 = (uint8_t)(address & 0x7F);
    uint8_t frame[6];
    frame[0] = BuildCmdByte(addr7, false);
    frame[1] = (uint8_t)(data & 0xFF);
    frame[2] = (uint8_t)((data >> 8) & 0xFF);
    frame[3] = (uint8_t)((data >> 16) & 0xFF);
    frame[4] = (uint8_t)((data >> 24) & 0xFF);
    frame[5] = CalculateCRC8(frame, 5);

    BeginTransaction();
    for (size_t i = 0; i < sizeof(frame); i++) {
        (void)this->spi_bus.transfer(frame[i]);
    }
    EndTransaction();

    // Datasheet: write operation does not return a valid response.
    return true;
}

uint32_t V93XX_SPI::RegisterRead(uint8_t address) {
    uint32_t value = 0;
    (void)RegisterReadChecked(address, value);
    return value;
}

bool V93XX_SPI::RegisterReadChecked(uint8_t address, uint32_t &out_value) {
    ApplyAddressOffsetModeIfNeeded(address);

    uint8_t addr7 = (uint8_t)(address & 0x7F);
    uint8_t tx[6] = {0};
    uint8_t rx[6] = {0};
    tx[0] = BuildCmdByte(addr7, true);

    BeginTransaction();
    for (size_t i = 0; i < sizeof(tx); i++) {
        rx[i] = this->spi_bus.transfer(tx[i]);
    }
    EndTransaction();

    // rx[0] is don't care. Data comes in rx[1..4], checksum in rx[5].
    uint8_t data_bytes[4] = { rx[1], rx[2], rx[3], rx[4] };
    uint8_t expected = 0;
    expected += tx[0];
    expected += data_bytes[0];
    expected += data_bytes[1];
    expected += data_bytes[2];
    expected += data_bytes[3];
    expected = 0x33 + (uint8_t)(~expected);

    bool ok = (expected == rx[5]);
    if (!ok) {
        Serial.printf("RegisterRead(): Checksum invalid (expected: 0x%02X, received: 0x%02X)\n", expected, rx[5]);
    }

    out_value = (uint32_t)data_bytes[0] |
                ((uint32_t)data_bytes[1] << 8) |
                ((uint32_t)data_bytes[2] << 16) |
                ((uint32_t)data_bytes[3] << 24);
    return ok;
}

void V93XX_SPI::ConfigureBlockRead(const uint8_t addresses[], uint8_t num_addresses)
{
    // NOTE: The chip's datasheet documents "block reading" as a UART feature (address mapping).
    // The SPI section does not define a dedicated block-read command/frame. We keep the mapping
    // registers configuration because it's useful documentation-wise and may be used by UART,
    // but RegisterBlockRead() in this SPI driver is an emulation (sequential reads).

    this->configured_block_addr_count = 0;

    uint8_t copy_count = num_addresses;
    if (copy_count > 16) {
        copy_count = 16;
    }
    for (uint8_t i = 0; i < copy_count; i++) {
        this->configured_block_addrs[i] = addresses[i];
    }
    this->configured_block_addr_count = copy_count;

    // Configure block read addresses in system registers (0x61-0x64)
    uint8_t const* address_ptr = addresses;
    for (int blk = 0; blk < 4; blk++){
        uint32_t combined_address = 0;
        for (int i = 0; (i < 4) && (num_addresses); i++) {
            combined_address |= (*address_ptr++ << (8 * i));
            num_addresses--;
        }
        RegisterWrite(SYS_BLK_ADDR0 + blk, combined_address);
    }
}

void V93XX_SPI::RegisterBlockRead(uint32_t (&values)[], uint8_t num_values)
{
    // IMPORTANT: SPI "block read" is not defined in the datasheet SPI protocol section.
    // The true block/mapped read mechanism is documented under UART.
    // For SPI, we emulate block reads by issuing repeated single-register reads.
    uint8_t to_read = num_values;
    if (to_read > this->configured_block_addr_count) {
        to_read = this->configured_block_addr_count;
    }

    for (uint8_t i = 0; i < to_read; i++) {
        uint32_t v = 0;
        (void)RegisterReadChecked(this->configured_block_addrs[i], v);
        values[i] = v;
    }

    for (uint8_t i = to_read; i < num_values; i++) {
        values[i] = 0;
    }
}

void V93XX_SPI::LoadConfiguration(const V93XX_SPI::ControlRegisters& ctrl, const V93XX_SPI::CalibrationRegisters& calibrations) {
    uint32_t checksum = 0;
    
    // Load control values [0x00 - 0x07]
    for (int i = 0; i < sizeof(V93XX_SPI::ControlRegisters)/sizeof(uint32_t); i++){
        this->RegisterWrite(ControlAddresses[i], ctrl._array[i]);
        checksum += ctrl._array[i];
    }

    // Load calibration values [0x25 - 0x3a]
    for (int i = 0; i < sizeof(V93XX_SPI::CalibrationRegisters)/sizeof(uint32_t); i++){
        this->RegisterWrite(CalibrationAddresses[i], calibrations._array[i]);
        checksum += calibrations._array[i];
    }

    // The sum of {0x00-0x07, 0x25-0x3a, 0x55-0x60} Needs to equal 0xFFFFFFFF to pass self-check.
    // Calculate DSP_CFG_CKSUM (0x38) to ensure this is the case
    checksum = 0xFFFFFFFF - checksum;
    this->RegisterWrite(DSP_CFG_CKSUM, checksum);
}
