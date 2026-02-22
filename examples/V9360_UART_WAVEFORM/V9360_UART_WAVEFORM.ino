#include "V93XX_UART.h"

const int V93XX_TX_PIN = 16;
const int V93XX_RX_PIN = 15;
const int V93XX_DEVICE_ADDRESS = 0x00;

V93XX_UART raccoon(V93XX_RX_PIN, V93XX_TX_PIN, Serial1, V93XX_DEVICE_ADDRESS);

void setup() {
    Serial.begin(115200);
    Serial.print("Starting V9360\n");

    raccoon.RxReset();
    raccoon.Init();

    uint32_t register_value;

    // Read system status
    raccoon.RegisterWrite(SYS_INTSTS, 0xFFFFFFFF);
    register_value = raccoon.RegisterRead(SYS_INTSTS);
    Serial.printf("Interrupt Register: %08X\n", register_value);

    register_value = raccoon.RegisterRead(SYS_VERSION);
    Serial.printf("System Version: %08X\n", register_value);

    // Load control and calibration values
    raccoon.LoadConfiguration((const V93XX_UART::ControlRegisters &){.DSP_ANA0 = 0x00100C00,
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

    // Configure IO Ports
    raccoon.RegisterWrite(SYS_IOCFG0, 0x00000000);
    raccoon.RegisterWrite(SYS_IOCFG1, 0x003C3A00);

    // Clear checksum error, not that calibration is loaded
    raccoon.RegisterWrite(SYS_INTSTS, SYS_INTSTS_CKERR);

    register_value = raccoon.RegisterRead(SYS_INTSTS);
    Serial.printf("Interrupt Register: %08X\n", register_value);
}

void loop() {
    uint32_t waveform_buffer[309];

    Serial.printf("Configure Waveform capture:\n");

    const uint32_t ctrl5 =
        // Transmit Starting by manual, stops on max cycle
        ((0 << DSP_CTRL5_DMAMODE_Pos) & DSP_CTRL5_DMAMODE_Msk)
        // Enable Manual
        | DSP_CTRL5_DMA_CTRL_ENABLE
        // Source: Channel U, HPF: None
        | DSP_CTRL5_WAVE_U
        // Wave length, 1 cycle
        | ((0 << DSP_CTRL5_WAVE_LEN_Pos) & DSP_CTRL5_WAVE_LEN_Msk)
        // Manual trigger mode
        | DSP_CTRL5_WAVEMEM_MODE_MANUAL_SINGLE;

    const int waveform_size = sizeof(waveform_buffer) / sizeof(waveform_buffer[0]);
    bool capture_ok = raccoon.CaptureWaveform(waveform_buffer, waveform_size, ctrl5, 1000, 16);
    if (!capture_ok) {
        Serial.println("Waveform capture failed or overflowed");
    }

    // Dump waveform data
    Serial.printf("\nwave_data = [ \n");
    for (int i = 0; i < waveform_size; i++) {
        int16_t upper = (waveform_buffer[i] >> 16) & 0xFFFF;
        int16_t lower = waveform_buffer[i] & 0xFFFF;
        Serial.printf("%5d, %5d%s", lower, upper, (i != waveform_size - 1) ? (i % 8 == 7) ? ",\n" : ", " : "");
    }
    Serial.printf("]\n");

    /* Data can be take from a serial terminal and plotted in Python
        ```python
        wave_data = [ ..... ]

        # plot wave_data
        import matplotlib.pyplot as plt
        plt.plot(wave_data)
        plt.show()
        ```
    */

    /*
    If FFT is desired on controlled use little kiss fft code vendored in
    https://github.com/stolk/little_kiss_fft
    */

    sleep(1);
}