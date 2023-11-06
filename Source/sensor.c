#include "..\Include\sensor.h"
#include "..\Include\CaptureKey.h"
//******************************************************************************
//! Variable -------------------------------------------------------------------
u8  bSensorLedMode=255;
u32 iSensorTickTime=0;
u32 iSensorTickFireClash=0;

u8  bSensorLedSetFlag=0;
 
//******************************************************************************
//! \brief  init sensor
//******************************************************************************
void SensorInit()
{
 
 //! sensor power on
 //GPIO_SetBits(SENSOR_PWR_SET);
}
//******************************************************************************
//! \brief  check sensor module data and led blink time and color
//******************************************************************************
void SensorMain( void )
{
  if(CapturekeyGetWorkMode()==CAPTUREKEY_WAIT_FOR_PUSHING_KEY_MODE)
    {
      //! get usart data for check fire clash 
      if( SystemTickGetMsecond()-iSensorTickFireClash>10)   
        {
          iSensorTickFireClash=SystemTickGetMsecond();
          UsartSendData(SENSOR_CMD_GET_FIRE );
          UsartSendData(1);
        }
     if(bSensorLedMode!=CAPTUREKEY_WAIT_FOR_PUSHING_KEY_MODE)
       {
         bSensorLedMode=CAPTUREKEY_WAIT_FOR_PUSHING_KEY_MODE;         
         UsartSendData(SENSOR_CMD_LED_SET);
         UsartSendData(CapturekeyGetWaitForPushingLED());
         UsartSendData(1);
         UsartSendData(1);
       }
    }
  //----------------
  if(CapturekeyGetWorkMode()==CAPTUREKEY_NORMAL_MODE)
    {
      if(bSensorLedMode!=CAPTUREKEY_NORMAL_MODE)
        {
          bSensorLedMode=CAPTUREKEY_NORMAL_MODE;
       
          UsartSendData(SENSOR_CMD_LED_SET);
          UsartSendData(CapturekeyGetNormalLED());
          UsartSendData(10);
          UsartSendData(10);
        }
      //! get usart data for check fire clash 
      if( SystemTickGetMsecond()-iSensorTickFireClash>10)   
        {
          iSensorTickFireClash=SystemTickGetMsecond();
          UsartSendData(SENSOR_CMD_GET_FIRE );
          UsartSendData(1);
        }
    }
  //----------------
  if((CapturekeyGetWorkMode()==CAPTUREKEY_STOP_MODE)||
    (CapturekeyGetWorkMode()==CAPTUREKEY_PUSHED_KEY_MODE)||
    (CapturekeyGetWorkMode()==CAPTUREKEY_PAUSE_MODE))  
   {
     if(bSensorLedMode!=CAPTUREKEY_STOP_MODE)
       {
         bSensorLedMode=CAPTUREKEY_STOP_MODE;
          UsartSendData(SENSOR_CMD_LED_SET);
          UsartSendData(0);
          UsartSendData(10);
          UsartSendData(10);
         
       }
   }
}
   
//******************************************************************************
//! \brief  empty usart buffer
//******************************************************************************
void SensorEmptyUsart( void )
{
   UsartSendData(SENSOR_CMD_GET_FIRE );
   UsartSendData(1);
   UsartRxSetFlag(0);
}