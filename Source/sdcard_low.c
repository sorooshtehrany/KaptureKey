/**
  ******************************************************************************
  * @file microsd.c
  * @brief This file contains basic functions to control the MicroSD card device.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.1.1
  * @date 06/05/2009
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "..\Include\sdcard_low.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @addtogroup STM8_EVAL_SPI_SD
  * @brief      This file includes the SD card driver of STM8-EVAL boards.
  * @{
  */
/**
  * @brief  DeInitializes the SD/SD communication.
  * @param  None
  * @retval None
  */
static void MSD_LowLevel_DeInit(void)
{
  SPI_Cmd(SD_SPI, DISABLE); /*!< SD_SPI disable */

  /*!< SD_SPI Periph clock disable */
  CLK_PeripheralClockConfig(SD_SPI_CLK, DISABLE);

  /*!< Configure SD_SPI pins: SCK */
  GPIO_Init(SD_SPI_SCK_GPIO_PORT, SD_SPI_SCK_PIN, GPIO_Mode_In_FL_No_IT);

  /*!< Configure SD_SPI pins: MISO */
  GPIO_Init(SD_SPI_MISO_GPIO_PORT, SD_SPI_MISO_PIN, GPIO_Mode_In_FL_No_IT);

  /*!< Configure SD_SPI pins: MOSI */
  GPIO_Init(SD_SPI_MOSI_GPIO_PORT, SD_SPI_MOSI_PIN, GPIO_Mode_In_FL_No_IT);

  /*!< Configure SD_SPI_CS_PIN pin: SD Card CS pin */
  GPIO_Init(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_Mode_In_FL_No_IT);

  /*!< Configure SD_SPI_DETECT_PIN pin: SD Card detect pin */
  //GPIO_Init(SD_DETECT_GPIO_PORT, SD_DETECT_PIN, GPIO_Mode_In_FL_No_IT);
}

/**
  * @brief  Initializes the SD_SPI and CS pins.
  * @param  None
  * @retval None
  */
static void MSD_LowLevel_Init(void)
{
  /* Enable SPI clock */
  CLK_PeripheralClockConfig(SD_SPI_CLK, ENABLE);

  /* Set the MOSI,MISO and SCK at high level */
  GPIO_ExternalPullUpConfig(SD_SPI_SCK_GPIO_PORT, SD_SPI_MISO_PIN | SD_SPI_MOSI_PIN | \
                            SD_SPI_SCK_PIN, ENABLE);

  /* SPI2 pin remap on Port I*/
  SYSCFG_REMAPPinConfig(REMAP_Pin_SPI2Full, ENABLE);

  /* SD_SPI Config */
  SPI_Init(SD_SPI, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2, SPI_Mode_Master,
           SPI_CPOL_High, SPI_CPHA_2Edge, SPI_Direction_2Lines_FullDuplex,
           SPI_NSS_Soft, 0x07);


  /* SD_SPI enable */
  SPI_Cmd(SD_SPI, ENABLE);

  /* Configure the SD_Detect pin */
  //GPIO_Init(SD_DETECT_GPIO_PORT, SD_DETECT_PIN, GPIO_Mode_In_PU_No_IT);

  /* Set MSD ChipSelect pin in Output push-pull high level */
  GPIO_Init(SD_CS_GPIO_PORT, SD_CS_PIN, GPIO_Mode_Out_PP_High_Slow);
}
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup STM8_EVAL_SPI_SD_Private_Functions
  * @{
  */

/**
  * @brief  DeInitializes the SD/SD communication.
  * @param  None
  * @retval None
  */
void MSD_DeInit(void)
{
  MSD_LowLevel_DeInit();
}

/**
  * @brief Write a byte in the MicroSD card the SPI peripheral.
  * @param[in] Data Data to be sent.
  * @retval None
  */
u8 MSD_WriteByte(u8 Data)
{
  /* Wait until the transmit buffer is empty */
  while (SPI_GetFlagStatus(SD_SPI, SPI_FLAG_TXE) == RESET);
  /* Send the byte */
  SPI_SendData(SD_SPI, Data);
  /* Wait to receive a byte*/
  while(SPI_GetFlagStatus(SD_SPI, SPI_FLAG_RXNE) == RESET);
  /*Return the byte read from the SPI bus */ 
  return SPI_ReceiveData(SD_SPI);
}

/**
  * @brief Read a byte from the MicroSD card the SPI peripheral.
  * @par Parameters:
  * None
  * @retval u8 Data read
  */
u8 MSD_ReadByte(void)
{
  __IO uint8_t Data = 0;
	

  /* Wait until the transmit buffer is empty */
  while (SPI_GetFlagStatus(SD_SPI, SPI_FLAG_TXE) == RESET);
  /* Send the byte */
  SPI_SendData(SD_SPI, DUMMY);

  /* Wait until a data is received */
  while (SPI_GetFlagStatus(SD_SPI, SPI_FLAG_RXNE) == RESET);
  /* Get the received data */
  Data = SPI_ReceiveData(SD_SPI);

  /* Return the shifted data */
  return Data;
}

/**
  * @brief Initialize the  MicroSD card the SPI peripheral.
  * The MSD Response:
  * - MSD_RESPONSE_FAILURE: Sequence failed.
  * - MSD_RESPONSE_NO_ERROR: Sequence succeed .
  * @par Parameters:
  * None
  * @retval u8 MSD_GoIdleState State of the MSD.
  */
u8 MSD_Init(void)
{
  uint32_t i = 0;

  /*!< Initialize SD_SPI */
  MSD_LowLevel_Init();

  /*!< SD chip select high */
  MSD_CS_LOW();

  /*!< Send dummy byte 0xFF, 10 times with CS high */
  /*!< Rise CS and MOSI for 80 clocks cycles */
  for (i = 0; i <= 9; i++)
  {
    /*!< Send dummy byte 0xFF */
    MSD_WriteByte(DUMMY);
  }
  /*------------Put SD in SPI mode--------------*/
  /*!< SD initialized and set to SPI mode properly */
  return (MSD_GoIdleState());
}

/**
  * @brief Enable or Disable the MicroSD card.
  * @param[in] NewState CS pin state
  * @retval None
  */
void MSD_ChipSelect(FunctionalState NewState)
{
  if (NewState == ENABLE)
  {
    GPIO_WriteBit(MSD_CS_PORT, MSD_CS_PIN, RESET); /* CS pin low: MSD enabled */
  }
  else
  {
    GPIO_WriteBit(MSD_CS_PORT, MSD_CS_PIN, SET); /* CS pin high: MSD disabled */
  }
}

/**
  * @brief Return the State of the MSD.
  * The MSD Response:
  * - MSD_RESPONSE_FAILURE: Sequence failed.
  * - MSD_RESPONSE_NO_ERROR: Sequence succeed.
  * @par Parameters:
  * None
  * @retval u8 State of MSD
  */
u8 MSD_GoIdleState(void)
{
  /* MSD chip select low */
  MSD_ChipSelect(ENABLE);
  /* Send CMD0 (GO_IDLE_STATE) to put MSD in SPI mode */
  MSD_SendCmd(MSD_GO_IDLE_STATE, 0, 0x95);

  /* Wait for In Idle State Response (R1 Format) equal to 0x01 */
  if (MSD_GetResponse(MSD_IN_IDLE_STATE))
  {
    /* No Idle State Response: return response failue */
    return MSD_RESPONSE_FAILURE;
  }
  /*----------Activates the card initialization process-----------*/
  do
  {
    /* MSD chip select high */
    MSD_ChipSelect(DISABLE);
    /* Send Dummy byte 0xFF */
    MSD_WriteByte(DUMMY);

    /* MSD chip select low */
    MSD_ChipSelect(ENABLE);

    /* Send CMD1 (Activates the card process) until response equal to 0x0 */
    MSD_SendCmd(MSD_SEND_OP_COND, 0, 0xFF);
    /* Wait for no error Response (R1 Format) equal to 0x00 */
  }
  while (MSD_GetResponse(MSD_RESPONSE_NO_ERROR));

  /* MSD chip select high */
  MSD_ChipSelect(DISABLE);
  /* Send dummy byte 0xFF */
  MSD_WriteByte(DUMMY);

  return MSD_RESPONSE_NO_ERROR;
}

/**
  * @brief Send 5 bytes command to the MSD.
  * @param[in] Cmd The user expected command to send to MSD card.
  * @param[in] Arg The command argument.
  * @param[in] Crc The CRC.
  * @retval None
  */
void MSD_SendCmd(u8 Cmd, u32 Arg, u8 Crc)
{
  u32 i = 0x00;
  u8 Frame[6];

  /* Construct byte1 */
  Frame[0] = (u8)(Cmd | 0x40);
  /* Construct byte2 */
  Frame[1] = (u8)(Arg >> 24);
  /* Construct byte3 */
  Frame[2] = (u8)(Arg >> 16);
  /* Construct byte4 */
  Frame[3] = (u8)(Arg >> 8);
  /* Construct byte5 */
  Frame[4] = (u8)(Arg);
  /* Construct CRC: byte6 */
  Frame[5] = (Crc);

  /* Send the Cmd bytes */
  for (i = 0; i < 6; i++)
  {
    MSD_WriteByte(Frame[i]);
  }
}

/**
  * @brief Get response from the MSD.
  * The MSD Response:
  * - MSD_RESPONSE_FAILURE: Sequence failed.
  * - MSD_RESPONSE_NO_ERROR: Sequence succeed 
  * @param[in] Response Response to get
  * @retval u8 Response value
  */
u8 MSD_GetResponse(u8 Response)
{
  u32 Count = 0xFFF;

  /* Check if response is got or a timeout is happen */
  while ((MSD_ReadByte() != Response) && Count)
  {
    Count--;
  }

  if (Count == 0)
  {
    /* After time out */
    return MSD_RESPONSE_FAILURE;
  }
  else
  {
    /* Right response got */
    return MSD_RESPONSE_NO_ERROR;
  }
}

/**
  * @brief Detect if MSD card is correctly plugged in the memory slot.
  * @par Parameters:
  * None
  * @retval u8 Return if MSD is detected or not
  */
u8 MSD_Detect(void)
{
  __IO uint8_t status = MICROSD_PRESENT;
  /* Check GPIO to detect microSD */
 // if (GPIO_ReadInputData(GPIOE) & GPIO_PIN_4)
  //{
    //status = MICROSD_NOT_PRESENT;
  //}
  return status;
}

/**
  * @brief Write a block in the MSD card.
	* If the amount of data to write is less than MSD card BLOCK size (512 Byte), DUMMY bytes
  *	will be send to fill the block size.
  * @param[in] pBuffer pointer to the buffer containing the data to be written on the MSD.
  * @param[in] WriteAddr address to write on.
  * @param[in] NumByteToWrite number of data to write
  * @retval u8 MSD response
  */
u8 MSD_WriteBlock(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  u32 i = 0;
  __IO uint8_t rvalue = MSD_RESPONSE_FAILURE;

  /* MSD chip select low */
  MSD_ChipSelect(ENABLE);
  /* Send CMD24 (MSD_WRITE_BLOCK) to write multiple block */
  MSD_SendCmd(MSD_WRITE_BLOCK, WriteAddr, 0xFF);

  /* Check if the MSD acknowledged the write block command: R1 response (0x00: no errors) */
  if (!MSD_GetResponse(MSD_RESPONSE_NO_ERROR))
  {
    /* Send a dummy byte */
    MSD_WriteByte(DUMMY);
    /* Send the data token to signify the start of the data */
    MSD_WriteByte(0xFE);
    /* Write the block data to MSD : write count data by block */
    for (i = 0; i < NumByteToWrite; i++)
    {
      /* Send the pointed byte */
      MSD_WriteByte(*pBuffer);
      /* Point to the next location where the byte read will be saved */
      pBuffer++;
    }
		
		/* Send DUMMY bytes when the number of data to be written are lower
		   than the MSD card BLOCK size (512 Byte) */
		for (; i != BLOCK_SIZE; i++)
    {
      /* Send the pointed byte */
      MSD_WriteByte(DUMMY);
	  }
	
    /* Put CRC bytes (not really needed by us, but required by MSD) */
    MSD_ReadByte();
    MSD_ReadByte();
    /* Read data response */
    if (MSD_GetDataResponse() == MSD_DATA_OK)
    {
      rvalue = MSD_RESPONSE_NO_ERROR;
    }
  }

  /* MSD chip select high */
  MSD_ChipSelect(DISABLE);
  /* Send dummy byte: 8 Clock pulses of delay */
  MSD_WriteByte(DUMMY);
  /* Returns the reponse */
  return rvalue;
}

/**
  * @brief Get the data MSD card reponse status.
  * @par Parameters:
  * None
  * @retval u8 The MSD response status
  * Read data response xxx0<status>1.
  * - status 010: Data accepted.
  * - status 101: Data rejected due to a crc error.
  * - status 110: Data rejected due to a Write error.
  * - status 111: Data rejected due to other error.
  */
u8 MSD_GetDataResponse(void)
{
  u32 i = 0;
  __IO uint8_t response, rvalue;

  while (i <= 64)
  {
    /* Read resonse */
    response = MSD_ReadByte();
    /* Mask unused bits */
    response &= 0x1F;

    switch (response)
    {
      case MSD_DATA_OK:
      {
        rvalue = MSD_DATA_OK;
        break;
      }

      case MSD_DATA_CRC_ERROR:
        return MSD_DATA_CRC_ERROR;

      case MSD_DATA_WRITE_ERROR:
        return MSD_DATA_WRITE_ERROR;

      default:
      {
        rvalue = MSD_DATA_OTHER_ERROR;
        break;
      }
    }
    /* Exit loop in case of data ok */
    if (rvalue == MSD_DATA_OK)
      break;
    /* Increment loop counter */
    i++;
  }
  /* Wait null data */
  while (MSD_ReadByte() == 0);
  /* Return response */
  return response;
}

/**
  * @brief Read a block from the MSD card.
  * @param[in] pBuffer pointer to the buffer that receives the data read from the MSD.
  * @param[in] ReadAddr MSD's internal address to read from.
  * @param[in] NumByteToRead number of bytes to read from the MSD.
  * @retval u8 The MSD response
  * - MSD_RESPONSE_FAILURE: Sequence failed.
  * - MSD_RESPONSE_NO_ERROR: Sequence succeed.
  */
u8 MSD_ReadBlock(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
  u32 i = 0;
  __IO uint8_t rvalue = MSD_RESPONSE_FAILURE;

  /* MSD chip select low */
  MSD_ChipSelect(ENABLE);
  /* Send CMD17 (MSD_READ_SINGLE_BLOCK) to read one block */
  MSD_SendCmd(MSD_READ_SINGLE_BLOCK, ReadAddr, 0xFF);

  /* Check if the MSD acknowledged the read block command: R1 response (0x00: no errors) */
  if (!MSD_GetResponse(MSD_RESPONSE_NO_ERROR))
  {
    /* Now look for the data token to signify the start of the data */
    if (!MSD_GetResponse(MSD_START_DATA_SINGLE_BLOCK_READ))
    {
      /* Read the MSD block data : read NumByteToRead data */
      for (i = 0; i < NumByteToRead; i++)
      {
        /* Save the received data */
        *pBuffer = MSD_ReadByte();
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
      }
      /* Get CRC bytes (not really needed by us, but required by MSD) */
      MSD_ReadByte();
      MSD_ReadByte();
      /* Set response value to success */
      rvalue = MSD_RESPONSE_NO_ERROR;
    }
  }

  /* MSD chip select high */
  MSD_ChipSelect(DISABLE);
  /* Send dummy byte: 8 Clock pulses of delay */
  MSD_WriteByte(DUMMY);
  /* Returns the reponse */
  return rvalue;
}

/**
  * @brief Read a block from the MSD card.
  * @param[in] pBuffer pointer to the buffer that receives the data read from the MSD.
  * @param[in] ReadAddr MSD's internal address to read from.
  * @param[in] NumByteToRead number of bytes to read from the MSD.
  * @retval u8 The MSD response
  * - MSD_RESPONSE_FAILURE: Sequence failed.
  * - MSD_RESPONSE_NO_ERROR: Sequence succeed.
  */
u8 MSD_ReadBytes(u8 *pBuffer, u32 ReadAddr, u32 offset, u16 NumByteToRead)
{
  u32 i = 0;
  __IO uint8_t rvalue = MSD_RESPONSE_FAILURE;
  u16 dump;

  /* MSD chip select low */
  MSD_ChipSelect(ENABLE);
  /* Send CMD17 (MSD_READ_SINGLE_BLOCK) to read one block */
  MSD_SendCmd(MSD_READ_SINGLE_BLOCK, ReadAddr * BLOCK_SIZE, 0xFF);

  /* Check if the MSD acknowledged the read block command: R1 response (0x00: no errors) */
  if (!MSD_GetResponse(MSD_RESPONSE_NO_ERROR))
  {
    /* Now look for the data token to signify the start of the data */
    if (!MSD_GetResponse(MSD_START_DATA_SINGLE_BLOCK_READ))
    {
	  if ( offset > 0 ) {
		dump = offset;
		do {
			MSD_ReadByte();
		} while (--dump);
	  }
      /* Read the MSD block data : read NumByteToRead data */
      for (i = 0; i < NumByteToRead; i++)
      {
        /* Save the received data */
        *pBuffer = MSD_ReadByte();
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
      }
	  dump = 514 - offset - NumByteToRead;
      do {
			MSD_ReadByte();
		} while (--dump);
      /* Set response value to success */
      rvalue = MSD_RESPONSE_NO_ERROR;
    }
  }

  /* MSD chip select high */
  MSD_ChipSelect(DISABLE);
  /* Send dummy byte: 8 Clock pulses of delay */
  MSD_WriteByte(DUMMY);
  /* Returns the reponse */
  return rvalue;
}

void Delay_(u16 nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}

/**
  * @brief Write a buffer (many blocks) in the MSD card.
	* The amount of data to write should be a multiple of MSD card BLOCK size (512 Byte).  
  * @param[in] pBuffer pointer to the buffer containing the data to be written on the MSD.
  * @param[in] WriteAddr address to write on.
  * @param[in] NumByteToWrite number of data to write.
  * @retval u8 The MSD response
  * - MSD_RESPONSE_FAILURE: Sequence failed
  * - MSD_RESPONSE_NO_ERROR: Sequence succeed
  */
u8 MSD_WriteBuffer(u8 *pBuffer, u32 WriteAddr, u32 NumByteToWrite)
{
  u32 i = 0, NbrOfBlock = 0, Offset = 0;
  __IO uint8_t rvalue = MSD_RESPONSE_FAILURE;

  /* Calculate number of blocks to write */
  NbrOfBlock = NumByteToWrite / BLOCK_SIZE;
  /* MSD chip select low */
  MSD_ChipSelect(ENABLE);

  /* Data transfer */
  while (NbrOfBlock--)
  {
    /* Send CMD24 (MSD_WRITE_BLOCK) to write blocks */
    MSD_SendCmd(MSD_WRITE_BLOCK, WriteAddr + Offset, 0xFF);

    /* Check if the MSD acknowledged the write block command: R1 response (0x00: no errors) */
    if (MSD_GetResponse(MSD_RESPONSE_NO_ERROR))
    {
      return MSD_RESPONSE_FAILURE;
    }
    /* Send dummy byte */
    MSD_WriteByte(DUMMY);
    /* Send the data token to signify the start of the data */
    MSD_WriteByte(MSD_START_DATA_SINGLE_BLOCK_WRITE);
    /* Write the block data to MSD : write count data by block */
    for (i = 0; i < BLOCK_SIZE; i++)
    {
      /* Send the pointed byte */
      MSD_WriteByte(*pBuffer);
      /* Point to the next location where the byte read will be saved */
      pBuffer++;
    }
    /* Set next write address */
    Offset += 512;
    /* Put CRC bytes (not really needed by us, but required by MSD) */
    MSD_ReadByte();
    MSD_ReadByte();
    /* Read data response */
    if (MSD_GetDataResponse() == MSD_DATA_OK)
    {
      /* Set response value to success */
      rvalue = MSD_RESPONSE_NO_ERROR;
    }
    else
    {
      /* Set response value to failure */
      rvalue = MSD_RESPONSE_FAILURE;
    }
  }

  /* MSD chip select high */
  MSD_ChipSelect(DISABLE);
  /* Send dummy byte: 8 Clock pulses of delay */
  MSD_WriteByte(DUMMY);
  /* Returns the reponse */
  return rvalue;
}

/**
  * @brief Read a buffer (many blocks) from the MSD card.
  * @param[in] pBuffer pointer to the buffer that receives the data read from the MSD.
  * @param[in] ReadAddr MSD's internal address to read from.
  * @param[in] NumByteToRead number of bytes to read from the MSD.
  * @retval u8 The MSD response
  * - MSD_RESPONSE_FAILURE: Sequence failed
  * - MSD_RESPONSE_NO_ERROR: Sequence succeed
  */
u8 MSD_ReadBuffer(u8 *pBuffer, u32 ReadAddr, u32 NumByteToRead)
{
  u32 i = 0, NbrOfBlock = 0, Offset = 0;
  __IO uint8_t rvalue = MSD_RESPONSE_FAILURE;

  /* Calculate number of blocks to read */
  NbrOfBlock = NumByteToRead / BLOCK_SIZE;
  /* MSD chip select low */
  MSD_ChipSelect(ENABLE);

  /* Data transfer */
  while (NbrOfBlock --)
  {
    /* Send CMD17 (MSD_READ_SINGLE_BLOCK) to read one block */
    MSD_SendCmd (MSD_READ_SINGLE_BLOCK, ReadAddr + Offset, 0xFF);
    /* Check if the MSD acknowledged the read block command: R1 response (0x00: no errors) */
    if (MSD_GetResponse(MSD_RESPONSE_NO_ERROR))
    {
      return  MSD_RESPONSE_FAILURE;
    }
    /* Now look for the data token to signify the start of the data */
    if (!MSD_GetResponse(MSD_START_DATA_SINGLE_BLOCK_READ))
    {
      /* Read the MSD block data : read NumByteToRead data */
      for (i = 0; i < BLOCK_SIZE; i++)
      {
        /* Read the pointed data */
        *pBuffer = MSD_ReadByte();
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
      }
      /* Set next read address*/
      Offset += 512;
      /* get CRC bytes (not really needed by us, but required by MSD) */
      MSD_ReadByte();
      MSD_ReadByte();
      /* Set response value to success */
      rvalue = MSD_RESPONSE_NO_ERROR;
    }
    else
    {
      /* Set response value to failure */
      rvalue = MSD_RESPONSE_FAILURE;
    }
  }

  /* MSD chip select high */
  MSD_ChipSelect(DISABLE);
  /* Send dummy byte: 8 Clock pulses of delay */
  MSD_WriteByte(DUMMY);
  /* Returns the reponse */
  return rvalue;
}

/**
  * @brief  Read the CSD card register.
  *         Reading the contents of the CSD register in SPI mode is a simple
  *         read-block transaction.
  * @param  SD_csd: pointer on an SCD register structure
  * @retval The SD Response:
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
uint8_t MSD_GetCSDRegister(MSD_CSD* SD_csd)
{
  uint32_t i = 0;
  uint8_t rvalue = MSD_RESPONSE_FAILURE;
  uint8_t CSD_Tab[16];

  /*!< SD chip select low */
  MSD_CS_LOW();
  /*!< Send CMD9 (CSD register) or CMD10(CSD register) */
  MSD_SendCmd(MSD_CMD_SEND_CSD, 0, 0xFF);
  /*!< Wait for response in the R1 format (0x00 is no errors) */
  if (!MSD_GetResponse(MSD_RESPONSE_NO_ERROR))
  {
    if (!MSD_GetResponse(MSD_START_DATA_SINGLE_BLOCK_READ))
    {
      for (i = 0; i < 16; i++)
      {
        /*!< Store CSD register value on CSD_Tab */
        CSD_Tab[i] = MSD_ReadByte();
      }
    }
    /*!< Get CRC bytes (not really needed by us, but required by SD) */
    MSD_WriteByte(DUMMY);
    MSD_WriteByte(DUMMY);
    /*!< Set response value to success */
    rvalue = MSD_RESPONSE_NO_ERROR;
  }
  /*!< SD chip select high */
  MSD_CS_HIGH();
  /*!< Send dummy byte: 8 Clock pulses of delay */
  MSD_WriteByte(DUMMY);

  /*!< Byte 0 */
  SD_csd->CSDStruct = (uint8_t)((CSD_Tab[0] & 0xC0) >> 6);
  SD_csd->SysSpecVersion = (uint8_t)((CSD_Tab[0] & 0x3C) >> 2);
  SD_csd->Reserved1 = (uint8_t)(CSD_Tab[0] & 0x03);

  /*!< Byte 1 */
  SD_csd->TAAC = CSD_Tab[1];

  /*!< Byte 2 */
  SD_csd->NSAC = CSD_Tab[2];

  /*!< Byte 3 */
  SD_csd->MaxBusClkFrec = CSD_Tab[3];

  /*!< Byte 4 */
  SD_csd->CardComdClasses = CSD_Tab[4] << 4;

  /*!< Byte 5 */
  SD_csd->CardComdClasses |= (CSD_Tab[5] & 0xF0) >> 4;
  SD_csd->RdBlockLen = (uint8_t)(CSD_Tab[5] & 0x0F);

  /*!< Byte 6 */
  SD_csd->PartBlockRead = (uint8_t)((CSD_Tab[6] & 0x80) >> 7);
  SD_csd->WrBlockMisalign = (uint8_t)((CSD_Tab[6] & 0x40) >> 6);
  SD_csd->RdBlockMisalign = (uint8_t)((CSD_Tab[6] & 0x20) >> 5);
  SD_csd->DSRImpl = (uint8_t)((CSD_Tab[6] & 0x10) >> 4);
  SD_csd->Reserved2 = 0; /*!< Reserved */

  SD_csd->DeviceSize = (CSD_Tab[6] & 0x03) << 10;

  /*!< Byte 7 */
  SD_csd->DeviceSize |= (CSD_Tab[7]) << 2;

  /*!< Byte 8 */
  SD_csd->DeviceSize |= (CSD_Tab[8] & 0xC0) >> 6;

  SD_csd->MaxRdCurrentVDDMin = (uint8_t)((CSD_Tab[8] & 0x38) >> 3);
  SD_csd->MaxRdCurrentVDDMax = (uint8_t)((CSD_Tab[8] & 0x07));

  /*!< Byte 9 */
  SD_csd->MaxWrCurrentVDDMin = (uint8_t)((CSD_Tab[9] & 0xE0) >> 5);
  SD_csd->MaxWrCurrentVDDMax = (uint8_t)((CSD_Tab[9] & 0x1C) >> 2);
  SD_csd->DeviceSizeMul = (uint8_t)((CSD_Tab[9] & 0x03) << 1);
  /*!< Byte 10 */
  SD_csd->DeviceSizeMul |= (uint8_t)((CSD_Tab[10] & 0x80) >> 7);

  SD_csd->EraseGrSize = (uint8_t)((CSD_Tab[10] & 0x40) >> 6);
  SD_csd->EraseGrMul = (uint8_t)((CSD_Tab[10] & 0x3F) << 1);

  /*!< Byte 11 */
  SD_csd->EraseGrMul |= (uint8_t)((CSD_Tab[11] & 0x80) >> 7);
  SD_csd->WrProtectGrSize = (uint8_t)((CSD_Tab[11] & 0x7F));

  /*!< Byte 12 */
  SD_csd->WrProtectGrEnable = (uint8_t)((CSD_Tab[12] & 0x80) >> 7);
  SD_csd->ManDeflECC = (uint8_t)((CSD_Tab[12] & 0x60) >> 5);
  SD_csd->WrSpeedFact = (uint8_t)((CSD_Tab[12] & 0x1C) >> 2);
  SD_csd->MaxWrBlockLen = (uint8_t)((CSD_Tab[12] & 0x03) << 2);

  /*!< Byte 13 */
  SD_csd->MaxWrBlockLen |= (uint8_t)((CSD_Tab[13] & 0xC0) >> 6);
  SD_csd->WriteBlockPaPartial = (uint8_t)((CSD_Tab[13] & 0x20) >> 5);
  SD_csd->Reserved3 = 0;
  SD_csd->ContentProtectAppli = (uint8_t)(CSD_Tab[13] & 0x01);

  /*!< Byte 14 */
  SD_csd->FileFormatGrouop = (uint8_t)((CSD_Tab[14] & 0x80) >> 7);
  SD_csd->CopyFlag = (uint8_t)((CSD_Tab[14] & 0x40) >> 6);
  SD_csd->PermWrProtect = (uint8_t)((CSD_Tab[14] & 0x20) >> 5);
  SD_csd->TempWrProtect = (uint8_t)((CSD_Tab[14] & 0x10) >> 4);
  SD_csd->FileFormat = (uint8_t)((CSD_Tab[14] & 0x0C) >> 2);
  SD_csd->ECC = (uint8_t)(CSD_Tab[14] & 0x03);

  /*!< Byte 15 */
  SD_csd->CSD_CRC = (uint8_t)((CSD_Tab[15] & 0xFE) >> 1);
  SD_csd->Reserved4 = 1;

  /*!< Return the reponse */
  return rvalue;
}

/**
  * @brief  Read the CID card register.
  *         Reading the contents of the CID register in SPI mode is a simple
  *         read-block transaction.
  * @param  MSD_cid: pointer on an CID register structure
  * @retval The SD Response:
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
uint8_t MSD_GetCIDRegister(MSD_CID* SD_cid)
{
  uint32_t i = 0;
  uint8_t rvalue = MSD_RESPONSE_FAILURE;
  uint8_t CID_Tab[16];

  /*!< SD chip select low */
  MSD_CS_LOW();

  /*!< Send CMD10 (CID register) */
  MSD_SendCmd(MSD_CMD_SEND_CID, 0, 0xFF);

  /*!< Wait for response in the R1 format (0x00 is no errors) */
  if (!MSD_GetResponse(MSD_RESPONSE_NO_ERROR))
  {
    if (!MSD_GetResponse(MSD_START_DATA_SINGLE_BLOCK_READ))
    {
      /*!< Store CID register value on CID_Tab */
      for (i = 0; i < 16; i++)
      {
        CID_Tab[i] = MSD_ReadByte();
      }
    }
    /*!< Get CRC bytes (not really needed by us, but required by SD) */
    MSD_WriteByte(DUMMY);
    MSD_WriteByte(DUMMY);
    /*!< Set response value to success */
    rvalue = MSD_RESPONSE_NO_ERROR;
  }
  /*!< SD chip select high */
  MSD_CS_HIGH();
  /*!< Send dummy byte: 8 Clock pulses of delay */
  MSD_WriteByte(DUMMY);

  /*!< Byte 0 */
  SD_cid->ManufacturerID = CID_Tab[0];

  /*!< Byte 1 */
  SD_cid->OEM_AppliID = CID_Tab[1] << 8;

  /*!< Byte 2 */
  SD_cid->OEM_AppliID |= CID_Tab[2];

  /*!< Byte 3 */
  SD_cid->ProdName1 = ((uint32_t)CID_Tab[3] << 24);

  /*!< Byte 4 */
  SD_cid->ProdName1 |= ((uint32_t)CID_Tab[4] << 16);

  /*!< Byte 5 */
  SD_cid->ProdName1 |= CID_Tab[5] << 8;

  /*!< Byte 6 */
  SD_cid->ProdName1 |= CID_Tab[6];

  /*!< Byte 7 */
  SD_cid->ProdName2 = CID_Tab[7];

  /*!< Byte 8 */
  SD_cid->ProdRev = CID_Tab[8];

  /*!< Byte 9 */
  SD_cid->ProdSN = ((uint32_t)CID_Tab[9] << 24);

  /*!< Byte 10 */
  SD_cid->ProdSN |= ((uint32_t)CID_Tab[10] << 16);

  /*!< Byte 11 */
  SD_cid->ProdSN |= CID_Tab[11] << 8;

  /*!< Byte 12 */
  SD_cid->ProdSN |= CID_Tab[12];

  /*!< Byte 13 */
  SD_cid->Reserved1 |= (uint8_t)((CID_Tab[13] & 0xF0) >> 4);
  SD_cid->ManufactDate = (CID_Tab[13] & 0x0F) << 8;

  /*!< Byte 14 */
  SD_cid->ManufactDate |= CID_Tab[14];

  /*!< Byte 15 */
  SD_cid->CID_CRC = (uint8_t)((CID_Tab[15] & 0xFE) >> 1);
  SD_cid->Reserved2 = 1;

  /*!< Return the reponse */
  return rvalue;
}

/**
  * @brief  Returns information about specific card.
  * @param  cardinfo: pointer to a SD_CardInfo structure that contains all SD
  *         card information.
  * @retval The SD Response:
  *         - SD_RESPONSE_FAILURE: Sequence failed
  *         - SD_RESPONSE_NO_ERROR: Sequence succeed
  */
uint8_t MSD_GetCardInfo(MSD_CardInfo *cardinfo)
{
  uint8_t status = MSD_RESPONSE_FAILURE;

  status = MSD_GetCSDRegister(&(cardinfo->SD_csd));
  status = MSD_GetCIDRegister(&(cardinfo->SD_cid));
  cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
  cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
  cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
  cardinfo->CardCapacity *= cardinfo->CardBlockSize;

  /*!< Returns the reponse */
  return status;
}
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/