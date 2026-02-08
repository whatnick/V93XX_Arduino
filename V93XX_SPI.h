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
    enum class WireMode : uint8_t {
        FourWire = 0,
        ThreeWire = 1,
    };

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
        * @brief Initialize SPI bus + chip-select behavior.
        *
        * Per datasheet, after RSTN/RX/global SW reset the chip defaults to UART.
        * If @p initialize_interface is true, this will also send the required SPI
        * interface initialization write to address 0x7F.
     */
        void Init(WireMode wire_mode = WireMode::FourWire, bool initialize_interface = true);

    /**
     * @brief Initialize SPI bus with explicit pin mapping (useful on ESP32/ESP32-S3).
     *
     * Uses `spi_bus.begin(sck, miso, mosi, cs_pin)` when pin values are provided.
     */
    void Init(WireMode wire_mode, bool initialize_interface, int8_t sck_pin, int8_t miso_pin, int8_t mosi_pin);

        /**
        * @brief Perform the SPI interface initialization sequence (write magic to 0x7F).
        * @return true if a follow-up read produced a valid checksum, false otherwise.
        */
        bool InitializeInterface();

        /**
        * @brief Enable/disable the automatic +0x80 address offset mode (datasheet high-address access).
        *
        * When enabled, the chip adds 0x80 to the 7-bit address carried in CMD.
        */
        void SetHighAddressOffsetEnabled(bool enabled);
    
    /**
     * @brief Write a value to a register
     * @param address Register address
     * @param data 32-bit data value to write
     */
    void RegisterWrite(uint8_t address, uint32_t data);

    /**
     * @brief Write a value to a register and optionally get a success indication.
     *
     * Note: datasheet states SPI write does not return a valid response; this returns true
     * if the frame was sent, not that the chip committed the write.
     */
    bool RegisterWriteChecked(uint8_t address, uint32_t data);
    
    /**
     * @brief Read a value from a register
     * @param address Register address
     * @return 32-bit register value
     */
    uint32_t RegisterRead(uint8_t address);

    /**
     * @brief Read a register and validate the checksum.
     * @param address Register address
     * @param out_value Filled with 32-bit value on success
     * @return true if checksum valid, false otherwise
     */
    bool RegisterReadChecked(uint8_t address, uint32_t &out_value);

    /**
     * @brief Configure registers for block read operation
     * @param addresses Array of register addresses to read
     * @param num_addresses Number of addresses in the array
        *
        * Note: The datasheet's "block reading" protocol is documented under UART.
        * The SPI section does not define a dedicated block-read command/frame.
        * This library keeps this API for convenience/compatibility; see RegisterBlockRead().
     */
    void ConfigureBlockRead(const uint8_t addresses[], uint8_t num_addresses);
    
    /**
     * @brief Perform a block read of configured registers
     * @param values Array to store read values
     * @param num_values Number of values to read
        *
        * IMPORTANT: For SPI, this function is implemented as repeated single-register reads
        * (one 48-clock SPI read per register). If you need the true mapped "block read"
        * feature, use the UART driver/protocol.
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

    WireMode wire_mode = WireMode::FourWire;
    bool high_address_offset_enabled = false;
    uint32_t last_op_end_us = 0;

    uint8_t configured_block_addrs[16] = {0};
    uint8_t configured_block_addr_count = 0;
    
    /**
     * @brief Calculate CRC8 checksum for SPI packets
     * @param data Pointer to data buffer
     * @param length Length of data
     * @return CRC8 checksum value
     */
    uint8_t CalculateCRC8(const uint8_t* data, size_t length);

    uint8_t BuildCmdByte(uint8_t address7, bool is_read) const;
    void EnforceInterOpTiming();
    void ApplyAddressOffsetModeIfNeeded(uint8_t address);
    
    /**
     * @brief Begin SPI transaction with chip select
     */
    inline void BeginTransaction();
    
    /**
     * @brief End SPI transaction and release chip select
     */
    inline void EndTransaction();
};

#endif
