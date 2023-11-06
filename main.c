// In The Name Of God

//! Library --------------------------------------------------------------------
#include "STM8L Library\Include\stm8l15x.h"
#include "STM8L Library\Include\stm8l15x_conf.h"
// -----------------------------------------------------------------------------
  //#include "Include\AT24C04.h"
  //#include "Include\at24cx.h"
#include "Include\audio.h"
#include "Include\battery.h"
#include "Include\CaptureKey.h"
#include "Include\delay.h"
  //#include "Include\eeprom.h"
#include "Include\integer.h"
#include "Include\LED.h"
#include "Include\MRF24J40.h"
#include "Include\power.h"
#include "Include\sdcard.h"
#include "Include\sdcard_fat.h"
#include "Include\sdcard_low.h"
#include "Include\segment.h"
#include "Include\sensor.h"
//#include "Include\stm8_eval_spi_sd.h"
#include "Include\systemTick.h"
  //#include "Include\time.h"
#include "Include\usart.h"

//******************************************************************************
//! Define ---------------------------------------------------------------------

//******************************************************************************
//! Variable -------------------------------------------------------------------

//******************************************************************************
//! Function -------------------------------------------------------------------
void MainClockInit  ( void );
void MainFlashInit  ( void );
//******************************************************************************
void main( void )
{
  u32 iSensorTickFireClash;
    //! check read out protection register
    /*
    if( OPT->ROP != 0x00 )
    {
        //! flash data unlock
        FLASH_Unlock( FLASH_MemType_Data );         
        
        //! active read out protection
        FLASH_ProgramOptionByte( 0x4800, 0x00 ); 
        
        //! flash data lock
       // FLASH_Lock( FLASH_MemType_Data );
    }
    */
//    PowerInit();   
    //! init main clock
    MainClockInit();    
    //! init flash
    MainFlashInit();    
    //! audio init
    AudioInit();
    //! battery init
    BatteryInit();

    FLASH_DeInit();
    //! MRF24J40 gpio init
    Mrf24j40PinInit();
    //! MRF24J40 chip init
    Mrf24j40ChipInit();
    //! MRF24J40 set tx power
    Mrf24j40SetTxPower(31);
    //! MRF24J40 set rx power
    Mrf24j40SetRxPower(255);
    //! MRF24J40 set freq channel
    Mrf24j40SetChannel(CHANNEL_11);    
    //! segment init
    SegmentInit();
    //! system tick init
    SystemTickInit();    
    //! usart init
    UsartInit();
    //! eeprom init
    EepromInit();
    //SD card init
    sdcard_init();
     
    CapturekeyMemoryReload();
    //! Enable general interrupts
    enableInterrupts();
    
       
    CapturekeySaveDataByte(CAPTUREKEY_ID_MEMORY_ADD,0);
    CapturekeySaveDataByte(CAPTUREKEY_GAME_TIME_LENGTH_MEMORY_ADD,15 )  ;
    CapturekeySaveDataByte(CAPTUREKY_SENSOR_LED_NORMAL_COLOR_MEMORY_ADD,2);
    CapturekeySaveDataByte(CAPTUREKY_SENSOR_LED_WAIT_FOR_PUSHING_KEY_MEMORY_ADD,1);
     
     
    UsartSendData(SENSOR_CMD_LED_SET);
         UsartSendData(3);
         UsartSendData(10);
         UsartSendData(10);
 
    AudioStartPlay("1.wav");
    
         while(1)
    {

      //! segment main control
//        SegmentMain();
        //! Sensor main control
//        SensorMain();
        //! Capturekey main control
//        CaptureKeyMain();
        //! CaptureKey usart data check
//        CapturekeyUsartDataCheck();
        //! CaptureKey rf pack check
//        CapturekeyRfPackCheck();
        //! CaptureKey rf data check
//        CapturekeyRfDataCheck();
        //! Capturekey push check
//        CapturekeyCheckKey();
        //! audio play check
//        AudioMain();
       AudioMainPlay(); 
        /*
     if( SystemTickGetMsecond()-iSensorTickFireClash>10)   
        {
          iSensorTickFireClash=SystemTickGetMsecond();
          UsartSendData(SENSOR_CMD_GET_FIRE );
          UsartSendData(1);
        }
*/
      
    }
}
//******************************************************************************
//! \brief  Configure System Clock 
//! \param  None
//! \retval None
//******************************************************************************
void MainClockInit( void )
{
  //! Select HSE as system clock source 
  CLK_SYSCLKSourceSwitchCmd(ENABLE);
  CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  //! system clock prescaler: 1
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI);
}
//******************************************************************************
//! \brief  eeprom init
//******************************************************************************
void MainFlashInit( void )
{
    //! Define flash programming Time
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    
    FLASH_Unlock(FLASH_MemType_Program);
    //! Wait until Flash Program area unlocked flag is set
    while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET)
    {}
    
    //! Unlock flash data eeprom memory 
    FLASH_Unlock(FLASH_MemType_Data);
    //! Wait until Data EEPROM area unlocked flag is set
    while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
    {}
}
//******************************************************************************
//! \brief  Reports the name of the source file and the source line number
//!   where the assert_param error has occurred.
//! \param  file: pointer to the source file name
//! \param  line: assert_param error line source number
//******************************************************************************
#ifdef  USE_FULL_ASSERT
void assert_failed( uint8_t* file, uint32_t line )
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
        for( u8 i=0 ; i<20 ; i++ )
        {
            //gameId=i;
            DelayMs(400);
        }
  }
}
#endif