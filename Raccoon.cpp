#define  RACCOONEXT

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include <HardwareSerial.h>
#include "V93XX_Include.h"

uint8_t cksdis_flag = 0;
/*=========================================================================================\n
* @function_name: Init_RacCtrl
* @function_file: Raccoon.c
* @describe:
* 
* @parameter: 
* @return: 
* @author:  (2013-04-15)
* @Remark: 
*-------------------------------------------------------------------------------------------
* @Modifier:
* @Modified content:
===========================================================================================*/
void Init_RacCtrl(void)
{
    gs_RacCtrl.ucSendLen=0;
    gs_RacCtrl.ucSendPoint=0;
    gs_RacCtrl.ucRevLen=0;
    gs_RacCtrl.ucRevPoint=0;
    gs_RacCtrl.ucStatus=Rac_Idle;
    
    //MemSet((uint8_t *)&gs_RmsData, 0, sizeof( gs_RmsData)); //Clear the effective value variable to 0
    memset((uint8_t *)&gs_RmsData, 0, sizeof( gs_RmsData));
}

/*=========================================================================================\n
* @function_name: RxReset_Raccoon
* @function_file: Raccoon.c
* @Description: Low level is valid. TX outputs low level for >70ms
* 
* @parameter:
* @return:
* @author:   lwb (2013-07-10)
* @Remark:
*-------------------------------------------------------------------------------------------
* @Modifier:
* @Modified content: 
===========================================================================================*/
void RxReset_Raccoon(void)
{
    //GPIO_InitType GPIO_InitStruct;

    //Raccoon_PwrOn();         //Metering chip power IO port control
    //GPIOB->OEN &=~ (BIT5);//iof0 1 Powering the marmot
    //GPIOB->IE  &=~ (BIT5);
    //GPIOB->DAT |= (BIT5);
    //Raccoon_SlaveAddrCfg();  //Device address setting
    
    //UART_DeInit(UART5);
    
    /* UART5 TX pin(IOB7), output mode */
    //GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUTPUT_CMOS;
    //GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    //GPIOBToF_Init(GPIOB, &GPIO_InitStruct);
    pinMode(TX_V93XX, OUTPUT);
    
    //IO_DAT( IOB_DAT, 7) = 1;
    //DelayXms(3);
    //IO_DAT( IOB_DAT, 7) = 0;

    //TX pin for UART , RX pin for ASIC need to be held low
    digitalWrite(TX_V93XX, HIGH);
    delayMicroseconds(3000);
    digitalWrite(TX_V93XX, LOW);
    
    //TODO: This looks like an interrupt based non-blocking delay, have to implement something similar
    //guc_CommDelayTime = 40;   //The RX pin needs to be continuously low for 70ms to reset the chip. //zzp0219 10->12
    //while(guc_CommDelayTime >0);
    delayMicroseconds(70000);
    //IO_DAT( IOB_DAT, 7) = 1;
    digitalWrite(TX_V93XX, HIGH);
    
    //TODO: This looks like an interrupt based non-blocking delay, have to implement something similar
    //guc_CommDelayTime = 2;   //The RX pin inputs a high level, and the RAM can be freely accessed after 2.15ms.
    //while(guc_CommDelayTime >0);
    delayMicroseconds(21500);
}
/*=========================================================================================\n
* @function_name: Init_UartRaccoon
* @function_file: Raccoon.c
* @describe: 
* 
* @parameter:
* @return:
* @author:  lwb (2013-01-22)
* @Remark: 
*-------------------------------------------------------------------------------------------
* @Modified by: 
* @Modifications:
===========================================================================================*/
void Init_UartRaccoon(void)
{
    //UART_InitType UART_InitStruct;
    //GPIO_InitType GPIO_InitStruct;
    
  
    /* UART5 RX pin(IOB1), input mode */
    /**
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_INPUT;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIOBToF_Init(GPIOB, &GPIO_InitStruct);
     
    UART_DeInit(UART5);
    UART_StructInit(&UART_InitStruct);
    
    UART_InitStruct.Mode     = UART_MODE_RX | UART_MODE_TX;
    UART_InitStruct.Parity   = UART_PARITY_ODD;           //Odd Parity
    UART_InitStruct.WordLen  = UART_WORDLEN_9B;           //9 data bits, 8 data bits + 1 check bit
    UART_InitStruct.FirstBit = UART_FIRSTBIT_LSB;
    UART_InitStruct.Baudrate = 4800;                      //Fixed baud rate 4800
    UART_Init(UART5, &UART_InitStruct);

    UART_INTConfig(UART5, UART_INT_RX, ENABLE);
    UART_INTConfig(UART5, UART_INT_TXDONE, ENABLE);
    CORTEX_SetPriority_ClearPending_EnableIRQ(UART5_IRQn, 1); //Enable UART transmit doen/receive interrupt
     */
    Serial1.begin(4800, SERIAL_8O1, RX_V93XX, TX_V93XX);
}

/*=========================================================================================\n
* @function_name: Raccoon_Dy10ms
* @function_file: Raccoon.c
* @Description: Timeout protection for analog serial port
*       To prevent locking and timing protection, when there is no data sent or received, the
* @parameter:
* @return:
* @author:   lwb (2012-03-28)
* @Remark:
*-------------------------------------------------------------------------------------------
* @Modified by:
* @Modifications:
===========================================================================================*/
void Raccoon_Dy10ms(void)
{
    if(guc_CommDelayTime > 0)
    {
        guc_CommDelayTime--;
    }
}

void Raccoon_Delay(uint8_t n)//zzp0113
{
  uint32_t i,j;
  for(i = 0;i<10;i++)
      for(j = 0;j<(25+n);j++);
}



/*=========================================================================================\n
* @function_name: Raccoon_UartTransmit
* @function_file: Raccoon.c
* @Description: UART4 sends, called in interrupt
*
* @parameter:
* @return:
* @author:   lwb (2012-03-28)
* @Remark:
*-------------------------------------------------------------------------------------------
* @Modified by:
* @Modifications:
===========================================================================================*/
void Raccoon_UartTransmit(void)
{
  if(gs_RacCtrl.ucStatus==Rac_Send)
  {
     if(gs_RacCtrl.ucSendPoint<gs_RacCtrl.ucSendLen)   
     {
         //UART_SendData( UART5, gs_RacCtrl.ucBuf[gs_RacCtrl.ucSendPoint++]);
         Serial1.write(gs_RacCtrl.ucBuf[gs_RacCtrl.ucSendPoint++]);
     }
     else
     {
         gs_RacCtrl.ucStatus=Rac_Rev;
     }
  }
}
/*=========================================================================================\n
* @function_name: Raccoon_UartReceive
* @function_file: Raccoon.c
* @Description: Call this function in an interrupt
*
* @parameter:
* @return:
* @author:  lwb (2012-03-28)
* @Remark:
*-------------------------------------------------------------------------------------------
* @Remark:
* @Modifications:
===========================================================================================*/
void Raccoon_UartReceive(void)
{
  uint8_t temp;
  if(gs_RacCtrl.ucStatus==Rac_Rev)
  {
     if(gs_RacCtrl.ucRevPoint<gs_RacCtrl.ucRevLen)
     {
         //temp = UART_ReceiveData( UART5);
         temp = Serial1.read();
         gs_RacCtrl.ucBuf[gs_RacCtrl.ucRevPoint++] = temp;
         
         if(gs_RacCtrl.ucRevPoint==gs_RacCtrl.ucRevLen)
         {
             gs_RacCtrl.ucStatus=Rac_WaitPro;
         }
     }
  }
}
/*=========================================================================================\n
* @function_name: WriteRaccoon
* @function_file: Raccoon.h
* @Description: Send data to raccoon
* 
* 
* @parameter: 
* @param:Data  Writing Data
*
* @param:addr Write address  
* control
* @return:
* @return: uint8_t 
* @Author: lwb (2013-07-04))
* @Remark:
*------------------------------------------------------------------------------------------
===========================================================================================*/
uint8_t  WriteRaccoon( uint32_t Data, uint8_t Addr)
{
    uint8_t i;
    volatile uint8_t ucSum; 
//    gs_RacCtrl.ucBuf[0] = 0xfe;
    gs_RacCtrl.ucBuf[0] = 0x7d;
    gs_RacCtrl.ucBuf[1] = (uint8_t)((0 << 4) | ((SlaveAddr) << 2) | RacWrite);
    gs_RacCtrl.ucBuf[2] = (uint8_t)(Addr&0x007f);   //Register address  Bit[6:0]
    
    gs_RacCtrl.ucBuf[3] = (uint8_t)(Data&0x000000ff);
    gs_RacCtrl.ucBuf[4] = (uint8_t)((Data&0x0000ff00)>>8);
    gs_RacCtrl.ucBuf[5] = (uint8_t)((Data&0x00ff0000)>>16);
    gs_RacCtrl.ucBuf[6] = (uint8_t)((Data&0xff000000)>>24);
    //Calculate Checksum
    gs_RacCtrl.ucBuf[7] =  0;
    for(i=1;i<7;i++)
    {
        gs_RacCtrl.ucBuf[7] += gs_RacCtrl.ucBuf[i]; 
    }
   
    gs_RacCtrl.ucBuf[7]  = ~gs_RacCtrl.ucBuf[7];
    gs_RacCtrl.ucBuf[7]  = gs_RacCtrl.ucBuf[7]+0x33;

    ucSum = gs_RacCtrl.ucBuf[7];
    
    gs_RacCtrl.ucStatus=Rac_Send;       //Enter sending state
    gs_RacCtrl.ucSendLen=8;
    gs_RacCtrl.ucSendPoint=0;
    gs_RacCtrl.ucRevLen=1;
    gs_RacCtrl.ucRevPoint=0;                 
    Raccoon_UartTransmit();                    //Start Sending
//    guc_CommDelayTime=7;
    guc_CommDelayTime = (uint8_t)((BAUDRate_1Byte_OverTime * 9 +10)/10)+1;//Send 8 bytes, receive 1 byte Unit: 10ms
    while(gs_RacCtrl.ucStatus!=Rac_WaitPro)
    {
        //ClearWDT();	//Feed the Dog
        if((guc_CommDelayTime==0))
        {
          
//            gs_LcdDisp.ulNum1 = testshow;
//            gs_LcdDisp.ulNum1Msk = 0;
//            Disp_Numb(gs_LcdDisp.ulNum1, gs_LcdDisp.ulNum1Msk);
//            LCD_RAMUpdata();
          cksdis_flag += 1;
          
          
            //MemSet((uint8_t *)&gs_RmsData, 0x5555, sizeof( gs_RmsData)); //zzp0124
            return false;                   //If timeout  
        }
    }
    //DelayXms(6);
    if(gs_RacCtrl.ucBuf[0]==ucSum)
    {
        return true;
    }else
    {
        return false;
    }   
}

/*=========================================================================================\n
* @function_name: ReadRaccoon
* @function_file: Raccoon.h
* @Description: Read data from raccoon
* 
* @parameter:
* @param:Addr  Read start address
* @param:num   Read word number, 32bit
* 
* @return: 
*        
* @return: uint8_t 
* @author:   lwb (2013-07-04)
* @Remark: 
*-------------------------------------------------------------------------------------------
* @Modified by: hjj
* @Modifications: Added Raccoon physical address, three-phase meter uses 3 Raccoons in total
* @param:Phyadd Raccoon address

==========================================================================================*/
uint8_t ReadRaccoon(uint8_t Addr,uint8_t num)
{
    uint8_t i;
    volatile uint8_t ucSum;
    volatile uint8_t Raccoon_Cmd1,Raccoon_Cmd2;
    
//    gs_RacCtrl.ucBuf[0] = 0xfe;
    gs_RacCtrl.ucBuf[0] = 0x7d;
    gs_RacCtrl.ucBuf[1] = (uint8_t)(((num -1)<< 4) | ((SlaveAddr) << 2) | RacRead); //num��How many words should be read?
    gs_RacCtrl.ucBuf[2] = (uint8_t)(Addr&0x007f);   //Register address  Bit[6:0]
    Raccoon_Cmd1 = gs_RacCtrl.ucBuf[1];
    Raccoon_Cmd2 = gs_RacCtrl.ucBuf[2];
    
    gs_RacCtrl.ucBuf[3] = 0;                    //sum Clear
    for(i=1;i<3;i++)
    {
        gs_RacCtrl.ucBuf[3] += gs_RacCtrl.ucBuf[i]; 
    }
   
    gs_RacCtrl.ucBuf[3]  = ~gs_RacCtrl.ucBuf[3];
    gs_RacCtrl.ucBuf[3]  = gs_RacCtrl.ucBuf[3]+0x33;
    
    gs_RacCtrl.ucStatus=Rac_Send;       // Enter sending state
    
    gs_RacCtrl.ucSendLen=4;
    gs_RacCtrl.ucSendPoint=0;
    gs_RacCtrl.ucRevLen=(num*4)+1;           
    gs_RacCtrl.ucRevPoint=0;
//    guc_CommDelayTime=17;
    guc_CommDelayTime = (uint8_t)((BAUDRate_1Byte_OverTime * (4*num+5) +10)/10)+1;//Send 4 bytes, receive 4*N+1 bytes Unit: 10ms
    Raccoon_UartTransmit();
    while(gs_RacCtrl.ucStatus!=Rac_WaitPro)
    {
        //ClearWDT();	//Feed the Dog
        if((guc_CommDelayTime==0))
        {
            
//            gs_LcdDisp.ulNum1 = 0x22222222;
//            gs_LcdDisp.ulNum1Msk = 0;
//            Disp_Numb(gs_LcdDisp.ulNum1, gs_LcdDisp.ulNum1Msk);
//            LCD_RAMUpdata();
          cksdis_flag += 2;
          
            //MemSet((uint8_t *)&gs_RmsData, 0x1111, sizeof( gs_RmsData)); //zzp0124
            return false;           //If timeout
        }
    }
    ucSum = Raccoon_Cmd1+Raccoon_Cmd2;
    for(i=0;i<(num*4);i++)               //Read no more than 255 bytes
    {
        ucSum += gs_RacCtrl.ucBuf[i]; 
    }
    
    ucSum=~ucSum;
    ucSum+=0x33;
    //DelayXms(6);
    if(gs_RacCtrl.ucBuf[num*4]==ucSum)
    {
        return true;
    }else
    {
        return false;
    }         
}


/*=========================================================================================\n
* @function_name: BroadcastWriteRaccoon
* @function_file: Raccoon.c
* @describe: 
* 
* 
* @parameter: 
* @param:Data  
* @param:Addr  
 
* @return:
* @author:   lwb (2013-09-25)
* @Remark:
*-------------------------------------------------------------------------------------------
* @Modified by:
* @Modifications: 
===========================================================================================*/ 
void BroadcastWriteRaccoon(uint32_t Data,uint8_t Addr)
{
    uint8_t i;
    volatile uint8_t ucSum;

//    gs_RacCtrl.ucBuf[0] = 0xfe;
    gs_RacCtrl.ucBuf[0] = 0x7d;
    gs_RacCtrl.ucBuf[1] = (uint8_t)((0 << 4) | ((SlaveAddr) << 2) | RacBroadcast);
    gs_RacCtrl.ucBuf[2] = (uint8_t)(Addr&0x007f);   //Register address  Bit[6:0]
    
    gs_RacCtrl.ucBuf[3] = (uint8_t)(Data&0x000000ff);
    gs_RacCtrl.ucBuf[4] = (uint8_t)((Data&0x0000ff00)>>8);
    gs_RacCtrl.ucBuf[5] = (uint8_t)((Data&0x00ff0000)>>16);
    gs_RacCtrl.ucBuf[6] = (uint8_t)((Data&0xff000000)>>24);
    //Calculate Checksum
    gs_RacCtrl.ucBuf[7] =  0;
    for(i=1;i<7;i++)
    {
        gs_RacCtrl.ucBuf[7] += gs_RacCtrl.ucBuf[i]; 
    }
   
    gs_RacCtrl.ucBuf[7]  = ~gs_RacCtrl.ucBuf[7];
    gs_RacCtrl.ucBuf[7]  = gs_RacCtrl.ucBuf[7]+0x33;

    gs_RacCtrl.ucStatus=Rac_Send;       //Enter sending state
    gs_RacCtrl.ucSendLen=8;
    gs_RacCtrl.ucSendPoint=0;
    gs_RacCtrl.ucRevLen=0;
    gs_RacCtrl.ucRevPoint=0;
    
    Raccoon_UartTransmit();
    guc_CommDelayTime = (uint8_t)((BAUDRate_1Byte_OverTime * 8 +10)/10);//Send 8 bytes, receive 0 bytes Unit: 10ms
    while(guc_CommDelayTime>0)
    {
      //ClearWDT();	//Feed the Dog
    }   
}

/*=========================================================================================\n
* @function_name: Raccoon_UpdatePar
* @function_file: Raccoon.c
* @Describes the update of calibration parameters and phase power-on status
* 
* @parameter:
* @return:
* @author:  hjj (2013-10-17)
* @Remark: 
*-------------------------------------------------------------------------------------------
* @Modified by: 
* @Modifications:
===========================================================================================*/
void Raccoon_UpdatePar(void)
{
    uint8_t i,tmp;
    uint32_t ucSum,RacReg_buf;
    
    ucSum = 0;
    //testshow = 0x22222;
    for( i = 0; i < (sizeof(RegAddr)/sizeof(uint8_t)); i++)
    {
        if(i < 9)   //control
        {
            if(i == 4)
            {
              if(gs_Channel.ucSta == SETA)
              {
                RacReg_buf = DSP_CTRL2_Value_ChA;
              }
              else
              {
                RacReg_buf = DSP_CTRL2_Value_ChB;
              }
            }
            else
            {
             RacReg_buf = RegValue[i]; 
            }
            
            //CLRWDT();

            tmp = WriteRaccoon( RacReg_buf, RegAddr[i]);

//            if(tmp ==false)
//            {
//              WriteRaccoon( RacReg_buf, RegAddr[i]);
//            }
            //CLRWDT(); 
            ucSum += RacReg_buf;
        }else
        {
            if(i<19)
            {
              RacReg_buf = (uint32_t)*((&gs_JbPm.ul_EGYTH) + i - 9);
            }
            else // after i>18, RegAddr[i] corresponds to &gs_JbPm.ul_EGYTH) + i - 5
            {
              RacReg_buf = (uint32_t)*((&gs_JbPm.ul_EGYTH) + i - 5);
            }
            
            if(i == 15)  //power
            {
                //CLRWDT();
                tmp = WriteRaccoon( RacReg_buf, RegAddr[i]);
//                if(tmp ==false)
//                {
//                  WriteRaccoon( RacReg_buf, RegAddr[i]);
//                }
                //CLRWDT(); 
                ucSum += RacReg_buf;
                
                tmp = WriteRaccoon(RacReg_buf, DSP_CFG_CALI_QA);  //Reactive power ratio difference = Active power ratio difference
//                if(tmp ==false)
//                {
//                  WriteRaccoon( RacReg_buf, DSP_CFG_CALI_QA);
//                }
                //CLRWDT();
                ucSum += RacReg_buf;
            }
            else if(i == 19)
            {
              //CLRWDT();
                tmp = WriteRaccoon( RacReg_buf, RegAddr[i]);
//                if(tmp ==false)
//                {
//                  WriteRaccoon( RacReg_buf, RegAddr[i]);
//                }
                //CLRWDT(); 
                ucSum += RacReg_buf;
                
                tmp = WriteRaccoon(RacReg_buf, DSP_CFG_CALI_QB);  //Reactive power ratio difference = Active power ratio difference
//                if(tmp ==false)
//                {
//                  WriteRaccoon( RacReg_buf, DSP_CFG_CALI_QB);
//                }
                //CLRWDT();
                ucSum += RacReg_buf;
            }
            
            else  //Threshold and Calibration
            {
                //CLRWDT();  
                tmp = WriteRaccoon( RacReg_buf, RegAddr[i]);
//                if(tmp ==false)
//                {
//                  WriteRaccoon( RacReg_buf, RegAddr[i]);
//                }
                //CLRWDT(); 
                ucSum += RacReg_buf;
            }
        }
    }
    
    ucSum = 0xFFFFFFFF-ucSum;    //Checksum register DSP_CFG_CKSUM  0x0~0x7��0x25~0x3a��0x55~0x60
    tmp = WriteRaccoon( ucSum, DSP_CFG_CKSUM);
//    if(tmp ==false)
//    {
//      WriteRaccoon( ucSum, DSP_CFG_CKSUM);
//    }
    //CLRWDT(); 
    
    tmp = WriteRaccoon( SYS_IOCFG0_Value, SYS_IOCFG0); //IO port configuration
//    if(tmp ==false)
//    {
//      WriteRaccoon( SYS_IOCFG0_Value, SYS_IOCFG0);
//    }
    //CLRWDT(); 
    tmp = WriteRaccoon( SYS_IOCFG1_Value, SYS_IOCFG1); //IO port configuration
//    if(tmp ==false)
//    {
//      WriteRaccoon( SYS_IOCFG1_Value, SYS_IOCFG1);
//    }
    //CLRWDT(); 
    
    
    WriteRaccoon(0x00000018, 0x75); //chksum Wrong not to close cf
//    Raccoon_UpdateChecksum();
//    CLRWDT(); 
    //testshow = 0x33333;
}

/*=========================================================================================\n
* @function_name: Raccoon_ReadRMS
* @function_file: Raccoon.c
* @describe: 
* 
* 
* @parameter: 
* @param:  
* @return:
* @author: lwb (2013-09-25)
* @Remark: 
*-------------------------------------------------------------------------------------------
* @Modified by:  
* @Modifications: 
===========================================================================================*/
void Raccoon_ReadRMS(void)
{
  uint8_t i,ReadRmsErrFlg;
  uint32_t *pRmsData;
  static uint8_t ReadRmsErrCnt = 0;
  uint32_t gs_RmsData_txbuf = 0;
  uint8_t a=0,j=0;
  
  ReadRmsErrFlg = 0;
  pRmsData = (uint32_t *)&gs_RmsData;
  
  for( i = 0; i < (sizeof(RMS_RegAddr)/sizeof(uint8_t)); i++)
  {
    if( ReadRaccoon( RMS_RegAddr[i], 1))
    {
        //MemCpy((uint8_t *)pRmsData, gs_RacCtrl.ucBuf, 4);//zzp0113
        memcpy((uint8_t *)pRmsData, gs_RacCtrl.ucBuf, 4);
        
        pRmsData++;
//******************************************************************************
        if(gs_Channel.ucSta == SETA)
        {
          if(i == 0)//Active power of channel A is used for CF accumulation
          {
            Buf_RMS.ul_P = gs_RmsData.ul_P;
            if((Buf_RMS.ul_P >>24)>0X7f)
            {
              Buf_RMS.ul_P = ~Buf_RMS.ul_P+1;
            }
            if((float)Buf_RMS.ul_P/gs_JbPm.ul_PG < (((float)(gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*1.5/1000))//Rated 1.5�� 1.1W(0.1%) *1.5
            {
              gs_power_energycal.ul_power[0] = 0;
//              gs_power_energycal.ul_power[0] = 0x1234567;
            }
            else
            {
              gs_power_energycal.ul_power[0] = Buf_RMS.ul_P;
            }
          }
          if(i == 1)//A-way reactive power
          {
            Buf_RMS.ul_Q = gs_RmsData.ul_Q;
            if((Buf_RMS.ul_Q >>24)>0X7f)
            {
              Buf_RMS.ul_Q = ~Buf_RMS.ul_Q+1;
            }
            if((float)Buf_RMS.ul_Q/gs_JbPm.ul_PG < (((float)(gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*1.5/1000))//Rated 1.5�� 1.1W(0.1%) *1.5
            {
              gs_power_energycal.ul_Npower[0] = 0;
//              gs_power_energycal.ul_Npower[0] = 0x1234567;
            }
            else
            {
              gs_power_energycal.ul_Npower[0] = Buf_RMS.ul_Q;
            }
          }
          
        }
        
        if(gs_Channel.ucSta == SETB)
        {
            if(i == 6)//BRoad is a success
            {
              Buf_RMS.ul_PB = gs_RmsData.ul_PB;
              if((Buf_RMS.ul_PB >>24)>0X7f)
              {
                Buf_RMS.ul_PB = ~Buf_RMS.ul_PB+1;
              }
              if((float)Buf_RMS.ul_PB/gs_JbPm.ul_PG < (((float)(gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*1.5/1000))//Rated 1.5�� 1.1W(0.1%) *1.5
              {
                gs_power_energycal.ul_power[0] = 0;
//                gs_power_energycal.ul_power[0] = 0x1234567;
              }
              else
              {
                gs_power_energycal.ul_power[0] = Buf_RMS.ul_PB;
              }
            }
            if(i == 7)//BRoad reactive
            {
              Buf_RMS.ul_QB = gs_RmsData.ul_QB;
              if((Buf_RMS.ul_QB >>24)>0X7f)
              {
                Buf_RMS.ul_QB = ~Buf_RMS.ul_QB+1;
              }
              if((float)Buf_RMS.ul_QB/gs_JbPm.ul_PG < (((float)(gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*1.5/1000))//Rated 1.5�� 1.1W(0.1%) *1.5
              {
                gs_power_energycal.ul_Npower[0] = 0;
//                gs_power_energycal.ul_Npower[0] = 0X1234567;
              }
              else
              {
                gs_power_energycal.ul_Npower[0] = Buf_RMS.ul_QB;
              }
            }
        }

        if(i == 2)
        {
          Rms_ul_U_Buf[0] = gs_RmsData.ul_U;
          Rms_ul_USum = 0;
          for(uint8_t z = 9;z>0;z--)
          {
            Rms_ul_U_Buf[z] = Rms_ul_U_Buf[z-1]; 
            Rms_ul_USum += Rms_ul_U_Buf[z];
          }
          Rms_ul_UAvg = Rms_ul_USum/9;
          Rms_ul_UC = Rms_ul_USum/9-Rms_ul_U_Buf[0];
          Rms_ul_Up = (float)Rms_ul_UC/Rms_ul_UAvg;
        }
        if(i == 3)
        {
          if(Rms_R_F == 1)
          {
            Rms_ul_I1_Data[Rms_R_C]= gs_RmsData.ul_I1;
            Rms_R_C++;
            if(Rms_R_C > 99)
            {
              Rms_R_F = 0;
            }
          }
          
          
          Rms_ul_I1_Buf[0] = gs_RmsData.ul_I1;
          Rms_ul_I1Sum = 0;
          for(uint8_t z = 9;z>0;z--)
          {
            Rms_ul_I1_Buf[z] = Rms_ul_I1_Buf[z-1];
            Rms_ul_I1Sum += Rms_ul_I1_Buf[z];
          }
          Rms_ul_I1Avg = Rms_ul_I1Sum/9;
          Rms_ul_I1C = Rms_ul_I1Sum/9-Rms_ul_I1_Buf[0];
          Rms_ul_I1p = (float)Rms_ul_I1C/Rms_ul_I1Avg;
        }
        Rms_ul_UpxI1p = Rms_ul_Up*Rms_ul_I1p;
//******************************************************************************        
    }else
    {
        ReadRmsErrFlg = 1;   //Communication anomaly when reading valid value
        cksdis_flag += 3;//zzp
        
    }
  }
  if( ReadRmsErrFlg)
  {
    if( ReadRmsErrCnt < 3)
    {
        ReadRmsErrCnt++;
    }else                   //If communication is abnormal for 3 times in a row, reset the metering chip
    {
//        MemSet((uint8_t *)&gs_RmsData, 0, sizeof( gs_RmsData)); //Clear the effective value variable to 0
//        RxReset_Raccoon();
//        Init_UartRaccoon();         //Metering serial port initialization
//        Raccoon_UpdatePar();        //Metering parameter refresh
      
//      ReadRmsErrCnt = 0;    //Reset the metering chip and clear the abnormal count value when reading the effective value
    }
  }else
  {
      ReadRmsErrCnt = 0;    //Communication is normal, reading the effective value is abnormal, the count value is cleared to 0
      
      //***********************************************************************
//         if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Idle) 
//          {
//            gs_ComGroup[ComIndex_Uart2].ucStt = ComStt_Send ;
//            Uart2_SendEn();//485ctrl tx en
//            //for(a=0;a<2;a++)
//            //{
//              UART2->CTRL&=0XEFF;
//              gs_RmsData_txbuf = *(pRmsData-4);//+a);
//              for(j=0;j<4;j++)
//              {
//                //while(UART_GetINTStatus(UART2, UART_INTSTS_TXDONE));
//                while(UART2->STATE&BIT5)
//                {
//                UART2->STATE |= BIT5;
//                }
//                //UART_SendData( UART2, gs_RmsData_txbuf>>(8*j));
//                UART_SendData( UART2, gs_RmsData_txbuf>>(24-8*j));
//                DelayXms(8);
//                //UART2->DATA = 0X55+j;
//                //UART_SendData( UART2, 0X55);
//              }
//            //} 
//               
//            Uart2_CtrIoIdle();//485ctrl idle
//            UART2->CTRL |= 0X100;
//            gs_ComGroup[ComIndex_Uart2].ucStt = ComStt_Idle;
//          }
      
//        if(gs_ComGroup[ComIndex_Uart2].ucStt == ComStt_Idle && DMA_Flag == 1/*rms_count > 1*/) 
//          {
//            
//            gs_ComGroup[ComIndex_Uart2].ucStt = ComStt_Send ;
//            Uart2_SendEn();//485ctrl tx en            
//              UART2->CTRL&=0XEFF;
//              //temp_buf[DMA_Count] = temp_buf[DMA_Count]>>8;
//             
//              for(j=0;j<4;j++)
//              {
//                while(UART2->STATE&BIT5)
//                {
//                UART2->STATE |= BIT5;
//                }
//                UART_SendData( UART2, temp_buf[DMA_Count]>>(24-8*j));
////                UART_SendData( UART2, Rms_ul_I1_Data[DMA_Count]>>(24-8*j));
//                DelayXms(8);
//
//              }
//              //gs_RmsData_txbuf++;
//              DMA_Count++;
//              if(DMA_Count > 1537)
//              //if(DMA_Count > 99)  
//              {
//                DMA_Count = 0;
//                //gs_RmsData_txbuf = (uint32_t *)&temp_buf[0];
//                DelayXms(20);
//                UART_SendData( UART2, 0xAA);
//                DelayXms(50);
//                UART_SendData( UART2, 0x55);
//                DMA_Flag = 0;
//                rms_count = 0;
//              }
//            
//            
//            Uart2_CtrIoIdle();//485ctrl idle
//            UART2->CTRL |= 0X100;
//            gs_ComGroup[ComIndex_Uart2].ucStt = ComStt_Idle;
//          }
        
        //*********************************************************************
         
  }
}

/*=========================================================================================\n
* @function_name: Raccoon_RunCheck
* @function_file: Raccoon.c
* @describe:
* 
* 
* @parameter:
* @param:  
* @return: 
* @author:   lwb (2013-09-25)
* @Remark: 
*-------------------------------------------------------------------------------------------
* @Modified by:  
* @Modifications:
===========================================================================================*/
void Raccoon_RunCheck(void)
{
//  uint8_t ucbuf[4];
//  if( ReadRaccoon( SYS_INTSTS, 1))
//  {
//    //MemCpy( ucbuf, gs_RacCtrl.ucBuf+3, 4);
//    MemCpy( ucbuf, gs_RacCtrl.ucBuf, 4);//zzp0114
//    
//    if((ucbuf[1] & BIT6) == BIT6)           //Checksum Error bit14
//    {
//      
//      MemSet((uint8_t *)&gs_RmsData, 0, sizeof( gs_RmsData)); //Clear the effective value variable to 0
//      RxReset_Raccoon();
//      Init_UartRaccoon();         //Metering serial port initialization
//      Raccoon_UpdatePar();        //Metering parameter refresh
//      WriteRaccoon(0x00004000,0x72);//zzp0116
//    }      
//  }
}
//----------------------------Function Info -----------------------
//**Function Name    : Raccoon_TMR1_Init
//**Input Parameters : None
//**Output Parameters: None
//**return           : None
//**Description      ��TMR1 initialization
//-----------------------------------------------------------------
void Raccoon_TMR1_Init(void)
{
//  TMR_InitType TMR_InitStruct;
//  
//  TMR_DeInit(TMR1);
//  TMR_InitStruct.ClockSource = TMR_CLKSRC_INTERNAL; //Clock source: internal clock(APB clock 6553600)
//  TMR_InitStruct.EXTGT = TMR_EXTGT_DISABLE;
//  TMR_InitStruct.Period = 65536 * 8 - 1;  //Overflow interval: 80ms
//  TMR_Init(TMR1, &TMR_InitStruct);
//  
//  TMR_INTConfig(TMR1, ENABLE);  //Enable Timer1 interrupt
//  CORTEX_SetPriority_ClearPending_EnableIRQ(TMR1_IRQn, 0); //High priority
//  
//  TMR_Cmd(TMR1, DISABLE);  //DISABLE Timer1
}

//----------------------------Function Info -----------------------
//**Function Name    : Raccoon_EXTI5_Init
//**Input Parameters : None
//**Output Parameters: None
//**return           : None
//**Description      ��EXTI5 initialization
//-----------------------------------------------------------------
void Raccoon_EXTI5_Init(void)
{
//  PMU_WakeUpPinConfig(GPIO_Pin_5, IOA_FALLING);
//  PMU_ClearIOAINTStatus(GPIO_Pin_5);
//  PMU_INTConfig(PMU_INT_IOAEN, ENABLE);
//  CORTEX_SetPriority_ClearPending_EnableIRQ(PMU_IRQn, 0); //High priority
}


//----------------------------Function Info -----------------------
//**Function Name    : Raccoon_CFCtrl
//**Input Parameters : None
//**Output Parameters: None
//**return           : None
//**Description      ��Send CF pulse light
//-----------------------------------------------------------------
void Raccoon_CFCtrl(void)
{
//    uint8_t  i;
//    
//    __disable_interrupt();
//    for( i=0;i<50;i++ )
//    {	
//        __no_operation();
//    }
//    if((GPIOA->STS & BIT5) == 0)
//    {
//        TMR1->VALUE  =  0;      //Clear the timer count value
//        TMR_Cmd(TMR1, ENABLE);  //openTimer1
//        
//        CF_ENABLE();
//        __enable_interrupt();        
//    }
//    __enable_interrupt();
}

void Raccoon_UpdateChecksum(void)
{
  static uint32_t sum = 0,Rbuf;
  uint8_t i;
  for(i = 0;i<42;i++)
  {
    if(i<8)//0-7
    {
       ReadRaccoon(i,1);
       for(uint8_t j=4;j>0;j--)
       {
          Rbuf += (uint32_t)gs_RacCtrl.ucBuf[j-1]<<8*(j-1);
       }
      sum += Rbuf; 
      Rbuf = 0;
    }
    if(i>7&&i<30&&i!=27)//0x25-0x30
    {
       ReadRaccoon(i+29,1);
       for(uint8_t j=4;j>0;j--)
       {
          Rbuf += (uint32_t)gs_RacCtrl.ucBuf[j-1]<<8*(j-1);
       }
      sum += Rbuf; 
      Rbuf = 0;
    }
    if(i>29&&i<42)//0x55-0x60
    {
       ReadRaccoon(i+55,1);
       for(uint8_t j=4;j>0;j--)
       {
       Rbuf += (uint32_t)gs_RacCtrl.ucBuf[j-1]<<8*(j-1);
       }
      sum += Rbuf; 
      Rbuf = 0;
    }
   
  }
  
  sum = 0xFFFFFFFF-sum;
  WriteRaccoon(sum,DSP_CFG_CKSUM);
  sum = 0;
  WriteRaccoon(0x0000FFFF,0x72);
  ReadRaccoon(0x72,1);//Read flag clear flag
}

void DMA_DataUpload(void)
{
  //SPI_Cmd(SPI2,ENABLE);
  //SPI_SmartModeCmd(SPI2, ENABLE);
  //DMA_StopTransmit(DMA_CHANNEL_0, DISABLE); //Clear stop
  //DMA_Cmd(DMA_CHANNEL_0, ENABLE);
  //Delay_ms(300);
  delayMicroseconds(300000);
  WriteRaccoon(0x000F0208, DSP_CTRL5); 
  //Delay_ms(200);
  delayMicroseconds(200000);
  //tmp_fra = DMA_GetFrameLenTransferred(DMA_CHANNEL_0);
  //tmp_pack = DMA_GetPackLenTransferred(DMA_CHANNEL_0);
  //DMA_StopTransmit(DMA_CHANNEL_0, ENABLE); //Set stop, clear frame/package counters 
}
