#ifndef __RACCOON_H__
#define __RACCOON_H__


#ifndef  RACCOONEXT
    #define RACCOONEXT extern
#else
    #define RACCOONSELF
#endif


#define	DSP_ANA0	        0x00	//Analog control register	0
#define	DSP_ANA1	        0X01	//Analog control register	1
#define	DSP_CTRL0	        0X02	//Digital control register	0
#define	DSP_CTRL1	        0x03	//Digital control register	1
#define	DSP_CTRL2	        0x04	//Digital control register2
#define	DSP_CTRL3	        0x05	//Digital control register3
#define	DSP_CTRL4	        0x06	//Digital control register4
#define	DSP_CTRL5	        0x07	//Digital control register5
#define	DSP_DAT_PA	        0x08	//A Channel instantaneous active power	
#define	DSP_DAT_QA	        0x09	//A Channel instantaneous reactive power	
#define	DSP_DAT_SA	        0x0a	//A Channel instantaneous apparent power	
#define	DSP_DAT_PB	        0x0b	//B Channel instantaneous active power
#define	DSP_DAT_QB	        0x0c	//B Channel instantaneous reactive power	
#define	DSP_DAT_SB	        0x0d	//B Channel instantaneous reactive power
#define	DSP_DAT_RMS0UA	        0x0e	//Voltage instantaneous effective value
#define	DSP_DAT_RMS0IA	        0x0f	//Current A instantaneous effective value
#define	DSP_DAT_RMS0IB	        0x10	//Current A instantaneous effective value
#define	DSP_DAT_CH1	        0x11	//	Fundamental optional channel 1 instantaneous value
#define	DSP_DAT_CH2	        0x12	//	Fundamental optional channel 2 instantaneous value
#define	DSP_DAT_PA1	        0x13	//	A Channel average active power
#define	DSP_DAT_QA1	        0x14	//	A Channel average reactive power
#define	DSP_DAT_SA1	        0x15	//	A Channel average apparent power
#define	DSP_DAT_PB1	        0x16	//	B Channel average active power
#define	DSP_DAT_QB1	        0x17	//	B Channel average reactive power
#define	DSP_DAT_SB1	        0x18	//	B Channel average apparent power
#define	DSP_DAT_RMS1UA	        0x19	//	Voltage average effective value
#define	DSP_DAT_RMS1IA	        0x1a	//  Average effective value of current A	
#define	DSP_DAT_RMS1IB	        0x1b	//	Current B average effective value
#define	DSP_DAT_CH1_AVG	        0x1c	//	Fundamental optional channel 1 average
#define	DSP_DAT_CH2_AVG	        0x1d	//	Fundamental optional channel 2 average
#define	DSP_DAT_RMSU_AVG	0x1e	//	Average voltage rms value over 10 or 12 cycles (selected by grid frequency)
#define	DSP_DAT_RMSIA_AVG	0x1f	//	Average RMS value of current IA over 10 or 12 cycles (selected by grid frequency)
#define	DSP_DAT_RMSIB_AVG	0x20	//	Average current IB rms value for 10 or 12 cycles (selected by grid frequency)
#define	DSP_DAT_FRQ	        0x21	//	Grid frequency
#define	DSP_DAT_DCU	        0x22	//	Voltage channel DC value
#define	DSP_DAT_DCI	        0x23	//	Current A channel DC value
#define	DSP_DAT_DCIB	        0x24	//	Current B channel DC value
#define	DSP_CFG_CALI_PA	        0x25	// Active power A ratio difference correction
#define	DSP_CFG_DC_PA	        0x26	//  Active power A small signal correction	
#define	DSP_CFG_CALI_QA	        0x27	//	Reactive power A ratio difference correction
#define	DSP_CFG_DC_QA	        0x28	//	Reactive power A small signal correction      
#define	DSP_CFG_CALI_PB	        0x29	//	Active power B ratio difference correction
#define	DSP_CFG_DC_PB	        0x2a	//	Active power B small signal correction
#define	DSP_CFG_CALI_QB	        0x2b	//	Reactive power B ratio difference correction
#define	DSP_CFG_DC_QB	        0x2c	//	Reactive power B small signal correction 
#define	DSP_CFG_CALI_RMSUA	0x2d	//	Voltage RMS ratio correction
#define	DSP_CFG_RMS_DCUA	0x2e	//	Voltage RMS small signal correction
#define	DSP_CFG_CALI_RMSIA	0x2f	//	Current A effective value ratio correction
#define	DSP_CFG_RMS_DCIA	0x30	//	Current A RMS small signal correction
#define	DSP_CFG_CALI_RMSIB	0x31	//	Current B effective value ratio difference correction
#define	DSP_CFG_RMS_DCIB	0x32	//	Current B RMS small signal correction
#define	DSP_CFG_PHC	        0x33	//	Angular error correction register
#define	DSP_CFG_DCUA	        0x34	//	Voltage channel DC correction value
#define	DSP_CFG_DCIA	        0x35	//	Current A channel DC correction value
#define	DSP_CFG_DCIB	        0x36	//	Current B channel DC correction value
#define	DSP_CFG_BPF	        0x37	//	Bandpass filter coefficients
#define	DSP_CFG_CKSUM	        0x38	//	Checksum Configuration Registers
#define	EGY_PROCTH	        0x39	//	Energy register anti-submarine threshold (less than it will not accumulate)
#define	EGY_PWRTH	        0x3a	//	Energy register accumulation threshold. Since the energy register is 46 bits, 
// the actual high-speed energy register accumulation threshold is this threshold * 16384; 
// the low-speed energy register accumulation threshold is this value * 4.
#define	EGY_CONST1	        0x3b	// Energy register 1 Accumulation constant	
#define	EGY_OUT1L	        0x3c	//	Energy register 1 Accumulated value low
#define	EGY_OUT1H	        0x3d	//	Energy register 1 Accumulated value high
#define	EGY_CFCNT1	        0x3e	//	Energy register 1 Pulse mixer
#define	EGY_CONST2	        0x3f	//	Energy register 2 Accumulation Constant
#define	EGY_OUT2L	        0x40	//	Energy register 2 accumulated value low
#define	EGY_OUT2H	        0x41	//	Energy register 2 Accumulated Value High
#define	EGY_CFCNT2	        0x42	//	Energy register 2 Pulse Counter
#define	EGY_CONST3	        0x43	//	Energy register 3 cumulative constant
#define	EGY_OUT3	        0x44	//	Energy register 3 Accumulated Value
#define	EGY_CFCNT3	        0x45	//	Energy register 3 Pulse Counter
#define	EGY_CONST4	        0x46	//	Energy register 4 cumulative constant
#define	EGY_OUT4	        0x47	//	Energy register 4 Accumulated Value
#define	EGY_CFCNT4	        0x48	//	Energy regsiter 4 Pulse Counter
#define	EGY_CONST5	        0x49	//  Energy register 5 Accumulation Constant
#define	EGY_OUT5	        0x4a	//	Energy register 5 Accumulated Value
#define	EGY_CFCNT5	        0x4b	//	Energy register 5 Pulse Counter
#define	EGY_CONST6	        0x4c	//	Energy register 6 cumulative constant
#define	EGY_OUT6	        0x4d	//	 Energy register 6 Accumulated Value 
#define	EGY_CFCNT6	        0x4e	//	Energy register 6 Pulse Counter
#define	EGY_CONST7	        0x4f	//	Energy register 7 cumulative constant
#define	EGY_OUT7	        0x50	//	Energy register 7 Accumulated Value
#define	EGY_CFCNT7	        0x51	//	Energy register 7 Pulse Counter
#define	EGY_CONST8	        0x52	//	Energy register 8 cumulative constant
#define	EGY_OUT8	        0x53	//	Energy register 8 Accumulated Value
#define	EGY_CFCNT8	        0x54	//	Energy register 8 Pulse Counter
#define	DSP_OV_THL	        0x55	//	Upper threshold of creep judgment
#define	DSP_OV_THH	        0x56	//  Lower threshold for creep judgment
#define	DSP_SWELL_THL	        0x57	//	Voltage swell lower threshold in current detection mode, 
// it is used to determine the effective value of current
#define	DSP_SWELL_THH	        0x58	// Voltage surge upper threshold. In current detection mode,
// it is used to determine the effective value of current.
#define	DSP_DIP_THL	        0x59	// Voltage sag lower threshold	
#define	DSP_DIP_THH	        0x5a	//	Voltage sag upper threshold
#define	FD_OVTH	                0x5b	//	Fast detection of overvoltage threshold
#define	FD_LVTH	                0x5c	//	Fast detection of undervoltage threshold
#define	FD_IA_OCTH	        0x5d	//	Fast detection of current channel A overcurrent threshold
#define	FD_IA_LCTH	        0x5e	//	Fast detection of current channel A undercurrent threshold
#define	FD_IB_OCTH	        0x5f	//	Fast detection of current channel B overcurrent threshold
#define	FD_IB_LCTH	        0x60	//	Fast detection of current channel B undercurrent threshold
#define	DSP_PHS_STT	        0x61	//	Phase measurement control bit, a write operation enables a phase measurement
#define	DSP_PHS_U	        0x62	//	Voltage phase value
#define	DSP_PHS_UN	        0x63	//  Waveform data value before voltage passes zero point	
#define	DSP_PHS_UP	        0x64	//	Waveform data value after voltage passes zero point
#define	DSP_PHS_I	        0x65	//	Current phase value
#define	DSP_PHS_IN	        0x66	//	Waveform data value before the current passes zero
#define	DSP_PHS_IP	        0x67	//	Waveform data value after the current passes zero point
#define	TEMPERATE	        0x68	//	Temperature register
#define	DAT_WAVE	        0x69	//  To read waveform data, you can read this address repeatedly to obtain complete waveform data 
//If you do not need to read all the data, you can reset the read address through DSP_CTRL5 bit 31.
#define	DAT_SWELL_CNT	        0x6a	//	Voltage surge time record, half wave as the unit. 24 bits are valid 
//Writing any value to this address can clear the count value
#define	DAT_DIP_CNT	        0x6b	//	Voltage sag time record, half wave as unit. 24 bits are valid 
//Writing any value to this address can clear the count value
#define	SYS_SFTRST	        0x6C	//	Software reset register
#define	SYS_FPGACW0	        0x6D	//	FPGA signal source control bit 0
#define	SYS_FPGACW1	        0x6E	//  FPGA signal source control bit 1
#define	SYS_FPGACW2	        0x6F	//	FPGA signal source control bit 2
#define	SYS_BAUDCNT1	        0x70	//	UART receive data 1-bit count value.
#define	SYS_BAUDCNT8	        0x71	//	UART receive data 8-bit count value.
#define	SYS_INTSTS	        0x72	//	Interrupt status register
#define	SYS_INTEN	        0x73	//	Interrupt enable register
#define	SYS_STS	                0x74	//	System Status Register
#define	SYS_MISC	        0x75	//	Hybrid Control Register
#define	SYS_ROMCS	        0x76	//	ROM checksum register
#define	SYS_RAMADDR	        0x77	//	Internal RAM address register.
#define	SYS_RAMDATA	        0x78	//	Internal RAM data register.
#define	SYS_BLK_ADDR0	        0x79	//	Block read and write address register 0
#define	SYS_BLK_ADDR1	        0x7a	//  Block read and write address register 1
#define	SYS_BLK_ADDR2	        0x7b	//	Block read and write address register 2
#define	SYS_BLK_ADDR3	        0x7c	//	Block read and write address register 3
#define	SYS_IOCFG0	        0x7d	//	P0, P1, P2, P3 output configuration register
#define	SYS_IOCFG1	        0x7e	//	P4, P5, P6 output configuration register
#define	SYS_VERSION	        0x7F	// Version Number Register

/*----------------------- Registers' bits definition -------------------------*/
///
///DSP_CTRL1
///
#define DSP_CTRL1_EGYLCEN             (1 << 15)
#define DSP_CTRL1_CALCEN1             (1 << 6)
#define DSP_CTRL1_CALCEN2             (1 << 7)
#define DSP_CTRL1_SLPMODE             (1 << 24)
///
///DSP_CTRL2
///
#define DSP_CTRL2_INMODE4_Msk         (3 << 30)
#define DSP_CTRL2_INMODE4_CONST       (2 << 30)
#define DSP_CTRL2_INMODE3_Msk         (3 << 22)
#define DSP_CTRL2_INMODE3_CONST       (2 << 22)
#define DSP_CTRL2_INMODE2_Msk         (3 << 14)
#define DSP_CTRL2_INMODE2_CONST       (2 << 14)   
#define DSP_CTRL2_INMODE1_Msk         (3 << 6)
#define DSP_CTRL2_INMODE1_CONST       (2 << 6)
///
///DSP_CTRL3
///
#define DSP_CTRL3_INMODE8_Msk         (uint32_t)(3 << 30)
#define DSP_CTRL3_INMODE8_CONST       (uint32_t)(2 << 30)
#define DSP_CTRL3_INMODE7_Msk         (3 << 22)
#define DSP_CTRL3_INMODE7_CONST       (2 << 22)
#define DSP_CTRL3_INMODE6_Msk         (3 << 14)
#define DSP_CTRL3_INMODE6_CONST       (2 << 14)   
#define DSP_CTRL3_INMODE5_Msk         (3 << 6)
#define DSP_CTRL3_INMODE5_CONST       (2 << 6)
///
///DSP_CTRL5
///
#define DSP_CTRL5_DMAMODE_Pos          0
#define DSP_CTRL5_DMAMODE_Msk         (7 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_0           (0 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_1           (1 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_2           (2 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_3           (3 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_4           (4 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_5           (4 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_6           (4 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAMODE_7           (4 << DSP_CTRL5_DMAMODE_Pos)
#define DSP_CTRL5_DMAEN               (1 << 3)
#define DSP_CTRL5_SPI_PHA             (1 << 4)
#define DSP_CTRL5_SPI_POL             (1 << 5)
#define DSP_CTRL5_SP_CHECK            (1 << 7)
#define DSP_CTRL5_WVAE_IB             (1 << 8)
#define DSP_CTRL5_WAVE_IA             (1 << 9)
#define DSP_CTRL5_WAVE_U              (1 << 10)
#define DSP_CTRL5_WAVE_LEN_Pos         16
#define DSP_CTRL5_WAVE_LEN_Msk        (15 << DSP_CTRL5_WAVE_LEN_Pos)
#define DSP_CTRL5_TRIG_U_OV           (1 << 20)
#define DSP_CTRL5_TRIG_U_LV           (1 << 21)
#define DSP_CTRL5_TRIG_IA_OC          (1 << 22)
#define DSP_CTRL5_TRIG_IA_LC          (1 << 23)
#define DSP_CTRL5_TRIG_IB_OC          (1 << 24)
#define DSP_CTRL5_TRIG_IB_LC          (1 << 25)
#define DSP_CTRL5_TRIG_U_SWELL        (1 << 25)
#define DSP_CTRL5_TRIG_U_DIP          (1 << 27)
///
///SYS_INTSTS
///
#define SYS_INTSTS_Msk                (0x0FF9FFFF)
#define SYS_INTSTS_PHSDONE            (1 << 0)
#define SYS_INTSTS_INTPDN             (1 << 1)
#define SYS_INTSTS_AVGPWRUPD          (1 << 2)
#define SYS_INTSTS_CURPWRUPD          (1 << 3)
#define SYS_INTSTS_AVGRMSUPD          (1 << 4)
#define SYS_INTSTS_CURRMSUPD          (1 << 5)
#define SYS_INTSTS_WAVEUPD            (1 << 6)
#define SYS_INTSTS_WAVESTORE          (1 << 7)
#define SYS_INTSTS_WAVEOV             (1 << 8)
#define SYS_INTSTS_USIGN              (1 << 9)
#define SYS_INTSTS_HSEFAIL            (1 << 13)
#define SYS_INTSTS_REFERR             (1 << 12)
#define SYS_INTSTS_BISTERR            (1 << 11)
#define SYS_INTSTS_ISIGN              (1 << 10)
#define SYS_INTSTS_CKERR              (1 << 14)
#define SYS_INTSTS_DMAFINISH          (1 << 15)
#define SYS_INTSTS_UARTERR            (1 << 16)
#define SYS_INTSTS_SPIERR             (1 << 19)
#define SYS_INTSTS_UAOV               (1 << 20)
#define SYS_INTSTS_UALV               (1 << 21)
#define SYS_INTSTS_IAOC               (1 << 22)
#define SYS_INTSTS_IALC               (1 << 23)
#define SYS_INTSTS_IBOC               (1 << 24)
#define SYS_INTSTS_IBLC               (1 << 25)
#define SYS_INTSTS_USWELL             (1 << 26)
#define SYS_INTSTS_UDIP               (1 << 27)
///
///SYS_STS
///
#define SYS_STS_CKERR                 (1U << 0)
#define SYS_STS_REFLK                 (1U << 1)
#define SYS_STS_PD                    (1U << 2)
#define SYS_STS_RAMINITIAL            (1U << 4)
#define SYS_STS_BISTERR               (1U << 5)
#define SYS_STS_HSEFAIL               (1U << 6)
#define SYS_STS_RSTSOURCE             (7U << 20)
#define SYS_STS_RSTSOURCE_POR         (1U << 20)
#define SYS_STS_RSTSOURCE_EXT         (2U << 20)
#define SYS_STS_RSTSOURCE_RX          (3U << 20)
#define SYS_STS_RSTSOURCE_SOFT        (4U << 20)
#define SYS_STS_UAOV                  (1 << 23)
#define SYS_STS_UALV                  (1 << 24)
#define SYS_STS_IAOC                  (1 << 25)
#define SYS_STS_IALC                  (1 << 26)
#define SYS_STS_IBOC                  (1 << 27)
#define SYS_STS_IBLC                  (1 << 28)
#define SYS_STS_USWELL                (1 << 29)
#define SYS_STS_UDIP                  (1 << 30)
///
///SYS_MISC
///
#define SYS_MISC_UARTBURSTEN_Pos      (1U)
#define SYS_MISC_UARTBURSTEN_Msk      (1U << SYS_MISC_UARTBURSTEN_Pos)    
#define SYS_MISC_UARTBURSTEN           SYS_MISC_UARTBURSTEN_Msk
#define SYS_MISC_UARTAUTOEN_Pos       (0U)
#define SYS_MISC_UARTAUTOEN_Msk       (1U << SYS_MISC_UARTAUTOEN_Pos)
#define SYS_MISC_UARTAUTOEN            SYS_MISC_UARTAUTOEN_Msk
#define SYS_MISC_INTPOL               (1U << 2)
#define SYS_MISC_CKEGYEN              (1 << 3)
///
///SYS_ROMCS
///
#define SYS_ROMCS_CSMODE              ((uint32_t)(1 << 31))
///
///SYS_RAMADDR
///
#define SYS_RAMADDR_REQ               (1U << 15)
#define SYS_RAMADDR_WR                (1U << 14)
#define SYS_RAMADDR_ADDR              (0X7FFU << 0)

//======================================================================
#define SysCtrl_F80PPM   0x88
#define SysCtrl_F70PPM   0x87
#define SysCtrl_F60PPM   0x86
#define SysCtrl_F50PPM   0x85
#define SysCtrl_F40PPM   0x84
#define SysCtrl_F30PPM   0x83
#define SysCtrl_F20PPM   0x82
#define SysCtrl_F10PPM   0x81
#define SysCtrl_0PPM     0x00
#define SysCtrl_10PPM    0x01
#define SysCtrl_20PPM    0x02
#define SysCtrl_30PPM    0x03
#define SysCtrl_40PPM    0x04
#define SysCtrl_50PPM    0x05
#define SysCtrl_60PPM    0x06
#define SysCtrl_70PPM    0x07
#define SysCtrl_80PPM    0x08

#define PPM_VALUE  SysCtrl_0PPM

#define FRC_50HZ    0x01
#define FRC_60HZ    0x02

#define HZ_CONFIG  FRC_50HZ
//--------------------------------------------------------------
//DSP_ANA1寄存器值  0x01   U: x4  IA: x32  IB: x4
//--------------------------------------------------------------
#if ( PPM_VALUE == SysCtrl_80PPM)

  #define DSP_ANA1_Value    0x00070094   // +79 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_70PPM)

  #define DSP_ANA1_Value    0x0007009A   // +70 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_60PPM)

  #define DSP_ANA1_Value    0x00070099   // +63 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_50PPM)

  #define DSP_ANA1_Value    0x0007009F   // +49 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_40PPM)

  #define DSP_ANA1_Value    0x0007009E   // +42 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_30PPM)

  #define DSP_ANA1_Value    0x00070083   // +28 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_20PPM)

  #define DSP_ANA1_Value    0x0007009C   // +24 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_10PPM)

  #define DSP_ANA1_Value    0x00070081   // +7 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_0PPM)

//  #define DSP_ANA1_Value    0x00170489          //电流I1增益由64倍改为32倍4->0 低4位0->9 0ppm改为-51ppm 
#define DSP_ANA1_Value    0x00870089 
//  #define DSP_ANA1_Value    0x00078089
// 0 PPM  0x00178480 00 01 10测外部晶振或8530提供高频时 4分频群脉冲没问题
//bit31-30 adc时钟频率800K  BIT22-21 6.5M打开 BGP打开 BIT18-16电压 A路 B路adc打开  BIT15 使用13M晶振才置1
//BIT14-12 B路电流adc增益 4倍  BIT11 电压adc增益 4倍  A路电流adc增益 64倍 BIT7 晶体停振检测使能
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_F10PPM)

  #define DSP_ANA1_Value    0x00070787   // -7 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_F20PPM)

  #define DSP_ANA1_Value    0x00070085   // -21 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_F30PPM)

  #define DSP_ANA1_Value    0x0007008B   // -30 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_F40PPM)

  #define DSP_ANA1_Value    0x0007008A   // -44 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_F50PPM)

  #define DSP_ANA1_Value    0x00070089   // -51 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_F60PPM)

  #define DSP_ANA1_Value    0x00070088   // -58 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_F70PPM)

  #define DSP_ANA1_Value    0x0007008E   // -72 PPM
//--------------------------------------------------------------
#elif ( PPM_VALUE == SysCtrl_F80PPM)

  #define DSP_ANA1_Value    0x0007008D   // -79 PPM
#else
 #error "PPM_VALUE is error"
#endif

//--------------------------------------------------------------
//DSP_CTRL0寄存器值  0x01   
//DSP工作模式:0        平均功率、有效值刷新时间:640ms  视在功率计算源选择:通过功率值计算
//平均电网频率(320ms)  瞬时功率值刷新时间:20ms         瞬时有效值刷新时间:10ms
//--------------------------------------------------------------
#if ( HZ_CONFIG == FRC_50HZ)

  #define DSP_CTRL0_Value   0x01000f07//0x00000F57   //DSP_CTRL0寄存器值   0x02   50Hz  0x01000F07 0x01000F37  群脉冲测试时有修改
//BIT31 瞬时有功率、效值刷新时间 0 20ms 10ms BIT29-28 平均频率刷新320ms  BIT24 视在功率计算源选择 0有效值 1功率值
//BIT23-20 基波通道选择 0 通道1基波有功A 通道2基波有功B BIT19-18 无功功率B A模式 0 全波无功 1基波
//BIT17-13 全波功率 计算基波数据 全波电压 电流A B有效值 是否经过高通滤波器 0经过 BIT11-10 平均有效值刷新时间 11 640ms 00 40ms
//BIT9-8 平均功率刷新 11 640ms 00 80ms 
//BIT7-4 DSP MODE 6：:3.2M 每周波采样点64
//BIT3 0：50Hz 1：60Hz
//BIT2 1 0 电压 电流B 电流A通道开关（包括adc和dsp）
//
#elif ( HZ_CONFIG == FRC_60HZ)

 // #define DSP_CTRL0_Value   0x01000F3F   //DSP_CTRL0寄存器值   0x02   60Hz
#define DSP_CTRL0_Value   0x01000F0F   //DSP_CTRL0寄存器值   0x02   60Hz
#else
  #error "HZ_CONFIG is error"
#endif

//#define freq_const 6400

#if ((( DSP_CTRL0_Value&0xf0)>>4) == 0)
    #define freq_const 6400

#elif ((( DSP_CTRL0_Value&0xf0)>>4) == 1)
    #define freq_const 6400

#elif ((( DSP_CTRL0_Value&0xf0)>>4) == 2)
    #define freq_const 6400

#elif ((( DSP_CTRL0_Value&0xf0)>>4) == 3)
    #define freq_const 12800

#elif ((( DSP_CTRL0_Value&0xf0)>>4) == 4)
    #define freq_const 12800

#elif ((( DSP_CTRL0_Value&0xf0)>>4) == 5)
    #define freq_const 12800

#elif ((( DSP_CTRL0_Value&0xf0)>>4) == 6)
    #define freq_const 3200

#elif ((( DSP_CTRL0_Value&0xf0)>>4) == 7)
    #define freq_const 3200

#elif ((( DSP_CTRL0_Value&0xf0)>>4) == 8)
    #define freq_const 1600

#endif


//--------------------------------------------------------------
//#define DSP_ANA0_Value    0x00100c00//0x02000C00    //DSP_ANA0寄存器值    0x00  0x02700C00 
#define DSP_ANA0_Value    0x00100C00
//#define DSP_ANA0_Value    0x02000C00   //DSP_ANA0寄存器值    0x00  0x02700C00 

//2对应BIT25 晶体时钟为adc时钟时分频使能1:1/2  C对应BIT11 10 电流 电压ADC输入短路
//#define DSP_CTRL1_Value   0x000C32C1   //DSP_CTRL1寄存器值   0x03
//**C对应BIT19-18 过零点方式3：关闭过零点功能 3对应BIT13 12 CF2输出使能 CF2输入源选择0：能量桶1 1：能量桶2**/
//**2对应BIT9 CF1输出使能 BIT8 CF1输入源选择0：能量桶1 1：能量桶2                                        **/
//**C对应BIT7-6 能量桶2使能 能量桶1使能 BIT5-4 脉冲宽度0:80ms  BIT0 告诉能量桶防潜开关1：使能             **/

//#define DSP_CTRL2_Value   0x00001723   //DSP_CTRL2寄存器值   0x04    1723
//**1对应BIT12 能量桶2 B通道累加开关  BIT11-10 1 无功功率累加     BIT9-8 3:能量桶累加绝对值**/
//**BIT7-6 能量桶1累加模式 0:功率累加 BIT5 能量桶1A通道累加开关   BIT1-0 3:能量桶累加绝对值**/

#define DSP_CTRL1_Value   0x000C32C1   //DSP_CTRL1寄存器值   0x03
#define DSP_CTRL2_Value   0x00002723//0x000027A3    //测试改为常数累加

//#define DSP_CTRL2_Value   0x00002723   //DSP_CTRL2寄存器值   0x04   //BIT13 12 能量桶2 AB通道开关 BIT54 能量桶1 AB通道开关
//0x00002723 能量桶2 A通道无功累加 能量桶1 A通道有功累加  BIT11-10 能量桶2累加源选择     BIT3-2 能量桶1累加源选择 
//0x00001713 能量桶2 B通道无功累加 能量桶1 B通道有功累加           0 3:有功 1:无功 2:视在
//******************************************************
//#define DSP_CTRL2_Value_ChA 0x000027A3
//#define DSP_CTRL2_Value_ChB 0x000027A3
#define DSP_CTRL2_Value_ChA 0x00002723
#define DSP_CTRL2_Value_ChB 0x00001713 
//******************************************************
#define DSP_CTRL3_Value   0x00000000   //DSP_CTRL3寄存器值   0x05
#define DSP_CTRL4_Value   0x00000000   //DSP_CTRL4寄存器值   0x06
#define DSP_CTRL5_Value   0x00000000   //DSP_CTRL5寄存器值   0x07
#define SYS_IOCFG0_Value  0X00000000//0x00004080//0x00001C19//0x00004080   //P0，P1,P2，P3输出配置寄存器  CF1 CF2 0x7d  bit7-0   00011001 晶体停中断
#define SYS_IOCFG1_Value  0x003C3A00  //P5P6配置为DMCK DMDO
//--------------------------------------------------------------            bit15-8  00011100 掉电中断
//Dsp_mode=0,1,2时，写入0x806764B6
//Dsp_mode=3,4,5时，写入0x8049d98F
//Dsp_mode=6,7时，  写入0x80DD7A8C
//Dsp_mode=8时，    写入0x82B465F0
//--------------------------------------------------------------
#define DSP_CFG_BPF_Value 0x806764B6   //DSP_CFG_BPF寄存器值 0x37


//======================================================================
#define Actual_BAUDRate 2400     //实际波特率

#define BAUDRate_1Byte_OverTime  (float)(11*1000/Actual_BAUDRate*1.4)  //通信超时时间率

//======================================================================

#define RacRead      1
#define RacWrite     2
#define RacBroadcast 0

//======================================================================
#define SlaveAddr    0

#define Raccoon_A1H()      {GPIOE->OEN &=~BIT13; GPIOE->IE &=~BIT13; GPIOE->ATT &=~BIT13; IO_DAT( IOE_DAT, 13) = 1;} //推挽输出高
#define Raccoon_A1L()      {GPIOE->OEN &=~BIT13; GPIOE->IE &=~BIT13; GPIOE->ATT &=~BIT13; IO_DAT( IOE_DAT, 13) = 0;} //推挽输出低

#define Raccoon_A0H()      {GPIOE->OEN &=~BIT14; GPIOE->IE &=~BIT14; GPIOE->ATT &=~BIT14; IO_DAT( IOE_DAT, 14) = 1;} //推挽输出高
#define Raccoon_A0L()      {GPIOE->OEN &=~BIT14; GPIOE->IE &=~BIT14; GPIOE->ATT &=~BIT14; IO_DAT( IOE_DAT, 14) = 0;} //推挽输出低

#define Raccoon_SlaveAddrCfg() {Raccoon_A1L();Raccoon_A0L();}

//======================================================================
#define Raccoon_PwrOn()    {GPIOB->OEN &=~BIT5; GPIOB->IE &=~BIT5; GPIOB->ATT &=~BIT5; IO_DAT( IOB_DAT, 5) = 1;} //推挽输出高
#define Raccoon_PwrOff()   {GPIOB->OEN &=~BIT5; GPIOB->IE &=~BIT5; GPIOB->ATT &=~BIT5; IO_DAT( IOB_DAT, 5) = 0;} //推挽输出低


enum
{
  Rac_Idle=0,
  Rac_Send,
  Rac_Rev,
  Rac_WaitPro,  
};

typedef struct 
{
  uint8_t ucStatus;               
  uint8_t ucSendLen;
  uint8_t ucSendPoint;
  uint8_t ucRevLen;
  uint8_t ucRevPoint;
  uint8_t ucBuf[100];  
}S_RacCtrl;

RACCOONEXT S_RacCtrl gs_RacCtrl;

RACCOONEXT volatile uint8_t guc_CommDelayTime;         //通讯等待延时

//======================= 应用层定义 ====================================
#define DATAP       DSP_DAT_PA1
#define DATAQ       DSP_DAT_QA1
#define RMSU        DSP_DAT_RMS1UA
#define RMSI1       DSP_DAT_RMS1IA
#define RMSI2       DSP_DAT_RMS1IB
#define FREQ        DSP_DAT_FRQ

#define DATAPB      DSP_DAT_PB1
#define DATAQB      DSP_DAT_QB1
#define DATASA      DSP_DAT_SA1
#define DATASB      DSP_DAT_SB1

//======================================================================
#ifdef RACCOONSELF

uint32_t const  RegValue[]=
{
  DSP_ANA0_Value,
  DSP_ANA1_Value,
  DSP_CTRL0_Value,
  DSP_CTRL1_Value,
  DSP_CTRL2_Value,
  DSP_CTRL3_Value,
  DSP_CTRL4_Value,
  DSP_CTRL5_Value,
  DSP_CFG_BPF_Value,
};

uint8_t const  RegAddr[]=
{
  DSP_ANA0,	//0 模拟控制寄存器0
  DSP_ANA1,	//1 模拟控制寄存器1
  DSP_CTRL0,	//2 数字控制寄存器0
  DSP_CTRL1,	//3 数字控制寄存器1
  DSP_CTRL2,	//4 数字控制寄存器2
  DSP_CTRL3,	//5 数字控制寄存器3
  DSP_CTRL4,	//6 数字控制寄存器4
  DSP_CTRL5,	//7 数字控制寄存器5
  DSP_CFG_BPF,	//8 带通滤波器系数寄存器
  
  EGY_PWRTH,	//9 有功门限值寄存器
  EGY_PROCTH,	//10 起动/潜动判断门限值寄存器
  
  DSP_CFG_PHC,  //11 角差校正寄存器
  DSP_CFG_CALI_RMSUA,	//12 全波电压有效值比差寄存器
  DSP_CFG_CALI_RMSIA,	//13 IA全波电流有效值比差寄存器
  DSP_CFG_CALI_RMSIB,	//14 IB全波电流有效值比差寄存器
  DSP_CFG_CALI_PA,	//15 有功功率A比差校正寄存器
  DSP_CFG_DC_PA,	//16 有功功率A二次补偿寄存器
  DSP_CFG_DC_QA,	//17 无功功率A二次补偿寄存器
  DSP_CFG_RMS_DCIA,      //18 电流A有效值小信号校正 ZZP0303
    
  DSP_CFG_CALI_PB,	//19 有功功率B比差校正寄存器
  DSP_CFG_DC_PB,	//20 有功功率B二次补偿寄存器
  DSP_CFG_DC_QB,	//21 无功功率B二次补偿寄存器
  DSP_CFG_RMS_DCIB,     //22 B路电流有效值小信号校正 zzp0314
};

uint8_t const  RMS_RegAddr[]=
{
  DATAP,        //第一通道有功功率
  DATAQ,        //无功功率
  RMSU,         //电压通道的有效值
  RMSI1,        //电流通道1的有效值
  RMSI2,        //电流通道2的有效值
  FREQ,         //电压频率
  DATAPB,       //B通道有功zzp0305
  DATAQB,       //B通道无功
  DATASA,       //A通道视在
  DATASB,       //B通道视在zzp0305
};
#else
extern uint8_t const  RegAddr[];
extern uint32_t const  RegValue[];
extern uint8_t const  RMS_RegAddr[];
#endif

typedef struct
{
    uint32  ul_P;
    uint32  ul_Q;
    uint32  ul_U;         
    uint32  ul_I1;    
    uint32  ul_I2;
    uint32  ul_Hz;
    uint32  ul_PB;//zzp0305
    uint32  ul_QB;
    uint32  ul_SA;
    uint32  ul_SB;//zzp0305
}S_RMS;

RACCOONEXT S_RMS gs_RmsData;

RACCOONEXT uint8  guc_RmsReadBegian;
RACCOONEXT uint8  guc_1280msCnt;

RACCOONEXT uint32 Rms_ul_U_Buf[10],Rms_ul_I1_Buf[10];
RACCOONEXT uint32 Rms_ul_I1_Data[200];

RACCOONEXT signed long Rms_ul_UC,Rms_ul_I1C;
RACCOONEXT unsigned long long Rms_ul_USum,Rms_ul_I1Sum;
RACCOONEXT uint32 Rms_ul_I1Avg,Rms_ul_UAvg;
RACCOONEXT double Rms_ul_I1p,Rms_ul_Up;
RACCOONEXT double Rms_ul_UpxI1p;
RACCOONEXT uint32_t  tmp_fra, tmp_pack;

//======================================================================
void Init_RacCtrl(void);
void RxReset_Raccoon(void);
void Init_UartRaccoon(void);
void Raccoon_Dy10ms(void);
void Raccoon_Delay(uint8_t n);//zzp0113
void Raccoon_UartTransmit(void);
void Raccoon_UartReceive(void);
void Raccoon_UpdatePar(void);
void BroadcastWriteRaccoon(uint32_t Data,uint8_t Addr);
uint8_t ReadRaccoon(uint8_t Addr,uint8_t num);
uint8_t WriteRaccoon( uint32_t Data, uint8_t Addr);
void Raccoon_RunCheck(void);
void Raccoon_EXTI5_Init(void);
void Raccoon_TMR1_Init(void);
void Raccoon_CFCtrl(void);
void Raccoon_ReadRMS(void);
void Raccoon_UpdateChecksum(void);

void DMA_DataUpload(void);

#endif


