
#include "V93XX_Raccoon.h"

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
    this->serial.onReceive(std::bind(&V93XX_Raccoon::UartReceive, this));
}

void V93XX_Raccoon::UartReceive() {

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
