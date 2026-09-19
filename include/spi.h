#ifndef SPI_H_
#define SPI_H_
#include "stm32c031c6.h"

typedef struct{
  uint8_t SPI_DeviceMode;
  uint8_t SPI_BusConfig;
  uint8_t SPI_SClkSpeed;
  uint8_t SPI_DataSize;
  uint8_t SPI_CRCL;
  uint8_t SPI_CPOL;
  uint8_t SPI_CPHA;
  uint8_t SPI_SSM;
}SPI_Config_t;


typedef struct{
  SPI_RegDef_t *pSPIx;
  SPI_Config_t SPIConfig;
  uint8_t *pTxBuffer;
  uint8_t *pRxBuffer;
  uint32_t TxLen;
  uint32_t RxLen;
  uint8_t TxState;
  uint8_t RxState;
}SPI_Handle_t;


#define SPI_DEVICE_MODE_MASTER 1
#define SPI_DEVICE_MODE_SLAVE  0

#define SPI_BUS_CONFIG_FD             1
#define SPI_BUS_CONFIG_HD             2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY 3

#define SPI_SCLK_SPEED_DIV2   0
#define SPI_SCLK_SPEED_DIV4   1
#define SPI_SCLK_SPEED_DIV8   2
#define SPI_SCLK_SPEED_DIV16  3
#define SPI_SCLK_SPEED_DIV32  4
#define SPI_SCLK_SPEED_DIV64  5
#define SPI_SCLK_SPEED_DIV128 6
#define SPI_SCLK_SPEED_DIV256 7

#define SPI_CRCL_8BITS   0
#define SPI_CRCL_16BITS  1

#define SPI_CPHA_HIGH 1
#define SPI_CPHA_LOW  0

#define SPI_CPOL_HIGH 1
#define SPI_CPOL_LOW  0

#define SPI_SSM_EN 1
#define SPI_SSM_DI 0

#define SPI_TXE_FLAG (1<<1)
#define SPI_RXNE_FLAG (1<<0)
#define SPI_BSY_FLAG (1<<7)

#define SPI_READY 0
#define SPI_BUSY_IN_RX 1
#define SPI_BUSY_IN_TX 2

#define SPI_EVENT_TX_CMPLT 1
#define SPI_EVENT_RX_CMPLT 2
#define SPI_EVENT_OVR_ERR  3

#define SPI_DATASIZE_8BIT   7
#define SPI_DATASIZE_16BIT  15





void SPI_PeriClkCtrl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);
void SPI_PeriCtrl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi);
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);



void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);


uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);



void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_PriorityConfig(uint8_t IRQPriority, uint8_t IRQNumber);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

void SPI_ApplicationEventCallBack(SPI_Handle_t *pSPIHandle, uint8_t AppEv);


#endif /*SPI_H_*/
