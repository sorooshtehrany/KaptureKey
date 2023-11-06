#include "..\Include\audio.h"
#include "..\Include\sdcard.h"
#include "..\Include\CaptureKey.h"
//******************************************************************************
//! Variable -------------------------------------------------------------------
u8  bAudioPlayBuf1[AUDIO_PLAY_MAX_BUFFER];
u8  bAudioPlayBuf2[AUDIO_PLAY_MAX_BUFFER];
u8  bAudioPlayBuf3[AUDIO_PLAY_MAX_BUFFER];
u8  bAudioPlayBuf4[AUDIO_PLAY_MAX_BUFFER];

u8  bAudioPlayBufInUsed=0;
u16 sAudioPlayIrqDataCounter=0;
u8  bAudioSegmentRefreshConter=0;

bool bAudioPlayBuf1UpdateFlag=FALSE;
bool bAudioPlayBuf2UpdateFlag=FALSE;
bool bAudioPlayBuf3UpdateFlag=FALSE;
bool bAudioPlayBuf4UpdateFlag=FALSE;
bool bAudioCastleCapturedFlag=FALSE;

u32 iAudioTickBuffer=0;


u8  bAudioRxDataArray[20];
u8  bAudioRxDataLength=0;
u8  bAudioTxDataArray[20];
u8  bAudioTxDataLength=0;
//******************************************************************************
//! \brief  Configure buzzer gpio and pwm timer
//******************************************************************************
void AudioInit( void )
{
  
    GPIO_Init(AUDIO_POWER,AUDIO_POWER_MODE); 
    //! Enable the Clock of DAC 
    CLK_PeripheralClockConfig( CLK_Peripheral_DAC, ENABLE );
    
    //! DAC1 init -------------------------------------------
    //! dac deinit
    DAC_DeInit();
    //! Init DAC channel 1
    DAC_Init( DAC_Channel_1, DAC_Trigger_Software, DAC_OutputBuffer_Enable );
    //! Enable DAC Channel1
    DAC_Cmd( DAC_Channel_1, ENABLE );
     //! set a dummy value
    DAC_SetChannel1Data( DAC_Align_8b_R, 128 );
    //! software trigger init
    DAC_SoftwareTriggerCmd( DAC_Channel_1, ENABLE );
   
    //! TIM3 Init ------------------------------------------
    //! Enable TIM3 clock 
    CLK_PeripheralClockConfig( CLK_Peripheral_TIM3, ENABLE );
    //! TIM3 Deinit
    TIM3_DeInit();
    //! Time base configuration (counter value) ( 11025 Hz == 1451) (16000 hz == 1000) (22050 hz == 725)(44100 hz == 362)
    TIM3_TimeBaseInit( TIM3_Prescaler_1, TIM3_CounterMode_Up, 1000 );
    //! Clear TIM3 update flag 
    TIM3_ClearFlag( TIM3_FLAG_Update );
    //! Enable update interrupt 
    TIM3_ITConfig( TIM3_IT_Update, ENABLE );
    //! Enable TIM3
    TIM3_Cmd( DISABLE );
} 
//******************************************************************************
//! \brief  Audio start play 
//! \param  bName : audio track name
//******************************************************************************
void AudioStartPlay( const char* bName )
{
    iAudioTickBuffer=SystemTickGetSecond();  
    //! Open file from microsd
    sdcard_open_file(bName);
    GPIO_SetBits(AUDIO_POWER);
    
    //! Fill all buffers in start-up
    sdcard_read_bytes( bAudioPlayBuf1, AUDIO_PLAY_MAX_BUFFER );
    sdcard_read_bytes( bAudioPlayBuf2, AUDIO_PLAY_MAX_BUFFER );
    sdcard_read_bytes( bAudioPlayBuf3, AUDIO_PLAY_MAX_BUFFER );
    sdcard_read_bytes( bAudioPlayBuf4, AUDIO_PLAY_MAX_BUFFER );
    
    //! Set bAudioPlayBufInUsed flag to first buffer
    bAudioPlayBufInUsed=1;
    //! reset audio play timer interrupt data counter
    sAudioPlayIrqDataCounter=0;
    
    //Set all update flag to false
    bAudioPlayBuf1UpdateFlag = FALSE;
    bAudioPlayBuf2UpdateFlag = FALSE;
    bAudioPlayBuf3UpdateFlag = FALSE;
    bAudioPlayBuf4UpdateFlag = FALSE;
    
   
    DAC_Cmd( DAC_Channel_1, ENABLE );
    TIM3_Cmd( ENABLE );
}
//******************************************************************************
//! \brief  audio main play controll
//******************************************************************************
void AudioMainPlay( void )
{
    //! check audio status
        //! check audio play buffer update flag for new update
        if( bAudioPlayBuf1UpdateFlag==TRUE ) 
        {
            sdcard_read_bytes( bAudioPlayBuf1, AUDIO_PLAY_MAX_BUFFER );
            bAudioPlayBuf1UpdateFlag=FALSE;
        }
        //! check audio play buffer update flag for new update
        if( bAudioPlayBuf2UpdateFlag==TRUE ) 
        {
            sdcard_read_bytes( bAudioPlayBuf2, AUDIO_PLAY_MAX_BUFFER );
            bAudioPlayBuf2UpdateFlag=FALSE;
        }
        //! check audio play buffer update flag for new update
        if( bAudioPlayBuf3UpdateFlag==TRUE ) 
        {
            sdcard_read_bytes( bAudioPlayBuf3, AUDIO_PLAY_MAX_BUFFER );
            bAudioPlayBuf3UpdateFlag=FALSE;
        }
        //! check audio play buffer update flag for new update
        if( bAudioPlayBuf4UpdateFlag==TRUE ) 
        {
            sdcard_read_bytes( bAudioPlayBuf4, AUDIO_PLAY_MAX_BUFFER );
            bAudioPlayBuf4UpdateFlag=FALSE;
        }
}
//******************************************************************************
//! \brief  audio timer play irq handeler
//******************************************************************************
void AudioTimPlayIrqHandeler( void )
{
    u8 bData;
    //! get data from proper buffer
    if( bAudioPlayBufInUsed==1 )
        bData=bAudioPlayBuf1[sAudioPlayIrqDataCounter++];
    else if( bAudioPlayBufInUsed==2 )
        bData=bAudioPlayBuf2[sAudioPlayIrqDataCounter++];
    else if( bAudioPlayBufInUsed==3 )
        bData=bAudioPlayBuf3[sAudioPlayIrqDataCounter++];
    else if( bAudioPlayBufInUsed==4 )
        bData=bAudioPlayBuf4[sAudioPlayIrqDataCounter++];

    //Set data in DAC
    DAC_SetChannel1Data( DAC_Align_8b_R,bData);
    //DAC_SetChannel2Data( DAC_Align_8b_R, data);
     
    DAC_SoftwareTriggerCmd( DAC_Channel_1, ENABLE );
    //DAC_SoftwareTriggerCmd( DAC_Channel_2, ENABLE );
    
    //! refresh segment because read sdcart is so long time
    bAudioSegmentRefreshConter=(bAudioSegmentRefreshConter+1)%32;
    if(bAudioSegmentRefreshConter==0)
    SegmentMain();
    
    //! Update flags and reset counter value
    if( sAudioPlayIrqDataCounter >= AUDIO_PLAY_MAX_BUFFER ) 
    {
        sAudioPlayIrqDataCounter=0;
        
        if( bAudioPlayBufInUsed==1 ) 
        {
            bAudioPlayBufInUsed=2;
            bAudioPlayBuf1UpdateFlag=TRUE;
        }
        else if( bAudioPlayBufInUsed==2 )
        {
            bAudioPlayBufInUsed=3;
            bAudioPlayBuf2UpdateFlag=TRUE;
        }
        else if( bAudioPlayBufInUsed==3 )
        {
            bAudioPlayBufInUsed=4;
            bAudioPlayBuf3UpdateFlag=TRUE;
        }
        else if( bAudioPlayBufInUsed==4 )
        {
            bAudioPlayBufInUsed=1;
            bAudioPlayBuf4UpdateFlag=TRUE;
        }
    }    
    TIM3_ClearFlag( TIM3_FLAG_Update );
}
//******************************************************************************
//! \brief  audio main controll
//******************************************************************************
void AudioMain( void )
{  
  if((CapturekeyGetWorkMode()==CAPTUREKEY_PUSHED_KEY_MODE)||
     (CapturekeyGetWorkMode()==CAPTUREKEY_STOP_MODE)||
     (CapturekeyGetWorkMode()==CAPTUREKEY_WAIT_FOR_PUSHING_KEY_MODE)) 
    {
      if(SystemTickGetSecond()-iAudioTickBuffer<5)
          AudioMainPlay();
        else
          {
           DAC_Cmd( DAC_Channel_1, DISABLE );
           TIM3_Cmd( DISABLE );
           GPIO_ResetBits(AUDIO_POWER);
          }      
    }          
  else
    {
      DAC_Cmd( DAC_Channel_1, DISABLE );
      TIM3_Cmd( DISABLE );
      GPIO_ResetBits(AUDIO_POWER);
    }
}
