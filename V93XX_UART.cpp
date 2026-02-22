
#include "V93XX_UART.h"

const uint8_t ControlAddresses[] = {
    DSP_ANA0,  // Analog Control 0
    DSP_ANA1,  // Analog Control 1
    DSP_CTRL0, // Digital Control 0
    DSP_CTRL1, // Digital Control 1
    DSP_CTRL2, // Digital Control 2
    DSP_CTRL3, // Digital Control 3
    DSP_CTRL4, // Digital Control 4
    DSP_CTRL5, // Digital Control 5
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

V93XX_UART::V93XX_UART(int rx_pin, int tx_pin, HardwareSerial &serial, int device_address)
    : serial(serial), serial_rx_buffer(std::list<uint8_t>(65)) // 65bytes = 16x u32 (block read) + u8 (CRC)
{
    this->device_address = device_address;
    this->tx_pin = tx_pin;
    this->rx_pin = rx_pin;

    while (this->serial_rx_buffer.size()) {
        this->serial_rx_buffer.pop();
    }
}

void V93XX_UART::RxReset() {
    pinMode(this->tx_pin, OUTPUT);

    // TX pin for UART , RX pin for ASIC need to be held low
    digitalWrite(this->tx_pin, HIGH);
    delayMicroseconds(3000);
    digitalWrite(this->tx_pin, LOW);

    delayMicroseconds(92500);
    digitalWrite(this->tx_pin, HIGH);
    delayMicroseconds(2150);
}

void V93XX_UART::Init(SerialConfig config, ChecksumMode checksum_mode) {
    this->checksum_mode = checksum_mode;

    pinMode(this->rx_pin, INPUT_PULLUP);
    this->serial.begin(19200, config, this->rx_pin, this->tx_pin);
    noInterrupts();
    this->serial.onReceive(std::bind(&V93XX_UART::RxReceive, this));
    while (this->serial_rx_buffer.size()) {
        this->serial_rx_buffer.pop();
    }
    interrupts();
}

bool V93XX_UART::WaitForRx(size_t count, uint32_t timeout_ms) {
    uint32_t start = millis();
    while (this->RxBufferCount() < count) {
        if ((millis() - start) >= timeout_ms) {
            return false;
        }
        delay(1);
    }
    return true;
}

void V93XX_UART::RxReceive() {
    while (this->serial.available() > 0) {
        uint8_t data = this->serial.read();
        noInterrupts();
        if ((this->serial_rx_buffer.size() < 65)) {
            this->serial_rx_buffer.push(data);
        }
        interrupts();
    }
}

unsigned int V93XX_UART::RxBufferCount() {
    unsigned int count = 0;
    noInterrupts();
    count = this->serial_rx_buffer.size();
    interrupts();
    return count;
}

uint8_t V93XX_UART::RxBufferPop() {
    uint8_t data = 0;
    noInterrupts();
    data = this->serial_rx_buffer.front();
    this->serial_rx_buffer.pop();
    interrupts();
    return data;
}

void V93XX_UART::RegisterWrite(uint8_t address, uint32_t data) {
    const int num_registers = 1;
    // Described in Section 7.4 of Datasheet
    uint8_t payload[8] = {// Header
                          0x7d,

                          // CMD1 (Payload length, addr, operation)
                          (uint8_t)(((num_registers - 1) << 4) | ((this->device_address) << 2) | CmdOperation::WRITE),
                          // CMD2 (7b Address)
                          (uint8_t)(address & 0x7f),

                          // Data (32b)
                          (uint8_t)(data & 0x000000ff), (uint8_t)((data & 0x0000ff00) >> 8),
                          (uint8_t)((data & 0x00ff0000) >> 16), (uint8_t)((data & 0xff000000) >> 24),

                          // Checksum
                          0x00};

    uint8_t checksum = 0;
    // Calculate Checksum, Sum of payload &0xFF
    for (int idx = 1; idx < sizeof(payload) / sizeof(uint8_t) - 1; idx++) {
        checksum += payload[idx];
    }
    checksum = 0x33 + ~checksum;
    payload[7] = checksum;

    // Transmit payload
    this->serial.write(payload, sizeof(payload) / sizeof(uint8_t));
    this->serial.flush();

    // wait for response
    if (!this->WaitForRx(1, 50)) {
        Serial.println("RegisterWrite(): timeout waiting for checksum response");
        return;
    }

    // Read response
    uint8_t checksum_response = this->RxBufferPop();

    // Check and report CRC
    bool checksum_valid = checksum_response == checksum;
    Serial.printf("RegisterWrite(0x%02X): CRC expected=0x%02X received=0x%02X %s", address, checksum, checksum_response,
                  checksum_valid ? "✓" : "✗");

    if (!checksum_valid && this->checksum_mode == ChecksumMode::Clean) {
        Serial.println(" - ERROR: CRC mismatch! (Clean mode)");
    } else if (!checksum_valid && this->checksum_mode == ChecksumMode::Dirty) {
        Serial.println(" - WARNING: CRC mismatch! (Dirty mode - proceeding)");
    } else {
        Serial.println();
    }
}

uint32_t V93XX_UART::RegisterRead(uint8_t address) {
    const int num_registers = 1;
    // Described in Section 7.3 of Datasheet
    uint8_t request[4] = {// Header
                          0x7d,

                          // CMD1 (request length, addr, operation)
                          (uint8_t)(((num_registers - 1) << 4) | ((this->device_address) << 2) | CmdOperation::READ),
                          // CMD2 (7b Address)
                          (uint8_t)(address & 0x7f),

                          // Checksum
                          0x00};

    // Calculate Checksum, Sum of request &0xFF
    request[3] = 0x33 + ~(request[1] + request[2]);

    // Transmit request
    this->serial.write(request, sizeof(request) / sizeof(uint8_t));
    this->serial.flush();

    // wait for response (6 bytes: marker + 4 data + checksum)
    if (!this->WaitForRx(6, 100)) {
        Serial.println("RegisterRead(): timeout waiting for response");
        return 0;
    }

    // Read response: marker + 4 data bytes + 1 checksum byte
    uint8_t marker = this->RxBufferPop(); // Skip marker byte (0x7D)
    uint8_t response[4];
    // Per datasheet: CKSUM = 0x33 + ~(CMD1 + CMD2 + sum of all data bytes)
    uint8_t checksum = request[1] + request[2]; // Start with CMD1 + CMD2
    uint32_t result = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t data = this->RxBufferPop();
        checksum += data;
        result |= data << (8 * i);
        response[i] = data;
    }
    // V9381 checksum per datasheet: 0x33 + ~(CMD1 + CMD2 + data bytes)
    checksum = 0x33 + ~(checksum);
    uint8_t checksum_response = this->RxBufferPop();

    // Debug output
    bool checksum_valid = checksum == checksum_response;
    Serial.printf(
        "RegisterRead(0x%02X): marker=0x%02X data=[0x%02X 0x%02X 0x%02X 0x%02X] CRC expected=0x%02X received=0x%02X %s",
        address, marker, response[0], response[1], response[2], response[3], checksum, checksum_response,
        checksum_valid ? "✓" : "✗");

    if (!checksum_valid && this->checksum_mode == ChecksumMode::Clean) {
        Serial.println(" - ERROR: CRC mismatch! (Clean mode)");
    } else if (!checksum_valid && this->checksum_mode == ChecksumMode::Dirty) {
        Serial.println(" - WARNING: CRC mismatch! (Dirty mode - returning data)");
    } else {
        Serial.println();
    }

    return result;
}

void V93XX_UART::ConfigureBlockRead(const uint8_t addresses[], uint8_t num_addresses) {
    uint8_t const *address_ptr = addresses;
    for (int blk = 0; blk < 4; blk++) {
        uint32_t combined_address = 0;
        for (int i = 0; (i < 4) && (num_addresses); i++) {
            combined_address |= (*address_ptr++ << (8 * i));
            num_addresses--;
        }
        RegisterWrite(SYS_BLK_ADDR0 + blk, combined_address);
    }
}

void V93XX_UART::RegisterBlockRead(uint32_t (&values)[], uint8_t num_values) {
    // Described in Section 7.5 of Datasheet
    uint8_t request[4] = {// Header
                          0x7d,

                          // CMD1 (request length, addr, operation)
                          (uint8_t)(((num_values - 1) << 4) | ((this->device_address) << 2) | CmdOperation::BLOCK),
                          // CMD2 (Start address)
                          0x00,

                          // Checksum
                          0x00};

    // Calculate Checksum, Sum of request &0xFF
    request[3] = 0x33 + ~(request[1] + request[2]);

    // Transmit request
    this->serial.write(request, sizeof(request) / sizeof(uint8_t));
    this->serial.flush();

    // wait for response (6 bytes per value: marker + 4 data + checksum)
    if (!this->WaitForRx((6 * num_values) + 1, 200)) {
        Serial.println("RegisterBlockRead(): timeout waiting for response");
        return;
    }

    // Read response: for each value, skip marker then read 4 data bytes, then final checksum
    // Per datasheet: CKSUM = 0x33 + ~(CMD1 + CMD2 + sum of all data bytes)
    uint8_t checksum = request[1] + request[2]; // Start with CMD1 + CMD2
    for (int i = 0; i < num_values; i++) {
        (void)this->RxBufferPop(); // Skip marker byte
        uint8_t response[4];
        for (int j = 0; j < 4; j++) {
            response[j] = this->RxBufferPop();
            checksum += response[j];
        }
        values[i] = response[0] | (response[1] << 8) | (response[2] << 16) | (response[3] << 24);
    }
    // V9381 checksum per datasheet: 0x33 + ~(CMD1 + CMD2 + data bytes)
    checksum = 0x33 + ~(checksum);

    // Validate checksum
    uint8_t response_checksum = this->RxBufferPop();
    bool checksum_valid = checksum == response_checksum;

    Serial.printf("RegisterBlockRead(%d values): CRC expected=0x%02X received=0x%02X %s", num_values, checksum,
                  response_checksum, checksum_valid ? "✓" : "✗");

    if (!checksum_valid && this->checksum_mode == ChecksumMode::Clean) {
        Serial.println(" - ERROR: CRC mismatch! (Clean mode)");
    } else if (!checksum_valid && this->checksum_mode == ChecksumMode::Dirty) {
        Serial.println(" - WARNING: CRC mismatch! (Dirty mode - data captured)");
    } else {
        Serial.println();
    }
}

void V93XX_UART::LoadConfiguration(const V93XX_UART::ControlRegisters &ctrl,
                                   const V93XX_UART::CalibrationRegisters &calibrations) {
    uint32_t checksum = 0;

    // Load control values [0x00 - 0x07]
    for (int i = 0; i < sizeof(V93XX_UART::ControlRegisters) / sizeof(uint32_t); i++) {
        this->RegisterWrite(ControlAddresses[i], ctrl._array[i]);
        checksum += ctrl._array[i];
    }

    // Load calibration values [0x25 - 0x3a]
    for (int i = 0; i < sizeof(V93XX_UART::CalibrationRegisters) / sizeof(uint32_t); i++) {
        this->RegisterWrite(CalibrationAddresses[i], calibrations._array[i]);
        checksum += calibrations._array[i];
    }

    // TODO: Update checksum when configuring [0x55-0x60]

    // The sum of {0x00-0x07, 0x25-0x3a, 0x55-0x60} Needs to equal 0xFFFF_FFFFF to pass self-check.
    // Calculate DSP_CFG_CKSUM (0x28) to ensure this is the case
    checksum = 0xFFFFFFFF - checksum;
    this->RegisterWrite(DSP_CFG_CKSUM, checksum);
}

void V93XX_UART::SetChecksumMode(ChecksumMode mode) {
    this->checksum_mode = mode;
    if (mode == ChecksumMode::Dirty) {
        Serial.println("Checksum Mode: Dirty (skip CRC validation, show expected vs received)");
    } else {
        Serial.println("Checksum Mode: Clean (enforce CRC validation)");
    }
}
