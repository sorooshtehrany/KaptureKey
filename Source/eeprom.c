#include "../Include/eeprom.h"
#include "../Include/at24cx.h"
#include "../Include/delay.h"
#include "../Include/sensor.h"
#include "../Include/vest.h"
//******************************************************************************
//! Variable -------------------------------------------------------------------
u8  bEepromRxData[150];
u16 sEepromRxLen=0;
u16 sEepromRxAdd=0;
u8  bEepromTxData[150];
u16 sEepromTxLen=0;
u16 sEepromTxAdd=0;

u8  bEepromFireDataLen=0;
u8  bEepromFireData[1100];
//******************************************************************************
//! \brief  eeprom init
//******************************************************************************
void EepromInit( void )
{
        At24Init();   
}
//******************************************************************************
//! \brief  eeprom main control
//******************************************************************************
void EepromMain( void )
{
    
}
//******************************************************************************
//! \brief  eeprom reload data
//******************************************************************************
void EepromReload( void )
{/*
    u32 iTimeOut=0;
    
    //! Disable general interrupts 
    //disableInterrupts();
    
    //! check eeprom reload byte
    if( EepromReadByte(EEPROM_RELOAD_FLAG_ADD)==EEPROM_RELOAD_FLAG_CHECK )
    {
        //! set rx address
        sEepromRxAdd=100;
        //! set rx data len
        sEepromRxLen=22;
        //! read data from eeprom
        At24ReadBuffer( bEepromRxData, sEepromRxAdd, &sEepromRxLen );
        while( sEepromRxLen!=0 )
        {
            iTimeOut++;
            if( iTimeOut>99999 )
                break;
        }
    
        //! reload vest ID
        VestSetId( bEepromRxData[EEPROM_VEST_ID_ADD-100] );
        //! reload vest game time
        VestSetGameTime( bEepromRxData[EEPROM_VEST_GAME_TIME_ADD-100] );
        //! reload vest sensor damage
        VestSetSensorDamage( 1, bEepromRxData[EEPROM_VEST_SENS1_DAMAGE_ADD-100] );
        VestSetSensorDamage( 2, bEepromRxData[EEPROM_VEST_SENS2_DAMAGE_ADD-100] );
        VestSetSensorDamage( 3, bEepromRxData[EEPROM_VEST_SENS3_DAMAGE_ADD-100] );
        VestSetSensorDamage( 4, bEepromRxData[EEPROM_VEST_SENS4_DAMAGE_ADD-100] );
        VestSetSensorDamage( 5, bEepromRxData[EEPROM_VEST_SENS5_DAMAGE_ADD-100] );
        VestSetSensorDamage( 6, bEepromRxData[EEPROM_VEST_SENS6_DAMAGE_ADD-100] );
        VestSetSensorDamage( 7, bEepromRxData[EEPROM_VEST_SENS7_DAMAGE_ADD-100] );
        //! reload vest sniper flag
        VestSetSniperFlag( bEepromRxData[EEPROM_VEST_SNIPER_FLAG_ADD-100] );
        //! reload vest alarm time
        VestSetAlarmTime( bEepromRxData[EEPROM_VEST_ALARM_TIME_ADD-100] );
        //! reload fire data length
        bEepromFireDataLen=bEepromRxData[EEPROM_VEST_DATA_LEN_ADD-100];
        //! reload sensor normal mode led status
        SensorSetOption( SENSOR_LED_COLOR_NORMAL, bEepromRxData[EEPROM_SENSOR_NORMAL_COLOR_ADD-100] );
        SensorSetOption( SENSOR_LED_ON_TIME_NORMAL, bEepromRxData[EEPROM_SENSOR_NORMAL_ON_TIME_ADD-100] );
        SensorSetOption( SENSOR_LED_OFF_TIME_NORMAL, bEepromRxData[EEPROM_SENSOR_NORMAL_OFF_TIME_ADD-100] );
        //! reload sensor danger mode led status
        SensorSetOption( SENSOR_LED_COLOR_DANGER, bEepromRxData[EEPROM_SENSOR_DANGER_COLOR_ADD-100] );
        SensorSetOption( SENSOR_LED_ON_TIME_DANGER, bEepromRxData[EEPROM_SENSOR_DANGER_ON_TIME_ADD-100] );
        SensorSetOption( SENSOR_LED_OFF_TIME_DANGER, bEepromRxData[EEPROM_SENSOR_DANGER_OFF_TIME_ADD-100] );
        
        //! buffer fire data -------------------------------
        //! set rx address
        sEepromRxAdd=EEPROM_VEST_DATA_START_ADD;
        //! set rx data len
        sEepromRxLen=bEepromFireDataLen*4;
        //! read data from eeprom
        At24ReadBuffer( bEepromFireData, sEepromRxAdd, &sEepromRxLen );
        while( sEepromRxLen!=0 )
        {
            iTimeOut++;
            if( iTimeOut>99999 )
                break;
        }
        //! ------------------------------------------------
    }
    else
    {
        //! save eeprom realod byte
        EepromWriteByte( EEPROM_RELOAD_FLAG_ADD, EEPROM_RELOAD_FLAG_CHECK );
        
        //! save vest ID
        EepromWriteByte( EEPROM_VEST_ID_ADD, VestGetId() );
        //! save vest game time
        EepromWriteByte( EEPROM_VEST_GAME_TIME_ADD, VestGetGameTime() );
        //! save vest sensor damage
        EepromWriteByte( EEPROM_VEST_SENS1_DAMAGE_ADD, VestGetSensorDamage(1) );
        EepromWriteByte( EEPROM_VEST_SENS2_DAMAGE_ADD, VestGetSensorDamage(2) );
        EepromWriteByte( EEPROM_VEST_SENS3_DAMAGE_ADD, VestGetSensorDamage(3) );
        EepromWriteByte( EEPROM_VEST_SENS4_DAMAGE_ADD, VestGetSensorDamage(4) );
        EepromWriteByte( EEPROM_VEST_SENS5_DAMAGE_ADD, VestGetSensorDamage(5) );
        EepromWriteByte( EEPROM_VEST_SENS6_DAMAGE_ADD, VestGetSensorDamage(6) );
        EepromWriteByte( EEPROM_VEST_SENS7_DAMAGE_ADD, VestGetSensorDamage(7) );
        //! save vest sniper flag
        EepromWriteByte( EEPROM_VEST_SNIPER_FLAG_ADD, VestGetSniperFlag() );
        //! save vest alarm time
        EepromWriteByte( EEPROM_VEST_ALARM_TIME_ADD, VestGetAlarmTime() );
        //! save fire data length
        EepromWriteByte( EEPROM_VEST_DATA_LEN_ADD, bEepromFireDataLen );
        //! save sensor normal mode led status
        EepromWriteByte( EEPROM_SENSOR_NORMAL_COLOR_ADD, SensorGetOption(SENSOR_LED_COLOR_NORMAL) );
        EepromWriteByte( EEPROM_SENSOR_NORMAL_ON_TIME_ADD, SensorGetOption(SENSOR_LED_ON_TIME_NORMAL) );
        EepromWriteByte( EEPROM_SENSOR_NORMAL_OFF_TIME_ADD, SensorGetOption(SENSOR_LED_OFF_TIME_NORMAL) );
        //! save sensor danger mode led status
        EepromWriteByte( EEPROM_SENSOR_DANGER_COLOR_ADD, SensorGetOption(SENSOR_LED_COLOR_DANGER) );
        EepromWriteByte( EEPROM_SENSOR_DANGER_ON_TIME_ADD, SensorGetOption(SENSOR_LED_ON_TIME_DANGER) );
        EepromWriteByte( EEPROM_SENSOR_DANGER_OFF_TIME_ADD, SensorGetOption(SENSOR_LED_OFF_TIME_DANGER) );
    }
    
    //! Enable general interrupts 
    //enableInterrupts();
  */
}
//******************************************************************************
//! \brief  eeprom read data byte 
//! \param  sAdd : data address
//! \retval eeprom data
//******************************************************************************
u8 EepromReadByte( u16 sAdd )
{
    //! Disable general interrupts 
    disableInterrupts();
    
    //! set rx data len
    sEepromRxLen=1;
    //! read data from eeprom
    At24ReadBuffer( bEepromRxData, sAdd, &sEepromRxLen );
    
    //! Enable general interrupts 
    enableInterrupts();
    
    //! return data
    return bEepromRxData[0];
}
//******************************************************************************
//! \brief  eeprom write data byte
//! \param  sAdd : data address
//! \param  bData : data write
//******************************************************************************
void EepromWriteByte( u16 sAdd, u8 bData )
{
    //! Disable general interrupts 
    disableInterrupts();
    
    //! set tx data
    bEepromTxData[0]=bData;
    //! write data to eeprom
    At24WriteByte( bEepromTxData, sAdd );

    //! Enable general interrupts 
    enableInterrupts();
    
    //! wait write complet
    DelayMs(5);
}
//******************************************************************************
//! \brief  eeprom read fire data
//! \param  bDataArray : data array for return fire data
//! \retval data length
//******************************************************************************
u8 EepromReadFireData( u8* bDataArray, u8 bStartPack, u8 bEndPack )
{
  /*
    u8  bStart=0;
    u8  bEnd=0;
    //u32 iTimeOut=0;
    u16 sDataLen=0;
    
    //! Disable general interrupts 
    //disableInterrupts();
    
    //! check start pack value
    if( bStartPack==0 )
        bStart=1;
    else if( bStartPack>bEepromFireDataLen )
        bStart=bEepromFireDataLen;
    else
        bStart=bStartPack;
    //! check end pack value
    if( bEndPack<bStart )
        bEnd=bStart;
    else if( bEndPack>bEepromFireDataLen )
        bEnd=bEepromFireDataLen;
    else 
        bEnd=bEndPack;
    //! check pack length
    if( (bEnd-bStart)>=25 )
        bEnd=bStart+24;
            
    //! set rx address
    sEepromRxAdd=(bStart-1)*4+EEPROM_VEST_DATA_START_ADD;
    //! set rx data len
    sEepromRxLen=(bEnd-bStart+1)*4;
    //! set report data
    bDataArray[0]=bEepromFireDataLen;
    bDataArray[1]=bStart;
    bDataArray[2]=bEnd;
    if( bEepromFireDataLen>0 )
    {
        //! read data from eeprom
//  ////////////////////////////////// desable     
        At24ReadBuffer( &bDataArray[3], sEepromRxAdd, &sEepromRxLen );
        while( sEepromRxLen!=0 )
        {
            iTimeOut++;
            if( iTimeOut>99999 )
                break;
        }
// ////////////////////////////////////       desable
        
        //! read data from RAM -----------------------------
        sEepromRxAdd-=EEPROM_VEST_DATA_START_ADD;
        for( u8 i=0 ; i<sEepromRxLen ; i++ )
            bDataArray[3+i]=bEepromFireData[sEepromRxAdd+i];
        //! ------------------------------------------------
        
        //! clalculate real data length
        sDataLen=((bEnd-bStart+1)*4)+3;
    }
    else
    {
        sDataLen=3;
    }
    
    //! Enable general interrupts 
    //enableInterrupts();
    
    //! return data
    return (u8)(sDataLen);
*/
}
//******************************************************************************
//! \brief  eeprom write fire data
//******************************************************************************
void EepromWriteFireData( u8* bDataarray, u16 sTime )
{
  /*
    u16 sAdd=0;
    
    //! Disable general interrupts 
    disableInterrupts();
    
    //! set tx data
    bEepromTxData[0]=bDataarray[0];
    bEepromTxData[1]=bDataarray[1];
    bEepromTxData[2]=sTime/60;
    bEepromTxData[3]=sTime%60;
    //! set tx address
    sEepromTxAdd=bEepromFireDataLen*4+EEPROM_VEST_DATA_START_ADD;
    
    //! write to RAM ---------------------------------------
    sAdd=sEepromTxAdd-EEPROM_VEST_DATA_START_ADD;
    for( u8 i=0 ; i<4 ; i++ )
        bEepromFireData[sAdd+i]=bEepromTxData[i];
    //! ----------------------------------------------------
    
    //! write data to eeprom
    for( u8 i=0 ; i<4 ; i++ )
        EepromWriteByte( sEepromTxAdd+i, bEepromTxData[i] );
    
    //! calculate and save fire data length
    bEepromFireDataLen++;
    //! write data to eeprom
    EepromWriteByte( EEPROM_VEST_DATA_LEN_ADD, bEepromFireDataLen );
  
    //! Enable general interrupts 
    enableInterrupts();
  */
}
//******************************************************************************
//! \brief  eeprom set fire data length
//! \param  bDataLen : fire data length
//******************************************************************************
void EepromSetFireDataLen( u8 bDataLen )
{
    //bEepromFireDataLen=bDataLen;
}
//******************************************************************************
//! \brief  eeprom write fire data
//******************************************************************************
u8 EepromGetFireDataLen( void )
{
    //return bEepromFireDataLen;
}