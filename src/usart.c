/**
 ******************************************************************************
 * @file    usart.c
 * @brief   USART peripheral driver implementation for STM32C031C6.
 *          Implements initialization, de-initialization, blocking transmit/receive,
 *          dynamic baud rate calculation, and status flag handling.
 ******************************************************************************
 */

#include "stm32c031c6.h"
#include "usart.h"

// Forward declaration of internal helper
static uint32_t USART_GetPCLKValue(void);

/**
 * @brief  Enables or disables peripheral clock for the given USART instance.
 * @param  pUSARTx : Base address of the USART peripheral (USART1 - USART4)
 * @param  EnorDi  : ENABLE or DISABLE
 */
void USART_PeriClkCtrl(USART_RegDef_t *pUSARTx, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    if(pUSARTx == USART1){
      USART1_PCLK_EN();
    }
    else if(pUSARTx == USART2){
      USART2_PCLK_EN();
    }
    else if(pUSARTx == USART3){
      USART3_PCLK_EN();
    }
    else if(pUSARTx == USART4){
      USART4_PCLK_EN();
    }
  }
  else{
    if(pUSARTx == USART1){
      USART1_PCLK_DI();
    }
    else if(pUSARTx == USART2){
      USART2_PCLK_DI();
    }
    else if(pUSARTx == USART3){
      USART3_PCLK_DI();
    }
    else if(pUSARTx == USART4){
      USART4_PCLK_DI();
    }
  }
}

/**
 * @brief  Enables or disables the USART peripheral (CR1 UE bit).
 * @param  pUSARTx : Base address of the USART peripheral
 * @param  EnorDi  : ENABLE (UE = 1) or DISABLE (UE = 0)
 */
void USART_PeriCtrl(USART_RegDef_t *pUSARTx, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    pUSARTx->CR1 |= (1 << 0);
  }
  else{
    pUSARTx->CR1 &= ~(1 << 0);
  }
}

/**
 * @brief  Checks whether a specific status flag in USART_ISR is SET or RESET.
 * @param  pUSARTx  : Base address of the USART peripheral
 * @param  FlagName : Flag mask to check (e.g. USART_FLAG_TXE, USART_FLAG_RXNE, USART_FLAG_TC)
 * @return FLAG_SET or FLAG_RESET
 */
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName){
  if(pUSARTx->ISR & FlagName){
    return FLAG_SET;
  }
  return FLAG_RESET;
}

/**
 * @brief  Clears a specific status flag by writing to the USART Interrupt Clear Register (ICR).
 * @param  pUSARTx  : Base address of the USART peripheral
 * @param  FlagName : Flag mask to clear
 */
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint32_t FlagName){
  pUSARTx->ICR |= FlagName;
}

/**
 * @brief  Helper function to calculate the current peripheral clock (PCLK) frequency
 *         by inspecting the RCC clock configuration and prescaler registers.
 * @return PCLK frequency in Hz
 */
static uint32_t USART_GetPCLKValue(void){
  uint32_t pclk = 48000000U;
  uint8_t temp;

  // HSI division factor in RCC_CR (HSIDIV bits [13:11])
  uint16_t HSI_Prescaler[8] = {1, 2, 4, 8, 16, 32, 64, 128};
  temp = (RCC->CR >> 11) & 0x07;
  pclk = 48000000U / HSI_Prescaler[temp];

  // AHB prescaler in RCC_CFGR (HPRE bits [11:8])
  uint16_t AHB_Prescaler[8] = {2, 4, 8, 16, 64, 128, 256, 512};
  temp = (RCC->CFGR >> 8) & 0x0F;
  if(temp >= 8){
    pclk = pclk / AHB_Prescaler[temp - 8];
  }

  // APB prescaler in RCC_CFGR (PPRE bits [14:12])
  uint16_t APB_Prescaler[4] = {2, 4, 8, 16};
  temp = (RCC->CFGR >> 12) & 0x07;
  if(temp >= 4){
    pclk = pclk / APB_Prescaler[temp - 4];
  }

  return pclk;
}

/**
 * @brief  Calculates and sets the baud rate in the USART Baud Rate Register (BRR).
 *         Supports both 16x oversampling (OVER8 = 0) and 8x oversampling (OVER8 = 1).
 * @param  pUSARTx      : Base address of the USART peripheral
 * @param  BaudRate     : Desired baud rate (e.g. 9600, 115200)
 * @param  Oversampling : USART_OVERSAMPLING_16 or USART_OVERSAMPLING_8
 */
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate, uint8_t Oversampling){
  uint32_t pclk = USART_GetPCLKValue();
  uint32_t usartdiv;

  if(Oversampling == USART_OVERSAMPLING_8){
    // When OVER8 = 1: BRR[2:0] = USARTDIV[3:0] >> 1, BRR[3] = 0, BRR[15:4] = USARTDIV[15:4]
    usartdiv = ((2U * pclk) + (BaudRate / 2U)) / BaudRate;
    pUSARTx->BRR = (usartdiv & 0xFFF0U) | ((usartdiv & 0x000FU) >> 1);
  }
  else{
    // When OVER8 = 0 (16x oversampling): BRR = USARTDIV
    usartdiv = (pclk + (BaudRate / 2U)) / BaudRate;
    pUSARTx->BRR = (uint16_t)(usartdiv & 0xFFFFU);
  }
}

/**
 * @brief  Initializes the USART peripheral according to the specified parameters in pUSARTHandle.
 *         Configures Word Length, Parity, Mode (TX/RX), Oversampling, Stop Bits, and Baud Rate.
 * @param  pUSARTHandle : Pointer to the USART handle structure
 */
void USART_Init(USART_Handle_t *pUSARTHandle){
  uint32_t tempreg = 0;

  // 1. Enable peripheral clock
  USART_PeriClkCtrl(pUSARTHandle->pUSARTx, ENABLE);

  // 2. Disable USART (UE = 0) during configuration
  pUSARTHandle->pUSARTx->CR1 &= ~(1 << 0);

  // 3. Configure Word Length (CR1 M0 bit 12, M1 bit 28)
  if(pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_9BITS){
    tempreg |= (1 << 12);
  }
  else if(pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_7BITS){
    tempreg |= (1 << 28);
  }

  // 4. Configure Parity Control (CR1 PCE bit 10, PS bit 9)
  if(pUSARTHandle->USARTConfig.USART_ParityControl == USART_PARITY_ENABLE){
    tempreg |= (1 << 10);
    if(pUSARTHandle->USARTConfig.USART_ParitySelection == USART_PARITY_ODD){
      tempreg |= (1 << 9);
    }
  }

  // 5. Configure Transmitter / Receiver mode (CR1 TE bit 3, RE bit 2)
  if(pUSARTHandle->USARTConfig.USART_Mode == USART_MODE_ONLY_TX){
    tempreg |= (1 << 3);
  }
  else if(pUSARTHandle->USARTConfig.USART_Mode == USART_MODE_ONLY_RX){
    tempreg |= (1 << 2);
  }
  else if(pUSARTHandle->USARTConfig.USART_Mode == USART_MODE_TXRX){
    tempreg |= (1 << 3) | (1 << 2);
  }

  // 6. Configure Oversampling (CR1 OVER8 bit 15)
  if(pUSARTHandle->USARTConfig.USART_Oversampling == USART_OVERSAMPLING_8){
    tempreg |= (1 << 15);
  }

  // 7. Configure FIFO Mode (CR1 FIFOEN bit 29)
  if(pUSARTHandle->USARTConfig.USART_FIFO == USART_FIFO_ENABLE){
    tempreg |= (1 << 29);
  }

  pUSARTHandle->pUSARTx->CR1 = tempreg;

  // 8. Configure Stop Bits (CR2 STOP bits [13:12])
  tempreg = 0;
  tempreg |= ((uint32_t)pUSARTHandle->USARTConfig.USART_StopBits << 12);
  pUSARTHandle->pUSARTx->CR2 = tempreg;

  // 9. Configure Baud Rate
  if(pUSARTHandle->USARTConfig.USART_BaudRate > 0){
    USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USARTConfig.USART_BaudRate, pUSARTHandle->USARTConfig.USART_Oversampling);
  }

  // 10. Enable USART peripheral (CR1 UE bit 0)
  pUSARTHandle->pUSARTx->CR1 |= (1 << 0);
}

/**
 * @brief  De-initializes the USART peripheral by triggering RCC reset.
 * @param  pUSARTx : Base address of the USART peripheral
 */
void USART_DeInit(USART_RegDef_t *pUSARTx){
  if(pUSARTx == USART1){
    USART1_REG_RESET();
  }
  else if(pUSARTx == USART2){
    USART2_REG_RESET();
  }
  else if(pUSARTx == USART3){
    USART3_REG_RESET();
  }
  else if(pUSARTx == USART4){
    USART4_REG_RESET();
  }
}

/**
 * @brief  Blocking data transmission over USART.
 *         Waits for TXE (Transmit data register empty) before writing each frame to TDR,
 *         then waits for TC (Transmission Complete) before returning.
 * @param  pUSARTHandle : Pointer to the USART handle structure
 * @param  pTxBuffer    : Pointer to the transmit data buffer
 * @param  Len          : Number of bytes to transmit
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len){
  while(Len > 0){
    // Wait until TXE flag is set
    while(USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXE) == FLAG_RESET);

    // 9-bit word length without parity
    if((pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_9BITS) &&
       (pUSARTHandle->USARTConfig.USART_ParityControl == USART_PARITY_DISABLE)){
      pUSARTHandle->pUSARTx->TDR = (*((uint16_t*)pTxBuffer) & 0x01FFU);
      if(Len > 1){
        pTxBuffer += 2;
        Len -= 2;
      }
      else{
        pTxBuffer++;
        Len--;
      }
    }
    else{
      // 8-bit or 7-bit data transfer
      pUSARTHandle->pUSARTx->TDR = (*pTxBuffer & 0xFFU);
      pTxBuffer++;
      Len--;
    }
  }

  // Wait until transmission complete (TC flag set)
  while(USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TC) == FLAG_RESET);
}

/**
 * @brief  Blocking data reception over USART.
 *         Waits for RXNE (Read data register not empty) before reading each frame from RDR.
 * @param  pUSARTHandle : Pointer to the USART handle structure
 * @param  pRxBuffer    : Pointer to the receive data buffer
 * @param  Len          : Number of bytes to receive
 */
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len){
  while(Len > 0){
    // Wait until RXNE flag is set
    while(USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE) == FLAG_RESET);

    // 9-bit word length without parity
    if((pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_9BITS) &&
       (pUSARTHandle->USARTConfig.USART_ParityControl == USART_PARITY_DISABLE)){
      *((uint16_t*)pRxBuffer) = (uint16_t)(pUSARTHandle->pUSARTx->RDR & 0x01FFU);
      if(Len > 1){
        pRxBuffer += 2;
        Len -= 2;
      }
      else{
        pRxBuffer++;
        Len--;
      }
    }
    else{
      // 8-bit or 7-bit data reception
      if(pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_7BITS){
        *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->RDR & 0x7FU);
      }
      else if((pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_8BITS) &&
              (pUSARTHandle->USARTConfig.USART_ParityControl == USART_PARITY_ENABLE)){
        *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->RDR & 0x7FU);
      }
      else{
        *pRxBuffer = (uint8_t)(pUSARTHandle->pUSARTx->RDR & 0xFFU);
      }
      pRxBuffer++;
      Len--;
    }
  }
}
