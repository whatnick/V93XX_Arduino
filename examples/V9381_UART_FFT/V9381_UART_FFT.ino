#include "V93XX_UART.h"
#include "esp_dsp.h"
#include <cmath>

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

constexpr size_t kWaveformWords = 309;
constexpr int kFftLen = 512;
constexpr float kSampleScale = 1.0f / 32768.0f;
constexpr float kPi = 3.14159265358979323846f;

static float fft_data[kFftLen * 2];
static float window[kFftLen];
static float time_samples[kFftLen];

static void ConfigureUartAddressPins(int address) {
    pinMode(V93XX_ADDR0_PIN, OUTPUT);
    pinMode(V93XX_ADDR1_PIN, OUTPUT);

    digitalWrite(V93XX_ADDR0_PIN, (address & 0x01) ? HIGH : LOW);
    digitalWrite(V93XX_ADDR1_PIN, (address & 0x02) ? HIGH : LOW);
}

static size_t UnpackSamples(const uint32_t *words, size_t word_count, float *out, size_t out_count) {
    size_t out_index = 0;
    for (size_t i = 0; i < word_count && out_index < out_count; i++) {
        int16_t lower = (int16_t)(words[i] & 0xFFFF);
        int16_t upper = (int16_t)((words[i] >> 16) & 0xFFFF);

        out[out_index++] = (float)lower * kSampleScale;
        if (out_index >= out_count) {
            break;
        }
        out[out_index++] = (float)upper * kSampleScale;
    }

    return out_index;
}

static void PrepareWindow() {
    for (int i = 0; i < kFftLen; i++) {
        float phase = (2.0f * kPi * i) / (float)(kFftLen - 1);
        window[i] = 0.5f * (1.0f - cosf(phase));
    }
}

static uint32_t BuildWaveformCtrl5() {
    return ((0 << DSP_CTRL5_DMAMODE_Pos) & DSP_CTRL5_DMAMODE_Msk) | DSP_CTRL5_DMA_CTRL_ENABLE | DSP_CTRL5_WAVE_U |
           ((0 << DSP_CTRL5_WAVE_LEN_Pos) & DSP_CTRL5_WAVE_LEN_Msk) | DSP_CTRL5_WAVEMEM_MODE_MANUAL_SINGLE;
}

void setup() {
    Serial.begin(115200);
    Serial.print("Starting V9381 UART FFT (ESP-DSP)\n");

    ConfigureUartAddressPins(V93XX_DEVICE_ADDRESS);

    v9381.RxReset();
    v9381.Init(SerialConfig::SERIAL_8O1, V93XX_UART::ChecksumMode::Dirty);

    uint32_t register_value = v9381.RegisterRead(SYS_VERSION);
    Serial.printf("System Version: 0x%08X\n", register_value);

    v9381.LoadConfiguration(
        (const V93XX_UART::ControlRegisters &){.DSP_ANA0 = 0x00100C00,
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

    esp_err_t ret = dsps_fft2r_init_fc32(NULL, kFftLen);
    if (ret != ESP_OK) {
        Serial.printf("Error initializing FFT: %d\n", ret);
        while (1) {
            delay(10);
        }
    }

    PrepareWindow();
    Serial.println("FFT initialized successfully.");
}

void loop() {
    uint32_t waveform_buffer[kWaveformWords] = {0};
    uint32_t ctrl5 = BuildWaveformCtrl5();

    bool capture_ok = v9381.CaptureWaveform(waveform_buffer, kWaveformWords, ctrl5, 2000, 4);
    if (!capture_ok) {
        Serial.println("Waveform capture failed or overflowed");
        delay(1000);
        return;
    }

    size_t samples_filled = UnpackSamples(waveform_buffer, kWaveformWords, time_samples, kFftLen);
    for (int i = 0; i < kFftLen; i++) {
        float sample = (i < (int)samples_filled) ? time_samples[i] : 0.0f;
        fft_data[2 * i] = sample * window[i];
        fft_data[2 * i + 1] = 0.0f;
    }

    dsps_fft2r_fc32(fft_data, kFftLen);
    dsps_bit_rev_fc32(fft_data, kFftLen);
    dsps_cplx2reC_fc32(fft_data, kFftLen);

    float max_mag = 0.0f;
    int max_bin = 0;
    for (int i = 1; i < kFftLen / 2; i++) {
        float re = fft_data[2 * i];
        float im = fft_data[2 * i + 1];
        float mag = sqrtf((re * re) + (im * im));
        if (mag > max_mag) {
            max_mag = mag;
            max_bin = i;
        }
    }

    Serial.printf("FFT peak bin=%d magnitude=%.4f\n", max_bin, max_mag);
    delay(1000);
}
