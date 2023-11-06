#include "..\Include\segment.h"
#include "..\Include\CaptureKey.h"
//******************************************************************************
//! Variable -------------------------------------------------------------------
u8  bSegmentNumberArray[10]={63,6,91,79,102,109,125,7,127,111};
u8  bStart=64;
u8  bStop[2]={0X73,109};
u8  bPause[2]={0X77,0X73};
u8  bSegmentPlexer=0;
u32 iSegmentMainTick=0;
u32 iSegmentShowTime=0;
u8  bCharge;

//******************************************************************************
//! \brief  Configure segment gpio
//******************************************************************************
void SegmentInit( void )
{
    GPIO_Init( SEGMENT_PORT, SEGMENT_1_PIN, SEGMENT_MODE );
    GPIO_Init( SEGMENT_PORT, SEGMENT_2_PIN, SEGMENT_MODE );
    GPIO_Init( SEGMENT_PORT, SEGMENT_3_PIN, SEGMENT_MODE );
    GPIO_Init( SEGMENT_PORT, SEGMENT_4_PIN, SEGMENT_MODE );
    GPIO_Init( SEGMENT_PORT, SEGMENT_5_PIN, SEGMENT_MODE );
    GPIO_Init( SEGMENT_PORT, SEGMENT_6_PIN, SEGMENT_MODE );
    GPIO_Init( SEGMENT_PORT, SEGMENT_7_PIN, SEGMENT_MODE );
    //GPIO_Init( SEGMENT_PORT, SEGMENT_DP_PIN, SEGMENT_MODE );
    
    GPIO_Init( SEGMENT_COM_1_PORT, SEGMENT_COM_1_PIN, SEGMENT_MODE );
    GPIO_Init( SEGMENT_COM_2_PORT, SEGMENT_COM_2_PIN, SEGMENT_MODE );
}
//******************************************************************************
//! \brief  segment timer interrupt handeler
//******************************************************************************
void SegmentMain( void )
{
  if( iSegmentMainTick-SystemTickGetMsecond()>1)
    {
     iSegmentMainTick=SystemTickGetMsecond();
     bSegmentPlexer=(bSegmentPlexer+1)%2;
     bCharge=BatteryGetCharge();
     //! segment 1
     if(bSegmentPlexer==0)
       {
         GPIO_Write( SEGMENT_PORT,0);
         GPIO_ResetBits( SEGMENT_COM_1_PORT, SEGMENT_COM_1_PIN);
         GPIO_SetBits( SEGMENT_COM_2_PORT, SEGMENT_COM_2_PIN);
         if(CapturekeyGetWorkMode()==CAPTUREKEY_WAIT_FOR_PUSHING_KEY_MODE)
              GPIO_Write( SEGMENT_PORT,bSegmentNumberArray[CapturekeyGetFireClashGunID()%10]);  
         if(CapturekeyGetWorkMode()==CAPTUREKEY_PUSHED_KEY_MODE)
              GPIO_Write( SEGMENT_PORT,bSegmentNumberArray[CapturekeyGetFireClashGunID()%10]);  
         if((CapturekeyGetWorkMode()==CAPTUREKEY_STOP_MODE)||
            (CapturekeyGetWorkMode()==CAPTUREKEY_PAUSE_MODE))
           {
             if(SystemTickGetMsecond()-iSegmentShowTime<3000)
               {
                 GPIO_Write( SEGMENT_PORT,bSegmentNumberArray[CapturekeyGetMainId()%10]);
               }
             else
               {
                 GPIO_Write( SEGMENT_PORT,bSegmentNumberArray[bCharge%10]);
                 if(SystemTickGetMsecond()-iSegmentShowTime>4000)
                      iSegmentShowTime=SystemTickGetMsecond();
               }
             
           }

       }
     //! segment 2  
     if(bSegmentPlexer==1)  
       {
         GPIO_Write( SEGMENT_PORT,0);
         GPIO_SetBits( SEGMENT_COM_1_PORT, SEGMENT_COM_1_PIN);
         GPIO_ResetBits( SEGMENT_COM_2_PORT, SEGMENT_COM_2_PIN);
         if(CapturekeyGetWorkMode()==CAPTUREKEY_WAIT_FOR_PUSHING_KEY_MODE)
              GPIO_Write( SEGMENT_PORT,bSegmentNumberArray[CapturekeyGetFireClashGunID()/10]);  
         if(CapturekeyGetWorkMode()==CAPTUREKEY_PUSHED_KEY_MODE)
              GPIO_Write( SEGMENT_PORT,bSegmentNumberArray[CapturekeyGetFireClashGunID()/10]);  
         if((CapturekeyGetWorkMode()==CAPTUREKEY_STOP_MODE)||
            (CapturekeyGetWorkMode()==CAPTUREKEY_PAUSE_MODE))
           {
             if(SystemTickGetMsecond()-iSegmentShowTime<3000)
               {
                 GPIO_Write( SEGMENT_PORT,bSegmentNumberArray[CapturekeyGetMainId()/10]);
               }
             else
               {
                 GPIO_Write( SEGMENT_PORT,bSegmentNumberArray[bCharge/10]);
                 if(SystemTickGetMsecond()-iSegmentShowTime>4000)
                      iSegmentShowTime=SystemTickGetMsecond();
               }
             
           }

       }
     //! Normal mode
     if((CapturekeyGetWorkMode()==CAPTUREKEY_NORMAL_MODE)||
        (CapturekeyGetWorkMode()==CAPTUREKEY_START_MODE))
       {
         GPIO_SetBits( SEGMENT_COM_1_PORT, SEGMENT_COM_1_PIN);
         GPIO_SetBits( SEGMENT_COM_2_PORT, SEGMENT_COM_2_PIN);
         GPIO_Write( SEGMENT_PORT,64);          
       }

    }
}