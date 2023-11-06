#include "..\Include\usart.h"

//******************************************************************************
//! Variable -------------------------------------------------------------------
u8 bUsartDataRxCounter=0;
u8 bUsartDataRxFlag=0;
u8 bUsartDataRxBuffer[10];
u8 bUsartDataRxLen=0;
//******************************************************************************
//! \brief  Configure usart
//******************************************************************************
void UsartInit( void )
{
    SYSCFG_REMAPPinConfig( REMAP_Pin_USART1TxRxPortA, ENABLE );
    
    //! Enable USART clock 
    CLK_PeripheralClockConfig( CLK_Peripheral_USART1, ENABLE );
    
    //! Configure USART Tx as alternate function push-pull  (software pull up)
    GPIO_ExternalPullUpConfig( USART_TX_GPIO, ENABLE );
    //GPIO_Init( DIAG_TX_PORT, DIAG_TX_PIN, GPIO_Mode_Out_OD_HiZ_Fast );
    
    //! Configure USART Rx as alternate function push-pull  (software pull up)*/
    GPIO_ExternalPullUpConfig( USART_RX_GPIO, ENABLE );
    //GPIO_Init( DIAG_RX_PORT, DIAG_RX_PIN, GPIO_Mode_In_PU_No_IT );
    
    //! USART configuration 
    USART_DeInit( USART1 );
    USART_Init( USART1, (u32) 19200, USART_WordLength_8b, USART_StopBits_1,
                USART_Parity_No, (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx) );

    //! Enable the USART Receive interrupt
    USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );
	
    //! Enable the USART Transmit interrupt
    //USART_ITConfig( USART1, USART_IT_TXE, DISABLE );
    
    USART_Cmd( USART1, ENABLE ); 
}
//******************************************************************************
//! \brief  usart send data
//! \param  bData : usart data send
//******************************************************************************
void UsartSendData( u8 bData )
{
    //! Write a data to the USART 
    USART_SendData8( USART1, bData );
  
    //! Loop until the end of transmission 
    while ( USART_GetFlagStatus( USART1, USART_FLAG_TC ) == RESET );
}
//******************************************************************************
//! \brief  usart rx interrupt handeler
//******************************************************************************
void UsartRxIrqHandeler( void )
{
    u8 bStatus=0;
    u8 bDataRx=0;
    
    bStatus=USART1->SR;
    bDataRx=USART_ReceiveData8( USART1 );
       
    if( (bStatus & (USART_FLAG_OR | USART_FLAG_NF | USART_FLAG_FE | USART_FLAG_PE)) == 0x00 ) 
    {
        //! check data cmd ( 0xAA get fire clash data cmd ) 
        if( bDataRx==0xAA && bUsartDataRxCounter==0 )
        {
            bUsartDataRxCounter++;
        }
        else if( bUsartDataRxCounter==1 )
        {
            bUsartDataRxCounter++;
            //! sensor ID
            bUsartDataRxBuffer[0]=bDataRx;
        }
        else if( bUsartDataRxCounter==2 )
        {
            bUsartDataRxCounter=0;
            //! gun fire ID
            bUsartDataRxBuffer[1]=bDataRx;
            
            bUsartDataRxFlag=1;
            bUsartDataRxLen=2;
        }
    }
    
    USART_ClearFlag( USART1, USART_FLAG_RXNE );   
}
//******************************************************************************
//! \brief  return usart rx get data flag
//! \retval rx data flag
//******************************************************************************
u8 UsartRxGetFlag( void )
{
    return bUsartDataRxFlag;
}
//******************************************************************************
//! \brief  set usart rx flag
//! \param  bRxFlag : rx data flag
//******************************************************************************
void UsartRxSetFlag( u8 bRxFlag )
{
    bUsartDataRxFlag=bRxFlag;
}
//******************************************************************************
//! \brief  return usart data rx
//! \param  bDataArray ; data rx array
//! \retval data rx len
//******************************************************************************
u8 UsartRxGetData( u8* bDataArray )
{
    for( u8 i=0 ; i<bUsartDataRxLen ; i++ )
        bDataArray[i]=bUsartDataRxBuffer[i];
        
    return bUsartDataRxLen;
}