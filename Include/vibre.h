#ifndef VIBRE_H
#define VIBRE_H

//******************************************************************************
//! Library --------------------------------------------------------------------
#include "..\STM8L Library\Include\stm8l15x.h"
#include "systemTick.h"
//******************************************************************************
//! Define ---------------------------------------------------------------------
#define VIBRE_ON_TIME       3   //! vibre on time unit second

#define VIBRE_1_GPIO        GPIOC, GPIO_Pin_2
#define VIBRE_2_GPIO        GPIOC, GPIO_Pin_3
//******************************************************************************
//! Variable -------------------------------------------------------------------

//******************************************************************************
//! Function -------------------------------------------------------------------
void VibreInit      ( void );
void VibreMain      ( void );
void VibreSetStatus ( u8 bStatus );
//******************************************************************************

#endif // VIBRE_H

