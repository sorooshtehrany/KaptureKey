#ifndef AUDIO_H
#define AUDIO_H

//******************************************************************************
//! Library --------------------------------------------------------------------
#include "..\STM8L Library\Include\stm8l15x.h"
//******************************************************************************
#define AUDIO_POWER             GPIOD,GPIO_Pin_3
#define AUDIO_POWER_MODE        GPIO_Mode_Out_PP_Low_Fast
//! Define ---------------------------------------------------------------------
#define AUDIO_PLAY_MAX_BUFFER           512
//! audio status -------------------------------------------
#define AUDIO_STOP_STATUS               0
#define AUDIO_PLAY_STATUS               1
#define AUDIO_PAUSE_STATUS              2
//! RFID id check ------------------------------------------
#define AUDIO_BASE_ADD                  100
#define AUDIO_SELECT_ALL_ADD            255
//! Variable -------------------------------------------------------------------
extern u8 bAudioStatus;
extern u8  bAudioTrackNumber;
extern u16 sAudioPlayTimeLimit;

//******************************************************************************
//! Function -------------------------------------------------------------------
void AudioInit                  ( void );
void AudioStartPlay             ( const char* bName );
void AudioMainPlay              ( void );
void AudioTimPlayIrqHandeler    ( void );
void AudioMain                  ( void );

#endif // AUDIO_H