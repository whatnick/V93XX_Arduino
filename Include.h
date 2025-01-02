#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#ifndef VAREXT
    #define VAREXT extern
#endif

//Import headers
#include "string.h"	
#include "stdlib.h"	
#include "math.h"
#include "Raccoon.h"

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

#endif
