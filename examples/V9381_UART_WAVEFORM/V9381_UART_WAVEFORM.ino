#include "V93XX_UART.h"

#if defined(ARDUINO_ARCH_ESP32)
const int V93XX_UART_TX_PIN = 11;
const int V93XX_UART_RX_PIN = 13;
const int V93XX_ADDR0_PIN = 12;
const int V93XX_ADDR1_PIN = 10;
#else
const int V93XX_UART_TX_PIN = 16;
const int V93XX_UART_RX_PIN = 15;
const int V93XX_ADDR0_PIN = 12;
const int V93XX_ADDR1_PIN = 10;
#endif

const int V93XX_DEVICE_ADDRESS = 0x00;

V93XX_UART v9381(V93XX_UART_RX_PIN, V93XX_UART_TX_PIN, Serial1, V93XX_DEVICE_ADDRESS);

static void ConfigureUartAddressPins(int address) {
    pinMode(V93XX_ADDR0_PIN, OUTPUT);
    pinMode(V93XX_ADDR1_PIN, OUTPUT);

    digitalWrite(V93XX_ADDR0_PIN, (address & 0x01) ? HIGH : LOW);
    digitalWrite(V93XX_ADDR1_PIN, (address & 0x02) ? HIGH : LOW);
}

void setup() {
    Serial.begin(115200);
    Serial.print("Starting V9381 UART Waveform\n");

    ConfigureUartAddressPins(V93XX_DEVICE_ADDRESS);

    v9381.RxReset();
    v9381.Init(SerialConfig::SERIAL_8O1, V93XX_UART::ChecksumMode::Dirty);

    uint32_t register_value = v9381.RegisterRead(SYS_INTSTS);
    Serial.printf("Interrupt Register: 0x%08X\n", register_value);

    register_value = v9381.RegisterRead(SYS_VERSION);
    Serial.printf("System Version: 0x%08X\n", register_value);

    v9381.LoadConfiguration((const V93XX_UART::ControlRegisters &){.DSP_ANA0 = 0x00100C00,
                                                                   .DSP_ANA1 = 0x000C32C1,
                                                                   .DSP_CTRL0 = 0x01000f07,
                                                                   .DSP_CTRL1 = 0x000C32C1,
                                                                   .DSP_CTRL2 = 0x00002723,
                                                                   .DSP_CTRL3 = 0x00000000,
                                                                   .DSP_CTRL4 = 0x00000000,
                                                                   .DSP_CTRL5 = 0x00000000},
                            (const V93XX_UART::CalibrationRegisters &){.DSP_CFG_CALI_PA = 0x00000000,
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

    v9381.RegisterWrite(SYS_IOCFG0, 0x00000000);
    v9381.RegisterWrite(SYS_IOCFG1, 0x003C3A00);

    v9381.RegisterWrite(SYS_INTSTS, SYS_INTSTS_CKERR);
}

void loop() {
    uint32_t waveform_buffer[309] = {0};

    const uint32_t ctrl5 = ((0 << DSP_CTRL5_DMAMODE_Pos) & DSP_CTRL5_DMAMODE_Msk) | DSP_CTRL5_DMA_CTRL_ENABLE |
                           DSP_CTRL5_WAVE_U | ((0 << DSP_CTRL5_WAVE_LEN_Pos) & DSP_CTRL5_WAVE_LEN_Msk) |
                           DSP_CTRL5_WAVEMEM_MODE_MANUAL_SINGLE;

    bool capture_ok = v9381.CaptureWaveform(waveform_buffer, 309, ctrl5, 1000, 16);
    if (!capture_ok) {
        Serial.println("Waveform capture failed or overflowed");
        delay(1000);
        return;
    }

    Serial.printf("\nwave_data = [ \n");
    for (int i = 0; i < 309; i++) {
        int16_t upper = (int16_t)((waveform_buffer[i] >> 16) & 0xFFFF);
        int16_t lower = (int16_t)(waveform_buffer[i] & 0xFFFF);
        Serial.printf("%5d, %5d%s", lower, upper, (i != 308) ? (i % 8 == 7) ? ",\n" : ", " : "");
    }
    Serial.printf("]\n");

    delay(1000);
}
