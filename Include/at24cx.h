#ifndef AT24CX_H
#define AT24CX_H

#include "..\STM8L Library\Include\stm8l15x.h"
//******************************************************************************
//! Define ---------------------------------------------------------------------
#define AT24_C256_128
#define AT24_BREAK_RST              iBreakTime=0
#define AT24_BREAK_RUN              iBreakTime++; if(iBreakTime>9999)break

#define AT24_I2C                    I2C1
#define AT24_I2C_CLK                CLK_Peripheral_I2C1
#define AT24_I2C_SCL_PIN            GPIO_Pin_1                  
#define AT24_I2C_SCL_GPIO_PORT      GPIOC                   
#define AT24_I2C_SDA_PIN            GPIO_Pin_0               
#define AT24_I2C_SDA_GPIO_PORT      GPIOC    
#define AT24_I2C_SPEED              50000     
#define AT24_I2C_SLAVE_ADDRESS7     0xA0

#define AT24_I2C_DMA                DMA1
#define AT24_I2C_DMA_CHANNEL_TX     DMA1_Channel3
#define AT24_I2C_DMA_CHANNEL_RX     DMA1_Channel0
#define AT24_I2C_DMA_FLAG_TX_TC     DMA1_FLAG_TC3
#define AT24_I2C_DMA_FLAG_RX_TC     DMA1_FLAG_TC0
#define AT24_I2C_DR_Address         ((u16)0x005216)
//! Defintions for the state of the DMA transfer   
#define AT24_DMA_STATE_READY        0
#define AT24_DMA_STATE_BUSY         1

#define AT24_DIRECTION_TX           0
#define AT24_DIRECTION_RX           1

//! For M24C32 and M24C64 devices, E0,E1 and E2 pins are all used for device 
//!  address selection (ne need for additional address lines). According to the 
//!   Harware connection on the board (on STM810C-EVAL board E0 = E1 = E2 = 0) 
#define AT24_HW_ADDRESS         0xA0   /* E0 = E1 = E2 = 0 */ 

#if defined (AT24_C08)
 #define AT24_PAGESIZE          16
#elif defined (AT24_C64_32)
 #define AT24_PAGESIZE          32
#elif defined (AT24_C256_128)
 #define AT24_PAGESIZE          64
#endif
   
//! Maximum timeout value for counting before exiting waiting loop on DMA 
//!  Trasnfer Complete. This value depends directly on the maximum page size and
//!   the sytem clock frequency. 
#define AT24_TIMEOUT_MAX        0x10000
#define AT24_TIMEOUT            0x01000
//******************************************************************************
//! Variable -------------------------------------------------------------------

//******************************************************************************
//! Function -------------------------------------------------------------------
void At24Deinit                 ( void );
void At24Init                   ( void );
void At24WriteByte              ( u8* pBuffer, u16 WriteAddr );
void At24WritePage              ( u8* pBuffer, u16 WriteAddr, u8* NumByteToWrite );
void At24WriteBuffer            ( u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite );
void At24ReadBuffer             ( u8* pBuffer, u16 ReadAddr, u16* NumByteToRead );
void At24WaitStandbyState       ( void );
void At24DmaRxIrqHandler        ( void );
void At24DmaTxIrqHandler        ( void );

u8   At24SaveByte               ( u8 address, u8 data );
u8   At24ReadByte               ( u8 address, u8 *data );
//******************************************************************************

#endif // AT24CX_H