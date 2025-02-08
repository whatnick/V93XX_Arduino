#include "V93XX_Include.h"
#include "Raccoon.h"

void setup() 
{
	Serial.begin(115200);
	Serial.print("Starting V9360");
	Init_RacCtrl();
	RxReset_Raccoon();
	Init_UartRaccoon();
}

void loop()
{
	// Read system status
	uint8_t status = ReadRaccoon(SYS_INTSTS, 1);
	status = ReadRaccoon(SYS_VERSION, 1);
	sleep(1);
}