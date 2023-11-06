#ifndef BUZZER_H
#define BUZZER_H

#include "..\STM8L Library\Include\stm8l15x.h"
//******************************************************************************
//! Define ---------------------------------------------------------------------
#define BUZZER_PORT         GPIOD
#define BUZZER_PIN          GPIO_Pin_4
#define BUZZER_MODE         GPIO_Mode_Out_PP_Low_Fast
//******************************************************************************
//! Variable -------------------------------------------------------------------
extern u8  buzzerBeepCounter;
extern u32 buzzerTimeCounter;
extern u8  buzzerStatus;
extern const u8  buzzerAudioError[8000];
//******************************************************************************
//! Function -------------------------------------------------------------------
void buzzerInit     ( void );
void buzzerBeep     ( void );
//******************************************************************************

#endif // BUZZER_H