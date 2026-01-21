
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

void V93XX_SPI::Init() {
    // Configure CS pin
    pinMode(this->cs_pin, OUTPUT);
    digitalWrite(this->cs_pin, HIGH);
    
    // Initialize SPI bus
    this->spi_bus.begin();
    
    // Small delay for chip stabilization
    delay(10);
}

inline void V93XX_SPI::BeginTransaction() {
    this->spi_bus.beginTransaction(this->spi_settings);
    digitalWrite(this->cs_pin, LOW);
    delayMicroseconds(1); // CS setup time
}

inline void V93XX_SPI::EndTransaction() {
    delayMicroseconds(1); // CS hold time
    digitalWrite(this->cs_pin, HIGH);
    this->spi_bus.endTransaction();
}

uint8_t V93XX_SPI::CalculateCRC8(const uint8_t* data, size_t length) {
    // CRC-8 calculation similar to UART checksum
    // Sum all bytes and apply the same algorithm as UART: 0x33 + ~sum
    uint8_t checksum = 0;
    for(size_t i = 0; i < length; i++){
        checksum += data[i];
    }
    return 0x33 + ~checksum;
}

void V93XX_SPI::RegisterWrite(uint8_t address, uint32_t data) {
    const int num_registers = 1;
    
    // SPI packet structure (similar to UART but without 0x7d header for SPI)
    uint8_t payload[7] = {
        // CMD1 (Payload length, addr=0, operation)
        (uint8_t)(((num_registers - 1) << 4) | CmdOperation::WRITE),
        // CMD2 (7b Address)
        (uint8_t)(address & 0x7f),
        // Data (32b, LSB first)
        (uint8_t)(data & 0x000000ff),
        (uint8_t)((data & 0x0000ff00) >> 8),
        (uint8_t)((data & 0x00ff0000) >> 16),
        (uint8_t)((data & 0xff000000) >> 24),
        // Checksum
        0x00
    };

    // Calculate Checksum
    payload[6] = CalculateCRC8(payload, 6);

    // Transmit payload
    BeginTransaction();
    for(size_t i = 0; i < sizeof(payload); i++){
        this->spi_bus.transfer(payload[i]);
    }
    
    // Read response checksum
    uint8_t checksum_response = this->spi_bus.transfer(0x00);
    EndTransaction();

    // Validate response
    bool checksum_valid = checksum_response == payload[6];
    if(!checksum_valid){
        Serial.printf("RegisterWrite(): Checksum invalid (expected: 0x%02X, received: 0x%02X)\n", payload[6], checksum_response);
    }
}

uint32_t V93XX_SPI::RegisterRead(uint8_t address) {
    const int num_registers = 1;
    
    // SPI read request packet
    uint8_t request[3] = {
        // CMD1 (request length, addr=0, operation)
        (uint8_t)(((num_registers - 1) << 4) | CmdOperation::READ),
        // CMD2 (7b Address)
        (uint8_t)(address & 0x7f),
        // Checksum
        0x00
    };

    // Calculate Checksum
    request[2] = CalculateCRC8(request, 2);

    // Transmit request and read response
    BeginTransaction();
    
    // Send request
    for(size_t i = 0; i < sizeof(request); i++){
        this->spi_bus.transfer(request[i]);
    }
    
    // Read 4 bytes of data
    uint8_t response[4];
    uint8_t checksum = request[0] + request[1];
    for(int i = 0; i < 4; i++){
        response[i] = this->spi_bus.transfer(0x00);
        checksum += response[i];
    }
    
    // Calculate expected checksum
    checksum = 0x33 + ~checksum;
    
    // Read checksum byte
    uint8_t checksum_response = this->spi_bus.transfer(0x00);
    EndTransaction();
    
    // Validate checksum
    bool checksum_valid = checksum == checksum_response;
    if(!checksum_valid){
        Serial.printf("RegisterRead(): Checksum invalid (expected: 0x%02X, received: 0x%02X)\n", checksum, checksum_response);
    }
    
    // Combine bytes into 32-bit result (LSB first)
    uint32_t result = response[0] | (response[1] << 8) | (response[2] << 16) | (response[3] << 24);
    return result;
}

void V93XX_SPI::ConfigureBlockRead(const uint8_t addresses[], uint8_t num_addresses)
{
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
    // SPI block read request
    uint8_t request[3] = {
        // CMD1 (request length, addr=0, operation)
        (uint8_t)(((num_values - 1) << 4) | CmdOperation::BLOCK),
        // CMD2 (Start address - typically 0 for block read)
        0x00,
        // Checksum
        0x00
    };

    // Calculate Checksum
    request[2] = CalculateCRC8(request, 2);

    // Transmit request
    BeginTransaction();
    for(size_t i = 0; i < sizeof(request); i++){
        this->spi_bus.transfer(request[i]);
    }
    
    // Read response
    uint8_t checksum = request[0] + request[1];
    for(int i = 0; i < num_values; i++){
        uint8_t response[4];
        for(int j = 0; j < 4; j++){
            response[j] = this->spi_bus.transfer(0x00);
            checksum += response[j];
        }
        values[i] = response[0] | (response[1] << 8) | (response[2] << 16) | (response[3] << 24);
    }
    
    // Calculate expected checksum
    checksum = 0x33 + ~checksum;
    
    // Read checksum byte
    uint8_t response_checksum = this->spi_bus.transfer(0x00);
    EndTransaction();
    
    // Validate checksum
    bool checksum_valid = checksum == response_checksum;
    if(!checksum_valid){
        Serial.printf("RegisterBlockRead(): Checksum invalid (expected: 0x%02X, received: 0x%02X)\n", checksum, response_checksum);
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
