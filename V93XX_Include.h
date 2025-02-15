#ifndef _V93XX_INCLUDE_H_
#define _V93XX_INCLUDE_H_

#ifndef VAREXT
    #define VAREXT extern
#endif

//Import headers
#include "string.h"	
#include "stdlib.h"	
#include "math.h"
#include "Raccoon.h"

/*** Host hardware definitions for multiple Arduinos variants as appropriate could be overriden in .ino */
#define RX_V93XX 15
#define TX_V93XX 16

typedef union
{
	uint32_t lword;
	uint16_t word[2];
	uint8_t  byte[4];
}Word32;

VAREXT uint8_t data[1000];
VAREXT uint32_t temp_buf[1000];
VAREXT uint8_t DMA_Flag;
VAREXT uint32_t DMA_Count;
VAREXT uint8_t DST_Count;

VAREXT uint8_t rms_count,Rms_R_F;
VAREXT uint32_t Rms_R_C;
//VAREXT uint32_t testshow;  //hjj 2020.3.31
typedef struct
{
    uint32_t  ul_P;
    uint32_t  ul_Q;
//    uint32  ul_U;         
//    uint32  ul_I1;    
//    uint32  ul_I2;
//    uint32  ul_Hz;
    uint32_t  ul_PB;//zzp0305
    uint32_t  ul_QB;
//    uint32  ul_SA;
//    uint32  ul_SB;//zzp0305
}B_RMS;
VAREXT B_RMS Buf_RMS;

typedef struct
{
    uint16_t  uc_80msCnt;            //Small signal 80ms
    uint16_t  uc_80msCntRE;      //Small signal 80ms no achievement
    uint16_t  uc_160msCnt;         //Large signal with 160ms
    uint16_t  uc_160msCntRE;   //Large signal has no achievement 160ms
    uint8_t  uc_1280msCnt;     //Average number of seconds
}S_IMPTIM;

VAREXT S_IMPTIM gs_ipmtim;

typedef struct
{
    uint32_t  ul_power[2];         
    uint32_t  ul_Npower[2];         
}S_POWER_ENERYCAL;

VAREXT S_POWER_ENERYCAL gs_power_energycal;

typedef struct
{
    //uint32  ul_power[4];         
    uint32_t  ul_power_all[2];          
    uint16_t  ui_power_rem[2];
    //uint32  ul_Npower[4];         
    uint32_t  ul_Npower_all[2];          
    uint16_t  ui_Npower_rem[2];
    uint32_t  ul_underpow;
    uint32_t  ul_Nunderpow;
}S_POWER;

VAREXT S_POWER gs_power;

#define SETA 0x00
#define SETB 0x55
typedef struct
{
    uint8_t ucSta;                //Current channel
    uint8_t ucACnt;               //A channel count
    uint8_t ucBCnt;               //B channel count
    uint8_t ucGate;               //Counting
}S_CHANNEL;
VAREXT S_CHANNEL gs_Channel;

typedef struct
{
    uint16_t ui_MeterC;   // Table constant
    uint16_t ui_Un;       // Nominal voltage
    uint16_t ui_Ib;       // Nominal current

    uint32_t ul_EGYTH;      //High -level limit register
    uint32_t ul_CTH;        //Starting/Submarine judgment door limit register 0.1%
    uint32_t ul_PHC;        //Corner correction register
    uint32_t ul_CALI_RMSUA; //Effective value ratio difference register of the full wave voltage
    uint32_t ul_CALI_RMSIA; //IA full -wave current valid value ratio difference register
    uint32_t ul_CALI_RMSIB; //IB full -wave current valid value ratio difference register
    uint32_t ul_CALI_PA;    //Full wave has power A ratio difference register
    uint32_t ul_DC_PA;      //Full wave has power A secondary compensation register
    uint32_t ul_DC_QA;      //Full waveless power A secondary compensation register
    uint32_t ul_DC_RMSIA;   //Current A valid value small signal correction ZZP0303
    
    uint32_t ul_PG;       //Power ratio coefficient without contributions and contributions to PG
    //uint32 ul_QG;       //Power ratio coefficient
    uint32_t ul_URmG;     //Voltage channel ratio coefficient coefficient
    uint32_t ul_I1RmG;    //Current channel 1 ratio coefficient coefficient
    uint32_t ul_I2RmG;    //Current channel 2 ratio coefficient coefficient
    //uint32 ul_SG;        //It is regarded in the power ratio coefficient ZZP0305
    uint32_t ul_CALI_PB;          //Full wave has a power B ratio differential register
    uint32_t ul_DC_PB;            //Full wave has power B secondary compensation register
    uint32_t ul_DC_QB;            //Full waveless power B secondary compensation register
    uint32_t ul_DC_RMSIB;         //Current B valid value small signal correction ZZP0313

    uint16_t ui_JbCRC;    // CRC result of school table parameters
}S_JBPM;

VAREXT S_JBPM gs_JbPm;            //School table parameters

/***********************ADC PARA****************/
#define ADCA1X    0
#define ADCA4X    BIT0
#define ADCA8X    BIT1
#define ADCA16X  BIT0+BIT1
#define ADCA32X  BIT0+BIT1+BIT2
#define ADCB1X    0
#define ADCB4X    BIT3
#define ADCB8X    BIT4
#define ADCB16X  BIT3+BIT4
#define ADCB32X  BIT3+BIT4+BIT5
#define ADCU1X    0
#define ADCU2X    BIT6
#define ADCM1X    0
#define ADCM2X    BIT7




 
#define Const_MaxCf     0x05//Maximum number of pulses
#define ADCFRE   819200
#define PLEVEL   65         //Startup power
#define IALEVEL  25         //Start current
#define IBLEVEL  70
#define ULEVEL   200         //The voltage displayed is 20.0V
#define CRIULEVEL1  1694
#define CRIULEVEL2  2640
#define gs_JbPm_SG 0X01E8

typedef struct 
{
    uint8_t PluseType:1;      //0 RTC 1 Rate Switch pluse
    uint8_t STAFLAG:1;
    uint8_t Imbalance:1;
    uint8_t Pdir:1;
    uint8_t Prog:1;
    uint8_t undefine5:1;
    uint8_t undefine6:1;
    uint8_t Current:1;
}GS_SYSRUNST;


VAREXT GS_SYSRUNST gs_SysRunSt;

/*=========================================================================================================\n
* @function_name: gCuc_HEX2BCDMap
* @function_file: Api.c
* @Description: HEX2BCD Table
*
* @parameters:
* @return: 
* @Author: lwb (2012-02-29)
* @Remark: 
*-------------------------------------------------------------------------------------------
* @Modified by:
* @Modify content:
============================================================================================================================*/
const uint8_t gCuc_HEX2BCDMap[]  =
{// 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, //0-9
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, //10-19
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, //20-29
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, //30-39
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, //40-49
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, //50-59
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, //60-69
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, //70-79
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, //80-89
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, //90-99
};

uint32_t CalRMS(uint16_t addr);
uint8_t APIByteHEX2BCD(uint8_t ucHEX);
uint16_t IntHex2BCD(uint16_t hex);
uint32_t Hex2BCD(uint32_t hex);

#endif
