#ifndef BATTERY_H
#define BATTERY_H

//******************************************************************************
//! Library --------------------------------------------------------------------
#include "..\STM8L Library\Include\stm8l15x.h"
//******************************************************************************
//! Define ---------------------------------------------------------------------
#define ADC_DR_ADDRESS                  ((uint16_t)0x5344)
#define ADC_BUFFER_SIZE                 1

//! Define pin config-----------------------------------------------------------
#define BATTERY_VOLTAGE_ADC_CHANNEL     ADC_Channel_22


#define BATTERY_ADC_BASE_VOLTAGE        0.0008056640625
#define BATTERY_VOLTAGE_RATIO           0.3197278911564

#define BATTERY_VOLTAGE_CHARGE_MIN      6.4
#define BATTERY_ADC_CHARGE_MIN          2535
#define BATTERY_VOLTAGE_CHARGE_MAX      8.4
#define BATTERY_ADC_CHARGE_MAX          3335

#define BATTERY_ADC_CHARGE_RATIO        8
//******************************************************************************
//! Variable -------------------------------------------------------------------

//******************************************************************************
//! Function -------------------------------------------------------------------
void    BatteryInit         ( void );
float   BatteryGetVoltage   ( void );
u8      BatteryGetCharge    ( void );
//******************************************************************************

#endif // BATTERY_h