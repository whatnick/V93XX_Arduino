#ifndef __RACCOON_H__
#define __RACCOON_H__


#ifndef  RACCOONEXT
    #define RACCOONEXT extern
#else
    #define RACCOONSELF
#endif


#define	DSP_ANA0	        0x00	//	ģ����ƼĴ���0
#define	DSP_ANA1	        0X01	//	ģ����ƼĴ���1
#define	DSP_CTRL0	        0X02	//	���ֿ��ƼĴ���0
#define	DSP_CTRL1	        0x03	//	���ֿ��ƼĴ���1
#define	DSP_CTRL2	        0x04	//	���ֿ��ƼĴ���2
#define	DSP_CTRL3	        0x05	//	���ֿ��ƼĴ���3
#define	DSP_CTRL4	        0x06	//	���ֿ��ƼĴ���4
#define	DSP_CTRL5	        0x07	//	���ֿ��ƼĴ���5
#define	DSP_DAT_PA	        0x08	//	Aͨ��˲ʱ�й�����
#define	DSP_DAT_QA	        0x09	//	Aͨ��˲ʱ�޹�����
#define	DSP_DAT_SA	        0x0a	//	Aͨ��˲ʱ���ڹ���
#define	DSP_DAT_PB	        0x0b	//	Bͨ��˲ʱ�й�����
#define	DSP_DAT_QB	        0x0c	//	Bͨ��˲ʱ�޹�����
#define	DSP_DAT_SB	        0x0d	//	Bͨ��˲ʱ���ڹ���
#define	DSP_DAT_RMS0UA	        0x0e	//	��ѹ˲ʱ��Чֵ
#define	DSP_DAT_RMS0IA	        0x0f	//	����A˲ʱ��Чֵ
#define	DSP_DAT_RMS0IB	        0x10	//	����B˲ʱ��Чֵ
#define	DSP_DAT_CH1	        0x11	//	������ѡͨ��1˲ʱֵ
#define	DSP_DAT_CH2	        0x12	//	������ѡͨ��2˲ʱֵ
#define	DSP_DAT_PA1	        0x13	//	Aͨ��ƽ���й�����
#define	DSP_DAT_QA1	        0x14	//	Aͨ��ƽ���޹�����
#define	DSP_DAT_SA1	        0x15	//	Aͨ��ƽ�����ڹ���
#define	DSP_DAT_PB1	        0x16	//	Bͨ��ƽ���й�����
#define	DSP_DAT_QB1	        0x17	//	Bͨ��ƽ���޹�����
#define	DSP_DAT_SB1	        0x18	//	Bͨ��ƽ�����ڹ���
#define	DSP_DAT_RMS1UA	        0x19	//	��ѹƽ����Чֵ
#define	DSP_DAT_RMS1IA	        0x1a	//	����Aƽ����Чֵ
#define	DSP_DAT_RMS1IB	        0x1b	//	����Bƽ����Чֵ
#define	DSP_DAT_CH1_AVG	        0x1c	//	������ѡͨ��1ƽ��ֵ
#define	DSP_DAT_CH2_AVG	        0x1d	//	������ѡͨ��2ƽ��ֵ
#define	DSP_DAT_RMSU_AVG	0x1e	//	10��12���ܲ���ͨ������Ƶ��ѡ�񣩵ĵ�ѹ��Чֵƽ��ֵ
#define	DSP_DAT_RMSIA_AVG	0x1f	//	10��12���ܲ���ͨ������Ƶ��ѡ�񣩵ĵ���IA��Чֵƽ��ֵ
#define	DSP_DAT_RMSIB_AVG	0x20	//	10��12���ܲ���ͨ������Ƶ��ѡ�񣩵ĵ���IB��Чֵƽ��ֵ
#define	DSP_DAT_FRQ	        0x21	//	����Ƶ��
#define	DSP_DAT_DCU	        0x22	//	��ѹͨ��ֱ��ֵ
#define	DSP_DAT_DCI	        0x23	//	����Aͨ��ֱ��ֵ
#define	DSP_DAT_DCIB	        0x24	//	����Bͨ��ֱ��ֵ
#define	DSP_CFG_CALI_PA	        0x25	//	�й�����A�Ȳ�У��
#define	DSP_CFG_DC_PA	        0x26	//	�й�����AС�ź�У��
#define	DSP_CFG_CALI_QA	        0x27	//	�޹�����A�Ȳ�У��
#define	DSP_CFG_DC_QA	        0x28	//	�޹�����AС�ź�У��
#define	DSP_CFG_CALI_PB	        0x29	//	�й�����B�Ȳ�У��
#define	DSP_CFG_DC_PB	        0x2a	//	�й�����BС�ź�У��
#define	DSP_CFG_CALI_QB	        0x2b	//	�޹�����B�Ȳ�У��
#define	DSP_CFG_DC_QB	        0x2c	//	�޹�����BС�ź�У��
#define	DSP_CFG_CALI_RMSUA	0x2d	//	��ѹ��Чֵ�Ȳ�У��
#define	DSP_CFG_RMS_DCUA	0x2e	//	��ѹ��ЧֵС�ź�У��
#define	DSP_CFG_CALI_RMSIA	0x2f	//	����A��Чֵ�Ȳ�У��
#define	DSP_CFG_RMS_DCIA	0x30	//	����A��ЧֵС�ź�У��
#define	DSP_CFG_CALI_RMSIB	0x31	//	����B��Чֵ�Ȳ�У��
#define	DSP_CFG_RMS_DCIB	0x32	//	����B��ЧֵС�ź�У��
#define	DSP_CFG_PHC	        0x33	//	�ǲ�У���Ĵ�����
#define	DSP_CFG_DCUA	        0x34	//	��ѹͨ��ֱ��У��ֵ
#define	DSP_CFG_DCIA	        0x35	//	����Aͨ��ֱ��У��ֵ
#define	DSP_CFG_DCIB	        0x36	//	����Bͨ��ֱ��У��ֵ
#define	DSP_CFG_BPF	        0x37	//	��ͨ�˲���ϵ����
#define	DSP_CFG_CKSUM	        0x38	//	У������üĴ���
#define	EGY_PROCTH	        0x39	//	����Ͱ��Ǳ��ֵ��С�������ۼӣ�
#define	EGY_PWRTH	        0x3a	//	����Ͱ�ۼ���ֵ����������ͰΪ46bit��ʵ�ʸ�������Ͱ�ۼ���ֵΪ����ֵ*16384����������Ͱ�ۼ���ֵΪ��ֵ*4��
#define	EGY_CONST1	        0x3b	//	����Ͱ1�ۼӳ���
#define	EGY_OUT1L	        0x3c	//	����Ͱ1�ۼ�ֵ��λ
#define	EGY_OUT1H	        0x3d	//	����Ͱ1�ۼ�ֵ��λ��
#define	EGY_CFCNT1	        0x3e	//	����Ͱ1�����������
#define	EGY_CONST2	        0x3f	//	����Ͱ2�ۼӳ�����
#define	EGY_OUT2L	        0x40	//	����Ͱ2�ۼ�ֵ��λ��
#define	EGY_OUT2H	        0x41	//	����Ͱ2�ۼ�ֵ��λ��
#define	EGY_CFCNT2	        0x42	//	����Ͱ2�����������
#define	EGY_CONST3	        0x43	//	����Ͱ3�ۼӳ�����
#define	EGY_OUT3	        0x44	//	����Ͱ3�ۼ�ֵ
#define	EGY_CFCNT3	        0x45	//	����Ͱ3���������
#define	EGY_CONST4	        0x46	//	����Ͱ4�ۼӳ���
#define	EGY_OUT4	        0x47	//	����Ͱ4�ۼ�ֵ
#define	EGY_CFCNT4	        0x48	//	����Ͱ4���������
#define	EGY_CONST5	        0x49	//	����Ͱ5�ۼӳ���
#define	EGY_OUT5	        0x4a	//	����Ͱ5�ۼ�ֵ
#define	EGY_CFCNT5	        0x4b	//	����Ͱ5���������
#define	EGY_CONST6	        0x4c	//	����Ͱ6�ۼӳ���
#define	EGY_OUT6	        0x4d	//	����Ͱ6�ۼ�ֵ
#define	EGY_CFCNT6	        0x4e	//	����Ͱ6���������
#define	EGY_CONST7	        0x4f	//	����Ͱ7�ۼӳ���
#define	EGY_OUT7	        0x50	//	����Ͱ7�ۼ�ֵ
#define	EGY_CFCNT7	        0x51	//	����Ͱ7���������
#define	EGY_CONST8	        0x52	//	����Ͱ8�ۼӳ���
#define	EGY_OUT8	        0x53	//	����Ͱ8�ۼ�ֵ
#define	EGY_CFCNT8	        0x54	//	����Ͱ8���������
#define	DSP_OV_THL	        0x55	//	Ǳ���ж�������ֵ
#define	DSP_OV_THH	        0x56	//	Ǳ���ж�������ֵ
#define	DSP_SWELL_THL	        0x57	//	��ѹ����������ֵ���������ģʽʱ�������жϵ�����Чֵ��
#define	DSP_SWELL_THH	        0x58	//	��ѹ����������ֵ���������ģʽʱ�������жϵ�����Чֵ��
#define	DSP_DIP_THL	        0x59	//	��ѹ�轵������ֵ
#define	DSP_DIP_THH	        0x5a	//	��ѹ�轵������ֵ
#define	FD_OVTH	                0x5b	//	���ټ���ѹ��ֵ��
#define	FD_LVTH	                0x5c	//	���ټ��Ƿѹ��ֵ��
#define	FD_IA_OCTH	        0x5d	//	���ټ�����Aͨ��������ֵ��
#define	FD_IA_LCTH	        0x5e	//	���ټ�����Aͨ��Ƿ����ֵ��
#define	FD_IB_OCTH	        0x5f	//	���ټ�����Bͨ��������ֵ��
#define	FD_IB_LCTH	        0x60	//	���ټ�����Bͨ��Ƿ����ֵ��
#define	DSP_PHS_STT	        0x61	//	��λ��������λ��д����ʹ��һ����λ������
#define	DSP_PHS_U	        0x62	//	��ѹ��λֵ
#define	DSP_PHS_UN	        0x63	//	��ѹ�����֮ǰ��������ֵ
#define	DSP_PHS_UP	        0x64	//	��ѹ�����֮��������ֵ
#define	DSP_PHS_I	        0x65	//	������λֵ
#define	DSP_PHS_IN	        0x66	//	���������֮ǰ��������ֵ
#define	DSP_PHS_IP	        0x67	//	���������֮��������ֵ
#define	TEMPERATE	        0x68	//	�¶ȼĴ���
#define	DAT_WAVE	        0x69	//	�������ݶ�ȡ�����ظ���ȡ�õ�ַ���Ӷ���������������ݡ�������Ҫ����ȫ�����ݣ���ͨ��DSP_CTRL5λ31��λ��ȡ��ַ��
#define	DAT_SWELL_CNT	        0x6a	//	��ѹ����ʱ���¼���벨Ϊ��λ��24bit��Ч����õ�ַд����ֵ��������ü���ֵ��
#define	DAT_DIP_CNT	        0x6b	//	��ѹ�轵ʱ���¼���벨Ϊ��λ��24bit��Ч����õ�ַд����ֵ��������ü���ֵ��
#define	SYS_SFTRST	        0x6C	//	�����λ�Ĵ�����
#define	SYS_FPGACW0	        0x6D	//	FPGA�ź�Դ����λ0
#define	SYS_FPGACW1	        0x6E	//	FPGA �ź�Դ����λ1
#define	SYS_FPGACW2	        0x6F	//	FPGA �ź�Դ����λ2
#define	SYS_BAUDCNT1	        0x70	//	UART�������� 1 λ����ֵ��
#define	SYS_BAUDCNT8	        0x71	//	UART ��������8 λ����ֵ��
#define	SYS_INTSTS	        0x72	//	�ж�״̬�Ĵ�����
#define	SYS_INTEN	        0x73	//	�ж�ʹ�ܼĴ�����
#define	SYS_STS	                0x74	//	ϵͳ״̬�Ĵ���
#define	SYS_MISC	        0x75	//	��Ͽ��ƼĴ���
#define	SYS_ROMCS	        0x76	//	ROM У��ͼĴ���.
#define	SYS_RAMADDR	        0x77	//	Internal RAM address register.
#define	SYS_RAMDATA	        0x78	//	Internal RAM data register.
#define	SYS_BLK_ADDR0	        0x79	//	���д��ַ�Ĵ���0
#define	SYS_BLK_ADDR1	        0x7a	//	���д��ַ�Ĵ���1
#define	SYS_BLK_ADDR2	        0x7b	//	���д��ַ�Ĵ���2
#define	SYS_BLK_ADDR3	        0x7c	//	���д��ַ�Ĵ���3
#define	SYS_IOCFG0	        0x7d	//	P0��P1,P2��P3������üĴ���
#define	SYS_IOCFG1	        0x7e	//	P4��P5��P6������üĴ���
#define	SYS_VERSION	        0x7F	//	�汾�żĴ�����

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
//DSP_ANA1�Ĵ���ֵ  0x01   U: x4  IA: x32  IB: x4
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

//  #define DSP_ANA1_Value    0x00170489          //����I1������64����Ϊ32��4->0 ��4λ0->9 0ppm��Ϊ-51ppm 
#define DSP_ANA1_Value    0x00870089 
//  #define DSP_ANA1_Value    0x00078089
// 0 PPM  0x00178480 00 01 10���ⲿ�����8530�ṩ��Ƶʱ 4��ƵȺ����û����
//bit31-30 adcʱ��Ƶ��800K  BIT22-21 6.5M�� BGP�� BIT18-16��ѹ A· B·adc��  BIT15 ʹ��13M�������1
//BIT14-12 B·����adc���� 4��  BIT11 ��ѹadc���� 4��  A·����adc���� 64�� BIT7 ����ͣ����ʹ��
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
//DSP_CTRL0�Ĵ���ֵ  0x01   
//DSP����ģʽ:0        ƽ�����ʡ���Чֵˢ��ʱ��:640ms  ���ڹ��ʼ���Դѡ��:ͨ������ֵ����
//ƽ������Ƶ��(320ms)  ˲ʱ����ֵˢ��ʱ��:20ms         ˲ʱ��Чֵˢ��ʱ��:10ms
//--------------------------------------------------------------
#if ( HZ_CONFIG == FRC_50HZ)

  #define DSP_CTRL0_Value   0x01000f07//0x00000F57   //DSP_CTRL0�Ĵ���ֵ   0x02   50Hz  0x01000F07 0x01000F37  Ⱥ�������ʱ���޸�
//BIT31 ˲ʱ�й��ʡ�Чֵˢ��ʱ�� 0 20ms 10ms BIT29-28 ƽ��Ƶ��ˢ��320ms  BIT24 ���ڹ��ʼ���Դѡ�� 0��Чֵ 1����ֵ
//BIT23-20 ����ͨ��ѡ�� 0 ͨ��1�����й�A ͨ��2�����й�B BIT19-18 �޹�����B Aģʽ 0 ȫ���޹� 1����
//BIT17-13 ȫ������ ����������� ȫ����ѹ ����A B��Чֵ �Ƿ񾭹���ͨ�˲��� 0���� BIT11-10 ƽ����Чֵˢ��ʱ�� 11 640ms 00 40ms
//BIT9-8 ƽ������ˢ�� 11 640ms 00 80ms 
//BIT7-4 DSP MODE 6��:3.2M ÿ�ܲ�������64
//BIT3 0��50Hz 1��60Hz
//BIT2 1 0 ��ѹ ����B ����Aͨ�����أ�����adc��dsp��
//
#elif ( HZ_CONFIG == FRC_60HZ)

 // #define DSP_CTRL0_Value   0x01000F3F   //DSP_CTRL0�Ĵ���ֵ   0x02   60Hz
#define DSP_CTRL0_Value   0x01000F0F   //DSP_CTRL0�Ĵ���ֵ   0x02   60Hz
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
//#define DSP_ANA0_Value    0x00100c00//0x02000C00    //DSP_ANA0�Ĵ���ֵ    0x00  0x02700C00 
#define DSP_ANA0_Value    0x00100C00
//#define DSP_ANA0_Value    0x02000C00   //DSP_ANA0�Ĵ���ֵ    0x00  0x02700C00 

//2��ӦBIT25 ����ʱ��Ϊadcʱ��ʱ��Ƶʹ��1:1/2  C��ӦBIT11 10 ���� ��ѹADC�����·
//#define DSP_CTRL1_Value   0x000C32C1   //DSP_CTRL1�Ĵ���ֵ   0x03
//**C��ӦBIT19-18 ����㷽ʽ3���رչ���㹦�� 3��ӦBIT13 12 CF2���ʹ�� CF2����Դѡ��0������Ͱ1 1������Ͱ2**/
//**2��ӦBIT9 CF1���ʹ�� BIT8 CF1����Դѡ��0������Ͱ1 1������Ͱ2                                        **/
//**C��ӦBIT7-6 ����Ͱ2ʹ�� ����Ͱ1ʹ�� BIT5-4 ������0:80ms  BIT0 ��������Ͱ��Ǳ����1��ʹ��             **/

//#define DSP_CTRL2_Value   0x00001723   //DSP_CTRL2�Ĵ���ֵ   0x04    1723
//**1��ӦBIT12 ����Ͱ2 Bͨ���ۼӿ���  BIT11-10 1 �޹������ۼ�     BIT9-8 3:����Ͱ�ۼӾ���ֵ**/
//**BIT7-6 ����Ͱ1�ۼ�ģʽ 0:�����ۼ� BIT5 ����Ͱ1Aͨ���ۼӿ���   BIT1-0 3:����Ͱ�ۼӾ���ֵ**/

#define DSP_CTRL1_Value   0x000C32C1   //DSP_CTRL1�Ĵ���ֵ   0x03
#define DSP_CTRL2_Value   0x00002723//0x000027A3    //���Ը�Ϊ�����ۼ�

//#define DSP_CTRL2_Value   0x00002723   //DSP_CTRL2�Ĵ���ֵ   0x04   //BIT13 12 ����Ͱ2 ABͨ������ BIT54 ����Ͱ1 ABͨ������
//0x00002723 ����Ͱ2 Aͨ���޹��ۼ� ����Ͱ1 Aͨ���й��ۼ�  BIT11-10 ����Ͱ2�ۼ�Դѡ��     BIT3-2 ����Ͱ1�ۼ�Դѡ�� 
//0x00001713 ����Ͱ2 Bͨ���޹��ۼ� ����Ͱ1 Bͨ���й��ۼ�           0 3:�й� 1:�޹� 2:����
//******************************************************
//#define DSP_CTRL2_Value_ChA 0x000027A3
//#define DSP_CTRL2_Value_ChB 0x000027A3
#define DSP_CTRL2_Value_ChA 0x00002723
#define DSP_CTRL2_Value_ChB 0x00001713 
//******************************************************
#define DSP_CTRL3_Value   0x00000000   //DSP_CTRL3�Ĵ���ֵ   0x05
#define DSP_CTRL4_Value   0x00000000   //DSP_CTRL4�Ĵ���ֵ   0x06
#define DSP_CTRL5_Value   0x00000000   //DSP_CTRL5�Ĵ���ֵ   0x07
#define SYS_IOCFG0_Value  0X00000000//0x00004080//0x00001C19//0x00004080   //P0��P1,P2��P3������üĴ���  CF1 CF2 0x7d  bit7-0   00011001 ����ͣ�ж�
#define SYS_IOCFG1_Value  0x003C3A00  //P5P6����ΪDMCK DMDO
//--------------------------------------------------------------            bit15-8  00011100 �����ж�
//Dsp_mode=0,1,2ʱ��д��0x806764B6
//Dsp_mode=3,4,5ʱ��д��0x8049d98F
//Dsp_mode=6,7ʱ��  д��0x80DD7A8C
//Dsp_mode=8ʱ��    д��0x82B465F0
//--------------------------------------------------------------
#define DSP_CFG_BPF_Value 0x806764B6   //DSP_CFG_BPF�Ĵ���ֵ 0x37


//======================================================================
#define Actual_BAUDRate 2400     //ʵ�ʲ�����

#define BAUDRate_1Byte_OverTime  (float)(11*1000/Actual_BAUDRate*1.4)  //ͨ�ų�ʱʱ����

//======================================================================

#define RacRead      1
#define RacWrite     2
#define RacBroadcast 0

//======================================================================
#define SlaveAddr    0

#define Raccoon_A1H()      {GPIOE->OEN &=~BIT13; GPIOE->IE &=~BIT13; GPIOE->ATT &=~BIT13; IO_DAT( IOE_DAT, 13) = 1;} //���������
#define Raccoon_A1L()      {GPIOE->OEN &=~BIT13; GPIOE->IE &=~BIT13; GPIOE->ATT &=~BIT13; IO_DAT( IOE_DAT, 13) = 0;} //���������

#define Raccoon_A0H()      {GPIOE->OEN &=~BIT14; GPIOE->IE &=~BIT14; GPIOE->ATT &=~BIT14; IO_DAT( IOE_DAT, 14) = 1;} //���������
#define Raccoon_A0L()      {GPIOE->OEN &=~BIT14; GPIOE->IE &=~BIT14; GPIOE->ATT &=~BIT14; IO_DAT( IOE_DAT, 14) = 0;} //���������

#define Raccoon_SlaveAddrCfg() {Raccoon_A1L();Raccoon_A0L();}

//======================================================================
#define Raccoon_PwrOn()    {GPIOB->OEN &=~BIT5; GPIOB->IE &=~BIT5; GPIOB->ATT &=~BIT5; IO_DAT( IOB_DAT, 5) = 1;} //���������
#define Raccoon_PwrOff()   {GPIOB->OEN &=~BIT5; GPIOB->IE &=~BIT5; GPIOB->ATT &=~BIT5; IO_DAT( IOB_DAT, 5) = 0;} //���������


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

RACCOONEXT volatile uint8_t guc_CommDelayTime;         //ͨѶ�ȴ���ʱ

//======================= Ӧ�ò㶨�� ====================================
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
  DSP_ANA0,	//0 ģ����ƼĴ���0
  DSP_ANA1,	//1 ģ����ƼĴ���1
  DSP_CTRL0,	//2 ���ֿ��ƼĴ���0
  DSP_CTRL1,	//3 ���ֿ��ƼĴ���1
  DSP_CTRL2,	//4 ���ֿ��ƼĴ���2
  DSP_CTRL3,	//5 ���ֿ��ƼĴ���3
  DSP_CTRL4,	//6 ���ֿ��ƼĴ���4
  DSP_CTRL5,	//7 ���ֿ��ƼĴ���5
  DSP_CFG_BPF,	//8 ��ͨ�˲���ϵ���Ĵ���
  
  EGY_PWRTH,	//9 �й�����ֵ�Ĵ���
  EGY_PROCTH,	//10 ��/Ǳ���ж�����ֵ�Ĵ���
  
  DSP_CFG_PHC,  //11 �ǲ�У���Ĵ���
  DSP_CFG_CALI_RMSUA,	//12 ȫ����ѹ��Чֵ�Ȳ�Ĵ���
  DSP_CFG_CALI_RMSIA,	//13 IAȫ��������Чֵ�Ȳ�Ĵ���
  DSP_CFG_CALI_RMSIB,	//14 IBȫ��������Чֵ�Ȳ�Ĵ���
  DSP_CFG_CALI_PA,	//15 �й�����A�Ȳ�У���Ĵ���
  DSP_CFG_DC_PA,	//16 �й�����A���β����Ĵ���
  DSP_CFG_DC_QA,	//17 �޹�����A���β����Ĵ���
  DSP_CFG_RMS_DCIA,      //18 ����A��ЧֵС�ź�У�� ZZP0303
    
  DSP_CFG_CALI_PB,	//19 �й�����B�Ȳ�У���Ĵ���
  DSP_CFG_DC_PB,	//20 �й�����B���β����Ĵ���
  DSP_CFG_DC_QB,	//21 �޹�����B���β����Ĵ���
  DSP_CFG_RMS_DCIB,     //22 B·������ЧֵС�ź�У�� zzp0314
};

uint8_t const  RMS_RegAddr[]=
{
  DATAP,        //��һͨ���й�����
  DATAQ,        //�޹�����
  RMSU,         //��ѹͨ������Чֵ
  RMSI1,        //����ͨ��1����Чֵ
  RMSI2,        //����ͨ��2����Чֵ
  FREQ,         //��ѹƵ��
  DATAPB,       //Bͨ���й�zzp0305
  DATAQB,       //Bͨ���޹�
  DATASA,       //Aͨ������
  DATASB,       //Bͨ������zzp0305
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


