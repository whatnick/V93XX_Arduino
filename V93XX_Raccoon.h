#ifndef V93XX_RACCOON_H__
#define V93XX_RACCOON_H__

#include <Arduino.h>
#include "V93XX_Raccoon_registers.h"

class V93XX_Raccoon
{ 
public:
    __attribute__((packed)) struct ControlRegisters {
        union
        {
            uint32_t _array[8];
            struct {
                uint32_t DSP_ANA0;
                uint32_t DSP_ANA1;
                uint32_t DSP_CTRL0;
                uint32_t DSP_CTRL1;
                uint32_t DSP_CTRL2;
                uint32_t DSP_CTRL3;
                uint32_t DSP_CTRL4;
                uint32_t DSP_CTRL5;
                uint32_t DSP_CFG_BPF;
            };
        };        
    };

    __attribute__((packed)) struct CalibrationRegisters {
        union
        {
            uint32_t _array[22];
            struct {
                uint32_t DSP_CFG_CALI_PA;
                uint32_t DSP_CFG_DC_PA;
                uint32_t DSP_CFG_CALI_QA;
                uint32_t DSP_CFG_DC_QA;
                uint32_t DSP_CFG_CALI_PB;
                uint32_t DSP_CFG_DC_PB;
                uint32_t DSP_CFG_CALI_QB;
                uint32_t DSP_CFG_DC_QB;
                uint32_t DSP_CFG_CALI_RMSUA;
                uint32_t DSP_CFG_RMS_DCUA;
                uint32_t DSP_CFG_CALI_RMSIA;
                uint32_t DSP_CFG_RMS_DCIA;
                uint32_t DSP_CFG_CALI_RMSIB;
                uint32_t DSP_CFG_RMS_DCIB;
                uint32_t DSP_CFG_PHC;
                uint32_t DSP_CFG_DCUA;
                uint32_t DSP_CFG_DCIA;
                uint32_t DSP_CFG_DCIB;
                uint32_t DSP_CFG_BPF;
                uint32_t DSP_CFG_CKSUM;
                uint32_t EGY_PROCTH;
                uint32_t EGY_PWRTH;
            };
        };
    };

    V93XX_Raccoon(int rx_pin, int tx_pin, HardwareSerial& serial, int device_address);
    void RxReset();
    void Init();
    
    void RegisterWrite(uint8_t address, uint32_t data);
    uint32_t RegisterRead(uint8_t address);

    void LoadConfiguration(const ControlRegisters& ctrl, const CalibrationRegisters& calibrations);
    
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