#ifndef VEST_H
#define VEST_H

//******************************************************************************
//! Library --------------------------------------------------------------------
#include "..\STM8L Library\Include\stm8l15x.h"
#include "audio.h"
#include "sensor.h"
#include "MRF24J40.h"
#include "usart.h"
#include "vibre.h"
//******************************************************************************
//! Define ---------------------------------------------------------------------
#define VEST_RELOAD_CHECK_MEMORY_BYTE           72
//! vest setting reset value -------------------------------
#define VEST_ID_RESET                           1
#define VEST_GAME_TIME_RESET                    20
#define VEST_LED_N_COLOR_RESET                  2
#define VEST_LED_N_ON_TIME_RESET                2
#define VEST_LED_N_OFF_TIME_RESET               20
#define VEST_LED_D_COLOR_RESET                  1
#define VEST_LED_D_ON_TIME_RESET                60
#define VEST_LED_D_OFF_TIME_RESET               0
//! eeprom setting -----------------------------------------
#define EEPROM_START_ADD        FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS
//! memory address -----------------------------------------
#define VEST_RELOAD_CHECK_MEMORY_ADD            0

#define VEST_ID_MEMORY_ADD                      1

#define VEST_LED_NORMAL_COLOR_MEMORY_ADD        2
#define VEST_LED_NORMAL_ON_TIME_MEMORY_ADD      3
#define VEST_LED_NORMAL_OFF_TIME_MEMORY_ADD     4

#define VEST_LED_DANGER_COLOR_MEMORY_ADD        5
#define VEST_LED_DANGER_ON_TIME_MEMORY_ADD      6
#define VEST_LED_DANGER_OFF_TIME_MEMORY_ADD     7

#define VEST_GAME_TIME_LENGTH_MEMORY_ADD        8   //! game time unit = minutes

#define VEST_FIRE_DATA_LEN_MEMORY_ADD           50
#define VEST_FIRE_DATA_START_MEMORY_ADD         51
//! device type --------------------------------------------
#define MAIN_DEVICE     1
#define VEST_DEVICE     2
#define GUN_DEVICE      3
//! device status ------------------------------------------
#define VEST_STOP_MODE              0
#define VEST_START_MODE             1
#define VEST_PAUSE_MODE             2
#define VEST_STOP_SPECIFIC_MODE     3
#define VEST_START_SPECIFIC_MODE    4
#define VEST_PAUSE_SPECIFIC_MODE    5
//! device id check ----------------------------------------
#define VEST_ALL_MAIN_ID            0xFF    //! all device
#define VEST_EVENT_MAIN_ID          0xFE    //! all gun device
#define VEST_OOD_MAIN_ID            0xFD    //! all vest device
#define VEST_EVENT_GAME_ID          0xFC    //! all vest and gun event ID
#define VEST_OOD_GAME_ID            0xFB    //! all vest and gun ood ID
//! network protocol ---------------------------------------
//! ping
#define VEST_PING_REQ_CMD                   1
#define VEST_PING_RES_CMD                   101
//! get setting
#define VEST_GET_SETTING_REQ_CMD            2       
#define VEST_GET_SETTING_RES_CMD            102
//! set ID
#define VEST_SET_ID_REQ_CMD                 10
#define VEST_SET_ID_RES_CMD                 110
//! set led normal mode
#define VEST_SET_LED_NORMAL_REQ_CMD         11
#define VEST_SET_LED_NORMAL_RES_CMD         111
//! set led danger mode
#define VEST_SET_LED_DANGER_REQ_CMD         12
#define VEST_SET_LED_DANGER_RES_CMD         112
//! set game time lengh
#define VEST_SET_GAME_TIME_REQ_CMD          13
#define VEST_SET_GAME_TIME_RES_CMD          113
//! reset vest all setting
#define VEST_RESET_ALL_SETTING_REQ_CMD      15
#define VEST_RESET_ALL_SETTING_RES_CMD      115
//! start game
#define VEST_START_GAME_REQ_CMD             20
#define VEST_START_GAME_RES_CMD             120
//! pause game
#define VEST_PAUSE_GAME_REQ_CMD             21
#define VEST_PAUSE_GAME_RES_CMD             121
//! stop game
#define VEST_STOP_GAME_REQ_CMD              22
#define VEST_STOP_GAME_RES_CMD              122
//! get fire data
#define VEST_GET_FIRE_DATA_REQ_CMD          30
#define VEST_GET_FIRE_DATA_RES_CMD          130
//! clear data
#define VEST_CLEAR_DATA_REQ_CMD             32
#define VEST_CLEAR_DATA_RES_CMD             132
//! fire clash report
#define VEST_FIRE_CLASH_REPORT_CMD          40      
#define VEST_FIRE_CLASH_RESPONSE_CMD        140
//! get gun fire clash 
#define VEST_GUN_FIRE_CLASH_REPORT_CMD      50      
#define VEST_GUN_FIRE_CLASH_RESPONSE_CMD    150
//******************************************************************************
//! Variable -------------------------------------------------------------------

//******************************************************************************
//! Function -------------------------------------------------------------------
void VestInit                       ( void );
void VestMain                       ( void );
void VestSaveDataByte               ( u16 sAdd, u8 bData );
u8   VestGetDataByte                ( u16 sAdd );
void VestMemoryReload               ( void );
void VestFireDataSave               ( u8 bSensorId, u8 bGunId );
void VestFireDataSend               ( u8 bStartPack, u8 bEndPack );
void VestUsartDataCheck             ( void );
void VestRfPackCheck                ( void );
void VestRfDataCheck                ( void );
void VestRfDataSend                 ( u8 bDesAdd, u8 bDataLen, u8* bDataArray );
void VestResetSetting               ( void );
u8   VestGetWorkMode                ( void );
u8   VestGetGameId                  ( void );
//******************************************************************************

#endif // VEST_H