#include "V93XX_SPI.h"
#include "esp_dsp.h"
#include <cmath>

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

// FFT configuration
constexpr size_t kWaveformWords = 309;  // Actual V9381 waveform buffer size
constexpr int kFftLen = 512;            // FFT length (power of 2)
constexpr float kSampleScale = 1.0f / 32768.0f;
constexpr float kPi = 3.14159265358979323846f;

// FFT buffers
static float fft_data[kFftLen * 2];  // Complex FFT data (real, imag pairs)
static float window[kFftLen];        // Hann window coefficients
static float time_samples[kFftLen];  // Time-domain samples

/**
 * @brief Unpack 32-bit waveform words into 16-bit samples
 * 
 * Each 32-bit word contains two 16-bit signed samples (lower, upper).
 * Converts to normalized float values [-1.0, 1.0].
 */
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

/**
 * @brief Prepare Hann window for FFT
 * 
 * Reduces spectral leakage by tapering signal edges to zero.
 */
static void PrepareWindow() {
    for (int i = 0; i < kFftLen; i++) {
        float phase = (2.0f * kPi * i) / (float)(kFftLen - 1);
        window[i] = 0.5f * (1.0f - cosf(phase));
    }
}

/**
 * @brief Build DSP_CTRL5 configuration for waveform capture
 * 
 * Configures:
 * - Manual single trigger mode
 * - Channel UA (Voltage A)
 * - 512-sample length (Wave_Len=0)
 */
static uint32_t BuildWaveformCtrl5() {
    return ((0 << DSP_CTRL5_DMAMODE_Pos) & DSP_CTRL5_DMAMODE_Msk) | 
           DSP_CTRL5_DMA_CTRL_ENABLE | 
           DSP_CTRL5_WAVE_U |
           ((0 << DSP_CTRL5_WAVE_LEN_Pos) & DSP_CTRL5_WAVE_LEN_Msk) | 
           DSP_CTRL5_WAVEMEM_MODE_MANUAL_SINGLE;
}

void setup() {
    Serial.begin(115200);
    Serial.println("V9381 SPI FFT Example (ESP-DSP)");
    Serial.println("Using Dirty mode for capture robustness");

    // Initialize SPI with Dirty mode for tolerance to CRC mismatches
#if defined(ARDUINO_ARCH_ESP32)
    v9381.Init(V93XX_SPI::WireMode::FourWire, true, V93XX_SPI::ChecksumMode::Dirty, 
               V93XX_SCK_PIN, V93XX_MISO_PIN, V93XX_MOSI_PIN);
#else
    v9381.Init(V93XX_SPI::WireMode::FourWire, true, V93XX_SPI::ChecksumMode::Dirty);
#endif

    delay(100);  // Chip stabilization

    // Verify SPI communication
    uint32_t register_value = 0;
    if (!v9381.RegisterReadChecked(SYS_INTSTS, register_value)) {
        Serial.println("ERROR: SPI communication failed");
        while (1) delay(10);
    }
    Serial.printf("SPI Ready. SYS_INTSTS: 0x%08X\n", register_value);

    // Load V9381 configuration
    v9381.LoadConfiguration(
        (const V93XX_SPI::ControlRegisters &){
            .DSP_ANA0 = 0x00100C00,
            .DSP_ANA1 = 0x000C32C1,
            .DSP_CTRL0 = 0x01000f07,
            .DSP_CTRL1 = 0x000C32C1,
            .DSP_CTRL2 = 0x00002723,
            .DSP_CTRL3 = 0x00000000,
            .DSP_CTRL4 = 0x00000000,
            .DSP_CTRL5 = 0x00000000
        },
        (const V93XX_SPI::CalibrationRegisters &){
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
            .EGY_PWRTH = 0x00000000
        }
    );

    // Configure IO ports
    v9381.RegisterWrite(SYS_IOCFG0, 0x00000000);
    v9381.RegisterWrite(SYS_IOCFG1, 0x003C3A00);

    // Initialize ESP-DSP FFT
    esp_err_t ret = dsps_fft2r_init_fc32(NULL, kFftLen);
    if (ret != ESP_OK) {
        Serial.printf("ERROR: FFT initialization failed: %d\n", ret);
        while (1) delay(10);
    }

    PrepareWindow();
    Serial.printf("FFT ready. Length: %d\n", kFftLen);
    Serial.println("Starting continuous waveform capture and FFT...\n");
}

void loop() {
    // Allocate waveform buffer
    uint32_t waveform_buffer[kWaveformWords] = {0};
    uint32_t ctrl5 = BuildWaveformCtrl5();

    // Capture waveform using CaptureWaveform() API
    // SPI is faster than UART, so we can use default timeout (1000ms) and larger block size (16)
    bool capture_ok = v9381.CaptureWaveform(waveform_buffer, kWaveformWords, ctrl5);
    if (!capture_ok) {
        Serial.println("Waveform capture failed or timed out");
        delay(1000);
        return;
    }

    // Unpack 16-bit samples from 32-bit words
    size_t samples_filled = UnpackSamples(waveform_buffer, kWaveformWords, time_samples, kFftLen);
    
    // Prepare complex FFT input with windowing
    for (int i = 0; i < kFftLen; i++) {
        float sample = (i < (int)samples_filled) ? time_samples[i] : 0.0f;
        fft_data[2 * i] = sample * window[i];      // Real part (windowed)
        fft_data[2 * i + 1] = 0.0f;                // Imaginary part (zero)
    }

    // Execute FFT
    dsps_fft2r_fc32(fft_data, kFftLen);          // Radix-2 FFT
    dsps_bit_rev_fc32(fft_data, kFftLen);        // Bit reversal
    dsps_cplx2reC_fc32(fft_data, kFftLen);       // Convert to magnitude

    // Find peak frequency bin
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

    // Output result
    Serial.printf("FFT peak: bin=%d magnitude=%.4f\n", max_bin, max_mag);
    
    delay(1000);  // 1Hz capture rate
}
