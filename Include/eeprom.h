#ifndef EEPROM_H
#define EEPROM_H

#include "..\STM8L Library\Include\stm8l15x.h"
//******************************************************************************
//! Define ---------------------------------------------------------------------
#define EEPROM_RELOAD_FLAG_ADD                  0
#define EEPROM_RELOAD_FLAG_CHECK                0x48

#define EEPROM_VEST_ID_ADD                      100               
#define EEPROM_VEST_GAME_TIME_ADD               101
#define EEPROM_VEST_SENS1_DAMAGE_ADD            102
#define EEPROM_VEST_SENS2_DAMAGE_ADD            103
#define EEPROM_VEST_SENS3_DAMAGE_ADD            104
#define EEPROM_VEST_SENS4_DAMAGE_ADD            105
#define EEPROM_VEST_SENS5_DAMAGE_ADD            106
#define EEPROM_VEST_SENS6_DAMAGE_ADD            107
#define EEPROM_VEST_SENS7_DAMAGE_ADD            108
#define EEPROM_VEST_SNIPER_FLAG_ADD             109
#define EEPROM_VEST_ALARM_TIME_ADD              110

#define EEPROM_SENSOR_NORMAL_COLOR_ADD          111
#define EEPROM_SENSOR_NORMAL_ON_TIME_ADD        112
#define EEPROM_SENSOR_NORMAL_OFF_TIME_ADD       113
#define EEPROM_SENSOR_DANGER_COLOR_ADD          114
#define EEPROM_SENSOR_DANGER_ON_TIME_ADD        115
#define EEPROM_SENSOR_DANGER_OFF_TIME_ADD       116

#define EEPROM_VEST_DATA_LEN_ADD                120
#define EEPROM_VEST_DATA_START_ADD              1000
//******************************************************************************
//! Variable -------------------------------------------------------------------

//******************************************************************************
//! Function -------------------------------------------------------------------
void EepromInit         ( void );
void EepromMain         ( void );
void EepromReload       ( void );
u8   EepromReadByte     ( u16 sAdd );
void EepromWriteByte    ( u16 sAdd, u8 bData );
u8   EepromReadFireData ( u8* bDataArray, u8 bStartPack, u8 bEndPack );
void EepromWriteFireData( u8* bDataarray, u16 sTime );
void EepromSetFireDataLen( u8 bDataLen );
u8   EepromGetFireDataLen( void );
//******************************************************************************

#endif // EEPROM_H