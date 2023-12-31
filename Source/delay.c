#include "..\Include\delay.h"
//******************************************************************************
//! Variable -------------------------------------------------------------------

//******************************************************************************
//! \brief  delay uS
//******************************************************************************
void DelayUs( u16 sDelayUs )
{
    u32 iDelayCounterCheck=sDelayUs/2;
    for( u32 i=0 ; i<iDelayCounterCheck ; i++ );
}
//******************************************************************************
//! \brief  delay mS
//******************************************************************************
void DelayMs( u16 sDelayMs )
{
    u32 iDelayCounterCheck=sDelayMs*DELAY_COUNTER_1MS;
    for( u32 i=0 ; i<iDelayCounterCheck ; i++ );
}