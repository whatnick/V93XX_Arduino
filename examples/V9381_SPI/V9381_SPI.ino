#include "V93XX_SPI.h"

// Define pins for SPI communication
// ESP32-S3 reference (FluidNC wiki): SPI2/VSPI default (IOMUX) pins
// http://wiki.fluidnc.com/en/hardware/ESP32-S3_Pin_Reference
#if defined(ARDUINO_ARCH_ESP32)
const int V93XX_SCK_PIN = 12;
const int V93XX_MOSI_PIN = 11;
const int V93XX_MISO_PIN = 13;
const int V93XX_CS_PIN = 10;
#else
const int V93XX_CS_PIN = 5; // Chip Select pin (adjust for your board)
#endif

// Create SPI driver instance
// Default SPI bus (MOSI, MISO, SCK) and 1MHz clock
V93XX_SPI v9381(V93XX_CS_PIN);

bool v9381_spi_ready = false;

void setup() {
    Serial.begin(115200);
    Serial.print("Starting V9381 SPI Driver\n");

    // Initialize SPI interface
    // Select SPI wiring mode:
    // - FourWire: CS toggles each transaction and enforces >=50us between ops.
    // - ThreeWire: CS is held low and enforces >=400us SCK-low time before ops.
#if defined(ARDUINO_ARCH_ESP32)
    // Explicitly pass SPI pins to use the ESP32-S3 VSPI/IOMUX defaults.
    v9381.Init(V93XX_SPI::WireMode::FourWire, true, V93XX_SPI::ChecksumMode::Dirty, V93XX_SCK_PIN, V93XX_MISO_PIN,
               V93XX_MOSI_PIN);
#else
    v9381.Init(V93XX_SPI::WireMode::FourWire, true, V93XX_SPI::ChecksumMode::Dirty);
#endif

    delay(100); // Allow chip to stabilize

    uint32_t register_value = 0;

    // Ensure SPI interface is initialized by a successful read.
    // Avoid 0x7F on SPI because it is reserved for interface control sequences.
    v9381_spi_ready = v9381.RegisterReadChecked(SYS_INTSTS, register_value);
    if (!v9381_spi_ready) {
        Serial.println("SPI init read failed; pausing further reads.");
        return;
    }
    Serial.printf("Interrupt Register: 0x%08X\n", register_value);

    // Read system status
    register_value = v9381.RegisterRead(SYS_STS);
    Serial.printf("System Status: 0x%08X\n", register_value);

    // Load control and calibration values
    v9381.LoadConfiguration(
        (const V93XX_SPI::ControlRegisters &){
            .DSP_ANA0 = 0x00100C00,  // Analog control 0
            .DSP_ANA1 = 0x000C32C1,  // Analog control 1
            .DSP_CTRL0 = 0x01000f07, // Digital control 0
            .DSP_CTRL1 = 0x000C32C1, // Digital control 1
            .DSP_CTRL2 = 0x00002723, // Digital control 2
            .DSP_CTRL3 = 0x00000000, // Digital control 3
            .DSP_CTRL4 = 0x00000000, // Digital control 4
            .DSP_CTRL5 = 0x00000000  // Digital control 5
        },
        (const V93XX_SPI::CalibrationRegisters &){.DSP_CFG_CALI_PA = 0x00000000,
                                                  .DSP_CFG_DC_PA = 0x00000000,
                                                  .DSP_CFG_CALI_QA = 0x00000000,
                                                  .DSP_CFG_DC_QA = 0x00000000,
                                                  .DSP_CFG_CALI_PB = 0x00000000,
                                                  .DSP_CFG_DC_PB = 0x00000000,
                                                  .DSP_CFG_CALI_QB = 0x00000000,
                                                  .DSP_CFG_DC_QB = 0x00000000,
                                                  .DSP_CFG_CALI_RMSUA = 0x00000000,
                                                  .DSP_CFG_RMS_DCUA = 0x00000000,
                                                  .DSP_CFG_CALI_RMSIA = 0x00000000,
                                                  .DSP_CFG_RMS_DCIA = 0x00000000,
                                                  .DSP_CFG_CALI_RMSIB = 0x00000000,
                                                  .DSP_CFG_RMS_DCIB = 0x00000000,
                                                  .DSP_CFG_PHC = 0x00000000,
                                                  .DSP_CFG_DCUA = 0x00000000,
                                                  .DSP_CFG_DCIA = 0x00000000,
                                                  .DSP_CFG_DCIB = 0x00000000,
                                                  .DSP_CFG_BPF = 0x806764B6,   // Bandpass filter coefficients
                                                  .DSP_CFG_CKSUM = 0x00000000, // Will be auto-calculated
                                                  .EGY_PROCTH = 0x00000000,
                                                  .EGY_PWRTH = 0x00000000});

    Serial.println("Configuration loaded successfully");

    // Configure IO Ports
    v9381.RegisterWrite(SYS_IOCFG0, 0x00000000);
    v9381.RegisterWrite(SYS_IOCFG1, 0x003C3A00);

    // NOTE: The datasheet's "block reading" protocol is documented for UART.
    // In this SPI driver, RegisterBlockRead() is an emulation (sequential SPI reads).
    // It's kept here to avoid rewriting the example logic.
    // Configure block read for commonly accessed registers
    v9381.ConfigureBlockRead(
        (const uint8_t[]){
            DSP_DAT_PA1,    // Active power A
            DSP_DAT_QA1,    // Reactive power A
            DSP_DAT_RMS1UA, // Voltage RMS
            DSP_DAT_RMS1IA, // Current A RMS
            DSP_DAT_RMS1IB, // Current B RMS
            DSP_DAT_FRQ,    // Grid frequency
            DSP_DAT_PB1,    // Active power B
            DSP_DAT_QB1,    // Reactive power B
            DSP_DAT_SA1,    // Apparent power A
            DSP_DAT_SB1     // Apparent power B
        },
        10);

    Serial.println("Block read configured");
    Serial.println("Starting measurements...\n");
}

void loop() {
    if (!v9381_spi_ready) {
        delay(1000);
        return;
    }

    // Perform block read to get all values efficiently
    uint32_t measurement_values[10];
    v9381.RegisterBlockRead(measurement_values, 10);

    // Display measurements
    Serial.println("=== Energy Measurements ===");
    Serial.printf("Active Power A:   0x%08X\n", measurement_values[0]);
    Serial.printf("Reactive Power A: 0x%08X\n", measurement_values[1]);
    Serial.printf("Voltage RMS:      0x%08X\n", measurement_values[2]);
    Serial.printf("Current A RMS:    0x%08X\n", measurement_values[3]);
    Serial.printf("Current B RMS:    0x%08X\n", measurement_values[4]);
    Serial.printf("Grid Frequency:   0x%08X\n", measurement_values[5]);
    Serial.printf("Active Power B:   0x%08X\n", measurement_values[6]);
    Serial.printf("Reactive Power B: 0x%08X\n", measurement_values[7]);
    Serial.printf("Apparent Power A: 0x%08X\n", measurement_values[8]);
    Serial.printf("Apparent Power B: 0x%08X\n", measurement_values[9]);
    Serial.println();

    // Alternative: Read individual registers
    // uint32_t voltage = v9381.RegisterRead(DSP_DAT_RMS1UA);
    // Serial.printf("Voltage: 0x%08X\n", voltage);

    delay(1000); // Update every second
}
