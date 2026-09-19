#include "stm32c031c6.h"


static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);
void SPI_PeriClkCtrl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    if(pSPIx == SPI1){
      SPI1_PCLK_EN();
    }
    else if(pSPIx == SPI2){
      SPI2_PCLK_EN();
    }
  }
  else{
    if(pSPIx == SPI1){
      SPI1_PCLK_DI();
    }
    else if(pSPIx == SPI2){
      SPI2_PCLK_DI();
    }
  }
}

void SPI_PeriCtrl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    pSPIx->CR1 |= (1<<6);
  }
  else{
    pSPIx->CR1 &= ~(1<<6);
  }
}

void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    pSPIx->CR1 |= (1<<8);
  }
  else{
    pSPIx->CR1 &= ~(1<<8);
  }
}

void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    pSPIx->CR2 |= (1<<2);
  }
  else{
    pSPIx->CR2 &= ~(1<<2);
  }
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName){
  if(pSPIx->SR & FlagName){
    return FLAG_SET;
  }

  return FLAG_RESET;
}
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx){
  uint8_t temp;
  temp = pSPIx->DR;
  temp = pSPIx->SR;
  (void)temp;
}

void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle){
  pSPIHandle->pSPIx->CR2 &= ~(1<<7);
  pSPIHandle->pTxBuffer = NULL;
  pSPIHandle->TxLen = 0;
  pSPIHandle->TxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle){
  pSPIHandle->pSPIx->CR2 &= ~(1<<6);
  pSPIHandle->pRxBuffer = NULL;
  pSPIHandle->RxLen = 0;
  pSPIHandle->RxState = SPI_READY;
}

void SPI_Init(SPI_Handle_t *pSPIHandle){
  uint32_t tempreg = 0;
  tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2 ;
  if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD){
    tempreg &= ~ (1 << 15);
  }
  else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD){
    tempreg |= (1<<15);
  }
  else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY){
    tempreg &= ~ (1 << 15);
    tempreg |= (1<<10);
  }

  tempreg |= pSPIHandle->SPIConfig.SPI_SClkSpeed << 3; // Baudrate

  tempreg |= pSPIHandle->SPIConfig.SPI_CRCL << 11;

  tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << 1;

  tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << 0;

  tempreg |= pSPIHandle->SPIConfig.SPI_SSM << 9;

  pSPIHandle->pSPIx->CR1 = tempreg;   

  tempreg = 0;
  tempreg |= pSPIHandle->SPIConfig.SPI_DataSize << 8;

  if(pSPIHandle->SPIConfig.SPI_DataSize == SPI_DATASIZE_8BIT)
  {
      tempreg |= (1 << 12);
  }

  pSPIHandle->pSPIx->CR2 = tempreg;

}

void SPI_DeInit(SPI_RegDef_t *pSPIx){
  if (pSPIx == SPI1){
      SPI1_REG_RESET();
    }
    else if (pSPIx == SPI2){
      SPI2_REG_RESET();
    }
}

void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len){
  uint8_t datasize = (pSPIx->CR2 >> 8) & 0xF;
  while(Len > 0){
    while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);
    if(datasize == SPI_DATASIZE_16BIT){
      *(volatile uint16_t *)&pSPIx->DR = *((uint16_t*) pTxBuffer);
      Len -= 2;
      pTxBuffer += 2;
    }
    else{
      *(volatile uint8_t *)&pSPIx->DR = *pTxBuffer;
      Len--;
      pTxBuffer++;
    }

  }
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
  uint8_t datasize = (pSPIx->CR2 >> 8) & 0xF;
  while(Len > 0){
    while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

    if(datasize  == SPI_DATASIZE_16BIT){
      *((uint16_t*) pRxBuffer) = *(volatile uint16_t *)&pSPIx->DR ;
      Len -= 2;
      pRxBuffer += 2;
    }
    else{
      *pRxBuffer = *(volatile uint8_t *)&pSPIx->DR;
      Len--;
      pRxBuffer++;
    }

  }
}

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len){
  uint8_t state = pSPIHandle->TxState;
  if(state != SPI_BUSY_IN_TX){
    pSPIHandle->pTxBuffer = pTxBuffer;
    pSPIHandle->TxLen = Len;
    pSPIHandle->TxState = SPI_BUSY_IN_TX;
    pSPIHandle->pSPIx->CR2 |= (1<<7);
  }
  
  return state;
}

uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len){
  uint8_t state = pSPIHandle->RxState;
  if(state != SPI_BUSY_IN_RX){
    pSPIHandle->pRxBuffer = pRxBuffer;
    pSPIHandle->RxLen = Len;
    pSPIHandle->RxState = SPI_BUSY_IN_RX;
    pSPIHandle->pSPIx->CR2 |= (1<<6);
  }
  
  return state;
}

void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    *NVIC_ISER |= (1<<IRQNumber);
  }
  else{
    *NVIC_ICER |= (1<<IRQNumber);
  }
}

void SPI_PriorityConfig(uint8_t IRQPriority, uint8_t IRQNumber){
  uint8_t iprx = IRQNumber / 4;
  uint8_t iprx_section = IRQNumber % 4;
  uint8_t shift_amount = (8 * iprx_section) + 6;
  NVIC_PR_BASEADDR[iprx] |= (IRQPriority << shift_amount);
}

void SPI_IRQHandling(SPI_Handle_t *pHandle){
  uint8_t temp1, temp2;
  temp1 = pHandle->pSPIx->SR & (1 << 1);
  temp2 = pHandle->pSPIx->CR2 & (1 << 7);
  if(temp1 && temp2){
    spi_txe_interrupt_handle(pHandle);
  }
  temp1 = pHandle->pSPIx->SR & (1 << 0);
  temp2 = pHandle->pSPIx->CR2 & (1 << 6);
  if(temp1 && temp2){
    spi_rxne_interrupt_handle(pHandle);
  }
  temp1 = pHandle->pSPIx->SR & (1 << 6);
  temp2 = pHandle->pSPIx->CR2 & (1 << 5);
  if(temp1 && temp2){
    spi_ovr_err_interrupt_handle(pHandle);
  }
}


static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle){
  uint8_t datasize = (pSPIHandle->pSPIx->CR2 >> 8) & 0xF;
  if(datasize  == SPI_DATASIZE_16BIT){
    (*(volatile uint16_t *)&pSPIHandle->pSPIx->DR) = *((uint16_t*) pSPIHandle->pTxBuffer);
    pSPIHandle->TxLen -= 2;
    pSPIHandle->pTxBuffer += 2;
  }
  else{
    (*(volatile uint8_t *)&pSPIHandle->pSPIx->DR) = *(pSPIHandle->pTxBuffer);
    pSPIHandle->TxLen--;
    pSPIHandle->pTxBuffer++;
  }
  if(! pSPIHandle->TxLen){
    SPI_CloseTransmission(pSPIHandle);
    SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_TX_CMPLT);
  }

}
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle){
  uint8_t datasize = (pSPIHandle->pSPIx->CR2 >> 8) & 0xF;
  if(datasize  == SPI_DATASIZE_16BIT){
    *((uint16_t*) pSPIHandle->pRxBuffer) = (*(volatile uint16_t *)&pSPIHandle->pSPIx->DR);
    pSPIHandle->RxLen -= 2;
    pSPIHandle->pRxBuffer += 2;
  }
  else{
    *(pSPIHandle->pRxBuffer) = (*(volatile uint8_t *)&pSPIHandle->pSPIx->DR);
    pSPIHandle->RxLen--;
    pSPIHandle->pRxBuffer++;
  }
  if(! pSPIHandle->RxLen){
    SPI_CloseReception(pSPIHandle);
    SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_RX_CMPLT);
  }
}

static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle){ 
  SPI_ClearOVRFlag(pSPIHandle->pSPIx);
  SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_OVR_ERR);
}

__attribute__ ((weak)) void SPI_ApplicationEventCallBack(SPI_Handle_t *pSPIHandle, uint8_t AppEv){

}
