#ifndef V93XX_UART_H__
#define V93XX_UART_H__

#include "V93XX_Registers.h"
#include <Arduino.h>
#include <list>
#include <queue>

class V93XX_UART {
  public:
    enum class ChecksumMode : uint8_t {
        Dirty = 0,
        Clean = 1,
    };

    struct __attribute__((packed)) ControlRegisters {
        union {
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
            };
        };
    };

    struct __attribute__((packed)) CalibrationRegisters {
        union {
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

    V93XX_UART(int rx_pin, int tx_pin, HardwareSerial &serial, int device_address);
    void RxReset();
    void Init(SerialConfig config = SerialConfig::SERIAL_8O1, ChecksumMode checksum_mode = ChecksumMode::Dirty);

    void RegisterWrite(uint8_t address, uint32_t data);
    uint32_t RegisterRead(uint8_t address);

    void ConfigureBlockRead(const uint8_t addresses[], uint8_t num_addresses);
    void RegisterBlockRead(uint32_t (&values)[], uint8_t num_values);

    bool CaptureWaveform(uint32_t *buffer, size_t word_count, uint32_t ctrl5, uint32_t timeout_ms = 1000,
                         uint8_t block_words = 16);

    void LoadConfiguration(const ControlRegisters &ctrl, const CalibrationRegisters &calibrations);

    void SetChecksumMode(ChecksumMode mode);

  private:
    HardwareSerial &serial;
    int device_address;
    int tx_pin;
    int rx_pin;
    ChecksumMode checksum_mode = ChecksumMode::Dirty;

    std::queue<uint8_t, std::list<uint8_t>> serial_rx_buffer{std::queue<uint8_t, std::list<uint8_t>>()};

    void RxReceive();
    uint8_t RxBufferPop();
    unsigned int RxBufferCount();
    bool WaitForRx(size_t count, uint32_t timeout_ms);

    enum CmdOperation {
        BROADCAST = 0,
        READ = 1,
        WRITE = 2,
        BLOCK = 3,
    };
};

#endif
