#include "..\Include\vest.h"

//******************************************************************************
//! Variable -------------------------------------------------------------------
u8  bVestMode=0;
u8  bVestMainId=VEST_ID_RESET;
u8  bVestGameId=VEST_ID_RESET/2+1;
u8  bVestGameTimeLen=VEST_GAME_TIME_RESET;

u8  bVestFireDataLen=0;
u8  bVestFireDataArray[1000];

u8  bVestRfPackRxArray[50];
u8  bVestRfDataRxArray[50];
u8  bVestRfDataRxFlag=0;

u8  bVestRfDataTxArray[200];

u16 sVestGameTimeCounter=0;

u32 iVestTickBuffer=0;
//******************************************************************************
//! \brief  vest init
//******************************************************************************
void VestInit( void )
{
    
}
//******************************************************************************
//! \brief  vest main control
//******************************************************************************
void VestMain( void )
{
    //! manage game time -----------------------------------
    //! check vest work mode
    if( bVestMode==VEST_START_MODE || bVestMode==VEST_START_SPECIFIC_MODE || bVestMode==VEST_PAUSE_SPECIFIC_MODE )
    {
        //! check tick time
        if( iVestTickBuffer != SystemTickGetSecond() )
        {
            //! update vest tick time 
            iVestTickBuffer=SystemTickGetSecond();
            //! set vest time counter
            sVestGameTimeCounter++;
            //! check vest game time length
            if( sVestGameTimeCounter>bVestGameTimeLen*60 )
            {
                bVestMode=VEST_STOP_MODE;
                
                sVestGameTimeCounter=0;
                
                //! set sensor led mode
                SensorSetLedMode( SENSOR_LED_OFF );
            }
        }
    }
    else if( bVestMode==VEST_STOP_MODE || bVestMode==VEST_STOP_SPECIFIC_MODE )
    {
        sVestGameTimeCounter=0;
    }
}
//******************************************************************************
//! \brief  save one byte data to memory
//! \param  sAdd : data address
//! \param  bData : data for save
//******************************************************************************
void VestSaveDataByte( u16 sAdd, u8 bData )
{
    //! disable interrupts
    //disableInterrupts();
    //! flash eeprom unlock
    FLASH_Unlock( FLASH_MemType_Data );
    //! write to eeprom
    FLASH_ProgramByte( sAdd+EEPROM_START_ADD   , bData );  
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
u8 VestGetDataByte( u16 sAdd )
{
    return FLASH_ReadByte( sAdd+EEPROM_START_ADD );
}
//******************************************************************************
//! \brief  reload memory data
//******************************************************************************
void VestMemoryReload( void )
{
    u8 bReloadCheck = VestGetDataByte( VEST_RELOAD_CHECK_MEMORY_ADD );
    u8 bDataBuffer=0;
    
    //! reload data 
    if( bReloadCheck==VEST_RELOAD_CHECK_MEMORY_BYTE )
    {
        //! reload vest id
        bDataBuffer=VestGetDataByte( VEST_ID_MEMORY_ADD ); 
        if( bDataBuffer==0 || bDataBuffer%2==0 )
            VestSaveDataByte( VEST_ID_MEMORY_ADD, bVestMainId );
        else
            bVestMainId=bDataBuffer;
        //! calculate game id of vest id
        bVestGameId=bVestMainId/2+1;
        
        //! reload led normal color
        SensorSetLedOption( SENSOR_LED_COLOR_NORMAL, VestGetDataByte( VEST_LED_NORMAL_COLOR_MEMORY_ADD ) );
        //! reload led normal on time
        SensorSetLedOption( SENSOR_LED_ON_TIME_NORMAL, VestGetDataByte( VEST_LED_NORMAL_ON_TIME_MEMORY_ADD ) ); 
        //! reload led normal off time
        SensorSetLedOption( SENSOR_LED_OFF_TIME_NORMAL, VestGetDataByte( VEST_LED_NORMAL_OFF_TIME_MEMORY_ADD ) );
        
        //! reload led danger color
        SensorSetLedOption( SENSOR_LED_COLOR_DANGER, VestGetDataByte( VEST_LED_DANGER_COLOR_MEMORY_ADD ) );
        //! reload led danger on time
        SensorSetLedOption( SENSOR_LED_ON_TIME_DANGER, VestGetDataByte( VEST_LED_DANGER_ON_TIME_MEMORY_ADD ) );
        //! reload led danger off time
        SensorSetLedOption( SENSOR_LED_OFF_TIME_DANGER, VestGetDataByte( VEST_LED_DANGER_OFF_TIME_MEMORY_ADD ) );   

        //! reload game time len
        bVestGameTimeLen=VestGetDataByte( VEST_GAME_TIME_LENGTH_MEMORY_ADD );
        
        //! reload fire clash data len
        //bVestFireDataLen=VestGetDataByte( VEST_FIRE_DATA_LEN_MEMORY_ADD );
    }
    //! first program default variable value save to memory
    else
    {
        //! save vest id default value
        VestSaveDataByte( VEST_ID_MEMORY_ADD, bVestMainId );
        //! calculate game id of vest id
        bVestGameId=bVestMainId/2+1;
        
        //! save led normal color default value
        VestSaveDataByte( VEST_LED_NORMAL_COLOR_MEMORY_ADD, SensorGetLedOption(SENSOR_LED_COLOR_NORMAL) );
        //! save led normal on time default value
        VestSaveDataByte( VEST_LED_NORMAL_ON_TIME_MEMORY_ADD, SensorGetLedOption(SENSOR_LED_ON_TIME_NORMAL) );
        //! save led normal off time default value
        VestSaveDataByte( VEST_LED_NORMAL_OFF_TIME_MEMORY_ADD, SensorGetLedOption(SENSOR_LED_OFF_TIME_NORMAL) );
        
        //! save led danger color default value
        VestSaveDataByte( VEST_LED_DANGER_COLOR_MEMORY_ADD, SensorGetLedOption(SENSOR_LED_COLOR_DANGER) );
        //! save led danger on time default value
        VestSaveDataByte( VEST_LED_DANGER_ON_TIME_MEMORY_ADD, SensorGetLedOption(SENSOR_LED_ON_TIME_DANGER) );
        //! save led danger off time default value
        VestSaveDataByte( VEST_LED_DANGER_OFF_TIME_MEMORY_ADD, SensorGetLedOption(SENSOR_LED_OFF_TIME_DANGER) );
        
        //! save game time min default value
        VestSaveDataByte( VEST_GAME_TIME_LENGTH_MEMORY_ADD, bVestGameTimeLen );
        
        //! save fire clash data len
        //VestSaveDataByte( VEST_FIRE_DATA_LEN_MEMORY_ADD, bVestFireDataLen );
        
        VestSaveDataByte( VEST_RELOAD_CHECK_MEMORY_ADD, VEST_RELOAD_CHECK_MEMORY_BYTE );
    }
}
//******************************************************************************
//! \brief  save fire data in memory
//! \param  bSensorId : sensor get gun fire data id
//! \param  bGunId : gun fire id
//******************************************************************************
void VestFireDataSave( u8 bSensorId, u8 bGunId )
{
    /*
    //! get fire data len value from eeprom
    bVestFireDataLen=VestGetDataByte( VEST_FIRE_DATA_LEN_MEMORY_ADD );
    //! save fire clash data to eeprom
    VestSaveDataByte( VEST_FIRE_DATA_START_MEMORY_ADD+(bVestFireDataLen*4)+0, bSensorId ); 
    VestSaveDataByte( VEST_FIRE_DATA_START_MEMORY_ADD+(bVestFireDataLen*4)+1, bGunId );
    VestSaveDataByte( VEST_FIRE_DATA_START_MEMORY_ADD+(bVestFireDataLen*4)+2, sVestGameTimeCounter/60 );
    VestSaveDataByte( VEST_FIRE_DATA_START_MEMORY_ADD+(bVestFireDataLen*4)+3, sVestGameTimeCounter%60 );     
    //! calculate fire clash data
    bVestFireDataLen++;
    //! save fire clash data counter to eeprom
    VestSaveDataByte( VEST_FIRE_DATA_LEN_MEMORY_ADD, bVestFireDataLen );
    */
    
    //! save fire clash data to fire clash data array
    bVestFireDataArray[bVestFireDataLen*4+0] = bSensorId; 
    bVestFireDataArray[bVestFireDataLen*4+1] = bGunId;
    bVestFireDataArray[bVestFireDataLen*4+2] = sVestGameTimeCounter/60;
    bVestFireDataArray[bVestFireDataLen*4+3] = sVestGameTimeCounter%60;     
    //! calculate fire clash data
    bVestFireDataLen++;
}
//******************************************************************************
//! \brief  send fire clash data to main board
//! \param  bStartPack : start pack number
//! \param  bEndPack : end pack number
//******************************************************************************
void VestFireDataSend( u8 bStartPack, u8 bEndPack )
{
    u8 bDatalength=0;
    u8 bRealStartPack=0;
    u8 bRealEndPack=0;

    //! get fire data len value from eeprom
    //bVestFireDataLen=VestGetDataByte( VEST_FIRE_DATA_LEN_MEMORY_ADD );
    
    //! check Start pack
    if( bStartPack==0 )
        bRealStartPack=1;
    else if( bStartPack>bVestFireDataLen )
        bRealStartPack=1;
    else
        bRealStartPack=bStartPack;
    
    //! check end pack
    if( bEndPack>bVestFireDataLen )
        bRealEndPack=bVestFireDataLen;
    else
        bRealEndPack=bEndPack;
    
    if( bRealEndPack-bRealStartPack>25 )
        bRealEndPack=bRealStartPack+25;
    
    bVestRfDataTxArray[0]=VEST_GET_FIRE_DATA_RES_CMD;
    bVestRfDataTxArray[1]=bVestFireDataLen;
    bVestRfDataTxArray[2]=bRealStartPack;
    bVestRfDataTxArray[3]=bRealEndPack;
    
    for( u8 i=bRealStartPack ; i<=bRealEndPack ; i++ )
    {
        //bVestRfDataTxArray[ (i-bRealStartPack)*4+4 ] = VestGetDataByte( VEST_FIRE_DATA_START_MEMORY_ADD+(i*4)+0 );
        //bVestRfDataTxArray[ (i-bRealStartPack)*4+5 ] = VestGetDataByte( VEST_FIRE_DATA_START_MEMORY_ADD+(i*4)+1 );
        //bVestRfDataTxArray[ (i-bRealStartPack)*4+6 ] = VestGetDataByte( VEST_FIRE_DATA_START_MEMORY_ADD+(i*4)+2 );
        //bVestRfDataTxArray[ (i-bRealStartPack)*4+7 ] = VestGetDataByte( VEST_FIRE_DATA_START_MEMORY_ADD+(i*4)+3 );
        
        bVestRfDataTxArray[ (i-bRealStartPack)*4+4 ] = bVestFireDataArray[(i-1)*4+0];
        bVestRfDataTxArray[ (i-bRealStartPack)*4+5 ] = bVestFireDataArray[(i-1)*4+1];
        bVestRfDataTxArray[ (i-bRealStartPack)*4+6 ] = bVestFireDataArray[(i-1)*4+2];
        bVestRfDataTxArray[ (i-bRealStartPack)*4+7 ] = bVestFireDataArray[(i-1)*4+3];
    }
    
    bDatalength=(bRealEndPack-bRealStartPack+1)*4+4;
    
    VestRfDataSend( 0, bDatalength, bVestRfDataTxArray );
}
//******************************************************************************
//! \brief  check usart data rx
//******************************************************************************
void VestUsartDataCheck( void )
{
    //!   ---------------------  ------------
    //! | fire clash Sensor Id | fire Gun Id | 
    
    u8 bUsartDataRx[10];
    
    //! check usart rx data flag
    if( UsartRxGetFlag()==1 )
    {
        //! clear usart rx data flag
        UsartRxSetFlag(0);
        //! get usart rx data
        UsartRxGetData(bUsartDataRx);
        
        //! check vest work mode ( stop=0, start=1, pause=2 )
        if( bVestMode==VEST_START_MODE || bVestMode==VEST_START_SPECIFIC_MODE )
        {
            //! set sensor led to danger mode
            SensorSetLedMode(SENSOR_LED_DANGER);
            //! play danger audio
            AudioDangerPlay();
            //! set vibre
            VibreSetStatus(1);
            
            //! send report to gun
            bVestRfDataTxArray[0]=VEST_FIRE_CLASH_REPORT_CMD;
            bVestRfDataTxArray[1]=bUsartDataRx[1];
            VestRfDataSend( bVestMainId+1, 2, bVestRfDataTxArray );
            
            //! save fire data 
            VestFireDataSave( bUsartDataRx[0], bUsartDataRx[1] );
        }
    }
}
//******************************************************************************
//! \brief  check rf data rx
//******************************************************************************
void VestRfPackCheck( void )
{   
    //!   __  ________________  ___________  _________  _________________  _____________________  __
    //! | *	| Destination Add |	Source Add | Data Len |	CMD + Data Array | sum of all data pack | # |

    u8 bDataLen=0;
    u8 bDataSum=0;
    
    //! check rf data rx flag
    if( Mrf24j40GetRxFlag()==1 )
    {
        //! clear rf data rx flag
        Mrf24j40SetRxFlag(0);
        //! get rf rx data
        Mrf24j40GetRxData(bVestRfPackRxArray);
        //! read pack data len
        bDataLen=bVestRfPackRxArray[3];
    
        //! check start and end byte
        if( bVestRfPackRxArray[0]=='*' && bVestRfPackRxArray[bDataLen+5]=='#' )
        {
            //! claculate data pack check sum
            for( u8 i=0 ; i<bDataLen ; i++ )
                bDataSum+=bVestRfPackRxArray[i+4];
            //! check check sum
            if( bDataSum==bVestRfPackRxArray[bDataLen+4] )
            {
                //! check Destination Address
                
                //! check Destination with vest main id
                if( bVestRfPackRxArray[1]==bVestMainId )
                {
                    //! set data rx flag    
                    bVestRfDataRxFlag=1;
                }
                //! check Destination with all address cmd
                else if( bVestRfPackRxArray[1]==VEST_ALL_MAIN_ID )
                {
                   //! set data rx flag    
                    bVestRfDataRxFlag=1; 
                }
                //! check Destination with all vest device
                else if( bVestRfPackRxArray[1]==VEST_OOD_MAIN_ID ) 
                {
                    //! set data rx flag    
                    bVestRfDataRxFlag=1;
                }
                //! check Destination with all vest and gun event ID
                else if( bVestRfPackRxArray[1]==VEST_EVENT_GAME_ID && bVestGameId%2==0 )
                {
                    //! set data rx flag    
                    bVestRfDataRxFlag=1;
                } 
                //! check Destination with all vest and gun ood ID
                else if( bVestRfPackRxArray[1]==VEST_OOD_GAME_ID && bVestGameId%2==1 ) 
                {
                    //! set data rx flag    
                    bVestRfDataRxFlag=1;
                }
                //! check Destination with vest game id
                else if( (bVestRfPackRxArray[1]&0xC0)==0x80 && (bVestRfPackRxArray[1]&0x3F)==bVestGameId ) 
                {
                    //! set data rx flag    
                    bVestRfDataRxFlag=1;
                }
                
                if( bVestRfDataRxFlag==1 )
                {
                    //! buffer data rx 
                    for( u8 i=0 ; i<bDataLen+3 ; i++ )
                        bVestRfDataRxArray[i]=bVestRfPackRxArray[i+1];
                }
            }
        }
    }
    
    
}
//******************************************************************************
//! \brief  check rf data rx
//******************************************************************************
void VestRfDataCheck( void )
{   
    //!   ________________  ___________  _________  _________________ 
    //! | Destination Add |	Source Add | Data Len |	CMD + Data Array | 
    
    u8 bDesAdd=0;
    u8 bSourceAdd=0;
    u8 bCmd=0;
    
    //! check vest rf data flag
    if( bVestRfDataRxFlag==1 )
    {
        //! clear vest rf data flag
        bVestRfDataRxFlag=0;
        //! read destination address
        bDesAdd=bVestRfDataRxArray[0];
        //! read source address
        bSourceAdd=bVestRfDataRxArray[1];
        //! read data CMD
        bCmd=bVestRfDataRxArray[3];
        
        //! check source address ( main device )
        if( bSourceAdd==0 )
        {
            //! ping ---------------------------------------
            if( bCmd==VEST_PING_REQ_CMD )
            {
                //! check destination address for select specific device
                if( bDesAdd==bVestMainId )
                {
                    //! set data for response
                    bVestRfDataTxArray[0]=VEST_PING_RES_CMD;
                    //! send data with rf module
                    VestRfDataSend( bSourceAdd, 1, bVestRfDataTxArray );
                }
            }
            //! get setting --------------------------------
            else if( bCmd==VEST_GET_SETTING_REQ_CMD )
            {
                //! check destination address for select specific device
                if( bDesAdd==bVestMainId )
                {
                    //! set data for response
                    bVestRfDataTxArray[0]=VEST_GET_SETTING_RES_CMD;
                    bVestRfDataTxArray[1]=SensorGetLedOption(SENSOR_LED_COLOR_NORMAL);
                    bVestRfDataTxArray[2]=SensorGetLedOption(SENSOR_LED_ON_TIME_NORMAL);
                    bVestRfDataTxArray[3]=SensorGetLedOption(SENSOR_LED_OFF_TIME_NORMAL);
                    bVestRfDataTxArray[4]=SensorGetLedOption(SENSOR_LED_COLOR_DANGER);
                    bVestRfDataTxArray[5]=SensorGetLedOption(SENSOR_LED_ON_TIME_DANGER);
                    bVestRfDataTxArray[6]=SensorGetLedOption(SENSOR_LED_OFF_TIME_DANGER);
                    bVestRfDataTxArray[7]=bVestGameTimeLen;
                    bVestRfDataTxArray[8]=0;
                    //! send data with rf module
                    VestRfDataSend( bSourceAdd, 9, bVestRfDataTxArray );
                }
            }
            //! set ID -------------------------------------
            else if( bCmd==VEST_SET_ID_REQ_CMD )
            {
                //! check destination address for select specific device
                if( bDesAdd==bVestMainId && bVestRfDataRxArray[4]%2==1 )
                {
                    //! set vest id
                    bVestMainId=bVestRfDataRxArray[4];
                    //! calculate game id of vest id
                    bVestGameId=bVestMainId/2+1;
                    //! save vest id to eeprom
                    VestSaveDataByte( VEST_ID_MEMORY_ADD, bVestMainId );
                
                    //! set data for response
                    bVestRfDataTxArray[0]=VEST_SET_ID_RES_CMD;
                    //! send data with rf module
                    VestRfDataSend( bSourceAdd, 1, bVestRfDataTxArray );
                }
            }
            //! set led normal mode ------------------------
            else if( bCmd==VEST_SET_LED_NORMAL_REQ_CMD )
            {
                //! set vest sensor led normal color
                SensorSetLedOption( SENSOR_LED_COLOR_NORMAL, bVestRfDataRxArray[4] );
                //! save vest sensor led normal color to eeprom
                VestSaveDataByte( VEST_LED_NORMAL_COLOR_MEMORY_ADD, bVestRfDataRxArray[4] );
                
                //! set vest sensor led normal on time
                SensorSetLedOption( SENSOR_LED_ON_TIME_NORMAL, bVestRfDataRxArray[5] );
                //! save vest sensor led normal on time to eeprom
                VestSaveDataByte( VEST_LED_NORMAL_ON_TIME_MEMORY_ADD, bVestRfDataRxArray[5] );
                
                //! set vest sensor led normal off time
                SensorSetLedOption( SENSOR_LED_OFF_TIME_NORMAL, bVestRfDataRxArray[6] );
                //! save vest sensor led normal off time to eeprom
                VestSaveDataByte( VEST_LED_NORMAL_OFF_TIME_MEMORY_ADD, bVestRfDataRxArray[6] );
                
                //! check destination address for select specific device
                if( bDesAdd==bVestMainId )
                {
                    //! set data for response
                    bVestRfDataTxArray[0]=VEST_SET_LED_NORMAL_RES_CMD;
                    //! send data with rf module
                    VestRfDataSend( bSourceAdd, 1, bVestRfDataTxArray );
                }
            }
            //! set led danger mode ------------------------
            else if( bCmd==VEST_SET_LED_DANGER_REQ_CMD )
            {
                //! set vest sensor led danger color
                SensorSetLedOption( SENSOR_LED_COLOR_DANGER, bVestRfDataRxArray[4] );
                //! save vest sensor led danger color to eeprom
                VestSaveDataByte( VEST_LED_DANGER_COLOR_MEMORY_ADD, bVestRfDataRxArray[4] );
                
                //! set vest sensor led danger on time
                SensorSetLedOption( SENSOR_LED_ON_TIME_DANGER, bVestRfDataRxArray[5] );
                //! save vest sensor led danger on time to eeprom
                VestSaveDataByte( VEST_LED_DANGER_ON_TIME_MEMORY_ADD, bVestRfDataRxArray[5] );
                
                //! set vest sensor led danger off time
                SensorSetLedOption( SENSOR_LED_OFF_TIME_DANGER, bVestRfDataRxArray[6] );
                //! save vest sensor led danger off time to eeprom
                VestSaveDataByte( VEST_LED_DANGER_OFF_TIME_MEMORY_ADD, bVestRfDataRxArray[6] );
            
                //! check destination address for select specific device
                if( bDesAdd==bVestMainId )
                {
                    //! set data for response
                    bVestRfDataTxArray[0]=VEST_SET_LED_DANGER_RES_CMD;
                    //! send data with rf module
                    VestRfDataSend( bSourceAdd, 1, bVestRfDataTxArray );
                }
            }
            //! set game time lengh ------------------------
            else if( bCmd==VEST_SET_GAME_TIME_REQ_CMD )
            {
                //! set vest game time length
                bVestGameTimeLen=bVestRfDataRxArray[4];
                //! save vest game time len to eeprom
                VestSaveDataByte( VEST_GAME_TIME_LENGTH_MEMORY_ADD, bVestRfDataRxArray[4] );
            
                //! check destination address for select specific device
                if( bDesAdd==bVestMainId )
                {
                    //! set data for response
                    bVestRfDataTxArray[0]=VEST_SET_GAME_TIME_RES_CMD;
                    //! send data with rf module
                    VestRfDataSend( bSourceAdd, 1, bVestRfDataTxArray );
                }
            }
            //! reset vest all setting ---------------------
            else if( bCmd==VEST_RESET_ALL_SETTING_REQ_CMD )
            {
                //! check destination address for select specific device
                if( bDesAdd==bVestMainId )
                {
                    //! reset all setting
                    VestResetSetting();
                
                    //! set data for response
                    bVestRfDataTxArray[0]=VEST_RESET_ALL_SETTING_RES_CMD;
                    //! send data with rf module
                    VestRfDataSend( bSourceAdd, 1, bVestRfDataTxArray );
                }
                else
                {
                    //! reset all setting
                    VestResetSetting();
                }
            }
            //! start game ---------------------------------
            else if( bCmd==VEST_START_GAME_REQ_CMD )
            {
                //! set vest mode
                if( bDesAdd==VEST_ALL_MAIN_ID )
                {
                    bVestMode=VEST_START_MODE;
                    
                    //! clear fire clash data
                    bVestFireDataLen=0;
                }
                else
                    bVestMode=VEST_START_SPECIFIC_MODE;
                
                //! set sensor led mode
                SensorSetLedMode( SENSOR_LED_NORMAL );
                
                //! check destination address for select specific device
                if( bDesAdd==bVestMainId )
                {
                    //! set data for response
                    bVestRfDataTxArray[0]=VEST_START_GAME_RES_CMD;
                    //! send data with rf module
                    VestRfDataSend( bSourceAdd, 1, bVestRfDataTxArray );
                }
            }
            //! pause game ---------------------------------
            else if( bCmd==VEST_PAUSE_GAME_REQ_CMD )
            {
                //! set vest mode
                if( bDesAdd==VEST_ALL_MAIN_ID )
                    bVestMode=VEST_PAUSE_MODE;
                else
                    bVestMode=VEST_PAUSE_SPECIFIC_MODE;
                
                //! set sensor led mode
                SensorSetLedMode( SENSOR_LED_OFF );
                
                //! check destination address for select specific device
                if( bDesAdd==bVestMainId )
                {
                    //! set data for response
                    bVestRfDataTxArray[0]=VEST_PAUSE_GAME_RES_CMD;
                    //! send data with rf module
                    VestRfDataSend( bSourceAdd, 1, bVestRfDataTxArray );
                }
            }
            //! stop game ---------------------------------
            else if( bCmd==VEST_STOP_GAME_REQ_CMD )
            {
                //! set vest mode
                if( bDesAdd==VEST_ALL_MAIN_ID )
                    bVestMode=VEST_STOP_MODE;
                else
                    bVestMode=VEST_STOP_SPECIFIC_MODE;
                
                //! set sensor led mode
                SensorSetLedMode( SENSOR_LED_OFF );
                
                //! check destination address for select specific device
                if( bDesAdd==bVestMainId )
                {
                    //! set data for response
                    bVestRfDataTxArray[0]=VEST_STOP_GAME_RES_CMD;
                    //! send data with rf module
                    VestRfDataSend( bSourceAdd, 1, bVestRfDataTxArray );
                }
            }
            //! get fire data ------------------------------
            else if( bCmd==VEST_GET_FIRE_DATA_REQ_CMD )
            {
                //! check destination address for select specific device
                if( bDesAdd==bVestMainId || bDesAdd==(bVestGameId|0x80) )
                {
                    //! send fire clash data
                    VestFireDataSend( bVestRfDataRxArray[4], bVestRfDataRxArray[5] );
                }
            }
            //! clear data ---------------------------------
            else if( bCmd==VEST_CLEAR_DATA_REQ_CMD )
            {
                //! clear fire clash data
                bVestFireDataLen=0;
                
                //! check destination address for select specific device
                if( bDesAdd==bVestMainId )
                {
                    //! set data for response
                    bVestRfDataTxArray[0]=VEST_CLEAR_DATA_RES_CMD;
                    //! send data with rf module
                    VestRfDataSend( bSourceAdd, 1, bVestRfDataTxArray );
                }
            }
            //! --------------------------------------------
        }
        //! check source address ( gun device )
        else if( bSourceAdd==bVestMainId+1 )
        {
            //! fire clash report --------------------------
            if( bCmd==VEST_GUN_FIRE_CLASH_REPORT_CMD )
            {
                //! check vest work mode ( stop=0, start=1, pause=2 )
                if( bVestMode==VEST_START_MODE || bVestMode==VEST_START_SPECIFIC_MODE )
                {
                    //! set sensor led to danger mode
                    SensorSetLedMode(SENSOR_LED_DANGER);
            
                    //! save fire data 
                    VestFireDataSave( 8, bVestRfDataRxArray[4] );
                }
            }
        }
    }
}
//******************************************************************************
//! \brief  send data with RF module
//! \param  bDesAdd : destination address
//! \param  bDataLen : data length
//! \param  bDataArray : data array
//******************************************************************************
void VestRfDataSend( u8 bDesAdd, u8 bDataLen, u8* bDataArray )
{
    //!   __  ________________  ___________  _________  _________________  _____________________  __
    //! | *	| Destination Add |	Source Add | Data Len |	CMD + Data Array | sum of all data pack | # |
    
    u8 bDataSend[130];
    u8 bDataSum=0;
    
    bDataSend[0]='*';
    bDataSend[1]=bDesAdd;
    bDataSend[2]=bVestMainId;
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
//! \brief  vest reset all setting
//******************************************************************************
void VestResetSetting( void )
{
    //! reset vest id 
    bVestMainId=VEST_ID_RESET;
    bVestGameId=VEST_ID_RESET/2+1;
    VestSaveDataByte( VEST_ID_MEMORY_ADD, bVestMainId );
    
    //! reset game time len
    bVestGameTimeLen=VEST_GAME_TIME_RESET;
    VestSaveDataByte( VEST_GAME_TIME_LENGTH_MEMORY_ADD, bVestGameTimeLen );
    
    //! reset led normal mode setting
    SensorSetLedOption( SENSOR_LED_COLOR_NORMAL, VEST_LED_N_COLOR_RESET );
    VestSaveDataByte( VEST_LED_NORMAL_COLOR_MEMORY_ADD, SensorGetLedOption(SENSOR_LED_COLOR_NORMAL) );
    SensorSetLedOption( SENSOR_LED_ON_TIME_NORMAL, VEST_LED_N_ON_TIME_RESET );
    VestSaveDataByte( VEST_LED_NORMAL_ON_TIME_MEMORY_ADD, SensorGetLedOption(SENSOR_LED_ON_TIME_NORMAL) );
    SensorSetLedOption( SENSOR_LED_OFF_TIME_NORMAL, VEST_LED_N_OFF_TIME_RESET );
    VestSaveDataByte( VEST_LED_NORMAL_OFF_TIME_MEMORY_ADD, SensorGetLedOption(SENSOR_LED_OFF_TIME_NORMAL) );
    
    //! reset led danger mode setting
    SensorSetLedOption( SENSOR_LED_COLOR_DANGER, VEST_LED_D_COLOR_RESET );
    VestSaveDataByte( VEST_LED_DANGER_COLOR_MEMORY_ADD, SensorGetLedOption(SENSOR_LED_COLOR_DANGER) );
    SensorSetLedOption( SENSOR_LED_ON_TIME_DANGER, VEST_LED_D_ON_TIME_RESET );
    VestSaveDataByte( VEST_LED_DANGER_ON_TIME_MEMORY_ADD, SensorGetLedOption(SENSOR_LED_ON_TIME_DANGER) );
    SensorSetLedOption( SENSOR_LED_OFF_TIME_DANGER, VEST_LED_D_OFF_TIME_RESET );
    VestSaveDataByte( VEST_LED_DANGER_OFF_TIME_MEMORY_ADD, SensorGetLedOption(SENSOR_LED_OFF_TIME_DANGER) );
    
    //! clear fire clash data
    bVestFireDataLen=0;
}
//******************************************************************************
//! \brief  get vest work mode
//! \retval vest work mode
//******************************************************************************
u8 VestGetWorkMode( void )
{
    return bVestMode;
}
//******************************************************************************
//! \brief  vest get game ID
//! \retval vest game ID
//******************************************************************************
u8 VestGetGameId( void )
{
    return bVestGameId;
}