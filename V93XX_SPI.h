#ifndef V93XX_SPI_H__
#define V93XX_SPI_H__

#include <Arduino.h>
#include <SPI.h>
#include "V93XX_Raccoon_registers.h"

/**
 * @brief Driver class for V9381 SPI interface
 * 
 * This class provides an SPI interface to communicate with the V9381 energy monitoring ASIC.
 * The V9381 supports SPI communication with configurable clock speeds and standard SPI modes.
 */
class V93XX_SPI
{ 
public:
    // Reuse register structures from UART version for compatibility
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

    /**
     * @brief Constructor for V9381 SPI driver
     * @param cs_pin Chip select pin number
     * @param spi_bus Reference to SPI bus (default SPIClass)
     * @param spi_freq SPI clock frequency in Hz (default 1MHz)
     */
    V93XX_SPI(int cs_pin, SPIClass& spi_bus = SPI, uint32_t spi_freq = 1000000);
    
    /**
     * @brief Initialize SPI interface and reset the V9381 chip
     */
    void Init();
    
    /**
     * @brief Write a value to a register
     * @param address Register address
     * @param data 32-bit data value to write
     */
    void RegisterWrite(uint8_t address, uint32_t data);
    
    /**
     * @brief Read a value from a register
     * @param address Register address
     * @return 32-bit register value
     */
    uint32_t RegisterRead(uint8_t address);

    /**
     * @brief Configure registers for block read operation
     * @param addresses Array of register addresses to read
     * @param num_addresses Number of addresses in the array
     */
    void ConfigureBlockRead(const uint8_t addresses[], uint8_t num_addresses);
    
    /**
     * @brief Perform a block read of configured registers
     * @param values Array to store read values
     * @param num_values Number of values to read
     */
    void RegisterBlockRead(uint32_t (&values)[], uint8_t num_values);

    /**
     * @brief Load complete configuration (control and calibration registers)
     * @param ctrl Control register values
     * @param calibrations Calibration register values
     */
    void LoadConfiguration(const ControlRegisters& ctrl, const CalibrationRegisters& calibrations);
    
private:
    SPIClass &spi_bus;
    int cs_pin;
    uint32_t spi_freq;
    SPISettings spi_settings;
    
    /**
     * @brief Calculate CRC8 checksum for SPI packets
     * @param data Pointer to data buffer
     * @param length Length of data
     * @return CRC8 checksum value
     */
    uint8_t CalculateCRC8(const uint8_t* data, size_t length);
    
    /**
     * @brief Begin SPI transaction with chip select
     */
    inline void BeginTransaction();
    
    /**
     * @brief End SPI transaction and release chip select
     */
    inline void EndTransaction();
    
    enum CmdOperation {
        BROADCAST = 0,
        READ = 1,
        WRITE = 2,
        BLOCK = 3,
    };
};

#endif
