#include "V93XX_Raccoon.h"
#include <cstring>

const int V93XX_TX_PIN = 16;
const int V93XX_RX_PIN = 15;
const int V93XX_DEVICE_ADDRESS = 0x00;

V93XX_Raccoon raccoon(V93XX_RX_PIN, V93XX_TX_PIN, Serial1, V93XX_DEVICE_ADDRESS);

void setup()
{
    Serial.begin(115200);
    Serial.print("Starting V9360\n");
}

void loop()
{
    Serial.println("V9360 UART FFT Output");
}