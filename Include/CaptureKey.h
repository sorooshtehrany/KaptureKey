#ifndef CaptureKey_H
#define CaptureKey_H

//******************************************************************************
//! Library --------------------------------------------------------------------
#include "..\STM8L Library\Include\stm8l15x.h"
#include "audio.h"
#include "sensor.h"
#include "MRF24J40.h"
#include "usart.h"

//******************************************************************************
//! Define ---------------------------------------------------------------------
#define  CaptureKey_GPIO                        GPIOC,GPIO_Pin_4
#define  KEY_GPIO_MODE                          GPIO_Mode_In_PU_No_IT

//******************************************************************************
//! Define ---------------------------------------------------------------------
#define CAPTUREKEY_RELOAD_CHECK_MEMORY_BYTE           72
#define CAPTUREKEY_MAX_OF_TEAMS                       10
#define CAPTUREKEY_MAX_OF_PLAYERS                     20
//******************************************************************************
//! reset setting value---------------------------------------------------------
#define CAPTUREKEY_ID_RESET                             23
#define CAPTUREKEY_GAME_TIME_RESET                      20
#define CAPTUREKEY_LED_NORMAL_COLOR_RESET               1
#define CAPTUREKEY_LED_WAIT_FOR_PUSHING_COLOR_RESET     2


//! eeprom setting -----------------------------------------
#define EEPROM_START_ADD        FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS
//! memory address -----------------------------------------
#define CAPTUREKEY_RELOAD_CHECK_MEMORY_ADD                     0
#define CAPTUREKEY_ID_MEMORY_ADD                               1
#define CAPTUREKEY_GAME_TIME_LENGTH_MEMORY_ADD                 2       
#define CAPTUREKY_SENSOR_LED_NORMAL_COLOR_MEMORY_ADD           3        
#define CAPTUREKY_SENSOR_LED_WAIT_FOR_PUSHING_KEY_MEMORY_ADD   4
       
//! device type --------------------------------------------
#define MASTER                 0      
#define MAIN_DEVICE            1      //player
#define VEST_DEVICE            2
#define GUN_DEVICE             3

//! device status ------------------------------------------
#define CAPTUREKEY_START_MODE                   1
#define CAPTUREKEY_WAIT_FOR_PUSHING_KEY_MODE    2
#define CAPTUREKEY_PUSHED_KEY_MODE              3
#define CAPTUREKEY_STOP_MODE                    4
#define CAPTUREKEY_PAUSE_MODE                   5
#define CAPTUREKEY_NORMAL_MODE                  6
//! device id check ----------------------------------------
#define CAPTUREKEY_ALL_MAIN_ID                   0xFF    //! all device
#define CAPTUREKEY_EVENT_MAIN_ID                 0xFE    //! all gun device
#define CAPTUREKEY_ODD_MAIN_ID                   0xFD    //! all vest device        
#define CAPTUREKEY_EVENT_GAME_ID                 0xFC    //! all vest and gun event ID
#define CAPTUREKEY_ODD_GAME_ID                   0xFB    //! all vest and gun ood ID
#define CAPTUREKEY_ALL_CAPTUREKEY_ID             102    //! all device
//******************************************************************************
//! network protocol -----------------------------------
#define CAPTUREKEY_PING_REQ_CMD                                 1
#define CAPTUREKEY_PING_RES_CMD                                 101
//------------------------------------------------------
#define CAPTUREKEY_SET_SENSOR_SETTING_REQ_CMD                   11
#define CAPTUREKEY_SET_SENSOR_SETTING_RES_CMD                   111
//------------------------------------------------------
#define CAPTUREKEY_GET_SETTING_REQ_CMD                          2
#define CAPTUREKEY_GET_SETTING_RES_CMD                          102
//------------------------------------------------------
#define CAPTUREKEY_SET_ID_REQ_CMD                               10
#define CAPTUREKEY_SET_ID_RES_CMD                               110
//------------------------------------------------------
#define CAPTUREKEY_SET_TIME_LENGTH_REQ_CMD                      13
#define CAPTUREKEY_SET_TIME_LENGTH_RES_CMD                      113
//------------------------------------------------------
#define CAPTUREKEY_START_REQ_CMD                                20
#define CAPTUREKEY_START_RES_CMD                                120
//------------------------------------------------------
#define CAPTUREKEY_PAUSE_REQ_CMD                                21
#define CAPTUREKEY_PAUSE_RES_CMD                                121
//------------------------------------------------------
#define CAPTUREKEY_STOP_REQ_CMD                                 22       
#define CAPTUREKEY_STOP_RES_CMD                                 122
//------------------------------------------------------
#define CAPTUREKEY_SET_RESET_SETTING_REQ_CMD                    15
#define CAPTUREKEY_SET_RESET_SETTING_RES_CMD                    115
//------------------------------------------------------
#define CAPTUREKEY_ARRENGMENT_REQ_CMD                           41      
#define CAPTUREKEY_ARRENGMENT_RES_CMD                           141    





//******************************************************************************
//! Variable -------------------------------------------------------------------



//******************************************************************************
//! Function -------------------------------------------------------------------
void CaptureKeyInit                             ( void );
void CaptureKeyResetSetting                     ( void );
void CaptureKeyKillLoserTeam                    ( void);
void CaptureKeyMain                             ( void );
void CapturekeyCheckKey                         ( void);        
void CapturekeySaveDataByte                     ( u16 sAdd, u8 bData );
u8   CapturekeyGetDataByte                      ( u16 sAdd );
void CapturekeyRfPackCheck                      ( void );
void CapturekeyRfDataCheck                      ( void );
void CapturekeyUsartDataCheck                   ( void );
void CapturekeyMemoryReload                     ( void );
void CapturekeyRfDataSend                       ( u8 bDesAdd, u8 bDataLen, u8* bDataArray );
u8   CapturekeyGetWorkMode                      ( void );
u8   CapturekeyGetMainId                        ( void );
u8   CapturekeyGetFireClashGunID                ( void);
u8 CapturekeyGetNormalLED                       (void);
u8 CapturekeyGetWaitForPushingLED               (void);




#endif // CaptureKey_H

