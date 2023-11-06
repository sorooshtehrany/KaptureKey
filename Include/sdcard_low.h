/**
  ******************************************************************************
  * @file microsd.h
  * @brief This file contains functions prototypes.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MICROSD_H
#define __MICROSD_H

/* Includes ------------------------------------------------------------------*/
#include "..\STM8L Library\Include\stm8l15x.h"
#include "..\STM8L Library\Include\stm8l15x_conf.h"

/* Private define ------------------------------------------------------------*/
/* Block Size */
#define BLOCK_SIZE ((u16)512)

/* Dummy byte */
#define DUMMY ((u8)0xFF)
/*
  * @brief  Select SD Card: ChipSelect pin low   
*/  
#define MSD_CS_LOW()     GPIO_ResetBits(SD_CS_GPIO_PORT, SD_CS_PIN)
/** 
  * @brief  Deselect SD Card: ChipSelect pin high   
  */ 
#define MSD_CS_HIGH()    GPIO_SetBits(SD_CS_GPIO_PORT, SD_CS_PIN)

/* MicroSD activation port & pin */
#define SD_CS_GPIO_PORT         GPIOD
#define SD_CS_PIN               GPIO_Pin_7

#define SD_SPI_SCK_GPIO_PORT    GPIOD
#define SD_SPI_SCK_PIN          GPIO_Pin_6

#define SD_SPI_MOSI_GPIO_PORT    GPIOD
#define SD_SPI_MOSI_PIN          GPIO_Pin_5

#define SD_SPI_MISO_GPIO_PORT    GPIOD
#define SD_SPI_MISO_PIN          GPIO_Pin_4

#define SD_SPI                  SPI2
#define SD_SPI_CLK              CLK_Peripheral_SPI2

#define MSD_CS_PORT (GPIOD)
#define MSD_CS_PIN  (GPIO_Pin_7)


/* MSD detection on its memory slot */
#define MICROSD_PRESENT     ((u8)1)
#define MICROSD_NOT_PRESENT ((u8)0)

/**
  * @brief  Commands: CMDxx = CMD-number | 0x40
  */
#define MSD_CMD_GO_IDLE_STATE          0   /*!< CMD0 = 0x40 */
#define MSD_CMD_SEND_OP_COND           1   /*!< CMD1 = 0x41 */
#define MSD_CMD_SEND_CSD               9   /*!< CMD9 = 0x49 */
#define MSD_CMD_SEND_CID               10  /*!< CMD10 = 0x4A */
#define MSD_CMD_STOP_TRANSMISSION      12  /*!< CMD12 = 0x4C */
#define MSD_CMD_SEND_STATUS            13  /*!< CMD13 = 0x4D */
#define MSD_CMD_SET_BLOCKLEN           16  /*!< CMD16 = 0x50 */
#define MSD_CMD_READ_SINGLE_BLOCK      17  /*!< CMD17 = 0x51 */
#define MSD_CMD_READ_MULT_BLOCK        18  /*!< CMD18 = 0x52 */
#define MSD_CMD_SET_BLOCK_COUNT        23  /*!< CMD23 = 0x57 */
#define MSD_CMD_WRITE_SINGLE_BLOCK     24  /*!< CMD24 = 0x58 */
#define MSD_CMD_WRITE_MULT_BLOCK       25  /*!< CMD25 = 0x59 */
#define MSD_CMD_PROG_CSD               27  /*!< CMD27 = 0x5B */
#define MSD_CMD_SET_WRITE_PROT         28  /*!< CMD28 = 0x5C */
#define MSD_CMD_CLR_WRITE_PROT         29  /*!< CMD29 = 0x5D */
#define MSD_CMD_SEND_WRITE_PROT        30  /*!< CMD30 = 0x5E */
#define MSD_CMD_SD_ERASE_GRP_START     32  /*!< CMD32 = 0x60 */
#define MSD_CMD_SD_ERASE_GRP_END       33  /*!< CMD33 = 0x61 */
#define MSD_CMD_UNTAG_SECTOR           34  /*!< CMD34 = 0x62 */
#define MSD_CMD_ERASE_GRP_START        35  /*!< CMD35 = 0x63 */
#define MSD_CMD_ERASE_GRP_END          36  /*!< CMD36 = 0x64 */
#define MSD_CMD_UNTAG_ERASE_GROUP      37  /*!< CMD37 = 0x65 */
#define MSD_CMD_ERASE                  38  /*!< CMD38 = 0x66 */

/* Start Data tokens  */
/* Tokens (necessary because at nop/idle (and CS active) only 0xff is on the data/command line) */
#define MSD_START_DATA_SINGLE_BLOCK_READ ((u8)0xFE)  /* Data token start byte, Start Single Block Read */
#define MSD_START_DATA_MULTIPLE_BLOCK_READ  ((u8)0xFE)  /* Data token start byte, Start Multiple Block Read */
#define MSD_START_DATA_SINGLE_BLOCK_WRITE ((u8)0xFE) /* Data token start byte, Start Single Block Write */
#define MSD_START_DATA_MULTIPLE_BLOCK_WRITE ((u8)0xFD)  /* Data token start byte, Start Multiple Block Write */
#define MSD_STOP_DATA_MULTIPLE_BLOCK_WRITE ((u8)0xFD)  /* Data toke stop byte, Stop Multiple Block Write */

/* MSD functions return */
#define MSD_SUCCESS       ((u8)0x00)
#define MSD_FAIL          ((u8)0xFF)

/* MSD reponses and error flags */
#define MSD_RESPONSE_NO_ERROR      ((u8)0x00)
#define MSD_IN_IDLE_STATE          ((u8)0x01)
#define MSD_ERASE_RESET            ((u8)0x02)
#define MSD_ILLEGAL_COMMAND        ((u8)0x04)
#define MSD_COM_CRC_ERROR          ((u8)0x08)
#define MSD_ERASE_SEQUENCE_ERROR   ((u8)0x10)
#define MSD_ADDRESS_ERROR          ((u8)0x20)
#define MSD_PARAMETER_ERROR        ((u8)0x40)
#define MSD_RESPONSE_FAILURE       ((u8)0xFF)

/* Data response error */
#define MSD_DATA_OK                ((u8)0x05)
#define MSD_DATA_CRC_ERROR         ((u8)0x0B)
#define MSD_DATA_WRITE_ERROR       ((u8)0x0D)
#define MSD_DATA_OTHER_ERROR       ((u8)0xFF)

/* Commands: CMDxx = CMD-number | 0x40 */
#define MSD_GO_IDLE_STATE          ((u8)0)   /* CMD0=0x40 */
#define MSD_SEND_OP_COND           ((u8)1)   /* CMD1=0x41 */
#define MSD_SEND_CSD               ((u8)9)   /* CMD9=0x49 */
#define MSD_SEND_CID               ((u8)10)  /* CMD10=0x4A */
#define MSD_STOP_TRANSMISSION      ((u8)12)  /* CMD12=0x4C */
#define MSD_SEND_STATUS            ((u8)13)  /* CMD13=0x4D */
#define MSD_SET_BLOCKLEN           ((u8)16)  /* CMD16=0x50 */
#define MSD_READ_SINGLE_BLOCK      ((u8)17)  /* CMD17=0x51 */
#define MSD_READ_MULTIPLE_BLOCK    ((u8)18)  /* CMD18=0x52 */
#define MSD_SET_BLOCK_COUNT        ((u8)23)  /* CMD23=0x57 */
#define MSD_WRITE_BLOCK            ((u8)24)  /* CMD24=0x58 */
#define MSD_WRITE_MULTIPLE_BLOCK   ((u8)25)  /* CMD25=0x59 */
#define MSD_PROGRAM_CSD            ((u8)27)  /* CMD27=0x5B */
#define MSD_SET_WRITE_PROT         ((u8)28)  /* CMD28=0x5C */
#define MSD_CLR_WRITE_PROT         ((u8)29)  /* CMD29=0x5D */
#define MSD_SEND_WRITE_PROT        ((u8)30)  /* CMD30=0x5E */
#define MSD_TAG_SECTOR_START       ((u8)32)  /* CMD32=0x60 */
#define MSD_TAG_SECTOR_END         ((u8)33)  /* CMD33=0x61 */
#define MSD_UNTAG_SECTOR           ((u8)34)  /* CMD34=0x62 */
#define MSD_TAG_ERASE_GROUP_START  ((u8)35)  /* CMD35=0x63 */
#define MSD_TAG_ERASE_GROUP_END    ((u8)36)  /* CMD36=0x64 */
#define MSD_UNTAG_ERASE_GROUP      ((u8)37)  /* CMD37=0x65 */
#define MSD_ERASE                  ((u8)38)  /* CMD38=0x66 */
#define MSD_READ_OCR               ((u8)39)  /* CMD39=0x67 */
#define MSD_CRC_ON_OFF             ((u8)40)  /* CMD40=0x68 */

/** 
  * @brief  Card Specific Data: CSD Register   
  */ 
typedef struct
{
  __IO uint8_t  CSDStruct;            /*!< CSD structure */
  __IO uint8_t  SysSpecVersion;       /*!< System specification version */
  __IO uint8_t  Reserved1;            /*!< Reserved */
  __IO uint8_t  TAAC;                 /*!< Data read access-time 1 */
  __IO uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
  __IO uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
  __IO uint16_t CardComdClasses;      /*!< Card command classes */
  __IO uint8_t  RdBlockLen;           /*!< Max. read data block length */
  __IO uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
  __IO uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
  __IO uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
  __IO uint8_t  DSRImpl;              /*!< DSR implemented */
  __IO uint8_t  Reserved2;            /*!< Reserved */
  __IO uint32_t DeviceSize;           /*!< Device Size */
  __IO uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
  __IO uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
  __IO uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
  __IO uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
  __IO uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
  __IO uint8_t  EraseGrSize;          /*!< Erase group size */
  __IO uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
  __IO uint8_t  WrProtectGrSize;      /*!< Write protect group size */
  __IO uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
  __IO uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
  __IO uint8_t  WrSpeedFact;          /*!< Write speed factor */
  __IO uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
  __IO uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
  __IO uint8_t  Reserved3;            /*!< Reserded */
  __IO uint8_t  ContentProtectAppli;  /*!< Content protection application */
  __IO uint8_t  FileFormatGrouop;     /*!< File format group */
  __IO uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
  __IO uint8_t  PermWrProtect;        /*!< Permanent write protection */
  __IO uint8_t  TempWrProtect;        /*!< Temporary write protection */
  __IO uint8_t  FileFormat;           /*!< File Format */
  __IO uint8_t  ECC;                  /*!< ECC code */
  __IO uint8_t  CSD_CRC;              /*!< CSD CRC */
  __IO uint8_t  Reserved4;            /*!< always 1*/
} MSD_CSD;

/** 
  * @brief  Card Identification Data: CID Register   
  */
typedef struct
{
  __IO uint8_t  ManufacturerID;       /*!< ManufacturerID */
  __IO uint16_t OEM_AppliID;          /*!< OEM/Application ID */
  __IO uint32_t ProdName1;            /*!< Product Name part1 */
  __IO uint8_t  ProdName2;            /*!< Product Name part2*/
  __IO uint8_t  ProdRev;              /*!< Product Revision */
  __IO uint32_t ProdSN;               /*!< Product Serial Number */
  __IO uint8_t  Reserved1;            /*!< Reserved1 */
  __IO uint16_t ManufactDate;         /*!< Manufacturing Date */
  __IO uint8_t  CID_CRC;              /*!< CID CRC */
  __IO uint8_t  Reserved2;            /*!< always 1 */
} MSD_CID;

/** 
  * @brief SD Card information 
  */
typedef struct
{
  MSD_CSD SD_csd;
  MSD_CID SD_cid;
  uint32_t CardCapacity;  /*!< Card Capacity */
  uint32_t CardBlockSize; /*!< Card Block Size */
} MSD_CardInfo;
/* Exported functions ------------------------------------------------------- */

/*----- High layer function -----*/
u8 MSD_Init(void);
void MSD_DeInit(void);
u8 MSD_WriteBlock(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
u8 MSD_ReadBlock(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u8 MSD_WriteBuffer(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite);
u8 MSD_ReadBuffer(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead);
u8 MSD_GetCardInfo(MSD_CardInfo *cardinfo);
u8 MSD_ReadBytes(u8 *pBuffer, u32 ReadAddr, u32 offset, u16 NumByteToRead);

/*----- Medium layer function -----*/
void MSD_SendCmd(u8 Cmd, u32 Arg, u8 Crc);
u8 MSD_GetResponse(u8 Response);
u8 MSD_GetDataResponse(void);
u8 MSD_GoIdleState(void);
void MSD_ChipSelect(FunctionalState NewState);
u8 MSD_Detect(void);

/*----- Low layer function -----*/
u8 MSD_WriteByte(u8 byte);
u8 MSD_ReadByte(void);

#endif /* __MSD_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/