#include "V93XX_UART.h"

// V9381 UART Dirty Mode Example
// This example demonstrates how to use dirty mode for CRC debugging
// Dirty mode skips CRC validation and shows expected vs received CRC

#if defined(ARDUINO_ARCH_ESP32)
// ESP32-S3 reference (FluidNC wiki): SPI2/VSPI default (IOMUX) pins
const int V93XX_UART_TX_PIN = 11; // MOSI -> V9381 RX/MOSI
const int V93XX_UART_RX_PIN = 13; // MISO <- V9381 TX/MISO
#else
const int V93XX_UART_TX_PIN = 16; // Adjust for your board
const int V93XX_UART_RX_PIN = 15; // Adjust for your board
#endif

const int V93XX_DEVICE_ADDRESS = 0x00;

V93XX_UART v9381(V93XX_UART_RX_PIN, V93XX_UART_TX_PIN, Serial1, V93XX_DEVICE_ADDRESS);

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n\n=== V9381 UART Checksum Mode CRC Debugging Example ===\n");
    
    // Initialize UART in Dirty mode (skip CRC validation)
    v9381.RxReset();
    v9381.Init(SerialConfig::SERIAL_8O1, V93XX_UART::ChecksumMode::Dirty);
    
    delay(500);
}

void loop() {
    Serial.println("\n--- Test Cycle ---");
    Serial.println("[1] Testing with CHECKSUM MODE: Clean (enforce CRC validation)");
    TestWithCleanMode();
    
    delay(2000);
    
    Serial.println("\n[2] Testing with CHECKSUM MODE: Dirty (skip CRC validation)");
    TestWithDirtyMode();
    
    delay(5000);
}

void TestWithCleanMode() {
    // Set to Clean mode - will enforce CRC validation
    v9381.SetChecksumMode(V93XX_UART::ChecksumMode::Clean);
    
    Serial.println("\nReading SYS_VERSION register 3 times:");
    for (int i = 0; i < 3; i++) {
        uint32_t version = v9381.RegisterRead(SYS_VERSION);
        Serial.printf("  Attempt %d: 0x%08X\n", i + 1, version);
    }
}

void TestWithDirtyMode() {
    // Set to Dirty mode - will skip CRC validation
    v9381.SetChecksumMode(V93XX_UART::ChecksumMode::Dirty);
    
    Serial.println("\nReading SYS_VERSION register 3 times:");
    for (int i = 0; i < 3; i++) {
        uint32_t version = v9381.RegisterRead(SYS_VERSION);
        Serial.printf("  Attempt %d: 0x%08X\n", i + 1, version);
    }
    
    Serial.println("\nReading multiple registers:");
    Serial.printf("  SYS_INTSTS = 0x%08X\n", v9381.RegisterRead(SYS_INTSTS));
    Serial.printf("  SYS_ROMCS = 0x%08X\n", v9381.RegisterRead(SYS_ROMCS));
    
    // Return to Clean mode for normal operation
    v9381.SetChecksumMode(V93XX_UART::ChecksumMode::Clean);
    Serial.println("\nReturned to Clean mode - normal operation");
}

// Alternative: Manual CRC Calculation for Verification
//
// Use this function to verify CRC calculation independently:
//
// uint8_t CalculateCRC8_V9381(const uint8_t *data, size_t length) {
//     // V9381 CRC formula: 0x33 + ~(sum of bytes)
//     uint8_t checksum = 0;
//     for (size_t i = 0; i < length; i++) {
//         checksum += data[i];
//     }
//     return 0x33 + ~checksum;
// }
//
// Example usage:
//   uint8_t request[] = {0x7d, 0x04, 0x02, 0x00};  // Some request
//   uint8_t expected_crc = CalculateCRC8_V9381(request, 3);
//   Serial.printf("Expected CRC: 0x%02X\n", expected_crc);
