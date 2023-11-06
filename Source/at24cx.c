#include "../Include/at24cx.h"

//******************************************************************************
//! Variable -------------------------------------------------------------------
__IO u16  sAt24Address = 0xA0;
__IO u32  iAt24Timeout = AT24_TIMEOUT_MAX;
__IO u16* sAt24DataReadPointer;
__IO u8*  bAt24DataWritePointer;
__IO u8   bAt24DataNum;
u32 iBreakTime=0;
//******************************************************************************
//! \brief  DeInitializes peripherals used by the I2C EEPROM driver.
//******************************************************************************
void At24Deinit(void)
{
    //! AT24_I2C Peripheral Disable 
    I2C_Cmd(AT24_I2C, DISABLE);
    
    //! AT24_I2C DeInit 
    I2C_DeInit(AT24_I2C);
    
    //!< AT24_I2C Periph clock disable 
    CLK_PeripheralClockConfig(AT24_I2C_CLK, DISABLE);
    
    //!< GPIO configuration 
    //!< Configure AT24_I2C pins: SCL
    GPIO_Init(AT24_I2C_SCL_GPIO_PORT, AT24_I2C_SCL_PIN, GPIO_Mode_In_PU_No_IT);
    
    //!< Configure AT24_I2C pins: SDA 
    GPIO_Init(AT24_I2C_SDA_GPIO_PORT, AT24_I2C_SDA_PIN, GPIO_Mode_In_PU_No_IT);
    
    //! Disable and Deinitialize the DMA channels 
    DMA_Cmd(AT24_I2C_DMA_CHANNEL_TX, DISABLE);
    DMA_Cmd(AT24_I2C_DMA_CHANNEL_RX, DISABLE);
    DMA_DeInit(AT24_I2C_DMA_CHANNEL_TX);
    DMA_DeInit(AT24_I2C_DMA_CHANNEL_RX);
}

//******************************************************************************
//! \brief  Initializes peripherals used by the I2C EEPROM driver.
//******************************************************************************
void At24Init( void )
{
    //!< AT24_I2C Periph clock enable 
    CLK_PeripheralClockConfig(AT24_I2C_CLK, ENABLE);
    
    //!< Enable the DMA clock 
    CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
    
    //! I2C TX DMA Channel configuration 
    DMA_DeInit(AT24_I2C_DMA_CHANNEL_TX);
    DMA_Init(AT24_I2C_DMA_CHANNEL_TX,
             0, //! This parameter will be configured durig communication 
             AT24_I2C_DR_Address,
             0xFF, //! This parameter will be configured durig communication 
             DMA_DIR_PeripheralToMemory,//! This parameter will be configured durig communication 
             DMA_Mode_Normal,
             DMA_MemoryIncMode_Inc,
             DMA_Priority_VeryHigh,
             DMA_MemoryDataSize_Byte);
    
    //! I2C RX DMA Channel configuration 
    DMA_DeInit(AT24_I2C_DMA_CHANNEL_RX);
    DMA_Init(AT24_I2C_DMA_CHANNEL_RX, 0, //! This parameter will be configured durig communication 
             AT24_I2C_DR_Address,
             0xFF, //! This parameter will be configured durig communication 
             DMA_DIR_PeripheralToMemory,//! This parameter will be configured durig communication 
             DMA_Mode_Normal,
             DMA_MemoryIncMode_Inc,
             DMA_Priority_VeryHigh,
             DMA_MemoryDataSize_Byte);
    
    //!< I2C configuration 
    //! AT24_I2C Peripheral Enable 
    I2C_Cmd(AT24_I2C, ENABLE);
    //! AT24_I2C configuration after enabling it 
    I2C_Init( AT24_I2C, AT24_I2C_SPEED, AT24_I2C_SLAVE_ADDRESS7, I2C_Mode_I2C, I2C_DutyCycle_2,
             I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit );
    
    //! Enable the AT24_I2C peripheral DMA requests 
    I2C_DMACmd(AT24_I2C, ENABLE);
    
    //!< Select the EEPROM address according to the state of E0, E1, E2 pins 
    sAt24Address = AT24_HW_ADDRESS;    

    //! SR2 register flags 
    I2C_ClearFlag( AT24_I2C, I2C_FLAG_SMBALERT );   
    I2C_ClearFlag( AT24_I2C, I2C_FLAG_TIMEOUT );     
    I2C_ClearFlag( AT24_I2C, I2C_FLAG_WUFH );       
    I2C_ClearFlag( AT24_I2C, I2C_FLAG_PECERR );     
    I2C_ClearFlag( AT24_I2C, I2C_FLAG_OVR );        
    I2C_ClearFlag( AT24_I2C, I2C_FLAG_AF );         
    I2C_ClearFlag( AT24_I2C, I2C_FLAG_ARLO );       
    I2C_ClearFlag( AT24_I2C, I2C_FLAG_BERR );      

}

//******************************************************************************
//! \brief  Writes one byte to the I2C EEPROM.
//! \param  pBuffer : pointer to the buffer  containing the data to be written to the EEPROM.
//! \param  WriteAddr : EEPROM's internal address to write to.
//******************************************************************************
void At24WriteByte(u8* pBuffer, u16 WriteAddr)
{
    //!< Send STRAT condition 
    I2C_GenerateSTART(AT24_I2C, ENABLE);
    
    //!< Test on EV5 and clear it 
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {AT24_BREAK_RUN;}
    
    //!< Send EEPROM address for write 
    I2C_Send7bitAddress(AT24_I2C, (u8)sAt24Address, I2C_Direction_Transmitter);
    
    //!< Test on EV6 and clear it 
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {AT24_BREAK_RUN;}
    
    //!< Send the EEPROM's internal address to write to : MSB of the address first 
    I2C_SendData(AT24_I2C, (u8)((WriteAddr & 0xFF00) >> 8));
    
    //!< Test on EV8 and clear it 
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {AT24_BREAK_RUN;}
    
    //!< Send the EEPROM's internal address to write to : LSB of the address 
    I2C_SendData(AT24_I2C, (u8)(WriteAddr & 0x00FF));
    
    //!< Test on EV8 and clear it 
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {AT24_BREAK_RUN;}
    
    //!< Send the byte to be written 
    I2C_SendData(AT24_I2C, *pBuffer);
    
    //!< Test on EV8 and clear it 
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {AT24_BREAK_RUN;}
    
    //!< Send STOP condition 
    I2C_GenerateSTOP(AT24_I2C, ENABLE);
}

//******************************************************************************
//! \brief  Reads a block of data from the EEPROM.
//! \param  pBuffer : pointer to the buffer that receives the data read from the EEPROM.
//! \param  ReadAddr : EEPROM's internal address to read from.
//! \param  NumByteToRead : pointer to the variable holding number of bytes to read from the EEPROM.

//! \note The variable pointed by NumByteToRead is reset to 0 when all the
//!   data are read from the EEPROM. Application should monitor this
//!    variable in order know when the transfer is complete.

//! \note When number of data to be read is higher than 1, this function just
//!   configure the communication and enable the DMA channel to transfer data.
//!    Meanwhile, the user application may perform other tasks.
//!     When number of data to be read is 1, then the DMA is not used.
//******************************************************************************
void At24ReadBuffer(u8* pBuffer, u16 ReadAddr, u16* NumByteToRead)
{
    __IO u32 timeout = 0xFFFF;
    
    //!< Wait the end of last communication 
    for (;timeout > 0; timeout--);
    
    //! Set the pointer to the Number of data to be read. This pointer will be used
    //!  by the DMA Transfer Complete interrupt Handler in order to reset the 
    //!   variable to 0. User should check on this variable in order to know if the 
    //!    DMA transfer has been completed or not. 
    sAt24DataReadPointer = NumByteToRead;
    
    //!< While the bus is busy 
    AT24_BREAK_RST;
    while (I2C_GetFlagStatus(AT24_I2C, I2C_FLAG_BUSY))
    {AT24_BREAK_RUN;}
    
    //!< Send START condition 
    I2C_GenerateSTART(AT24_I2C, ENABLE);
    
    //!< Test on EV5 and clear it 
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {AT24_BREAK_RUN;}
    
    //!< Send EEPROM address for write 
    I2C_Send7bitAddress(AT24_I2C, (u8)sAt24Address, I2C_Direction_Transmitter);
    
    //!< Test on EV6 and clear it 
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {AT24_BREAK_RUN;}
    
    //!< Send the EEPROM's internal address to read from: MSB of the address first 
    I2C_SendData(AT24_I2C, (u8)((ReadAddr & 0xFF00) >> 8));
    
    //!< Test on EV8 and clear it 
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {AT24_BREAK_RUN;}
    
    //!< Send the EEPROM's internal address to read from: LSB of the address 
    I2C_SendData(AT24_I2C, (u8)(ReadAddr & 0x00FF));
    
    //!< Test on EV8 and clear it 
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {AT24_BREAK_RUN;}
    
    //!< Send STRAT condition a second time 
    I2C_GenerateSTART(AT24_I2C, ENABLE);
    
    //!< Test on EV5 and clear it 
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {AT24_BREAK_RUN;}
    
    //!< Send EEPROM address for read 
    I2C_Send7bitAddress(AT24_I2C, (u8)sAt24Address, I2C_Direction_Receiver);
    
    //!< Test on EV6 and clear it 
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {AT24_BREAK_RUN;}
    
    //! If number of data to be read is 1, then DMA couldn't be used 
    if ((u16)(*NumByteToRead) < 2)
    {
        //!< Disable Acknowledgement 
        I2C_AcknowledgeConfig(AT24_I2C, DISABLE);
        
        //!< Send STOP Condition 
        I2C_GenerateSTOP(AT24_I2C, ENABLE);
        
        //!< Test on EV7 and clear it 
        AT24_BREAK_RST;
        while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
        {AT24_BREAK_RUN;}
        
        //!< Read a byte from the EEPROM 
        *pBuffer = I2C_ReceiveData(AT24_I2C);
        
        //!< Decrement the read bytes counter 
        (u16)(*NumByteToRead)--;
        
        //!< Enable Acknowledgement to be ready for another reception 
        I2C_AcknowledgeConfig(AT24_I2C, ENABLE);
    }
    //! DMA could be used for number of data higher than 1 
    else
    {
        //! Configure the DMA Rx Channel with the buffer address and the buffer size 
        DMA_Init(AT24_I2C_DMA_CHANNEL_RX, (u16)pBuffer, AT24_I2C_DR_Address, (u8)(*NumByteToRead),
                 DMA_DIR_PeripheralToMemory, DMA_Mode_Normal, DMA_MemoryIncMode_Inc,
                 DMA_Priority_VeryHigh, DMA_MemoryDataSize_Byte);
        
        //! Enable the DMA Channels Interrupts 
        DMA_ITConfig(AT24_I2C_DMA_CHANNEL_TX, DMA_ITx_TC, ENABLE);
        DMA_ITConfig(AT24_I2C_DMA_CHANNEL_RX, DMA_ITx_TC, ENABLE);
        
        //! Inform the DMA that the next End Of Transfer Signal will be the last one 
        I2C_DMALastTransferCmd(AT24_I2C, ENABLE);
        
        //! Enable the DMA Rx Channel 
        DMA_Cmd(AT24_I2C_DMA_CHANNEL_RX, ENABLE);
        
        //! Global DMA Enable 
        DMA_GlobalCmd(ENABLE);
    }
}
//******************************************************************************
//! \brief  Writes buffer of data to the I2C EEPROM.
//! \param  pBuffer : pointer to the buffer  containing the data to be written to the EEPROM.
//! \param  WriteAddr : EEPROM's internal address to write to.
//! \param  NumByteToWrite : number of bytes to write to the EEPROM.
//******************************************************************************
void At24WriteBuffer( u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite )
{
    u8  NumOfPage=0; 
    u8  NumOfSingle=0; 
    u8  count=0;
    u16 Addr=0;
    
    Addr = WriteAddr % AT24_PAGESIZE;
    count = (u8)(AT24_PAGESIZE - (u16)Addr);
    NumOfPage =  (u8)(NumByteToWrite / AT24_PAGESIZE);
    NumOfSingle = (u8)(NumByteToWrite % AT24_PAGESIZE);
    
    //!< If WriteAddr is AT24_PAGESIZE aligned  
    if (Addr == 0)
    {
        //!< If NumByteToWrite < AT24_PAGESIZE 
        if (NumOfPage == 0)
        {
            //! Store the number of data to be written 
            bAt24DataNum = NumOfSingle;
            //! Start writing data 
            At24WritePage(pBuffer, WriteAddr, (u8*)(&bAt24DataNum));
            //! Wait transfer through DMA to be complete 
            iAt24Timeout = AT24_TIMEOUT_MAX;
            while ((bAt24DataNum > 0) && (iAt24Timeout-- > 0))
            {}
            At24WaitStandbyState();
        }
        //!< If NumByteToWrite > AT24_PAGESIZE 
        else
        {
            while (NumOfPage--)
            {
                //! Store the number of data to be written 
                bAt24DataNum = AT24_PAGESIZE;
                At24WritePage(pBuffer, WriteAddr, (u8*)(&bAt24DataNum));
                //! Wait transfer through DMA to be complete 
                iAt24Timeout = AT24_TIMEOUT_MAX;
                while ((bAt24DataNum > 0) && (iAt24Timeout-- > 0))
                {}
                At24WaitStandbyState();
                WriteAddr +=  AT24_PAGESIZE;
                pBuffer += AT24_PAGESIZE;
            }
            
            if ( NumOfSingle!=0 )
            {
                //! Store the number of data to be written 
                bAt24DataNum = NumOfSingle;
                At24WritePage(pBuffer, WriteAddr, (u8*)(&bAt24DataNum));
                //! Wait transfer through DMA to be complete 
                iAt24Timeout = AT24_TIMEOUT_MAX;
                while ((bAt24DataNum > 0) && (iAt24Timeout-- > 0))
                {}
                At24WaitStandbyState();
            }
        }
    }
    //!< If WriteAddr is not AT24_PAGESIZE aligned
    else
    {
        //!< If NumByteToWrite < AT24_PAGESIZE 
        if (NumOfPage == 0)
        {
            //!< If the number of data to be written is more than the remaining space in the current page: 
                if (NumByteToWrite > count)
                {
                    //! Store the number of data to be written 
                    bAt24DataNum = count;
                    //!< Write the data conained in same page 
                    At24WritePage(pBuffer, WriteAddr, (u8*)(&bAt24DataNum));
                    //! Wait transfer through DMA to be complete 
                    iAt24Timeout = AT24_TIMEOUT_MAX;
                    while ((bAt24DataNum > 0) && (iAt24Timeout-- > 0))
                    {}
                    At24WaitStandbyState();
                    
                    //! Store the number of data to be written 
                    bAt24DataNum = (u8)(NumByteToWrite - count);
                    //!< Write the remaining data in the following page 
                    At24WritePage((u8*)(pBuffer + count), (WriteAddr + count), (u8*)(&bAt24DataNum));
                    //! Wait transfer through DMA to be complete 
                    iAt24Timeout = AT24_TIMEOUT_MAX;
                    while ((bAt24DataNum > 0) && (iAt24Timeout-- > 0))
                    {}
                    At24WaitStandbyState();
                }
                else
                {
                    //! Store the number of data to be written 
                    bAt24DataNum = NumOfSingle;
                    At24WritePage(pBuffer, WriteAddr, (u8*)(&bAt24DataNum));
                    //! Wait transfer through DMA to be complete 
                    iAt24Timeout = AT24_TIMEOUT_MAX;
                    while ((bAt24DataNum > 0) && (iAt24Timeout-- > 0))
                    {}
                    At24WaitStandbyState();
                }
        }
        //!< If NumByteToWrite > AT24_PAGESIZE 
        else
        {
            NumByteToWrite -= count;
            NumOfPage = (u8)(NumByteToWrite / AT24_PAGESIZE);
            NumOfSingle = (u8)(NumByteToWrite % AT24_PAGESIZE);
            
            if (count != 0)
            {
                //! Store the number of data to be written 
                bAt24DataNum = count;
                At24WritePage(pBuffer, WriteAddr, (u8*)(&bAt24DataNum));
                //! Wait transfer through DMA to be complete 
                iAt24Timeout = AT24_TIMEOUT_MAX;
                while ((bAt24DataNum > 0) && (iAt24Timeout-- > 0))
                {}
                At24WaitStandbyState();
                WriteAddr += count;
                pBuffer += count;
            }
            
            while (NumOfPage--)
            {
                //! Store the number of data to be written 
                bAt24DataNum = AT24_PAGESIZE;
                At24WritePage(pBuffer, WriteAddr, (u8*)(&bAt24DataNum));
                //! Wait transfer through DMA to be complete 
                iAt24Timeout = AT24_TIMEOUT_MAX;
                while ((bAt24DataNum > 0) && (iAt24Timeout-- > 0))
                {}
                At24WaitStandbyState();
                WriteAddr +=  AT24_PAGESIZE;
                pBuffer += AT24_PAGESIZE;
            }
            if (NumOfSingle != 0)
            {
                //! Store the number of data to be written 
                bAt24DataNum = NumOfSingle;
                At24WritePage(pBuffer, WriteAddr, (u8*)(&bAt24DataNum));
                //! Wait transfer through DMA to be complete 
                iAt24Timeout = AT24_TIMEOUT_MAX;
                while ((bAt24DataNum > 0) && (iAt24Timeout-- > 0))
                {}
                At24WaitStandbyState();
            }
        }
    }
}
//******************************************************************************
//! \brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
//! \note   The number of byte can't exceed the EEPROM page size.
//! \param  pBuffer : pointer to the buffer containing the data to be written to the EEPROM.
//! \param  WriteAddr : EEPROM's internal address to write to.
//! \param  NumByteToWrite : pointer to the variable holding number of bytes to written to the EEPROM.

//! \note The variable pointed by NumByteToWrite is reset to 0 when all the
//!   data are read from the EEPROM. Application should monitor this
//!    variable in order know when the transfer is complete.

//! \note When number of data to be written is higher than 1, this function just
//!   configure the communication and enable the DMA channel to transfer data.
//!    Meanwhile, the user application may perform other tasks.
//!     When number of data to be written is 1, then the DMA is not used.
//******************************************************************************
void At24WritePage(u8* pBuffer, u16 WriteAddr, u8* NumByteToWrite)
{
    __IO u32 timeout = 0xFFFF;
    
    //!< Wait the end of last communication 
    for (;timeout > 0; timeout--);
    
    //! Set the pointer to the Number of data to be written. This pointer will be used
    //!  by the DMA Transfer Completer interrupt Handler in order to reset the 
    //!   variable to 0. User should check on this variable in order to know if the 
    //!    DMA transfer has been complete or not. 
    bAt24DataWritePointer = NumByteToWrite;
    
    //!< While the bus is busy 
    AT24_BREAK_RST;
    while (I2C_GetFlagStatus(AT24_I2C, I2C_FLAG_BUSY))
    {AT24_BREAK_RUN;}
    
    //!< Send START condition 
    I2C_GenerateSTART(AT24_I2C, ENABLE);
    
    //!< Test on EV5 and clear it 
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {AT24_BREAK_RUN;}
    
    //!< Send EEPROM address for write 
    I2C_Send7bitAddress(AT24_I2C, (u8)sAt24Address, I2C_Direction_Transmitter);
    
    //!< Test on EV6 and clear it 
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {AT24_BREAK_RUN;}
    
    //!< Send the EEPROM's internal address to write to : MSB of the address first */
    I2C_SendData(AT24_I2C, (u8)((WriteAddr & 0xFF00) >> 8));
    
    //!< Test on EV8 and clear it */
    AT24_BREAK_RST;
    while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {AT24_BREAK_RUN;}
    
    //!< Send the EEPROM's internal address to write to : LSB of the address */
    I2C_SendData(AT24_I2C, (u8)(WriteAddr & 0x00FF));
    
    //!< Test on EV8 and clear it */
    AT24_BREAK_RST;
    while (! I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {AT24_BREAK_RUN;}
    
    //! If number of data to be written is 1, then DMA couldn't be used 
    if ((u16)(*NumByteToWrite) < 2)
    {
        //!< Send the byte to be written 
        I2C_SendData(AT24_I2C, *pBuffer);
        
        //!< Test on EV8 and clear it 
        AT24_BREAK_RST;
        while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {AT24_BREAK_RUN;}
        
        //!< Send STOP condition 
        I2C_GenerateSTOP(AT24_I2C, ENABLE);
        
        (u8)(*NumByteToWrite)--;
    }
    //! DMA could be used for number of data higher than 1 
    else
    {
        //! Configure the DMA Tx Channel with the buffer address and the buffer size 
        DMA_Init(AT24_I2C_DMA_CHANNEL_TX, (u16)pBuffer, AT24_I2C_DR_Address, (u8)(*NumByteToWrite),
                 DMA_DIR_MemoryToPeripheral, DMA_Mode_Normal, DMA_MemoryIncMode_Inc,
                 DMA_Priority_VeryHigh, DMA_MemoryDataSize_Byte);
        
        //! Enable the DMA Channels Interrupts 
        DMA_ITConfig(AT24_I2C_DMA_CHANNEL_TX, DMA_ITx_TC, ENABLE);
        DMA_ITConfig(AT24_I2C_DMA_CHANNEL_RX, DMA_ITx_TC, ENABLE);
     
        //! Enable the DMA Tx Channel 
        DMA_Cmd(AT24_I2C_DMA_CHANNEL_TX, ENABLE);
        
        //! Global DMA Enable 
        DMA_GlobalCmd(ENABLE);
    }
}
//******************************************************************************
//! \brief  Wait for EEPROM Standby state
//******************************************************************************
void At24WaitStandbyState( void )
{
    __IO u8 tempreg = 0;
    __IO u32 timeout = 0xFFFF;
    
    do
    {
        //!< Send START condition 
        I2C_GenerateSTART(AT24_I2C, ENABLE);
        
        //! Test on EEPROM_I2C EV5 and clear it 
        AT24_BREAK_RST;
        while (!I2C_GetFlagStatus(AT24_I2C, I2C_FLAG_SB))  /* EV5 */
        {AT24_BREAK_RUN;}
        
        //!< Send EEPROM address for write 
        I2C_Send7bitAddress(AT24_I2C, (u8)sAt24Address, I2C_Direction_Transmitter);
        
        //!< Wait for address aknowledgement 
        for (;timeout > 0; timeout--);
        
        //!< Read sEE SR1 register to clear pending flags 
        tempreg = I2C_ReadRegister(AT24_I2C, I2C_Register_SR1);
        
    }
    while (!(tempreg & 0x02));
    
    //!< Clear AF flag 
    I2C_ClearFlag(AT24_I2C, I2C_FLAG_AF);
    
    //!< STOP condition 
    I2C_GenerateSTOP(AT24_I2C, ENABLE);
}
//******************************************************************************
//! \brief  This function handles the DMA Tx Channel interrupt Handler.
//! \note This function should be called in the
//!   DMA1_CHANNEL2_3_IRQHandler in the stm8l15x_it.c file.

//!         INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler, 3)
//!         {
//!             AT24_I2C_DMA_TX_IRQHandler();
//!         }
//******************************************************************************
void At24DmaTxIrqHandler(void)
{
    //! Check if the DMA transfer is complete 
    if (DMA_GetFlagStatus(AT24_I2C_DMA_FLAG_TX_TC) != RESET)
    {
        //! Disable the DMA Tx Channel and Clear all its Flags 
        DMA_Cmd(AT24_I2C_DMA_CHANNEL_TX, DISABLE);
        DMA_ClearFlag(AT24_I2C_DMA_FLAG_TX_TC);
        
        //!< Wait till all data have been physically transferred on the bus 
        AT24_BREAK_RST;
        while (!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {AT24_BREAK_RUN;}
        
        //!< Send STOP condition 
        I2C_GenerateSTOP(AT24_I2C, ENABLE);
        
        //! Reset the variable holding the number of data to be written 
        *bAt24DataWritePointer = 0;
    }
}

//******************************************************************************
//! \brief  This function handles the DMA Rx Channel interrupt Handler.
//! \note This function should be called in the
//!   DMA1_CHANNEL0_1_IRQHandler in the stm8l15x_it.c file just as follow.

//!         INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler, 2)
//!         {
//!             AT24_I2C_DMA_RX_IRQHandler();
//!         }
//******************************************************************************
void At24DmaRxIrqHandler(void)
{
    //! Check if the DMA transfer is complete 
    if (DMA_GetFlagStatus(AT24_I2C_DMA_FLAG_RX_TC) != RESET)
    {
        //!< Send STOP Condition 
        I2C_GenerateSTOP(AT24_I2C, ENABLE);
        
        //! Disable the DMA Rx Channel and Clear all its Flags 
        DMA_Cmd(AT24_I2C_DMA_CHANNEL_RX, DISABLE);
        DMA_ClearFlag(AT24_I2C_DMA_FLAG_RX_TC);
        
        //! Reset the variable holding the number of data to be read 
        *sAt24DataReadPointer = 0;
    }
}
//*************************************************
//
//*************************************************
u8 At24SaveByte( u8 address, u8 data )
{
    u32 wC=0;
    
    //! Wait For EEPROM Ready
    while(I2C_GetFlagStatus(AT24_I2C, I2C_FLAG_BUSY))
    {
      if( wC++ > AT24_TIMEOUT )
        return 0;
    }
    
    /*!< Send START condition */
    I2C_GenerateSTART( AT24_I2C , ENABLE );
    
    wC=0;
    //! Wait for EV5
    while(!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
      if( wC++ > AT24_TIMEOUT )
        return 0;
    }
    
    I2C_Send7bitAddress(AT24_I2C, AT24_I2C_SLAVE_ADDRESS7 , I2C_Direction_Transmitter);
    
    wC=0;
    while(!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
      if( wC++ > AT24_TIMEOUT )
        return 0;
    }
    
    /*!< Send the EEPROM's internal address to write to : only one byte Address */
    I2C_SendData( AT24_I2C, address );
    
    wC=0;
    while(!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
      if( wC++ > AT24_TIMEOUT )
        return 0;
    }
    
    I2C_SendData(AT24_I2C, data );
    
    
    /*!< Send START condition  */
    I2C_GenerateSTOP( AT24_I2C , ENABLE );
    
    return 1;
}
//*************************************************
//
//*************************************************
u8 At24ReadByte( u8 address, u8 *data )
{
    u32 wC=0;
        //! Wait For EEPROM Ready
    while(I2C_GetFlagStatus(AT24_I2C, I2C_FLAG_BUSY))
    {
      if( wC++ > AT24_TIMEOUT )
        return 0;
    }
    
    /*!< Send START condition */
    I2C_GenerateSTART( AT24_I2C , ENABLE );
    
    wC=0;
    //! Wait for EV5
    while(!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
      if( wC++ > AT24_TIMEOUT )
        return 0;
    }
    
    I2C_Send7bitAddress(AT24_I2C, AT24_I2C_SLAVE_ADDRESS7 , I2C_Direction_Transmitter);
    
    wC=0;
    while(!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
      if( wC++ > AT24_TIMEOUT )
        return 0;
    }
    
    /*!< Send the EEPROM's internal address to read from : only one byte Address */
    I2C_SendData( AT24_I2C, address );
    
    wC=0;
    while(I2C_GetFlagStatus(AT24_I2C, I2C_FLAG_BTF) == RESET)
    {
      if( wC++ > AT24_TIMEOUT )
        return 0;
    }
    
    /*!< Send STRAT condition a second time */  
    I2C_GenerateSTART(AT24_I2C, ENABLE);
  
    wC=0;
    /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
    while(!I2C_CheckEvent(AT24_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
      if( wC++ > AT24_TIMEOUT )
        return 0;
    }
    
    /*!< Send EEPROM address for read */
    I2C_Send7bitAddress(AT24_I2C, AT24_I2C_SLAVE_ADDRESS7 , I2C_Direction_Receiver);  
    
    wC=0;
    while(I2C_GetFlagStatus(AT24_I2C, I2C_FLAG_ADDR) == RESET)
    {
      if( wC++ > AT24_TIMEOUT )
        return 0;
    }
    
    /*!< Disable Acknowledgement */
    I2C_AcknowledgeConfig(AT24_I2C, DISABLE);   
    
    disableInterrupts();
    
    /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
    (void)AT24_I2C->SR2;
    
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(AT24_I2C, ENABLE);
    
    /* Call User callback for critical section end (should typically re-enable interrupts) */
    enableInterrupts();
    
    wC=0;
    while(I2C_GetFlagStatus(AT24_I2C, I2C_FLAG_RXNE) == RESET)
    {
      if( wC++ > AT24_TIMEOUT )
        return 0;
    }
    
    *data = I2C_ReceiveData(AT24_I2C);
    
    wC=0;
    //! check CR2 register stop bit
    while(AT24_I2C->CR2 & 0x02)
    {
      if( wC++ > AT24_TIMEOUT )
        return 0;
    }
    
    I2C_AcknowledgeConfig(AT24_I2C, ENABLE);
    
    return 1;
}