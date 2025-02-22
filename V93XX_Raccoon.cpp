
#include "V93XX_Raccoon.h"

const uint8_t ControlAddresses[] = {
    REG_DSP_ANA0,   // Analog Control 0
    REG_DSP_ANA1,   // Analog Control 1
    REG_DSP_CTRL0,  // Digital Control 0
    REG_DSP_CTRL1,  // Digital Control 1
    REG_DSP_CTRL2,  // Digital Control 2
    REG_DSP_CTRL3,  // Digital Control 3
    REG_DSP_CTRL4,  // Digital Control 4
    REG_DSP_CTRL5,  // Digital Control 5
};

const uint8_t CalibrationAddresses[] = {
    REG_DSP_CFG_CALI_PA,    // Active power A ratio difference correction
    REG_DSP_CFG_DC_PA,      //  Active power A small signal correction
    REG_DSP_CFG_CALI_QA,    //	Reactive power A ratio difference correction
    REG_DSP_CFG_DC_QA,      //	Reactive power A small signal correction
    REG_DSP_CFG_CALI_PB,    //	Active power B ratio difference correction
    REG_DSP_CFG_DC_PB,      //	Active power B small signal correction
    REG_DSP_CFG_CALI_QB,    //	Reactive power B ratio difference correction
    REG_DSP_CFG_DC_QB,      //	Reactive power B small signal correction
    REG_DSP_CFG_CALI_RMSUA, //	Voltage RMS ratio correction
    REG_DSP_CFG_RMS_DCUA,   //	Voltage RMS small signal correction
    REG_DSP_CFG_CALI_RMSIA, //	Current A effective value ratio correction
    REG_DSP_CFG_RMS_DCIA,   //	Current A RMS small signal correction
    REG_DSP_CFG_CALI_RMSIB, //	Current B effective value ratio difference correction
    REG_DSP_CFG_RMS_DCIB,   //	Current B RMS small signal correction
    REG_DSP_CFG_PHC,        //	Angular error correction register
    REG_DSP_CFG_DCUA,       //	Voltage channel DC correction value
    REG_DSP_CFG_DCIA,       //	Current A channel DC correction value
    REG_DSP_CFG_DCIB,       //	Current B channel DC correction value
    REG_DSP_CFG_BPF,        //	Bandpass filter coefficients
    REG_DSP_CFG_CKSUM,      //	Checksum Configuration Registers
    REG_EGY_PROCTH,         //	Energy register anti-submarine threshold (less than it will not accumulate)
    REG_EGY_PWRTH,          //	Energy register accumulation threshold. Since the energy register is 46 bits
};

V93XX_Raccoon::V93XX_Raccoon(int rx_pin, int tx_pin, HardwareSerial& serial, int device_address) : serial(serial) {
    this->device_address = device_address;
    this->tx_pin = tx_pin;
    this->rx_pin = rx_pin;
}

void V93XX_Raccoon::RxReset() {
    pinMode(this->tx_pin, OUTPUT);

    //TX pin for UART , RX pin for ASIC need to be held low
    digitalWrite(this->tx_pin, HIGH);
    delayMicroseconds(3000);
    digitalWrite(this->tx_pin, LOW);


    delayMicroseconds(92500);
    digitalWrite(this->tx_pin, HIGH);
    delayMicroseconds(2150);
}

void V93XX_Raccoon::Init() {
    this->serial.begin(19200, SerialConfig::SERIAL_8O1, this->rx_pin, this->tx_pin);
}

void V93XX_Raccoon::RegisterWrite(uint8_t address, uint32_t data) {
    uint8_t i;

    const int num_registers = 1;
    // Described in Section 7.4 of Datasheet
    uint8_t payload[8] = {
        // Header
        0x7d,

        // CMD1 (Payload length, addr, operation)
        (uint8_t)(((num_registers - 1) << 4) | ((this->device_address) << 2) | CmdOperation::WRITE),
        // CMD2 (7b Address)
        (uint8_t)(address & 0x7f),

        // Data (32b)
        (uint8_t)(data & 0x000000ff),
        (uint8_t)((data & 0x0000ff00) >> 8),
        (uint8_t)((data & 0x00ff0000) >> 16),
        (uint8_t)((data & 0xff000000) >> 24),
        
        // Checksum
        0x00
    };

    uint8_t checksum = 0;
    // Calculate Checksum, Sum of payload &0xFF
    for(int idx = 1 ; idx < sizeof(payload)/sizeof(uint8_t) - 1; idx++){
        checksum += payload[idx];
    }
    checksum = 0x33 + ~checksum;
    payload[7] = checksum;

    // Transmit payload
    this->serial.write(payload, sizeof(payload)/sizeof(uint8_t));

    // wait for response
    while(this->serial.available() < 1){
        delay(1);
    }

    // Read response
    uint8_t response[1];
    this->serial.readBytes(response, 1);

    // TODO:: Proper error handling
    bool checksum_valid = response[0] == checksum;
    if(!checksum_valid){
        Serial.printf("RegisterWrite(): Checksum invalid (expected: h%02X, received: h%02X)\n", checksum, response[4]);
    }
}


uint32_t V93XX_Raccoon::RegisterRead(uint8_t address) {
    uint8_t i;

    const int num_registers = 1;
    // Described in Section 7.3 of Datasheet
    uint8_t request[4] = {
        // Header
        0x7d,

        // CMD1 (request length, addr, operation)
        (uint8_t)(((num_registers - 1) << 4) | ((this->device_address) << 2) | CmdOperation::READ),
        // CMD2 (7b Address)
        (uint8_t)(address & 0x7f),
                
        // Checksum
        0x00
    };

    // Calculate Checksum, Sum of request &0xFF
    request[3] = 0x33 + ~(request[1] + request[2]);

    // Transmit request
    this->serial.write(request, sizeof(request)/sizeof(uint8_t));

    // wait for response
    while(this->serial.available() < 5){
        delay(1);
    }

    // Read response
    uint8_t response[5];
    this->serial.readBytes(response, 5);

    uint32_t result = response[0] | (response[1] << 8) | (response[2] << 16) | (response[3] << 24);
    uint8_t checksum = 0x33 + ~(request[1] + request[2] + response[0] + response[1] + response[2] + response[3]);

    // TODO:: Proper error handling
    bool checksum_valid = checksum == response[4];
    if(!checksum_valid){
        Serial.printf("RegisterRead(): Checksum invalid (expected: h%02X, received: h%02X)\n", checksum, response[4]);
    }
    return result;
}

void V93XX_Raccoon::ConfigureBlockRead(const uint8_t addresses[], uint8_t num_addresses)
{
    for (int blk = 0; blk < 4; blk++){
        uint32_t combined_address = 0;
        for (int i = 0; (i < 4) && (num_addresses); i++) {
            combined_address |= (addresses[i] << (8 * i));
            num_addresses--;
        }
        RegisterWrite(REG_SYS_BLK_ADDR0 + blk, combined_address);
    }
}

void V93XX_Raccoon::RegisterBlockRead(uint32_t (&values)[], uint8_t num_values)
{
    // Described in Section 7.5 of Datasheet
    uint8_t request[4] = {
        // Header
        0x7d,

        // CMD1 (request length, addr, operation)
        (uint8_t)(((num_values - 1) << 4) | ((this->device_address) << 2) | CmdOperation::BLOCK),
        // CMD2 (Start address)
        0x00,
                
        // Checksum
        0x00
    };

    // Calculate Checksum, Sum of request &0xFF
    request[3] = 0x33 + ~(request[1] + request[2]);

    // Transmit request
    this->serial.write(request, sizeof(request)/sizeof(uint8_t));

    // wait for response
    while(this->serial.available() < ((4*num_values)+1)){
        delay(1);
    }

    // Read response
    uint8_t checksum = request[1] + request[2];
    for(int i = 0; i < num_values; i++){
        uint8_t response[4];
        this->serial.readBytes(response, 4);
        values[i] = response[0] | (response[1] << 8) | (response[2] << 16) | (response[3] << 24);
        checksum += response[0] + response[1] + response[2] + response[3];
    }
    checksum = 0x33 + ~(checksum);

    // TODO:: Proper error handling
    uint8_t respose_checksum = this->serial.read();
    bool checksum_valid = checksum == respose_checksum;
    if(!checksum_valid){
        Serial.printf("RegisterBlockRead(): Checksum invalid (expected: h%02X, received: h%02X)\n", checksum, respose_checksum);
    }
}

void V93XX_Raccoon::LoadConfiguration(const V93XX_Raccoon::ControlRegisters& ctrl, const V93XX_Raccoon::CalibrationRegisters& calibrations) {
    uint8_t checksum = 0;
    
    // Load control values [0x00 - 0x07]
    for (int i = 0; i < sizeof(V93XX_Raccoon::ControlRegisters)/sizeof(uint32_t); i++){
        this->RegisterWrite(ControlAddresses[i], ctrl._array[i]);
        checksum += ctrl._array[i];
    }

    // Load calibration values [0x25 - 0x3a]
    for (int i = 0; i < sizeof(V93XX_Raccoon::CalibrationRegisters)/sizeof(uint32_t); i++){
        this->RegisterWrite(CalibrationAddresses[i], calibrations._array[i]);
        checksum += calibrations._array[i];
    }

    // TODO: Update checksum when configuring [0x55-0x60]

    // The sum of {0x00-0x07, 0x25-0x3a, 0x55-0x60} Needs to equal 0xFFFF_FFFFF to pass self-check.
    // Calculate DSP_CFG_CKSUM (0x28) to ensure this is the case
    checksum = 0xFFFFFFFF - checksum;
    this->RegisterWrite(checksum, REG_DSP_CFG_CKSUM);
}