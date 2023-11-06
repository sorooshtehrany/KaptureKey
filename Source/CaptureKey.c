#include "..\Include\CaptureKey.h"
#include "..\Include\systemTick.h"
#include "..\Include\segment.h"
//******************************************************************************
//! Variable -------------------------------------------------------------------
u8 bCapturekeyNumberOfTeams;
u8 bCapturekeyNumberOfPlayers[CAPTUREKEY_MAX_OF_TEAMS][CAPTUREKEY_MAX_OF_PLAYERS];
                    
u8  bCapturekeyMode=CAPTUREKEY_STOP_MODE;
u8  bCapturekeyMainId=CAPTUREKEY_ID_RESET;
u8  bCapturekeyGameTimeLen=CAPTUREKEY_GAME_TIME_RESET;
u8  bCapturekeyNormalLED;
u8  bCapturekeyWaitForPushingLED;
u8  bCapturekeyFireClashGunID;

u8  bCapturekeyRfPackRxArray[50];
u8  bCapturekeyRfDataRxArray[50];
u8  bCapturekeyRfDataRxFlag=0;
u8  bCapturekeyRfDataTxArray[200];
u8  bCapturekeyPuaseFlag=0;
u8  bCapturekeyKillingCounter=0;

u32 iCapturekeyGameTimeCounter=0;
u32 iCapturekeyTickBuffer=0;


//******************************************************************************
//! \brief 
//******************************************************************************
void CaptureKeyInit            ( void )
{
  //! init key gpio
  GPIO_Init( CaptureKey_GPIO, KEY_GPIO_MODE );
}
//******************************************************************************
//! \brief default setting
//******************************************************************************
void CaptureKeyResetSetting( void )
{
 //! reset capturekey ID 
 bCapturekeyMainId=CAPTUREKEY_ID_RESET;
 CapturekeySaveDataByte( CAPTUREKEY_ID_MEMORY_ADD, bCapturekeyMainId ); 
 //! reset game time len
 bCapturekeyGameTimeLen=CAPTUREKEY_GAME_TIME_RESET;
 CapturekeySaveDataByte( CAPTUREKEY_GAME_TIME_LENGTH_MEMORY_ADD, bCapturekeyGameTimeLen );
 //! sensor LED normal
 bCapturekeyNormalLED=CAPTUREKEY_LED_NORMAL_COLOR_RESET;
 CapturekeySaveDataByte(CAPTUREKY_SENSOR_LED_NORMAL_COLOR_MEMORY_ADD,bCapturekeyNormalLED);
 //! sensor LED wait for pushing
 bCapturekeyWaitForPushingLED=CAPTUREKEY_LED_WAIT_FOR_PUSHING_COLOR_RESET;
 CapturekeySaveDataByte(CAPTUREKY_SENSOR_LED_WAIT_FOR_PUSHING_KEY_MEMORY_ADD,bCapturekeyWaitForPushingLED);
}
//******************************************************************************
//! \brief 
//******************************************************************************
void CaptureKeyMain( void )
{
  //! Count Secound time of game
  if(iCapturekeyTickBuffer!=SystemTickGetSecond())
    {
      

     iCapturekeyTickBuffer=SystemTickGetSecond();
     if((bCapturekeyMode!=CAPTUREKEY_PAUSE_MODE)&&
        (bCapturekeyMode!=CAPTUREKEY_STOP_MODE)&&
        (bCapturekeyMode!=CAPTUREKEY_PUSHED_KEY_MODE))
       { 
        //! 1S plus to the game time  
        iCapturekeyGameTimeCounter++;
        
        if(iCapturekeyGameTimeCounter>bCapturekeyGameTimeLen*60)
          {
            iCapturekeyGameTimeCounter=0;
            bCapturekeyMode=CAPTUREKEY_STOP_MODE;
          }
       }
     //! when key is pushed       
     if(bCapturekeyMode==CAPTUREKEY_PUSHED_KEY_MODE) 
       {
        if(bCapturekeyKillingCounter<5)
          {
           bCapturekeyRfDataTxArray[0]=CAPTUREKEY_STOP_REQ_CMD;
           if(bCapturekeyFireClashGunID%2==0)
               CapturekeyRfDataSend(CAPTUREKEY_ODD_GAME_ID,1,bCapturekeyRfDataTxArray);
           if(bCapturekeyFireClashGunID%2==1)
               CapturekeyRfDataSend(CAPTUREKEY_EVENT_GAME_ID,1,bCapturekeyRfDataTxArray);
           bCapturekeyKillingCounter++;
          }
        else
          bCapturekeyMode=CAPTUREKEY_STOP_MODE;
       }
     if(bCapturekeyMode== CAPTUREKEY_STOP_MODE)   
       {
        //! reset game time 
        iCapturekeyGameTimeCounter=0;
       }
     
    }
  //! Start mode
  if(bCapturekeyMode==CAPTUREKEY_START_MODE)
    {    
     if(bCapturekeyPuaseFlag!=1)
       {
        GPIO_SetBits( SEGMENT_COM_1_PORT, SEGMENT_COM_1_PIN);
        GPIO_SetBits( SEGMENT_COM_2_PORT, SEGMENT_COM_2_PIN);                    
        GPIO_Write( SEGMENT_PORT,64); 
        UsartSendData(SENSOR_CMD_GET_FIRE );
        UsartSendData(1);
        iCapturekeyTickBuffer=SystemTickGetSecond();
        while(SystemTickGetSecond()-iCapturekeyTickBuffer<1);
        iCapturekeyGameTimeCounter=0;
       }
     bCapturekeyKillingCounter=0;
     UsartRxSetFlag(0);
     bCapturekeyPuaseFlag=0;
     bCapturekeyMode=CAPTUREKEY_NORMAL_MODE;
    }
  if(bCapturekeyMode==CAPTUREKEY_PAUSE_MODE)
    {
      SensorEmptyUsart();
    }
 
}      
//******************************************************************************
//! \brief read capturekey 
//******************************************************************************       
 void CapturekeyCheckKey(void)        
 {
  if(SystemTickGetMsecond()%10==0)
    {
     if(bCapturekeyMode== CAPTUREKEY_WAIT_FOR_PUSHING_KEY_MODE)  
       {
           //! Read capturekey
           if( GPIO_ReadInputDataBit(CaptureKey_GPIO)==0 )
             {
              bCapturekeyMode=CAPTUREKEY_PUSHED_KEY_MODE;     
              DAC_Cmd( DAC_Channel_1, DISABLE );
              TIM3_Cmd( DISABLE );
              GPIO_ResetBits(AUDIO_POWER);
              AudioStartPlay("survivor.wav"); 
             }      
       }
    }         
 }     
//******************************************************************************
//! \brief  save one byte data to memory
//! \param  sAdd : data address
//! \param  bData : data for save
//******************************************************************************       
void CapturekeySaveDataByte( u16 sAdd, u8 bData )
{
    //! disable interrupts
    //disableInterrupts();
    //! flash eeprom unlock
    FLASH_Unlock( FLASH_MemType_Data );
    //! write to eeprom
    FLASH_ProgramByte(sAdd+EEPROM_START_ADD,bData );
    //! flash eeprom lock
    FLASH_Lock( FLASH_MemType_Data );
    //! enable interrupts
    //enableInterrupts();
}
//******************************************************************************
//! \brief  get one byte data of memory
//! \param  sAdd : data address
//! \retval data value
//******************************************************************************
u8 CapturekeyGetDataByte( u16 sAdd )
{
    u8 bTemp;
    FLASH_Unlock( FLASH_MemType_Data );
    bTemp=FLASH_ReadByte( sAdd+EEPROM_START_ADD );    
    FLASH_Lock( FLASH_MemType_Data );
    return(bTemp);
}
//******************************************************************************
//! \brief  check rf data rx
//******************************************************************************
void CapturekeyRfPackCheck( void)
{
    //!   __  ________________  ___________  _________  _________________  _____________________  __
    //!  | *| Destination Add |	Source Add | Data Len |	CMD + Data Array | sum of all data pack | # |
    u8 bDataLen=0;
    u8 bDataSum=0;
    
    //! check rf data rx flag
    if( Mrf24j40GetRxFlag()==1 )
    {        
        //! clear rf data rx flag
        Mrf24j40SetRxFlag(0);
        //! get rf rx data
        Mrf24j40GetRxData(bCapturekeyRfPackRxArray);
        //! read pack data len
        bDataLen=bCapturekeyRfPackRxArray[3];    
        //! check start and end byte
        if( bCapturekeyRfPackRxArray[0]=='*' && bCapturekeyRfPackRxArray[bDataLen+5]=='#' )
          {
            //! claculate data pack check sum
            for( u8 i=0 ; i<bDataLen ; i++ )
                bDataSum+=bCapturekeyRfPackRxArray[i+4];
            //! check check sum
            if( bDataSum==bCapturekeyRfPackRxArray[bDataLen+4] )
              {
               //! check Destination Address
               //! check Destination with capturekey main id
               if( bCapturekeyRfPackRxArray[1]==bCapturekeyMainId )
                 {
                  //! set data rx flag    
                  bCapturekeyRfDataRxFlag=1;
                 }
               if( bCapturekeyRfPackRxArray[1]==CAPTUREKEY_ALL_MAIN_ID )
                 {
                  //! set data rx flag    
                  bCapturekeyRfDataRxFlag=1;
                 }               
               //! check Destination with all capturekey address cmd
               else if( bCapturekeyRfPackRxArray[1]==CAPTUREKEY_ALL_CAPTUREKEY_ID)
                 {
                  //! set data rx flag    
                  bCapturekeyRfDataRxFlag=1;
                 }
              }
          }
        if(bCapturekeyRfDataRxFlag==1)
          {
           //! buffer data rx 
           for( u8 i=0 ; i<bDataLen+3 ; i++ )
           bCapturekeyRfDataRxArray[i]=bCapturekeyRfPackRxArray[i+1];
          }       
    }  
}
//******************************************************************************
//! \brief  check rf data rx
//******************************************************************************
void CapturekeyRfDataCheck( void )
{  
    //!  ________________  ___________ _________  _________________ 
    //! |Destination Add |Source Add | Data Len |CMD + Data Array | 
    u8 bCounterOfPlayers=0;
    u8 bDesAdd=0;
    u8 bSourceAdd=0;
    u8 bCmd=0;
    
    //! check Capturekey rf data flag
    if( bCapturekeyRfDataRxFlag==1 )
      {      
        //! clear Capturekey rf data flag
        bCapturekeyRfDataRxFlag=0;
        //! read destination address
        bDesAdd=bCapturekeyRfDataRxArray[0];
        //! read source address
        bSourceAdd=bCapturekeyRfDataRxArray[1];
        //! read data CMD
        bCmd=bCapturekeyRfDataRxArray[3];
        //! check source address ( main device )
        
           if(bSourceAdd==MASTER)
             {
              //! Ping --------------------------------------      
              if( bCmd==CAPTUREKEY_PING_REQ_CMD)
                {
                 bCapturekeyRfDataTxArray[0]=CAPTUREKEY_PING_RES_CMD;
                 CapturekeyRfDataSend( bSourceAdd, 1, bCapturekeyRfDataTxArray );
                }                      
              //! Start --------------------------------------      
              if( bCmd==CAPTUREKEY_START_REQ_CMD)
                {
                 if(bCapturekeyMode==CAPTUREKEY_PAUSE_MODE)
                   bCapturekeyPuaseFlag=1;
                 bCapturekeyMode=CAPTUREKEY_START_MODE;
                 bCapturekeyRfDataTxArray[0]=CAPTUREKEY_START_RES_CMD;
                 CapturekeyRfDataSend( bSourceAdd, 1, bCapturekeyRfDataTxArray );
                }                       
              //! Pause --------------------------------------      
              if( bCmd==CAPTUREKEY_PAUSE_REQ_CMD)
                {                 
                 if((bCapturekeyMode==CAPTUREKEY_START_MODE)||
                    (bCapturekeyMode==CAPTUREKEY_NORMAL_MODE))
                   {
                    bCapturekeyMode=CAPTUREKEY_PAUSE_MODE;
                    bCapturekeyRfDataTxArray[0]=CAPTUREKEY_PAUSE_RES_CMD;
                    CapturekeyRfDataSend(bSourceAdd,1,bCapturekeyRfDataTxArray);
                   }
                }                   
              //! Stop --------------------------------------      
              if( bCmd==CAPTUREKEY_STOP_REQ_CMD)
                {
                 bCapturekeyMode=CAPTUREKEY_STOP_MODE;
                 bCapturekeyRfDataTxArray[0]=CAPTUREKEY_STOP_RES_CMD;
                 CapturekeyRfDataSend(bSourceAdd,1,bCapturekeyRfDataTxArray);
                }
              //! Reset setting --------------------------------------      
              if( bCmd==CAPTUREKEY_SET_RESET_SETTING_REQ_CMD)
                {
                 bCapturekeyRfDataTxArray[0]=CAPTUREKEY_SET_RESET_SETTING_RES_CMD;
                 CaptureKeyResetSetting();
                 CapturekeyRfDataSend(bSourceAdd,1,bCapturekeyRfDataTxArray);
                }
              //! Set ID----------------------------------------------
              if(bCmd==CAPTUREKEY_SET_ID_REQ_CMD)
                {
                 bCapturekeyMainId=bCapturekeyRfDataRxArray[4];
                 CapturekeySaveDataByte( CAPTUREKEY_ID_MEMORY_ADD, bCapturekeyMainId );                   
                 bCapturekeyRfDataTxArray[0]=CAPTUREKEY_SET_ID_RES_CMD;
                 CapturekeyRfDataSend(bSourceAdd,1,bCapturekeyRfDataTxArray); 
                }
              //! set time length--------------------------------------
              if(bCmd==CAPTUREKEY_SET_TIME_LENGTH_REQ_CMD)
                {
                 bCapturekeyGameTimeLen=bCapturekeyRfDataRxArray[4];
                 CapturekeySaveDataByte( CAPTUREKEY_GAME_TIME_LENGTH_MEMORY_ADD, bCapturekeyGameTimeLen );
                 bCapturekeyRfDataTxArray[0]=CAPTUREKEY_SET_TIME_LENGTH_RES_CMD;
                 CapturekeyRfDataSend(bSourceAdd,1,bCapturekeyRfDataTxArray); 
                }
              //! set setting --------------------------------------      
              if(bCmd==CAPTUREKEY_SET_SENSOR_SETTING_REQ_CMD)
                {
                 bCapturekeyNormalLED=bCapturekeyRfDataRxArray[4];
                 CapturekeySaveDataByte(CAPTUREKY_SENSOR_LED_NORMAL_COLOR_MEMORY_ADD,bCapturekeyNormalLED);
                 bCapturekeyWaitForPushingLED=bCapturekeyRfDataRxArray[5];                  
                 CapturekeySaveDataByte(CAPTUREKY_SENSOR_LED_WAIT_FOR_PUSHING_KEY_MEMORY_ADD,bCapturekeyWaitForPushingLED); 
                 bCapturekeyRfDataTxArray[0]=CAPTUREKEY_SET_SENSOR_SETTING_RES_CMD;
                 CapturekeyRfDataSend(bSourceAdd,1,bCapturekeyRfDataTxArray);                   
                }
              //! get setting --------------------------------------      
              if(bCmd==CAPTUREKEY_GET_SETTING_REQ_CMD )
                {
                 bCapturekeyRfDataTxArray[0]=CAPTUREKEY_GET_SETTING_RES_CMD;
                 bCapturekeyRfDataTxArray[1]=bCapturekeyMainId;
                 bCapturekeyRfDataTxArray[2]=bCapturekeyGameTimeLen;
                 bCapturekeyRfDataTxArray[3]=bCapturekeyNormalLED;
                 bCapturekeyRfDataTxArray[4]=bCapturekeyWaitForPushingLED;
                 CapturekeyRfDataSend(bSourceAdd,5,bCapturekeyRfDataTxArray);                           
                }
              //! Arrengment setting --------------------------------------      
              if(bCmd==CAPTUREKEY_ARRENGMENT_REQ_CMD)
                {              
                  //!  |B4       |B5        |B6                  |B7                  |...|B15       |B16                 |...                
                  //!  |N of tems|N of team0|ID of player 0 team0|ID of player 1 team0|...|N of team1|ID of player 0 team1|...
                  bCapturekeyNumberOfTeams=bCapturekeyRfDataRxArray[4];                 
                  for(u8 i=0;i<bCapturekeyNumberOfTeams;i++)
                     {
                      bCapturekeyNumberOfPlayers[i][0]=bCapturekeyRfDataRxArray[(i*CAPTUREKEY_MAX_OF_PLAYERS)+5];                   
                      for(u8 j=1;j<bCapturekeyNumberOfPlayers[i][0];j++)
                         {
                           bCapturekeyNumberOfPlayers[i][j]=bCapturekeyRfDataRxArray[(i*CAPTUREKEY_MAX_OF_PLAYERS)+j+5];
                         }
                     }
                 bCapturekeyRfDataTxArray[0]=CAPTUREKEY_ARRENGMENT_RES_CMD;
                 CapturekeyRfDataSend(bSourceAdd,1,bCapturekeyRfDataTxArray);                 
                }
             }
      }
}
//******************************************************************************
//! \brief  check usart data rx
//******************************************************************************
void CapturekeyUsartDataCheck( void )
{
    //!  ------------- 
    //! | fire Gun Id | sensor ID    
    u8 bUsartDataRx[10];
    //! check usart rx data flag
    if( UsartRxGetFlag()==1 )
      {
        //! clear usart rx data flag
        UsartRxSetFlag(0);
        //! get usart rx data
        UsartRxGetData(bUsartDataRx);
        //! just check in normal mode
//        if((bCapturekeyMode==CAPTUREKEY_NORMAL_MODE)||
//           (bCapturekeyMode==CAPTUREKEY_WAIT_FOR_PUSHING_KEY_MODE)) 
//          {            
            bCapturekeyFireClashGunID=bUsartDataRx[1];
                                      UsartSendData(SENSOR_CMD_LED_SET);
                                      UsartSendData(4);
                                      UsartSendData(1);
                                      UsartSendData(1);

            //! change mode 
            bCapturekeyMode= CAPTUREKEY_WAIT_FOR_PUSHING_KEY_MODE;
//          }                             
     }
  
}
//******************************************************************************
//! \brief  reload memory data
//******************************************************************************
void CapturekeyMemoryReload( void )
{
    u8 bReloadCheck;
  
//    bReloadCheck=CapturekeyGetDataByte( CAPTUREKEY_RELOAD_CHECK_MEMORY_ADD );  
    //! reload data 
//    if( bReloadCheck==CAPTUREKEY_RELOAD_CHECK_MEMORY_BYTE )
//    {
      //! reload Capturekey id
      bCapturekeyMainId=CapturekeyGetDataByte( CAPTUREKEY_ID_MEMORY_ADD);     
      //! reload Capturekey time len
      bCapturekeyGameTimeLen=CapturekeyGetDataByte( CAPTUREKEY_GAME_TIME_LENGTH_MEMORY_ADD );        
      //! reload sensor led color normal
      bCapturekeyNormalLED=CapturekeyGetDataByte( CAPTUREKY_SENSOR_LED_NORMAL_COLOR_MEMORY_ADD );
      //! reload sensor led color wait for pushing
      bCapturekeyWaitForPushingLED=CapturekeyGetDataByte( CAPTUREKY_SENSOR_LED_WAIT_FOR_PUSHING_KEY_MEMORY_ADD );
//    }
/*    else
    {
      CapturekeySaveDataByte( CAPTUREKEY_RELOAD_CHECK_MEMORY_ADD,CAPTUREKEY_RELOAD_CHECK_MEMORY_BYTE );
       
      CapturekeySaveDataByte( CAPTUREKEY_ID_MEMORY_ADD, bCapturekeyMainId );
      
      CapturekeySaveDataByte( CAPTUREKEY_GAME_TIME_LENGTH_MEMORY_ADD, bCapturekeyGameTimeLen );
      
      CapturekeySaveDataByte( CAPTUREKY_SENSOR_LED_NORMAL_COLOR_MEMORY_ADD,SensorGetLedOption(SENSOR_LED_NORMAL));
                              
      CapturekeySaveDataByte( CAPTUREKY_SENSOR_LED_WAIT_FOR_PUSHING_KEY_MEMORY_ADD,SensorGetLedOption(SENSOR_LED_WAIT_FOR_PUSHING));
    }*/
}
//******************************************************************************
//! \brief  send data with RF module
//! \param  bDesAdd : destination address
//! \param  bDataLen : data length
//! \param  bDataArray : data array
//******************************************************************************
void CapturekeyRfDataSend( u8 bDesAdd, u8 bDataLen, u8* bDataArray )
{
    //!   __  ________________  ___________  _________  _________________  _____________________  __
    //!  |*| Destination Add |	Source Add | Data Len |	CMD + Data Array | sum of all data pack | # |
    
    u8 bDataSend[130];
    u8 bDataSum=0;
    
    bDataSend[0]='*';
    bDataSend[1]=bDesAdd;
    bDataSend[2]=bCapturekeyMainId;
    bDataSend[3]=bDataLen;
    
    for( u8 i=0 ; i<bDataLen ; i++ )
    {
        bDataSend[i+4]=bDataArray[i];
        bDataSum+=bDataArray[i];
    }
    
    bDataSend[bDataLen+4]=bDataSum;
    bDataSend[bDataLen+5]='#';
    
    Mrf24j40SendData( bDataSend, bDataLen+6+4 );
}
//******************************************************************************
//! \brief  get capturekey work mode
//! \retval capturekey work mode
//******************************************************************************
u8 CapturekeyGetWorkMode( void )
{
    return bCapturekeyMode;
}
//******************************************************************************
//! \brief  Capturekey get game ID
//! \retval Capturekey game ID
//******************************************************************************
u8 CapturekeyGetMainId( void )
{
    return bCapturekeyMainId;
}
//******************************************************************************
//! \brief  Capturekey get Firer ID
//! \retval Capturekey Firer ID
//******************************************************************************
u8 CapturekeyGetFireClashGunID(void)
{
  return(bCapturekeyFireClashGunID);
}
//******************************************************************************
//! \brief  Capturekey Normal LED
//! \retval
//******************************************************************************
u8 CapturekeyGetNormalLED(void)
{
  return(bCapturekeyNormalLED);
}
//******************************************************************************
//! \brief  Capturekey Normal LED
//! \retval
//******************************************************************************
u8 CapturekeyGetWaitForPushingLED()
{
  return(bCapturekeyWaitForPushingLED);
}
