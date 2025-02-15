#include <cstdint>
#include "V93XX_Include.h"

// Globals defined as externs in header
GS_SYSRUNST gs_SysRunSt;
uint32_t abc;

/*=========================================================================================\n
* @function_name: Hex2BCD
* @function_file: Api.c
* @Description: Hex code to be converted (hex < 100000000)
*
*
* @parameters:
* @param:hex
*
* @return: 
* @return: uint32_t The result after converting the hex code to bcd
* @Author: lwb (2012-02-29)
* @Remark: 
*-------------------------------------------------------------------------------------------
* @Modified by:
* @Modify content:
===========================================================================================*/
uint32_t Hex2BCD(uint32_t hex)
{
    uint32_t result = 0;
    uint32_t cmp, bitcap;
    uint8_t uc_i;

    while(hex > 99999999)
    {
        hex-=100000000;
        //return 0xFFFFFFFF;
    }

    for(cmp = 80000000, bitcap = 0x80000000, uc_i = 0; uc_i < 16; uc_i++)
    {
        if(hex >= cmp)
        {
            result |= bitcap;
            hex -= cmp;
        }
        bitcap >>= 1;
        switch(uc_i)
        {
            case 3:
                cmp = 8000000;
                break;
            case 7:
                cmp = 800000;
                break;
            case 11:
                cmp = 80000;
                break;
            default:
                cmp >>= 1;
        }
    }

    result |= IntHex2BCD(hex);
    return result;
}

/*=========================================================================================================\n
* @function_name: IntHex2BCD
* @function_file: Api.c
* @Description: Hex code to be converted (hex < 10000)
*
*
* @parameters:
* @param:hex
*
* @return: 
* @return: uint16 The result after converting the hex code to bcd
* @Author: lwb (2012-02-29)
* @Remark: 
*-------------------------------------------------------------------------------------------
* @Modified by:
* @Modify content:
============================================================================================================================*/
uint16_t IntHex2BCD(uint16_t hex)
{
    uint16_t result = 0;

    if(hex > 9999)
    {
        return 0xFFFF;
    }

    if(hex >= 8000)
    {
        result |= 0x8000;
        hex -= 8000;
    }

    if(hex >= 4000)
    {
        result |= 0x4000;
        hex -= 4000;
    }

    if(hex >= 2000)
    {
        result |= 0x2000;
        hex -= 2000;
    }

    if(hex >= 1000)
    {
        result |= 0x1000;
        hex -= 1000;
    }

    if(hex >= 800)
    {
        result |= 0x800;
        hex -= 800;
    }

    if(hex >= 400)
    {
        result |= 0x400;
        hex -= 400;
    }

    if(hex >= 200)
    {
        result |= 0x200;
        hex -= 200;
    }

    if(hex >= 100)
    {
        result |= 0x100;
        hex -= 100;
    }

    result |= APIByteHEX2BCD(hex);
    return result;

}


/*=========================================================================================================\n
* @function_name: APIByteHEX2BCD
* @function_file: Api.c
* @describe: 
*
*
* @parameters:
* @param:ucHEX
*
* @return: 
* @return: uint8
* @Author: lwb (2012-02-29)
* @Remark: 
*-------------------------------------------------------------------------------------------
* @Modified by:
* @Modify content:
===========================================================================================================================*/
uint8_t APIByteHEX2BCD(uint8_t ucHEX)
{
    if(ucHEX > 99)
    {
        return 0xFF;
    }
    return gCuc_HEX2BCDMap[ucHEX];
}

/*=========================================================================================\n
* @function_name: CalRMS
* @function_file: EnergyBottom.c
* @Description: Calculate instantaneous amount
* 
* 
* @parameter: 
* @param:addr  Register address
* 
* @return: 
* @return: uint32_t Return valid value
* @author:   lwb (2012-05-22)
* @Remark:
*-------------------------------------------------------------------------------------------
* @Modified by:
* @Modify content: 
===========================================================================================*/
uint32_t CalRMS(uint16_t addr)
{
  Word32 TempValue,TempValue2;
  uint32_t TempPara,TempValue1;
  //TempValue.lword=0;
  
//  TempValue.lword=EnyB_ReadMeterParaACK(addr);

  switch (addr)
  {
    case DATAP:
    case DATAPB:
      if(gs_Channel.ucSta == SETA)
      {
        TempValue.lword = gs_RmsData.ul_P;
        //TempValue.lword = 190973120;
        TempPara=gs_JbPm.ul_PG;        //Read the scale coefficient
      }
      else
      {
        TempValue.lword = gs_RmsData.ul_PB;
//        TempValue.lword = 4278776953;//4278789009;
//        TempPara=5*gs_JbPm.ul_PG/9;        //Read the proportional coefficient B channel 1.4 times
        TempPara=gs_JbPm.ul_PG; 

      }
        
        if(TempValue.byte[3]>0x7f)          //If it is a negative number, it will first become a positive number
        {
            TempValue.lword=~TempValue.lword+1;
        }
     
        if(TempPara==0)
        {    
            TempValue1=0;
        }
         else
        {     
            TempValue.lword=(uint32_t)((10*((float)TempValue.lword)/TempPara)*10);
            //LCDPower display one more decimal
        }
//        if(TempValue.lword<(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100))
//        {
//             TempValue1=0;//zzp0311 Test not to filter the current
//        }
//        else
//        {
             TempValue1=Hex2BCD(TempValue.lword);

             if(gs_SysRunSt.Pdir==true)   //Add negative sign in reverse
             {
                 TempValue1|=0x800000;
             }
//        }
        break;
    case DATAQ:
    case DATAQB:
        if(gs_Channel.ucSta == SETA)
        {
          TempValue.lword = gs_RmsData.ul_Q;
//          TempPara=gs_JbPm.ul_QG;        //Read the scale coefficient
          TempPara = gs_JbPm.ul_PG;
        }
        else
        {
          TempValue.lword = gs_RmsData.ul_QB;
//          TempPara=5*gs_JbPm.ul_QG/9;        //Read the scale coefficient
          TempPara = gs_JbPm.ul_PG;
        }
        if(TempValue.byte[3]>0x7f)          //If it is a negative number, it will first become a positive number
        {
            TempValue.lword=~TempValue.lword+1;
        }
       
        if(TempPara==0)
        {    
            TempValue1=0;
        }
         else
        {     

            TempValue.lword=(uint32_t)((10*((float)TempValue.lword)/TempPara)*10);
            //LCD power display one more decimal
        }
//        if(TempValue.lword<(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100))
//        {
//             TempValue1=0;//zzp0311 Test not to filter the current
//        }
//        else
//        {
             TempValue1=Hex2BCD(TempValue.lword);

             if(gs_SysRunSt.Pdir==true)   //Add negative sign in reverse
             {
                 TempValue1|=0x800000;
             }
//        }
        break;
    case RMSU:
         TempValue.lword = gs_RmsData.ul_U;
      
         TempPara=gs_JbPm.ul_URmG;   //ReadRMSPara(1);
         if(TempPara==0)
         {    
            return 0;
         }
         else
         {   
             abc =TempValue.lword/TempPara;
//             TempValue.lword=TempValue.lword/TempPara;
             TempValue.lword=(TempValue.lword/TempPara)*10;//Voltage displays 2 decimal places
              
         }

//         if(TempValue.lword<ULEVEL && guc_FactoryType != FactorMd)
//         {    
//             return 0;
//         }else
         {
             TempValue1=Hex2BCD(TempValue.lword);
         }
         break;
    case RMSI1:
        //Calculation of current
        TempValue.lword = gs_RmsData.ul_I1;
        
      
        TempPara=gs_JbPm.ul_I1RmG;    //ReadRMSPara(2);
        if(TempPara==0)
        {    
            return 0;
        }
        else
        {
            TempValue.lword=10*TempValue.lword/TempPara;//LCDCurrent displays one more decimal 
        }       
//        if(TempValue.lword<((gs_JbPm.ui_Ib/1000)*2) //zzp03011 Test not to filter the current
//           )//&& guc_FactoryType != FactorMd)
//        {    
//            return 0;
//        }else
//        {        
            TempValue1=Hex2BCD(TempValue.lword);

            //Power calculation
//            TempValue2.lword=EnyB_ReadMeterParaACK(DATAP);
            TempValue2.lword = gs_RmsData.ul_P;
            
            if(TempValue2.byte[3]>0x7f)                         //如果是负数的数，先变成正数
            {
                TempValue2.lword=~TempValue2.lword+1;
            }

            TempPara=gs_JbPm.ul_PG;        //读取比例系数

            if(TempPara==0)
            {    
                TempValue2.lword=0;
            }
            else
            {     
                TempValue2.lword=(uint32_t)(((float)TempValue2.lword/TempPara)*10);
            }

            if(TempValue2.lword<(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100)
               )//&& guc_FactoryType != FactorMd)
            {
                //如果功率小于启动功率，这是电流小于0.2A,电流清零
                if(TempValue1<0x200)
                {
//                    TempValue1=0;//zzp03011 测试不对电流做过滤
                }
            }           
            
            if(gs_SysRunSt.Pdir==true)   //反向添加负号
            {
                TempValue1|=0x800000;
            }
//        }
        break;
    case RMSI2:
        TempValue.lword = gs_RmsData.ul_I2;
      //TempValue.lword = 2553816932;
        
        TempPara=gs_JbPm.ul_I2RmG;    //ReadRMSPara(3);
        if(TempPara==0)
        {    
            return 0;
        }
        else
        {    
            TempValue.lword=10*TempValue.lword/TempPara;//LCDCurrent displays one more decimal
        }
//        if(TempValue.lword<((gs_JbPm.ui_Ib/1000)*2)//zzp0311 Test temporary blocking
//           )//&& guc_FactoryType != FactorMd)
//        {    
//            return 0;
//        }
//        else 
//        {    
            TempValue1=Hex2BCD(TempValue.lword);

            //Power calculation
//            TempValue2.lword=EnyB_ReadMeterParaACK(DATAQ);
            TempValue2.lword = gs_RmsData.ul_QB;//ZZP
            
            if(TempValue2.byte[3]>0x7f)                         //If it is a negative number, it will first become a positive number
            {
                TempValue2.lword=~TempValue2.lword+1;
            }

            //ZZP TempPara=gs_JbPm.ul_QG/256;        //Read the scale coefficient
//            TempPara=5*gs_JbPm.ul_QG/9;//zzp0307
            TempPara = 5*gs_JbPm.ul_PG/9;//zzp0307

            if(TempPara==0)
            {    
                TempValue2.lword=0;
            }
            else
            {     
                //zzp TempValue2.lword=((TempValue2.lword*32)/TempPara)*8;
                TempValue2.lword=(uint32_t)(((float)TempValue2.lword/TempPara)*10);
            }

            if(TempValue2.lword<(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100)
               )//&& guc_FactoryType != FactorMd)
            {
                //If the power is less than the start power, this is the current is less than 0.2A, and the current is cleared
                if(TempValue1<0x200)
                {
//                    TempValue1=0;//Test not to filter the current zzp0311
                }
            }           
            
            if(gs_SysRunSt.Pdir==true)   //Add negative sign in reverse
            {
                TempValue1|=0x800000;
            }
//        }       
        break;
      case FREQ:
          TempValue.lword = gs_RmsData.ul_Hz;
          
          TempValue.lword = 100*freq_const/TempValue.lword;
          if((DSP_CTRL0_Value &0x30000000)== 0)
          {
            TempValue.lword = TempValue.lword*16;
          }
          else if((DSP_CTRL0_Value &0x30000000)== 2)
          {
            TempValue.lword = TempValue.lword*64;
          }
          
          
          /*
          TempValue.lword=ADCFRE*100/TempValue.lword;
          if(TempValue.lword>4950&&TempValue.lword<5050)
          {
              TempValue.lword=5000;
          }else if(TempValue.lword<4500)
          {    
              TempValue.lword=4500;
          }else if(TempValue.lword>5500)
          {    
              TempValue.lword=5500;
          }
          *///zzp
          TempValue1=Hex2BCD(TempValue.lword);
         break;
//    case RMSIU:
//         TempPara=gs_JbPm.ul_URmG/65536;   //ReadRMSPara(1);
//         if(TempPara==0)
//         {    
//            return 0;
//         }
//         else
//         {    
//             TempValue.lword=TempValue.lword/TempPara;
//         }
//
//         if(TempValue.lword<ULEVEL)
//         {    
//             return 0;
//         }else
//         {
//             TempValue1=TempValue.lword;
//         }
//         break;
      case DATASA:
      case DATASB:
        if(gs_Channel.ucSta == SETA)
        {  
          TempValue.lword = gs_RmsData.ul_SA;
          TempPara = gs_JbPm_SG;
        }
        else
        {
          TempValue.lword = gs_RmsData.ul_SB;
          TempPara = 5*gs_JbPm_SG/9;
        }
        
        if(TempValue.byte[3]>0x7f)          //If it is a negative number, it will first become a positive number
        {
            TempValue.lword=~TempValue.lword+1;
        }
       
        if(TempPara==0)
        {    
            TempValue1=0;
        }
         else
        {     
             TempValue.lword=(uint32_t)(((float)TempValue.lword/TempPara)*10);
        }
//        if(TempValue.lword<(((gs_JbPm.ui_Ib/1000)*(gs_JbPm.ui_Un/100))*2/100))
//        {//上面是
//             TempValue1=0;
//        }
//        else
//        {
             TempValue1=Hex2BCD(TempValue.lword);

             if(gs_SysRunSt.Pdir==true)   //Add negative sign in reverse
             {
                 TempValue1|=0x800000;
             }
//        }
        break;
    default:
        TempValue1=0;
        break;
  }

  return TempValue1;
}