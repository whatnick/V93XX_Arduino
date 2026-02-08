#ifndef V93XX_RACCOON_REGISTERS_H__
#define V93XX_RACCOON_REGISTERS_H__

#include <stdint.h>

enum V93XX_REG {
    DSP_ANA0 = 0x00,           // Analog control register	0
    DSP_ANA1 = 0X01,           // Analog control register	1
    DSP_CTRL0 = 0X02,          // Digital control register	0
    DSP_CTRL1 = 0x03,          // Digital control register	1
    DSP_CTRL2 = 0x04,          // Digital control register2
    DSP_CTRL3 = 0x05,          // Digital control register3
    DSP_CTRL4 = 0x06,          // Digital control register4
    DSP_CTRL5 = 0x07,          // Digital control register5
    DSP_DAT_PA = 0x08,         // A Channel instantaneous active power
    DSP_DAT_QA = 0x09,         // A Channel instantaneous reactive power
    DSP_DAT_SA = 0x0a,         // A Channel instantaneous apparent power
    DSP_DAT_PB = 0x0b,         // B Channel instantaneous active power
    DSP_DAT_QB = 0x0c,         // B Channel instantaneous reactive power
    DSP_DAT_SB = 0x0d,         // B Channel instantaneous reactive power
    DSP_DAT_RMS0UA = 0x0e,     // Voltage instantaneous effective value
    DSP_DAT_RMS0IA = 0x0f,     // Current A instantaneous effective value
    DSP_DAT_RMS0IB = 0x10,     // Current A instantaneous effective value
    DSP_DAT_CH1 = 0x11,        //	Fundamental optional channel 1 instantaneous value
    DSP_DAT_CH2 = 0x12,        //	Fundamental optional channel 2 instantaneous value
    DSP_DAT_PA1 = 0x13,        //	A Channel average active power
    DSP_DAT_QA1 = 0x14,        //	A Channel average reactive power
    DSP_DAT_SA1 = 0x15,        //	A Channel average apparent power
    DSP_DAT_PB1 = 0x16,        //	B Channel average active power
    DSP_DAT_QB1 = 0x17,        //	B Channel average reactive power
    DSP_DAT_SB1 = 0x18,        //	B Channel average apparent power
    DSP_DAT_RMS1UA = 0x19,     //	Voltage average effective value
    DSP_DAT_RMS1IA = 0x1a,     //  Average effective value of current A
    DSP_DAT_RMS1IB = 0x1b,     //	Current B average effective value
    DSP_DAT_CH1_AVG = 0x1c,    //	Fundamental optional channel 1 average
    DSP_DAT_CH2_AVG = 0x1d,    //	Fundamental optional channel 2 average
    DSP_DAT_RMSU_AVG = 0x1e,   //	Average voltage rms value over 10 or 12 cycles (selected by grid frequency)
    DSP_DAT_RMSIA_AVG = 0x1f,  //	Average RMS value of current IA over 10 or 12 cycles (selected by grid frequency)
    DSP_DAT_RMSIB_AVG = 0x20,  //	Average current IB rms value for 10 or 12 cycles (selected by grid frequency)
    DSP_DAT_FRQ = 0x21,        //	Grid frequency
    DSP_DAT_DCU = 0x22,        //	Voltage channel DC value
    DSP_DAT_DCI = 0x23,        //	Current A channel DC value
    DSP_DAT_DCIB = 0x24,       //	Current B channel DC value
    DSP_CFG_CALI_PA = 0x25,    // Active power A ratio difference correction
    DSP_CFG_DC_PA = 0x26,      //  Active power A small signal correction
    DSP_CFG_CALI_QA = 0x27,    //	Reactive power A ratio difference correction
    DSP_CFG_DC_QA = 0x28,      //	Reactive power A small signal correction
    DSP_CFG_CALI_PB = 0x29,    //	Active power B ratio difference correction
    DSP_CFG_DC_PB = 0x2a,      //	Active power B small signal correction
    DSP_CFG_CALI_QB = 0x2b,    //	Reactive power B ratio difference correction
    DSP_CFG_DC_QB = 0x2c,      //	Reactive power B small signal correction
    DSP_CFG_CALI_RMSUA = 0x2d, //	Voltage RMS ratio correction
    DSP_CFG_RMS_DCUA = 0x2e,   //	Voltage RMS small signal correction
    DSP_CFG_CALI_RMSIA = 0x2f, //	Current A effective value ratio correction
    DSP_CFG_RMS_DCIA = 0x30,   //	Current A RMS small signal correction
    DSP_CFG_CALI_RMSIB = 0x31, //	Current B effective value ratio difference correction
    DSP_CFG_RMS_DCIB = 0x32,   //	Current B RMS small signal correction
    DSP_CFG_PHC = 0x33,        //	Angular error correction register
    DSP_CFG_DCUA = 0x34,       //	Voltage channel DC correction value
    DSP_CFG_DCIA = 0x35,       //	Current A channel DC correction value
    DSP_CFG_DCIB = 0x36,       //	Current B channel DC correction value
    DSP_CFG_BPF = 0x37,        //	Bandpass filter coefficients
    DSP_CFG_CKSUM = 0x38,      //	Checksum Configuration Registers
    EGY_PROCTH = 0x39,         //	Energy register anti-submarine threshold (less than it will not accumulate)
    EGY_PWRTH = 0x3a,          //	Energy register accumulation threshold. Since the energy register is 46 bits,
                      // the actual high-speed energy register accumulation threshold is this threshold * 16384;
                      // the low-speed energy register accumulation threshold is this value * 4.
    EGY_CONST1 = 0x3b,    // Energy register 1 Accumulation constant
    EGY_OUT1L = 0x3c,     //	Energy register 1 Accumulated value low
    EGY_OUT1H = 0x3d,     //	Energy register 1 Accumulated value high
    EGY_CFCNT1 = 0x3e,    //	Energy register 1 Pulse mixer
    EGY_CONST2 = 0x3f,    //	Energy register 2 Accumulation Constant
    EGY_OUT2L = 0x40,     //	Energy register 2 accumulated value low
    EGY_OUT2H = 0x41,     //	Energy register 2 Accumulated Value High
    EGY_CFCNT2 = 0x42,    //	Energy register 2 Pulse Counter
    EGY_CONST3 = 0x43,    //	Energy register 3 cumulative constant
    EGY_OUT3 = 0x44,      //	Energy register 3 Accumulated Value
    EGY_CFCNT3 = 0x45,    //	Energy register 3 Pulse Counter
    EGY_CONST4 = 0x46,    //	Energy register 4 cumulative constant
    EGY_OUT4 = 0x47,      //	Energy register 4 Accumulated Value
    EGY_CFCNT4 = 0x48,    //	Energy regsiter 4 Pulse Counter
    EGY_CONST5 = 0x49,    //  Energy register 5 Accumulation Constant
    EGY_OUT5 = 0x4a,      //	Energy register 5 Accumulated Value
    EGY_CFCNT5 = 0x4b,    //	Energy register 5 Pulse Counter
    EGY_CONST6 = 0x4c,    //	Energy register 6 cumulative constant
    EGY_OUT6 = 0x4d,      //	 Energy register 6 Accumulated Value
    EGY_CFCNT6 = 0x4e,    //	Energy register 6 Pulse Counter
    EGY_CONST7 = 0x4f,    //	Energy register 7 cumulative constant
    EGY_OUT7 = 0x50,      //	Energy register 7 Accumulated Value
    EGY_CFCNT7 = 0x51,    //	Energy register 7 Pulse Counter
    EGY_CONST8 = 0x52,    //	Energy register 8 cumulative constant
    EGY_OUT8 = 0x53,      //	Energy register 8 Accumulated Value
    EGY_CFCNT8 = 0x54,    //	Energy register 8 Pulse Counter
    DSP_OV_THL = 0x55,    //	Upper threshold of creep judgment
    DSP_OV_THH = 0x56,    //  Lower threshold for creep judgment
    DSP_SWELL_THL = 0x57, //	Voltage swell lower threshold in current detection mode,
                          // it is used to determine the effective value of current
    DSP_SWELL_THH = 0x58, // Voltage surge upper threshold. In current detection mode,
                          // it is used to determine the effective value of current.
    DSP_DIP_THL = 0x59,   // Voltage sag lower threshold
    DSP_DIP_THH = 0x5a,   //	Voltage sag upper threshold
    FD_OVTH = 0x5b,       //	Fast detection of overvoltage threshold
    FD_LVTH = 0x5c,       //	Fast detection of undervoltage threshold
    FD_IA_OCTH = 0x5d,    //	Fast detection of current channel A overcurrent threshold
    FD_IA_LCTH = 0x5e,    //	Fast detection of current channel A undercurrent threshold
    FD_IB_OCTH = 0x5f,    //	Fast detection of current channel B overcurrent threshold
    FD_IB_LCTH = 0x60,    //	Fast detection of current channel B undercurrent threshold
    DSP_PHS_STT = 0x61,   //	Phase measurement control bit, a write operation enables a phase measurement
    DSP_PHS_U = 0x62,     //	Voltage phase value
    DSP_PHS_UN = 0x63,    //  Waveform data value before voltage passes zero point
    DSP_PHS_UP = 0x64,    //	Waveform data value after voltage passes zero point
    DSP_PHS_I = 0x65,     //	Current phase value
    DSP_PHS_IN = 0x66,    //	Waveform data value before the current passes zero
    DSP_PHS_IP = 0x67,    //	Waveform data value after the current passes zero point
    TEMPERATE = 0x68,     //	Temperature register
    DAT_WAVE =
        0x69, //  To read waveform data, you can read this address repeatedly to obtain complete waveform data
              // If you do not need to read all the data, you can reset the read address through DSP_CTRL5 bit 31.
    DAT_SWELL_CNT = 0x6a, //	Voltage surge time record, half wave as the unit. 24 bits are valid
                          // Writing any value to this address can clear the count value
    DAT_DIP_CNT = 0x6b,   //	Voltage sag time record, half wave as unit. 24 bits are valid
                          // Writing any value to this address can clear the count value
    SYS_SFTRST = 0x6C,    //	Software reset register
    SYS_FPGACW0 = 0x6D,   //	FPGA signal source control bit 0
    SYS_FPGACW1 = 0x6E,   //  FPGA signal source control bit 1
    SYS_FPGACW2 = 0x6F,   //	FPGA signal source control bit 2
    SYS_BAUDCNT1 = 0x70,  //	UART receive data 1-bit count value.
    SYS_BAUDCNT8 = 0x71,  //	UART receive data 8-bit count value.
    SYS_INTSTS = 0x72,    //	Interrupt status register
    SYS_INTEN = 0x73,     //	Interrupt enable register
    SYS_STS = 0x74,       //	System Status Register
    SYS_MISC = 0x75,      //	Hybrid Control Register
    SYS_ROMCS = 0x76,     //	ROM checksum register
    SYS_RAMADDR = 0x77,   //	Internal RAM address register.
    SYS_RAMDATA = 0x78,   //	Internal RAM data register.
    SYS_BLK_ADDR0 = 0x79, //	Block read and write address register 0
    SYS_BLK_ADDR1 = 0x7a, //  Block read and write address register 1
    SYS_BLK_ADDR2 = 0x7b, //	Block read and write address register 2
    SYS_BLK_ADDR3 = 0x7c, //	Block read and write address register 3
    SYS_IOCFG0 = 0x7d,    //	P0, P1, P2, P3 output configuration register
    SYS_IOCFG1 = 0x7e,    //	P4, P5, P6 output configuration register
    SYS_VERSION = 0x7F,   // Version Number Register
};

/*----------------------- Registers' bits definition -------------------------*/
///
/// DSP_CTRL1
///
#define DSP_CTRL1_EGYLCEN (1 << 15)
#define DSP_CTRL1_CALCEN1 (1 << 6)
#define DSP_CTRL1_CALCEN2 (1 << 7)
#define DSP_CTRL1_SLPMODE (1 << 24)
///
/// DSP_CTRL2
///
#define DSP_CTRL2_INMODE4_Msk   (3 << 30)
#define DSP_CTRL2_INMODE4_CONST (2 << 30)
#define DSP_CTRL2_INMODE3_Msk   (3 << 22)
#define DSP_CTRL2_INMODE3_CONST (2 << 22)
#define DSP_CTRL2_INMODE2_Msk   (3 << 14)
#define DSP_CTRL2_INMODE2_CONST (2 << 14)
#define DSP_CTRL2_INMODE1_Msk   (3 << 6)
#define DSP_CTRL2_INMODE1_CONST (2 << 6)
///
/// DSP_CTRL3
///
#define DSP_CTRL3_INMODE8_Msk   (uint32_t)(3 << 30)
#define DSP_CTRL3_INMODE8_CONST (uint32_t)(2 << 30)
#define DSP_CTRL3_INMODE7_Msk   (3 << 22)
#define DSP_CTRL3_INMODE7_CONST (2 << 22)
#define DSP_CTRL3_INMODE6_Msk   (3 << 14)
#define DSP_CTRL3_INMODE6_CONST (2 << 14)
#define DSP_CTRL3_INMODE5_Msk   (3 << 6)
#define DSP_CTRL3_INMODE5_CONST (2 << 6)
///
/// DSP_CTRL5
///
#define DSP_CTRL5_DMAMODE_Pos                 0
#define DSP_CTRL5_DMAMODE_Msk                 (7 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_0                   (0 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_1                   (1 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_2                   (2 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_3                   (3 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_4                   (4 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_5                   (4 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_6                   (4 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_7                   (4 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMA_CTRL_Pos                0
#define DSP_CTRL5_DMA_CTRL_Msk                (3 << DSP_CTRL5_DMA_CTRL_Pos)
#define DSP_CTRL5_DMA_CTRL_DISABLE            (0 << DSP_CTRL5_DMA_CTRL_Pos)
#define DSP_CTRL5_DMA_CTRL_ENABLE             (1 << DSP_CTRL5_DMA_CTRL_Pos)
#define DSP_CTRL5_DMA_CTRL_STOP               (2 << DSP_CTRL5_DMA_CTRL_Pos)
#define DSP_CTRL5_DMAMODE_Msk                 (7 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAEN                       (1 << 3)
#define DSP_CTRL5_SPI_PHA                     (1 << 5)
#define DSP_CTRL5_SPI_POL                     (1 << 6)
#define DSP_CTRL5_SP_CHECK                    (1 << 7)
#define DSP_CTRL5_WVAE_IB                     (1 << 8)
#define DSP_CTRL5_WAVE_IA                     (1 << 9)
#define DSP_CTRL5_WAVE_U                      (1 << 10)
#define DSP_CTRL5_WAVE_LEN_Pos                16
#define DSP_CTRL5_WAVE_LEN_Msk                (15 << DSP_CTRL5_WAVE_LEN_Pos)
#define DSP_CTRL5_TRIG_U_OV                   (1 << 20)
#define DSP_CTRL5_TRIG_U_LV                   (1 << 21)
#define DSP_CTRL5_TRIG_IA_OC                  (1 << 22)
#define DSP_CTRL5_TRIG_IA_LC                  (1 << 23)
#define DSP_CTRL5_TRIG_IB_OC                  (1 << 24)
#define DSP_CTRL5_TRIG_IB_LC                  (1 << 25)
#define DSP_CTRL5_TRIG_U_SWELL                (1 << 26)
#define DSP_CTRL5_TRIG_U_DIP                  (1 << 27)
#define DSP_CTRL5_TRIG_MANUAL                 (1 << 28)
#define DSP_CTRL5_WAVEMEM_MODE_Pos            29
#define DSP_CTRL5_WAVEMEM_MODE_Msk            (3 << DSP_CTRL5_WAVEMEM_MODE_Pos)
#define DSP_CTRL5_WAVEMEM_MODE_MANUAL_SINGLE  (0 << DSP_CTRL5_WAVEMEM_MODE_Pos)
#define DSP_CTRL5_WAVEMEM_MODE_CYCLIC         (1 << DSP_CTRL5_WAVEMEM_MODE_Pos)
#define DSP_CTRL5_WAVEMEM_MODE_TRIGGER_SINGLE (2 << DSP_CTRL5_WAVEMEM_MODE_Pos)
#define DSP_CTRL5_WAVEMEM_MODE_DISABLE        (3 << DSP_CTRL5_WAVEMEM_MODE_Pos)
#define DSP_CTRL5_WAVE_ADDR_CLR               (1 << 31)
///
/// SYS_INTSTS
///
#define SYS_INTSTS_Msk       (0x0FF9FFFF)
#define SYS_INTSTS_PHSDONE   (1 << 0)
#define SYS_INTSTS_INTPDN    (1 << 1)
#define SYS_INTSTS_AVGPWRUPD (1 << 2)
#define SYS_INTSTS_CURPWRUPD (1 << 3)
#define SYS_INTSTS_AVGRMSUPD (1 << 4)
#define SYS_INTSTS_CURRMSUPD (1 << 5)
#define SYS_INTSTS_WAVEUPD   (1 << 6)
#define SYS_INTSTS_WAVESTORE (1 << 7)
#define SYS_INTSTS_WAVEOV    (1 << 8)
#define SYS_INTSTS_USIGN     (1 << 9)
#define SYS_INTSTS_HSEFAIL   (1 << 13)
#define SYS_INTSTS_REFERR    (1 << 12)
#define SYS_INTSTS_BISTERR   (1 << 11)
#define SYS_INTSTS_ISIGN     (1 << 10)
#define SYS_INTSTS_CKERR     (1 << 14)
#define SYS_INTSTS_DMAFINISH (1 << 15)
#define SYS_INTSTS_UARTERR   (1 << 16)
#define SYS_INTSTS_SPIERR    (1 << 19)
#define SYS_INTSTS_UAOV      (1 << 20)
#define SYS_INTSTS_UALV      (1 << 21)
#define SYS_INTSTS_IAOC      (1 << 22)
#define SYS_INTSTS_IALC      (1 << 23)
#define SYS_INTSTS_IBOC      (1 << 24)
#define SYS_INTSTS_IBLC      (1 << 25)
#define SYS_INTSTS_USWELL    (1 << 26)
#define SYS_INTSTS_UDIP      (1 << 27)
///
/// SYS_STS
///
#define SYS_STS_CKERR          (1U << 0)
#define SYS_STS_REFLK          (1U << 1)
#define SYS_STS_PD             (1U << 2)
#define SYS_STS_RAMINITIAL     (1U << 4)
#define SYS_STS_BISTERR        (1U << 5)
#define SYS_STS_HSEFAIL        (1U << 6)
#define SYS_STS_RSTSOURCE      (7U << 20)
#define SYS_STS_RSTSOURCE_POR  (1U << 20)
#define SYS_STS_RSTSOURCE_EXT  (2U << 20)
#define SYS_STS_RSTSOURCE_RX   (3U << 20)
#define SYS_STS_RSTSOURCE_SOFT (4U << 20)
#define SYS_STS_UAOV           (1 << 23)
#define SYS_STS_UALV           (1 << 24)
#define SYS_STS_IAOC           (1 << 25)
#define SYS_STS_IALC           (1 << 26)
#define SYS_STS_IBOC           (1 << 27)
#define SYS_STS_IBLC           (1 << 28)
#define SYS_STS_USWELL         (1 << 29)
#define SYS_STS_UDIP           (1 << 30)
///
/// SYS_MISC
///
#define SYS_MISC_UARTBURSTEN_Pos   (1U)
#define SYS_MISC_UARTBURSTEN_Msk   (1U << SYS_MISC_UARTBURSTEN_Pos)
#define SYS_MISC_UARTBURSTEN       SYS_MISC_UARTBURSTEN_Msk
#define SYS_MISC_UARTAUTOEN_Pos    (0U)
#define SYS_MISC_UARTAUTOEN_Msk    (1U << SYS_MISC_UARTAUTOEN_Pos)
#define SYS_MISC_UARTAUTOEN        SYS_MISC_UARTAUTOEN_Msk
#define SYS_MISC_INTPOL            (1U << 2)
#define SYS_MISC_CKEGYEN           (1 << 3)
#define SYS_MISC_WAVESTORE_CNT_Pos (16)
#define SYS_MISC_WAVESTORE_CNT_Msk (511 << SYS_MISC_WAVESTORE_CNT_Pos)
///
/// SYS_ROMCS
///
#define SYS_ROMCS_CSMODE ((uint32_t)(1 << 31))
///
/// SYS_RAMADDR
///
#define SYS_RAMADDR_REQ  (1U << 15)
#define SYS_RAMADDR_WR   (1U << 14)
#define SYS_RAMADDR_ADDR (0X7FFU << 0)

//======================================================================
#define SysCtrl_F80PPM 0x88
#define SysCtrl_F70PPM 0x87
#define SysCtrl_F60PPM 0x86
#define SysCtrl_F50PPM 0x85
#define SysCtrl_F40PPM 0x84
#define SysCtrl_F30PPM 0x83
#define SysCtrl_F20PPM 0x82
#define SysCtrl_F10PPM 0x81
#define SysCtrl_0PPM   0x00
#define SysCtrl_10PPM  0x01
#define SysCtrl_20PPM  0x02
#define SysCtrl_30PPM  0x03
#define SysCtrl_40PPM  0x04
#define SysCtrl_50PPM  0x05
#define SysCtrl_60PPM  0x06
#define SysCtrl_70PPM  0x07
#define SysCtrl_80PPM  0x08

#define PPM_VALUE SysCtrl_0PPM

#define FRC_50HZ 0x01
#define FRC_60HZ 0x02

#endif