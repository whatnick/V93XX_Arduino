#include "V93XX_Include.h"
#include "Raccoon.h"

//Define DEBUG to have raw serial read hex bytes echoed
//#define DEBUG

void setup() 
{
	Serial.begin(115200);
	Serial.print("Starting V9360");
	Init_Para();
	Init_RacCtrl();
	RxReset_Raccoon();
	Init_UartRaccoon();
	// Read system status
	uint8_t status = ReadRaccoon(SYS_INTSTS, 1);
	status = ReadRaccoon(SYS_VERSION, 1);
	// Fix externs to make this work
	Raccoon_UpdatePar();
}

void loop()
{
	Raccoon_ReadRMS();
	Serial.printf("Voltage_P: %d\n",gs_RmsData.ul_P);
	Serial.printf("Voltage_Q: %d\n",gs_RmsData.ul_Q);
	Serial.printf("Voltage_RMS: %d\n",gs_RmsData.ul_U);
	Serial.printf("Current 1: %d\n",gs_RmsData.ul_I1);
	Serial.printf("Current 2: %d\n",gs_RmsData.ul_I2);
	Serial.printf("Frequency: %d\n",gs_RmsData.ul_Hz);
	sleep(1);
}