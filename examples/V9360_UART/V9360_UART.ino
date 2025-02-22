#include "V93XX_Raccoon.h"

//Define DEBUG to have raw serial read hex bytes echoed
#define DEBUG

const int V93XX_TX_PIN = 6;
const int V93XX_RX_PIN = 7;
const int V93XX_DEVICE_ADDRESS = 0x00;

V93XX_Raccoon raccoon(V93XX_RX_PIN, V93XX_TX_PIN, Serial1, V93XX_DEVICE_ADDRESS);

void setup() 
{
	Serial.begin(115200);
	Serial.print("Starting V9360\n");

	raccoon.RxReset();
	raccoon.Init();

	uint32_t register_value;

	// Read system status
	register_value = raccoon.RegisterRead(REG_SYS_INTSTS);
	Serial.printf("Interrupt Register: %08X\n", register_value);

	register_value = raccoon.RegisterRead(REG_SYS_VERSION);
	Serial.printf("System Version: %08X\n", register_value);

	// Load control and calibration values
	raccoon.LoadConfiguration(
		(const V93XX_Raccoon::ControlRegisters&){
			.DSP_ANA0 = 0x00100C00,
			.DSP_ANA1 = 0x000C32C1,
			.DSP_CTRL0 = 0x01000f07,
			.DSP_CTRL1 = 0x000C32C1,
			.DSP_CTRL2 = 0x00002723,
			.DSP_CTRL3 = 0x00000000,
			.DSP_CTRL4 = 0x00000000,
			.DSP_CTRL5 = 0x00000000
		},
		(const V93XX_Raccoon::CalibrationRegisters&){
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

	// Configure IO Ports
	raccoon.RegisterWrite(REG_SYS_IOCFG0, 0x00000000);
	raccoon.RegisterWrite(REG_SYS_IOCFG1, 0x003C3A00);


}

void loop()
{
	sleep(1);
	digitalWrite(13, HIGH);
	sleep(1);
	digitalWrite(13, LOW);
}