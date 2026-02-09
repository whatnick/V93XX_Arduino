#include "V93XX_Raccoon.h"

// V9381 UART example using the same wires as SPI on ESP32-S3.
// RX/MOSI and TX/MISO share the same pins, and A0/A1 share SCK/CS.
// This lets you switch between UART and SPI without rewiring, only reconfiguring pins.

#if defined(ARDUINO_ARCH_ESP32)
// ESP32-S3 reference (FluidNC wiki): SPI2/VSPI default (IOMUX) pins
// http://wiki.fluidnc.com/en/hardware/ESP32-S3_Pin_Reference
const int V93XX_UART_TX_PIN = 11; // MOSI -> V9381 RX/MOSI
const int V93XX_UART_RX_PIN = 13; // MISO <- V9381 TX/MISO
const int V93XX_ADDR0_PIN = 12;   // A0/SPCK (shared with SCK)
const int V93XX_ADDR1_PIN = 10;   // A1/SPCSN (shared with CS)
#else
const int V93XX_UART_TX_PIN = 16; // Adjust for your board
const int V93XX_UART_RX_PIN = 15; // Adjust for your board
const int V93XX_ADDR0_PIN = 12;   // Adjust for your board
const int V93XX_ADDR1_PIN = 10;   // Adjust for your board
#endif

const int V93XX_DEVICE_ADDRESS = 0x00;

V93XX_Raccoon v9381(V93XX_UART_RX_PIN, V93XX_UART_TX_PIN, Serial1, V93XX_DEVICE_ADDRESS);

static void ConfigureUartAddressPins(int address) {
    pinMode(V93XX_ADDR0_PIN, OUTPUT);
    pinMode(V93XX_ADDR1_PIN, OUTPUT);

    digitalWrite(V93XX_ADDR0_PIN, (address & 0x01) ? HIGH : LOW);
    digitalWrite(V93XX_ADDR1_PIN, (address & 0x02) ? HIGH : LOW);
}

static void ReleaseUartAddressPinsForSpi() {
    // When switching to SPI, release A0/A1 so SPI can drive SCK/CS.
    pinMode(V93XX_ADDR0_PIN, INPUT);
    pinMode(V93XX_ADDR1_PIN, INPUT);
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting V9381 UART (shared bus example)");

    ConfigureUartAddressPins(V93XX_DEVICE_ADDRESS);

    // NOTE: If you later switch to SPI in the same sketch, call
    // ReleaseUartAddressPinsForSpi() before SPI.begin().

    v9381.RxReset();
    v9381.Init();

    uint32_t register_value = v9381.RegisterRead(SYS_INTSTS);
    Serial.printf("Interrupt Register: 0x%08X\n", register_value);

    register_value = v9381.RegisterRead(SYS_VERSION);
    Serial.printf("System Version: 0x%08X\n", register_value);

    v9381.LoadConfiguration(
        (const V93XX_Raccoon::ControlRegisters &){
            .DSP_ANA0 = 0x00100C00,
            .DSP_ANA1 = 0x000C32C1,
            .DSP_CTRL0 = 0x01000F07,
            .DSP_CTRL1 = 0x000C32C1,
            .DSP_CTRL2 = 0x00002723,
            .DSP_CTRL3 = 0x00000000,
            .DSP_CTRL4 = 0x00000000,
            .DSP_CTRL5 = 0x00000000,
        },
        (const V93XX_Raccoon::CalibrationRegisters &){
            .DSP_CFG_CALI_PA = 0x00000000,
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
            .DSP_CFG_BPF = 0x806764B6,
            .DSP_CFG_CKSUM = 0x00000000,
            .EGY_PROCTH = 0x00000000,
            .EGY_PWRTH = 0x00000000,
        });

    v9381.RegisterWrite(SYS_IOCFG0, 0x00000000);
    v9381.RegisterWrite(SYS_IOCFG1, 0x003C3A00);

    v9381.ConfigureBlockRead(
        (const uint8_t[]){
            DSP_DAT_PA1,
            DSP_DAT_QA1,
            DSP_DAT_RMS1UA,
            DSP_DAT_RMS1IA,
            DSP_DAT_RMS1IB,
            DSP_DAT_FRQ,
            DSP_DAT_PB1,
            DSP_DAT_QB1,
            DSP_DAT_SA1,
            DSP_DAT_SB1,
        },
        10);
}

void loop() {
    uint32_t values[10] = {0};
    v9381.RegisterBlockRead(values, 10);

    Serial.println("=== Energy Measurements ===");
    Serial.printf("Active Power A:   0x%08X\n", values[0]);
    Serial.printf("Reactive Power A: 0x%08X\n", values[1]);
    Serial.printf("Voltage RMS:      0x%08X\n", values[2]);
    Serial.printf("Current A RMS:    0x%08X\n", values[3]);
    Serial.printf("Current B RMS:    0x%08X\n", values[4]);
    Serial.printf("Grid Frequency:   0x%08X\n", values[5]);
    Serial.printf("Active Power B:   0x%08X\n", values[6]);
    Serial.printf("Reactive Power B: 0x%08X\n", values[7]);
    Serial.printf("Apparent Power A: 0x%08X\n", values[8]);
    Serial.printf("Apparent Power B: 0x%08X\n", values[9]);
    Serial.println();

    delay(1000);
}
