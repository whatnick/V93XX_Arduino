#ifndef __RACCOON_H__
#define __RACCOON_H__


#ifndef  RACCOONEXT
    #define RACCOONEXT extern
#else
    #define RACCOONSELF
#endif


#define	DSP_ANA0	        0x00	//	模拟控制寄存器0
#define	DSP_ANA1	        0X01	//	模拟控制寄存器1
#define	DSP_CTRL0	        0X02	//	数字控制寄存器0
#define	DSP_CTRL1	        0x03	//	数字控制寄存器1
#define	DSP_CTRL2	        0x04	//	数字控制寄存器2
#define	DSP_CTRL3	        0x05	//	数字控制寄存器3
#define	DSP_CTRL4	        0x06	//	数字控制寄存器4
#define	DSP_CTRL5	        0x07	//	数字控制寄存器5
#define	DSP_DAT_PA	        0x08	//	A通道瞬时有功功率
#define	DSP_DAT_QA	        0x09	//	A通道瞬时无功功率
#define	DSP_DAT_SA	        0x0a	//	A通道瞬时视在功率
#define	DSP_DAT_PB	        0x0b	//	B通道瞬时有功功率
#define	DSP_DAT_QB	        0x0c	//	B通道瞬时无功功率
#define	DSP_DAT_SB	        0x0d	//	B通道瞬时视在功率
#define	DSP_DAT_RMS0UA	        0x0e	//	电压瞬时有效值
#define	DSP_DAT_RMS0IA	        0x0f	//	电流A瞬时有效值
#define	DSP_DAT_RMS0IB	        0x10	//	电流B瞬时有效值
#define	DSP_DAT_CH1	        0x11	//	基波可选通道1瞬时值
#define	DSP_DAT_CH2	        0x12	//	基波可选通道2瞬时值
#define	DSP_DAT_PA1	        0x13	//	A通道平均有功功率
#define	DSP_DAT_QA1	        0x14	//	A通道平均无功功率
#define	DSP_DAT_SA1	        0x15	//	A通道平均视在功率
#define	DSP_DAT_PB1	        0x16	//	B通道平均有功功率
#define	DSP_DAT_QB1	        0x17	//	B通道平均无功功率
#define	DSP_DAT_SB1	        0x18	//	B通道平均视在功率
#define	DSP_DAT_RMS1UA	        0x19	//	电压平均有效值
#define	DSP_DAT_RMS1IA	        0x1a	//	电流A平均有效值
#define	DSP_DAT_RMS1IB	        0x1b	//	电流B平均有效值
#define	DSP_DAT_CH1_AVG	        0x1c	//	基波可选通道1平均值
#define	DSP_DAT_CH2_AVG	        0x1d	//	基波可选通道2平均值
#define	DSP_DAT_RMSU_AVG	0x1e	//	10或12个周波（通过电网频率选择）的电压有效值平均值
#define	DSP_DAT_RMSIA_AVG	0x1f	//	10或12个周波（通过电网频率选择）的电流IA有效值平均值
#define	DSP_DAT_RMSIB_AVG	0x20	//	10或12个周波（通过电网频率选择）的电流IB有效值平均值
#define	DSP_DAT_FRQ	        0x21	//	电网频率
#define	DSP_DAT_DCU	        0x22	//	电压通道直流值
#define	DSP_DAT_DCI	        0x23	//	电流A通道直流值
#define	DSP_DAT_DCIB	        0x24	//	电流B通道直流值
#define	DSP_CFG_CALI_PA	        0x25	//	有功功率A比差校正
#define	DSP_CFG_DC_PA	        0x26	//	有功功率A小信号校正
#define	DSP_CFG_CALI_QA	        0x27	//	无功功率A比差校正
#define	DSP_CFG_DC_QA	        0x28	//	无功功率A小信号校正
#define	DSP_CFG_CALI_PB	        0x29	//	有功功率B比差校正
#define	DSP_CFG_DC_PB	        0x2a	//	有功功率B小信号校正
#define	DSP_CFG_CALI_QB	        0x2b	//	无功功率B比差校正
#define	DSP_CFG_DC_QB	        0x2c	//	无功功率B小信号校正
#define	DSP_CFG_CALI_RMSUA	0x2d	//	电压有效值比差校正
#define	DSP_CFG_RMS_DCUA	0x2e	//	电压有效值小信号校正
#define	DSP_CFG_CALI_RMSIA	0x2f	//	电流A有效值比差校正
#define	DSP_CFG_RMS_DCIA	0x30	//	电流A有效值小信号校正
#define	DSP_CFG_CALI_RMSIB	0x31	//	电流B有效值比差校正
#define	DSP_CFG_RMS_DCIB	0x32	//	电流B有效值小信号校正
#define	DSP_CFG_PHC	        0x33	//	角差校正寄存器。
#define	DSP_CFG_DCUA	        0x34	//	电压通道直流校正值
#define	DSP_CFG_DCIA	        0x35	//	电流A通道直流校正值
#define	DSP_CFG_DCIB	        0x36	//	电流B通道直流校正值
#define	DSP_CFG_BPF	        0x37	//	带通滤波器系数。
#define	DSP_CFG_CKSUM	        0x38	//	校验和配置寄存器
#define	EGY_PROCTH	        0x39	//	能量桶防潜阈值（小于他不累加）
#define	EGY_PWRTH	        0x3a	//	能量桶累加阈值。由于能量桶为46bit，实际高速能量桶累加阈值为该阈值*16384；低速能量桶累加阈值为该值*4。
#define	EGY_CONST1	        0x3b	//	能量桶1累加常数
#define	EGY_OUT1L	        0x3c	//	能量桶1累加值低位
#define	EGY_OUT1H	        0x3d	//	能量桶1累加值高位。
#define	EGY_CFCNT1	        0x3e	//	能量桶1脉冲计数器。
#define	EGY_CONST2	        0x3f	//	能量桶2累加常数。
#define	EGY_OUT2L	        0x40	//	能量桶2累加值低位。
#define	EGY_OUT2H	        0x41	//	能量桶2累加值高位。
#define	EGY_CFCNT2	        0x42	//	能量桶2脉冲计数器。
#define	EGY_CONST3	        0x43	//	能量桶3累加常数。
#define	EGY_OUT3	        0x44	//	能量桶3累加值
#define	EGY_CFCNT3	        0x45	//	能量桶3脉冲计数器
#define	EGY_CONST4	        0x46	//	能量桶4累加常数
#define	EGY_OUT4	        0x47	//	能量桶4累加值
#define	EGY_CFCNT4	        0x48	//	能量桶4脉冲计数器
#define	EGY_CONST5	        0x49	//	能量桶5累加常数
#define	EGY_OUT5	        0x4a	//	能量桶5累加值
#define	EGY_CFCNT5	        0x4b	//	能量桶5脉冲计数器
#define	EGY_CONST6	        0x4c	//	能量桶6累加常数
#define	EGY_OUT6	        0x4d	//	能量桶6累加值
#define	EGY_CFCNT6	        0x4e	//	能量桶6脉冲计数器
#define	EGY_CONST7	        0x4f	//	能量桶7累加常数
#define	EGY_OUT7	        0x50	//	能量桶7累加值
#define	EGY_CFCNT7	        0x51	//	能量桶7脉冲计数器
#define	EGY_CONST8	        0x52	//	能量桶8累加常数
#define	EGY_OUT8	        0x53	//	能量桶8累加值
#define	EGY_CFCNT8	        0x54	//	能量桶8脉冲计数器
#define	DSP_OV_THL	        0x55	//	潜动判断上限阈值
#define	DSP_OV_THH	        0x56	//	潜动判断下限阈值
#define	DSP_SWELL_THL	        0x57	//	电压骤升下限阈值。电流检测模式时，用来判断电流有效值。
#define	DSP_SWELL_THH	        0x58	//	电压骤升上限阈值。电流检测模式时，用来判断电流有效值。
#define	DSP_DIP_THL	        0x59	//	电压骤降下限阈值
#define	DSP_DIP_THH	        0x5a	//	电压骤降上限阈值
#define	FD_OVTH	                0x5b	//	快速检测过压阈值。
#define	FD_LVTH	                0x5c	//	快速检测欠压阈值。
#define	FD_IA_OCTH	        0x5d	//	快速检测电流A通道过流阈值。
#define	FD_IA_LCTH	        0x5e	//	快速检测电流A通道欠流阈值。
#define	FD_IB_OCTH	        0x5f	//	快速检测电流B通道过流阈值。
#define	FD_IB_LCTH	        0x60	//	快速检测电流B通道欠流阈值。
#define	DSP_PHS_STT	        0x61	//	相位测量控制位，写操作使能一次相位测量。
#define	DSP_PHS_U	        0x62	//	电压相位值
#define	DSP_PHS_UN	        0x63	//	电压过零点之前波形数据值
#define	DSP_PHS_UP	        0x64	//	电压过零点之后波形数据值
#define	DSP_PHS_I	        0x65	//	电流相位值
#define	DSP_PHS_IN	        0x66	//	电流过零点之前波形数据值
#define	DSP_PHS_IP	        0x67	//	电流过零点之后波形数据值
#define	TEMPERATE	        0x68	//	温度寄存器
#define	DAT_WAVE	        0x69	//	波形数据读取，可重复读取该地址，从而获得完整波形数据。若不需要读完全部数据，可通过DSP_CTRL5位31复位读取地址。
#define	DAT_SWELL_CNT	        0x6a	//	电压骤升时间记录，半波为单位。24bit有效。向该地址写任意值，可清零该计数值。
#define	DAT_DIP_CNT	        0x6b	//	电压骤降时间记录，半波为单位。24bit有效。向该地址写任意值，可清零该计数值。
#define	SYS_SFTRST	        0x6C	//	软件复位寄存器。
#define	SYS_FPGACW0	        0x6D	//	FPGA信号源控制位0
#define	SYS_FPGACW1	        0x6E	//	FPGA 信号源控制位1
#define	SYS_FPGACW2	        0x6F	//	FPGA 信号源控制位2
#define	SYS_BAUDCNT1	        0x70	//	UART接收数据 1 位计数值。
#define	SYS_BAUDCNT8	        0x71	//	UART 接收数据8 位计数值。
#define	SYS_INTSTS	        0x72	//	中断状态寄存器。
#define	SYS_INTEN	        0x73	//	中断使能寄存器。
#define	SYS_STS	                0x74	//	系统状态寄存器
#define	SYS_MISC	        0x75	//	混合控制寄存器
#define	SYS_ROMCS	        0x76	//	ROM 校验和寄存器.
#define	SYS_RAMADDR	        0x77	//	Internal RAM address register.
#define	SYS_RAMDATA	        0x78	//	Internal RAM data register.
#define	SYS_BLK_ADDR0	        0x79	//	块读写地址寄存器0
#define	SYS_BLK_ADDR1	        0x7a	//	块读写地址寄存器1
#define	SYS_BLK_ADDR2	        0x7b	//	块读写地址寄存器2
#define	SYS_BLK_ADDR3	        0x7c	//	块读写地址寄存器3
#define	SYS_IOCFG0	        0x7d	//	P0，P1,P2，P3输出配置寄存器
#define	SYS_IOCFG1	        0x7e	//	P4，P5，P6输出配置寄存器
#define	SYS_VERSION	        0x7F	//	版本号寄存器。

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


