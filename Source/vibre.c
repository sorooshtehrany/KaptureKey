#include "..\Include\vibre.h"

//******************************************************************************
//! Variable -------------------------------------------------------------------
u32 iVibreTimeBuffer=0;
u8  bVibreStatus=0;
u8  bVibreStartFlag=0;
//******************************************************************************
//! \brief  vibration init
//******************************************************************************
void VibreInit( void )
{
  GPIO_Init( VIBRE_1_GPIO, GPIO_Mode_Out_PP_Low_Fast );
  GPIO_Init( VIBRE_2_GPIO, GPIO_Mode_Out_PP_Low_Fast );
}
//******************************************************************************
//! \brief  vibration main control
//******************************************************************************
void VibreMain( void )
{
    if( bVibreStatus!=0 )
    {
        //! check vibre start flag
        if( bVibreStartFlag==0 )
        {
            //! set vibre start falg 
            bVibreStartFlag=1;
            //! buffer start time
            iVibreTimeBuffer=SystemTickGetMsecond();
            //! set vibre status
            GPIO_SetBits( VIBRE_1_GPIO );
            GPIO_SetBits( VIBRE_2_GPIO );
        }
        //! check vibre time 
        if( SystemTickGetMsecond()>VIBRE_ON_TIME*1000+iVibreTimeBuffer )
        {
            //! clear vibre status
            bVibreStatus=0;
            //! clear vibre start flag
            bVibreStartFlag=0;
            //! clear vibre status
            GPIO_ResetBits( VIBRE_1_GPIO );
            GPIO_ResetBits( VIBRE_2_GPIO );
        }
    }
}
//******************************************************************************
//! \brief  vibration set status 
//! \param  bStatus : vibre status
//******************************************************************************
void VibreSetStatus( u8 bStatus )
{
    bVibreStatus=bStatus;
}