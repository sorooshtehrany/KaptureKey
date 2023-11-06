#ifndef POWER_H
#define POWER_H

//******************************************************************************
//! Library --------------------------------------------------------------------
#include "..\STM8L Library\Include\stm8l15x.h"
//******************************************************************************
//! Define ---------------------------------------------------------------------
#define POWER_Insert_GPIO            GPIOD,GPIO_Pin_0
#define POWER_SW_GPIO                GPIOD,GPIO_Pin_2
#define POWER_Set_GPIO               GPIOD,GPIO_Pin_3

//******************************************************************************
//! Variable -------------------------------------------------------------------

//******************************************************************************
//! Function -------------------------------------------------------------------
void PowerInit      ( void );
void PowerOn        ( void );
void PowerOff       ( void );
void PowerSWRead    ( void );
void PowerMain      ( void );
//******************************************************************************

#endif // POWER_H