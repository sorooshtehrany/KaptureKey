#include "..\Include\power.h"
//******************************************************************************
//! Variable -------------------------------------------------------------------
u32 iPowerCounter=0;
//******************************************************************************
//! \brief  power on and power sw pin init
//******************************************************************************
void PowerInit( void )
{
    GPIO_Init( POWER_Insert_GPIO,GPIO_Mode_In_FL_No_IT); 
    GPIO_Init( POWER_SW_GPIO,GPIO_Mode_In_FL_No_IT ); 
    GPIO_Init( POWER_Set_GPIO,GPIO_Mode_Out_PP_High_Fast );  
    PowerOn();
}
//******************************************************************************
//! \brief  power on
//******************************************************************************
void PowerOn( void )
{
    GPIO_SetBits( POWER_Set_GPIO );
    while( GPIO_ReadInputDataBit(POWER_SW_GPIO)!=0 );
}
//******************************************************************************
//! \brief  power off
//******************************************************************************
void PowerOff( void )
{
    GPIO_ResetBits( POWER_Set_GPIO );
}
//******************************************************************************
//! \brief  power sw check 
//******************************************************************************
void PowerMain( void )
{
    if( GPIO_ReadInputDataBit( POWER_SW_GPIO )!=0 )
    {
        while( GPIO_ReadInputDataBit(POWER_SW_GPIO)!=0 )
        {
            iPowerCounter++;
            if( iPowerCounter>250000 )
            {
                PowerOff(); 
            }
        }
        iPowerCounter=0; 
    }
}