#include "stm32c031c6.h"

// Forward declarations of static ISR helper functions
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle);

/**
 * @brief  Enables or disables the peripheral clock for SPI1 or SPI2.
 * @param  pSPIx  : Base address of the SPI peripheral
 * @param  EnorDi : ENABLE or DISABLE
 */
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

/**
 * @brief  Enables or disables the SPI peripheral via the SPE bit in CR1.
 * @param  pSPIx  : Base address of the SPI peripheral
 * @param  EnorDi : ENABLE or DISABLE
 */
void SPI_PeriCtrl(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    pSPIx->CR1 |= (1 << 6);  // SPE = 1
  }
  else{
    pSPIx->CR1 &= ~(1 << 6); // SPE = 0
  }
}

/**
 * @brief  Configures the Internal Slave Select (SSI) bit in CR1 when SSM is enabled.
 *         Prevents MODF (Mode Fault) errors in Master mode when SSM=1.
 * @param  pSPIx  : Base address of the SPI peripheral
 * @param  EnorDi : ENABLE (SSI = 1) or DISABLE (SSI = 0)
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    pSPIx->CR1 |= (1 << 8);  // SSI = 1
  }
  else{
    pSPIx->CR1 &= ~(1 << 8); // SSI = 0
  }
}

/**
 * @brief  Configures SS Output Enable (SSOE) in CR2.
 *         When SSOE = 1 in Master mode, NSS pin is automatically pulled LOW during transmission.
 * @param  pSPIx  : Base address of the SPI peripheral
 * @param  EnorDi : ENABLE or DISABLE
 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    pSPIx->CR2 |= (1 << 2);  // SSOE = 1
  }
  else{
    pSPIx->CR2 &= ~(1 << 2); // SSOE = 0
  }
}

/**
 * @brief  Checks whether a specific status flag in SR is SET or RESET.
 * @param  pSPIx    : Base address of the SPI peripheral
 * @param  FlagName : Flag mask (e.g. SPI_TXE_FLAG, SPI_RXNE_FLAG, SPI_BSY_FLAG)
 * @return FLAG_SET or FLAG_RESET
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName){
  if(pSPIx->SR & FlagName){
    return FLAG_SET;
  }
  return FLAG_RESET;
}

/**
 * @brief  Clears the Overrun (OVR) error flag by reading DR followed by SR.
 * @param  pSPIx : Base address of the SPI peripheral
 */
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx){
  uint8_t temp;
  temp = pSPIx->DR;
  temp = pSPIx->SR;
  (void)temp; // Prevent unused variable warning
}

/**
 * @brief  Closes an interrupt-driven SPI transmission by disabling TXEIE and resetting handle state.
 * @param  pSPIHandle : Pointer to SPI handle
 */
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle){
  pSPIHandle->pSPIx->CR2 &= ~(1 << 7); // Disable TXEIE
  pSPIHandle->pTxBuffer = NULL;
  pSPIHandle->TxLen = 0;
  pSPIHandle->TxState = SPI_READY;
}

/**
 * @brief  Closes an interrupt-driven SPI reception by disabling RXNEIE and resetting handle state.
 * @param  pSPIHandle : Pointer to SPI handle
 */
void SPI_CloseReception(SPI_Handle_t *pSPIHandle){
  pSPIHandle->pSPIx->CR2 &= ~(1 << 6); // Disable RXNEIE
  pSPIHandle->pRxBuffer = NULL;
  pSPIHandle->RxLen = 0;
  pSPIHandle->RxState = SPI_READY;
}

/**
 * @brief  Initializes the SPI peripheral registers according to settings in pSPIHandle.
 *         Configures CR1 (Mode, Bus config, Baud rate, CPOL, CPHA, SSM) and CR2 (Data size, FRXTH).
 * @param  pSPIHandle : Pointer to SPI handle
 */
void SPI_Init(SPI_Handle_t *pSPIHandle){
  uint32_t tempreg = 0;

  // 1. Configure Device Mode: Master (MSTR=1) or Slave (MSTR=0)
  tempreg |= (pSPIHandle->SPIConfig.SPI_DeviceMode << 2);

  // 2. Configure Bus Architecture (Full-duplex, Half-duplex, or Simplex Rx-only)
  if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD){
    tempreg &= ~(1 << 15); // BIDIMODE = 0 (2-line unidirectional)
  }
  else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD){
    tempreg |= (1 << 15);  // BIDIMODE = 1 (1-line bidirectional)
  }
  else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY){
    tempreg &= ~(1 << 15); // BIDIMODE = 0
    tempreg |= (1 << 10);  // RXONLY = 1 (output disabled, receive-only)
  }

  // 3. Configure Baud Rate prescaler (BR[2:0] bits 5:3)
  tempreg |= (pSPIHandle->SPIConfig.SPI_SClkSpeed << 3);

  // 4. Configure CRC Length (CRCL bit 11)
  tempreg |= (pSPIHandle->SPIConfig.SPI_CRCL << 11);

  // 5. Configure Clock Polarity (CPOL bit 1)
  tempreg |= (pSPIHandle->SPIConfig.SPI_CPOL << 1);

  // 6. Configure Clock Phase (CPHA bit 0)
  tempreg |= (pSPIHandle->SPIConfig.SPI_CPHA << 0);

  // 7. Configure Software Slave Management (SSM bit 9)
  tempreg |= (pSPIHandle->SPIConfig.SPI_SSM << 9);

  pSPIHandle->pSPIx->CR1 = tempreg;

  // 8. Configure Data Size in CR2 (DS[3:0] bits 11:8)
  tempreg = 0;
  tempreg |= (pSPIHandle->SPIConfig.SPI_DataSize << 8);

  // On STM32, if data size is <= 8 bits, set FRXTH (bit 12) so RXNE triggers on 1 byte (FIFO 1/4 full)
  if(pSPIHandle->SPIConfig.SPI_DataSize == SPI_DATASIZE_8BIT){
    tempreg |= (1 << 12); // FRXTH = 1 (8-bit threshold)
  }

  pSPIHandle->pSPIx->CR2 = tempreg;
}

/**
 * @brief  De-initializes the SPI registers by triggering RCC reset.
 * @param  pSPIx : Base address of the SPI peripheral
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx){
  if (pSPIx == SPI1){
    SPI1_REG_RESET();
  }
  else if (pSPIx == SPI2){
    SPI2_REG_RESET();
  }
}

/**
 * @brief  Blocking data transmission over SPI.
 *         Waits for TXE flag before writing each byte/halfword to DR.
 * @param  pSPIx     : Base address of the SPI peripheral
 * @param  pTxBuffer : Pointer to transmit data buffer
 * @param  Len       : Length of data in bytes
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len){
  uint8_t datasize = (pSPIx->CR2 >> 8) & 0xF;
  while(Len > 0){
    // Wait until TXE is set (transmit buffer is empty)
    while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

    if(datasize == SPI_DATASIZE_16BIT){
      // 16-bit frame format
      *(volatile uint16_t *)&pSPIx->DR = *((uint16_t*) pTxBuffer);
      Len -= 2;
      pTxBuffer += 2;
    }
    else{
      // 8-bit frame format
      *(volatile uint8_t *)&pSPIx->DR = *pTxBuffer;
      Len--;
      pTxBuffer++;
    }
  }
}

/**
 * @brief  Blocking data reception over SPI.
 *         Waits for RXNE flag before reading each byte/halfword from DR.
 * @param  pSPIx     : Base address of the SPI peripheral
 * @param  pRxBuffer : Pointer to receive data buffer
 * @param  Len       : Length of data in bytes
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len){
  uint8_t datasize = (pSPIx->CR2 >> 8) & 0xF;
  while(Len > 0){
    // Wait until RXNE is set (receive buffer has data)
    while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

    if(datasize == SPI_DATASIZE_16BIT){
      // 16-bit frame format
      *((uint16_t*) pRxBuffer) = *(volatile uint16_t *)&pSPIx->DR;
      Len -= 2;
      pRxBuffer += 2;
    }
    else{
      // 8-bit frame format
      *pRxBuffer = *(volatile uint8_t *)&pSPIx->DR;
      Len--;
      pRxBuffer++;
    }
  }
}

/**
 * @brief  Initiates a non-blocking transmission using interrupts.
 *         Sets up the handle state and enables TXEIE in CR2.
 * @param  pSPIHandle : Pointer to SPI handle
 * @param  pTxBuffer  : Pointer to data to send
 * @param  Len        : Number of bytes to send
 * @return SPI state (SPI_READY or SPI_BUSY_IN_TX)
 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len){
  uint8_t state = pSPIHandle->TxState;
  if(state != SPI_BUSY_IN_TX){
    pSPIHandle->pTxBuffer = pTxBuffer;
    pSPIHandle->TxLen = Len;
    pSPIHandle->TxState = SPI_BUSY_IN_TX;
    pSPIHandle->pSPIx->CR2 |= (1 << 7); // Enable TXEIE
  }
  return state;
}

/**
 * @brief  Initiates a non-blocking reception using interrupts.
 *         Sets up the handle state and enables RXNEIE in CR2.
 * @param  pSPIHandle : Pointer to SPI handle
 * @param  pRxBuffer  : Pointer to buffer for incoming data
 * @param  Len        : Number of bytes to receive
 * @return SPI state (SPI_READY or SPI_BUSY_IN_RX)
 */
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len){
  uint8_t state = pSPIHandle->RxState;
  if(state != SPI_BUSY_IN_RX){
    pSPIHandle->pRxBuffer = pRxBuffer;
    pSPIHandle->RxLen = Len;
    pSPIHandle->RxState = SPI_BUSY_IN_RX;
    pSPIHandle->pSPIx->CR2 |= (1 << 6); // Enable RXNEIE
  }
  return state;
}

/**
 * @brief  Enables or disables SPI IRQ in the ARM Cortex-M0+ NVIC.
 * @param  IRQNumber : NVIC IRQ line
 * @param  EnorDi    : ENABLE or DISABLE
 */
void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    *NVIC_ISER |= (1 << IRQNumber);
  }
  else{
    *NVIC_ICER |= (1 << IRQNumber);
  }
}

/**
 * @brief  Sets priority for the SPI IRQ in the NVIC.
 * @param  IRQPriority : Priority level (0 to 3)
 * @param  IRQNumber   : NVIC IRQ line
 */
void SPI_PriorityConfig(uint8_t IRQPriority, uint8_t IRQNumber){
  uint8_t iprx = IRQNumber / 4;
  uint8_t iprx_section = IRQNumber % 4;
  uint8_t shift_amount = (8 * iprx_section) + 6;
  NVIC_PR_BASEADDR[iprx] |= (IRQPriority << shift_amount);
}

/**
 * @brief  SPI Interrupt Handler. Dispatches to TXE, RXNE, or error handlers
 *         based on active interrupt enable bits and status flags.
 * @param  pHandle : Pointer to SPI handle
 */
void SPI_IRQHandling(SPI_Handle_t *pHandle){
  uint8_t temp1, temp2;

  // 1. Check for TXE event
  temp1 = pHandle->pSPIx->SR & (1 << 1);  // TXE flag
  temp2 = pHandle->pSPIx->CR2 & (1 << 7); // TXEIE bit
  if(temp1 && temp2){
    spi_txe_interrupt_handle(pHandle);
  }

  // 2. Check for RXNE event
  temp1 = pHandle->pSPIx->SR & (1 << 0);  // RXNE flag
  temp2 = pHandle->pSPIx->CR2 & (1 << 6); // RXNEIE bit
  if(temp1 && temp2){
    spi_rxne_interrupt_handle(pHandle);
  }

  // 3. Check for Overrun (OVR) error event
  temp1 = pHandle->pSPIx->SR & (1 << 6);  // OVR flag
  temp2 = pHandle->pSPIx->CR2 & (1 << 5); // ERRIE bit
  if(temp1 && temp2){
    spi_ovr_err_interrupt_handle(pHandle);
  }
}

/**
 * @brief  Helper function to handle TXE interrupt during non-blocking transmission.
 */
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle){
  uint8_t datasize = (pSPIHandle->pSPIx->CR2 >> 8) & 0xF;
  if(datasize == SPI_DATASIZE_16BIT){
    (*(volatile uint16_t *)&pSPIHandle->pSPIx->DR) = *((uint16_t*) pSPIHandle->pTxBuffer);
    pSPIHandle->TxLen -= 2;
    pSPIHandle->pTxBuffer += 2;
  }
  else{
    (*(volatile uint8_t *)&pSPIHandle->pSPIx->DR) = *(pSPIHandle->pTxBuffer);
    pSPIHandle->TxLen--;
    pSPIHandle->pTxBuffer++;
  }

  if(!pSPIHandle->TxLen){
    SPI_CloseTransmission(pSPIHandle);
    SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_TX_CMPLT);
  }
}

/**
 * @brief  Helper function to handle RXNE interrupt during non-blocking reception.
 */
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle){
  uint8_t datasize = (pSPIHandle->pSPIx->CR2 >> 8) & 0xF;
  if(datasize == SPI_DATASIZE_16BIT){
    *((uint16_t*) pSPIHandle->pRxBuffer) = (*(volatile uint16_t *)&pSPIHandle->pSPIx->DR);
    pSPIHandle->RxLen -= 2;
    pSPIHandle->pRxBuffer += 2;
  }
  else{
    *(pSPIHandle->pRxBuffer) = (*(volatile uint8_t *)&pSPIHandle->pSPIx->DR);
    pSPIHandle->RxLen--;
    pSPIHandle->pRxBuffer++;
  }

  if(!pSPIHandle->RxLen){
    SPI_CloseReception(pSPIHandle);
    SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_RX_CMPLT);
  }
}

/**
 * @brief  Helper function to handle Overrun error interrupt.
 */
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pSPIHandle){ 
  SPI_ClearOVRFlag(pSPIHandle->pSPIx);
  SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_OVR_ERR);
}

/**
 * @brief  Weak callback function. The user application can override this function.
 */
__attribute__ ((weak)) void SPI_ApplicationEventCallBack(SPI_Handle_t *pSPIHandle, uint8_t AppEv){
  (void)pSPIHandle;
  (void)AppEv;
}
