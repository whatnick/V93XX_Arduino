
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
    this->serial.begin(4800, SerialConfig::SERIAL_8O1, this->rx_pin, this->tx_pin);
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
        (uint8_t)(((num_registers - 1) << 4) | ((this->device_address) << 2) | CMD_WRITE),
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

    // Calculate Checksum, Sum of payload &0xFF
    for(int idx = 1 ; idx < sizeof(payload)/sizeof(uint8_t) - 1; idx++){
        payload[7] += payload[idx];
    }
    payload[7] = 0x33 + ~payload[7];


    // Transmit payload
    this->serial.write(payload, sizeof(payload)/sizeof(uint8_t));
}
