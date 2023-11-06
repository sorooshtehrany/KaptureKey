#include "..\Include\LED.h"
#include "..\Include\CaptureKey.h"
#include "..\Include\systemTick.h"
//******************************************************************************
//! Variable -------------------------------------------------------------------
u8 bLEDFiredMode=0;
u32 iLEDTickBuffer=0;
u8 bLEDFireModeCounter=0;
//******************************************************************************
//! \brief  
//******************************************************************************
void LEDInit          ( void )
{
   GPIO_Init( LED1, GPIO_Mode_Out_PP_Low_Fast );
   GPIO_Init( LED2, GPIO_Mode_Out_PP_Low_Fast );
   GPIO_Init( LED3, GPIO_Mode_Out_PP_Low_Fast );
   GPIO_Init( LED4, GPIO_Mode_Out_PP_Low_Fast );
   GPIO_Init( LED5, GPIO_Mode_Out_PP_Low_Fast );
}
//******************************************************************************
//! \brief  
//******************************************************************************
void LEDGetStatus     ( u8 bStatus )
{
  
}
//******************************************************************************
//! \brief  
//******************************************************************************
void LEDMain  ( void )
{
  u8 LEDChargeViewer;  
  //! show demo for wait for push
  if(CapturekeyGetWorkMode()==CAPTUREKEY_WAIT_FOR_PUSHING_KEY_MODE)
    {
     if(iLEDTickBuffer-SystemTickGetMsecond()>1000)
       {
        iLEDTickBuffer=SystemTickGetMsecond();
        bLEDFireModeCounter=(bLEDFireModeCounter+1)%5;
        LEDsOnOff(1<<bLEDFireModeCounter);   
       }
    }
  //! show demo for fire
  if(CapturekeyGetWorkMode()==CAPTUREKEY_PUSHED_KEY_MODE)    
    {
     if(iLEDTickBuffer-SystemTickGetMsecond()>100)
       {
        iLEDTickBuffer=SystemTickGetMsecond(); 
        bLEDFireModeCounter=(bLEDFireModeCounter+1)%2;         
        LEDsOnOff(bLEDFireModeCounter*0X1F);   
       }
     //! show battery charge
     if((CapturekeyGetWorkMode()==CAPTUREKEY_NORMAL_MODE)||
        (CapturekeyGetWorkMode()==CAPTUREKEY_STOP_MODE)||
        (CapturekeyGetWorkMode()==CAPTUREKEY_PAUSE_MODE))
       {
        if(iLEDTickBuffer!=SystemTickGetSecond())
          {
           iLEDTickBuffer=SystemTickGetSecond();
           LEDChargeViewer=BatteryGetCharge()/20;
         //  LEDChargeViewer=1<<LEDChargeViewer;
           LEDsOnOff(LEDChargeViewer);
          }
       }
    }
}
//******************************************************************************
//! \brief  On or off LEDs
//******************************************************************************
void LEDsOnOff(u8 Options)
{
  
 if((1&Options)!=0)        GPIO_ResetBits(LED1);
 else                      GPIO_SetBits(LED1);
 
 if((2&Options)!=0)        GPIO_ResetBits(LED2);
 else                      GPIO_SetBits(LED2);
 
 if((4&Options)!=0)        GPIO_ResetBits(LED3);
 else                      GPIO_SetBits(LED3);
 
 if((8&Options)!=0)        GPIO_ResetBits(LED4);
 else                      GPIO_SetBits(LED4);
 
 if((16&Options)!=0)       GPIO_ResetBits(LED5);
 else                      GPIO_SetBits(LED5); 
}