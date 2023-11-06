#include "..\Include\battery.h"

//******************************************************************************
//! Variable -------------------------------------------------------------------
#define ADC_BUFFER_ADDRESS              ((uint16_t)(&sBatteryAdcValue))
u16   sBatteryAdcValue=0;
float fBatteryVoltage=0.0;
u8    bBatteryCharge=0;
//******************************************************************************
//! \brief  Configure battery
//******************************************************************************
void BatteryInit( void )
{
    //! init gpio
    
    //! adc init ---------------------------------------------------------------
    //! Enable ADC1 clock 
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);
    
    //! Initialize and configure ADC1 
    ADC_Init( ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2 );
    ADC_SamplingTimeConfig( ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles );
    ADC_SamplingTimeConfig( ADC1, ADC_Group_FastChannels, ADC_SamplingTime_384Cycles );

    ADC_VrefintCmd( ENABLE );
    
    //! Enable ADC1 
    ADC_Cmd( ADC1, ENABLE );

    //! Enable ADC1 Channels 22
    ADC_ChannelCmd( ADC1,BATTERY_VOLTAGE_ADC_CHANNEL, ENABLE ); 
    
    //! Start ADC1 Conversion using Software trigger
    ADC_SoftwareStartConv(ADC1);
    //  ------------------------------------------------------------------------
    
    //! dma init ---------------------------------------------------------------
    //! Enable DMA1 clock 
    CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
    
    //! Connect ADC to DMA 1 channel 0
    SYSCFG_REMAPDMAChannelConfig( REMAP_DMA1Channel_ADC1ToChannel0 );

    DMA_Init( DMA1_Channel0, ADC_BUFFER_ADDRESS, ADC_DR_ADDRESS, ADC_BUFFER_SIZE,
              DMA_DIR_PeripheralToMemory, DMA_Mode_Circular, DMA_MemoryIncMode_Inc,
              DMA_Priority_High, DMA_MemoryDataSize_HalfWord );

    //! DMA Channel0 enable 
    DMA_Cmd( DMA1_Channel0, ENABLE );

    //! Enable DMA1 channel0 Transfer complete interrupt 
    //DMA_ITConfig( DMA1_Channel0, DMA_ITx_TC, ENABLE );

    //! DMA enable 
    DMA_GlobalCmd( ENABLE );
    //  ------------------------------------------------------------------------
    
    //! Enable ADC1 DMA requests
    ADC_DMACmd( ADC1, ENABLE );
}
//******************************************************************************
//! \brief  get battery voltage
//! \retval battery voltage
//******************************************************************************
float BatteryGetVoltage( void )
{
    fBatteryVoltage=BATTERY_ADC_BASE_VOLTAGE*sBatteryAdcValue;
    fBatteryVoltage/=BATTERY_VOLTAGE_RATIO;
    
    return fBatteryVoltage;
}
//******************************************************************************
//! \brief  get battery charge
//! \retval battery charge
//******************************************************************************
u8 BatteryGetCharge( void )
{
    if( sBatteryAdcValue<=BATTERY_ADC_CHARGE_MIN )
        bBatteryCharge=0;
    else if( sBatteryAdcValue>=BATTERY_ADC_CHARGE_MAX )
        bBatteryCharge=99;
    else
        bBatteryCharge=(sBatteryAdcValue-BATTERY_ADC_CHARGE_MIN)/BATTERY_ADC_CHARGE_RATIO;
    
    return bBatteryCharge;
}