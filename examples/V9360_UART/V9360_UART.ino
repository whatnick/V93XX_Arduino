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
	register_value = raccoon.RegisterRead(SYS_INTSTS);
	Serial.printf("Interrupt Register: %08X\n", register_value);

	register_value = raccoon.RegisterRead(SYS_VERSION);
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
	raccoon.RegisterWrite(SYS_IOCFG0, 0x00000000);
	raccoon.RegisterWrite(SYS_IOCFG1, 0x003C3A00);

	raccoon.ConfigureBlockRead((const uint8_t[]){
		DSP_DAT_PA1,
		DSP_DAT_QA1,
		DSP_DAT_RMS1UA,
		DSP_DAT_RMS1IA,
		DSP_DAT_RMS1IB,
		DSP_DAT_FRQ,
		DSP_DAT_PB1,
		DSP_DAT_QB1,
		DSP_DAT_SA1,
		DSP_DAT_SB1
	}, 10);

}

void loop()
{
	uint32_t rms_register_values[10];
	raccoon.RegisterBlockRead(rms_register_values, 10);

	raccoon.RegisterRead(DSP_DAT_PA1);

	Serial.printf("DSP_DAT: PA1=%08X ", rms_register_values[0]);
	Serial.printf("QA1=%08X ", rms_register_values[1]);
	Serial.printf("RMS1UA=%08X ", rms_register_values[2]);
	Serial.printf("RMS1IA=%08X ", rms_register_values[3]);
	Serial.printf("RMS1IB=%08X ", rms_register_values[4]);
	Serial.printf("FRQ=%08X ", rms_register_values[5]);
	Serial.printf("PB1=%08X ", rms_register_values[6]);
	Serial.printf("QB1=%08X ", rms_register_values[7]);
	Serial.printf("SA1=%08X ", rms_register_values[8]);
	Serial.printf("SB1=%08X\n", rms_register_values[9]);



	sleep(1);
}