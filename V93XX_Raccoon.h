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

    enum CmdOperation {
        BROADCAST = 0,
        READ = 1,
        WRITE = 2,
    };
};


#endif