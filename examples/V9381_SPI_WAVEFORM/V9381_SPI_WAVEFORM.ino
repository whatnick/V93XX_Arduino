#include "V93XX_SPI.h"

// SPI pin definitions (ESP32-S3 VSPI/IOMUX defaults)
#if defined(ARDUINO_ARCH_ESP32)
const int V93XX_SCK_PIN = 12;
const int V93XX_MOSI_PIN = 11;
const int V93XX_MISO_PIN = 13;
const int V93XX_CS_PIN = 10;
#else
const int V93XX_CS_PIN = 5;
#endif

V93XX_SPI v9381(V93XX_CS_PIN);

void setup() {
    Serial.begin(115200);
    Serial.println("V9381 SPI Waveform Capture");

    // Initialize SPI with Dirty mode for CRC tolerance
#if defined(ARDUINO_ARCH_ESP32)
    v9381.Init(V93XX_SPI::WireMode::FourWire, true, V93XX_SPI::ChecksumMode::Dirty, V93XX_SCK_PIN, V93XX_MISO_PIN,
               V93XX_MOSI_PIN);
#else
    v9381.Init(V93XX_SPI::WireMode::FourWire, true, V93XX_SPI::ChecksumMode::Dirty);
#endif

    delay(100); // Chip stabilization

    // Verify SPI communication
    uint32_t register_value = 0;
    if (!v9381.RegisterReadChecked(SYS_INTSTS, register_value)) {
        Serial.println("ERROR: SPI communication failed");
        while (1)
            delay(10);
    }
    Serial.printf("SPI Ready. SYS_INTSTS: 0x%08X\n", register_value);

    // Read system version
    register_value = v9381.RegisterRead(SYS_VERSION);
    Serial.printf("System Version: 0x%08X\n", register_value);

    // Load V9381 configuration
    v9381.LoadConfiguration((const V93XX_SPI::ControlRegisters &){.DSP_ANA0 = 0x00100C00,
                                                                  .DSP_ANA1 = 0x000C32C1,
                                                                  .DSP_CTRL0 = 0x01000f07,
                                                                  .DSP_CTRL1 = 0x000C32C1,
                                                                  .DSP_CTRL2 = 0x00002723,
                                                                  .DSP_CTRL3 = 0x00000000,
                                                                  .DSP_CTRL4 = 0x00000000,
                                                                  .DSP_CTRL5 = 0x00000000},
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
                                                                      .DSP_CFG_BPF = 0x806764B6,
                                                                      .DSP_CFG_CKSUM = 0x00000000,
                                                                      .EGY_PROCTH = 0x00000000,
                                                                      .EGY_PWRTH = 0x00000000});

    // Configure IO ports
    v9381.RegisterWrite(SYS_IOCFG0, 0x00000000);
    v9381.RegisterWrite(SYS_IOCFG1, 0x003C3A00);

    // Clear interrupt status
    v9381.RegisterWrite(SYS_INTSTS, SYS_INTSTS_CKERR);

    Serial.println("Configuration complete. Starting waveform capture...\n");
}

void loop() {
    // Allocate waveform buffer (309 words = 618 samples)
    uint32_t waveform_buffer[309] = {0};

    // Configure DSP_CTRL5 for waveform capture
    // - Manual single trigger mode
    // - Channel UA (Voltage A)
    // - 512-sample length (Wave_Len=0)
    const uint32_t ctrl5 = ((0 << DSP_CTRL5_DMAMODE_Pos) & DSP_CTRL5_DMAMODE_Msk) | DSP_CTRL5_DMA_CTRL_ENABLE |
                           DSP_CTRL5_WAVE_U | ((0 << DSP_CTRL5_WAVE_LEN_Pos) & DSP_CTRL5_WAVE_LEN_Msk) |
                           DSP_CTRL5_WAVEMEM_MODE_MANUAL_SINGLE;

    // Capture waveform using CaptureWaveform() API
    // SPI is fast, so default timeout (1000ms) and block size (16) work well
    bool capture_ok = v9381.CaptureWaveform(waveform_buffer, 309, ctrl5);
    if (!capture_ok) {
        Serial.println("Waveform capture failed or timed out");
        delay(1000);
        return;
    }

    // Print waveform data as Python-compatible array
    // Each 32-bit word contains two 16-bit signed samples (lower, upper)
    Serial.println("\nWaveform Data (Python format):");
    Serial.println("wave_data = [");
    for (int i = 0; i < 309; i++) {
        int16_t lower = (int16_t)(waveform_buffer[i] & 0xFFFF);
        int16_t upper = (int16_t)((waveform_buffer[i] >> 16) & 0xFFFF);

        // Format: 8 pairs per line, comma-separated
        Serial.printf("%5d, %5d%s", lower, upper, (i != 308) ? ((i % 8 == 7) ? ",\n" : ", ") : "");
    }
    Serial.println("\n]");

    Serial.printf("\nCaptured 309 words (618 samples)\n");
    Serial.println("Copy data to Python for plotting with tools/plot_v9360_waveform.py\n");

    delay(2000); // 0.5 Hz capture rate
}
