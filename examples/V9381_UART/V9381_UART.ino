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

// Set to 1 to emit a known UART pattern for pin verification.
#define V93XX_UART_PIN_TEST 1
// Set to 1 to run pin swap/config probe sequences.
#define V93XX_UART_PROBE 0
// Set to 1 to run minimal UART read tests only.
#define V93XX_UART_DEBUG_MINIMAL 1
// Set to 1 to toggle A0/A1 so logic analyzer confirms address pin mapping.
#define V93XX_UART_ADDR_BLINK 1

#if V93XX_UART_PROBE
static void RunUartProbe(int rx_pin, int tx_pin, const char *label) {
    Serial.printf("\n=== UART probe: %s (RX=%d, TX=%d) ===\n", label, rx_pin, tx_pin);

    V93XX_Raccoon probe(rx_pin, tx_pin, Serial1, V93XX_DEVICE_ADDRESS);

    v9381.RxReset();

#if V93XX_UART_PIN_TEST
    Serial1.begin(19200, SerialConfig::SERIAL_8N1, V93XX_UART_RX_PIN, V93XX_UART_TX_PIN);
    Serial.println("UART pin test (8N1): sending 0x55 pattern on TX");
    for (int i = 0; i < 10; i++) {
        Serial1.write(0x55);
        delay(5);
    }
    Serial1.flush();
    Serial.println("UART pin test complete");
    delay(10);
#endif

    v9381.Init();

#if V93XX_UART_PROBE
    RunUartProbe(V93XX_UART_RX_PIN, V93XX_UART_TX_PIN, "normal");
    RunUartProbe(V93XX_UART_TX_PIN, V93XX_UART_RX_PIN, "swapped");
#endif

    const UartConfigOption configs[] = {
        {SerialConfig::SERIAL_8O1, "8O1"},
        {SerialConfig::SERIAL_8E1, "8E1"},
        {SerialConfig::SERIAL_8N1, "8N1"},
    };

    for (const auto &option : configs) {
        Serial.printf("\nConfig %s\n", option.name);
        probe.RxReset();
        probe.Init(option.config);
        delay(2);

        for (int addr = 0; addr < 4; addr++) {
            ConfigureUartAddressPins(addr);
            Serial.printf("  Address 0x%02X\n", addr);

            uint32_t version = probe.RegisterRead(SYS_VERSION);
            Serial.printf("    SYS_VERSION: 0x%08X\n", version);

            uint32_t intsts = probe.RegisterRead(SYS_INTSTS);
            Serial.printf("    SYS_INTSTS:  0x%08X\n", intsts);

            delay(5);
        }
    }
}
#endif

V93XX_Raccoon v9381(V93XX_UART_RX_PIN, V93XX_UART_TX_PIN, Serial1, V93XX_DEVICE_ADDRESS);

static void ConfigureUartAddressPins(int address) {
    pinMode(V93XX_ADDR0_PIN, OUTPUT);
    pinMode(V93XX_ADDR1_PIN, OUTPUT);

    digitalWrite(V93XX_ADDR0_PIN, (address & 0x01) ? HIGH : LOW);
    digitalWrite(V93XX_ADDR1_PIN, (address & 0x02) ? HIGH : LOW);
}

static void BlinkUartAddressPins() {
    for (int addr = 0; addr < 4; addr++) {
        ConfigureUartAddressPins(addr);
        Serial.printf("Blink A1/A0 = %d%d\n", (addr & 0x02) ? 1 : 0, (addr & 0x01) ? 1 : 0);
        delay(50);
    }
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

#if V93XX_UART_ADDR_BLINK
    BlinkUartAddressPins();
    ConfigureUartAddressPins(V93XX_DEVICE_ADDRESS);
#endif

    // NOTE: If you later switch to SPI in the same sketch, call
    // ReleaseUartAddressPinsForSpi() before SPI.begin().

    v9381.RxReset();
    v9381.Init();

#if V93XX_UART_PROBE
    RunUartProbe(V93XX_UART_RX_PIN, V93XX_UART_TX_PIN, "normal");
    RunUartProbe(V93XX_UART_TX_PIN, V93XX_UART_RX_PIN, "swapped");
#endif

    uint32_t register_value = v9381.RegisterRead(SYS_INTSTS);
    Serial.printf("Interrupt Register: 0x%08X\n", register_value);

    register_value = v9381.RegisterRead(SYS_VERSION);
    Serial.printf("System Version: 0x%08X\n", register_value);

#if V93XX_UART_DEBUG_MINIMAL
    Serial.println("UART minimal debug mode: skipping configuration load");
    return;
#endif

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
#if V93XX_UART_DEBUG_MINIMAL
    delay(1000);
    return;
#endif
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
