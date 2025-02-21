#ifndef V9XX_H__
#define V9XX_H__

#include <Arduino.h>

class V93XX_Raccoon
{ 
public:
    V93XX_Raccoon(int rx_pin, int tx_pin, HardwareSerial& serial, int device_address);
    void RxReset();
    void Init();
    
    void RegisterWrite(uint8_t address, uint32_t data);
    
    void UartReceive();

private:
    HardwareSerial &serial;
    int device_address;
    int tx_pin;
    int rx_pin;

    enum {
        CMD_BROADCAST = 0,
        CMD_READ = 1,
        CMD_WRITE = 2,
    };
};


#endif