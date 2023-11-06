#ifndef SEGMENT_H
#define SEGMENT_H

//******************************************************************************
//! Library --------------------------------------------------------------------
#include "..\STM8L Library\Include\stm8l15x.h"
#include "battery.h"
#include "vest.h"
//******************************************************************************
//! Define ---------------------------------------------------------------------
#define SEGMENT_PORT        GPIOE
#define SEGMENT_MODE        GPIO_Mode_Out_PP_Low_Fast

#define SEGMENT_1_PIN       GPIO_Pin_0
#define SEGMENT_2_PIN       GPIO_Pin_1
#define SEGMENT_3_PIN       GPIO_Pin_2
#define SEGMENT_4_PIN       GPIO_Pin_3
#define SEGMENT_5_PIN       GPIO_Pin_4
#define SEGMENT_6_PIN       GPIO_Pin_5
#define SEGMENT_7_PIN       GPIO_Pin_6


#define SEGMENT_COM_1_PORT  GPIOD
#define SEGMENT_COM_1_PIN   GPIO_Pin_1

#define SEGMENT_COM_2_PORT  GPIOD
#define SEGMENT_COM_2_PIN   GPIO_Pin_2
//******************************************************************************
//! Variable -------------------------------------------------------------------

//******************************************************************************
//! Function -------------------------------------------------------------------
void SegmentInit( void );
void SegmentMain( void );

#endif // SEGMENT_H