#ifndef SENSOR_H
#define SENSOR_H

//******************************************************************************
//! Library --------------------------------------------------------------------
#include "..\STM8L Library\Include\stm8l15x.h"
#include "systemTick.h"
#include "usart.h"
//#include "vest.h"
//******************************************************************************
//! Define -----------------------------------------

//******************************************************************************
//! Define ---------------------------------------------------------------------

//! sensor color----------------------------------------------------------------
#define SENSOR_LED_OFF                            0
#define SENSOR_LED_NORMAL                         1
#define SENSOR_LED_WAIT_FOR_PUSHING               2




#define SENSOR_CMD_LED_SET          0xBB    //! cmd + LED color + LED on time + LED off time  ( unit 50 mS )
#define SENSOR_CMD_GET_FIRE         0xAA    //! cmd + sensor Add
#define SENSOR_CMD_LED_ONE_SET      0xDD    //! cmd + sensor Add + LED color + LED on time + LED off time  ( unit 50 mS )
#define SENSOR_CMD_SET_WORK         0xCC    //! cmd + sensor Add + work flag  (Add 255 for select all sensor)(1 is start work and 0 is stop work)
//******************************************************************************
//! Variable -------------------------------------------------------------------

//******************************************************************************
//! Function -------------------------------------------------------------------
void SensorInit ( void);
void SensorMain ( void );
void SensorEmptyUsart( void );
//******************************************************************************

#endif // SENSOR_H