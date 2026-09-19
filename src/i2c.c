#include "stm32c031c6.h"

/*
  // This is assuming that pclk is our source of clock for i2c and hsi is our clock for the system.
  uint16_t HSI_Prescaler[8] = {1, 2, 4, 8, 16, 32, 64, 128}
  uint16_t AHB_Prescaler[8] = {2, 4, 8, 16, 64, 128, 256, 512}
  uint16_t APB_Prescaler[4] = {2, 4, 8, 16}
  uint32_t GetClkVal(void){
  uint32_t pclk, HSISysClk;
  uint8_t temp, ahbp, apbp, hsip;
  HSISysClk = 48000000;
  temp = ((RCC->CR >> 11) & 0x07);
  hsip = HSI_Prescaler[temp];
  temp = ((RCC->CFGR >> 8) & 0x0F);
  if(temp < 8){
    ahbp = 1;
  }
  else{
    ahbp = AHB_Prescaler[temp - 8];
  }
  temp = ((RCC->CFGR >> 12) & 0x07);
  if(temp < 4){
    ahbp = 1;
  }
  else{
    ahbp = AHB_Prescaler[temp - 4];
  }
  pclk = ((HSISysClk/hsip)/ahbp)/apbp;
  return pclk;

  }*/



void I2C_PeriClkCtrl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi) {
  if (EnorDi == ENABLE) {
    if (pI2Cx == I2C1) {
      I2C1_PCLK_EN();
    }
    else if (pI2Cx == I2C2) {
      I2C2_PCLK_EN();
    }
  }
  else {
    if (pI2Cx == I2C1) {
      I2C1_PCLK_DI();
    }
    else if (pI2Cx == I2C2) {
      I2C2_PCLK_DI();
    }
  }
}

void I2C_PeriCtrl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi) {
  if (EnorDi == ENABLE) {
    pI2Cx->CR1 |= (1 << 0);
  }
  else {
    pI2Cx->CR1 &= ~(1 << 0);
  }
}


uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName) {
  if (pI2Cx->ISR & FlagName) {
    return FLAG_SET;
  }

  return FLAG_RESET;
}

void I2C_Init(I2C_Handle_t *pI2CHandle) {
  if (pI2CHandle->I2CConfig.I2C_SCLSpeed == I2C_SCL_SPEED_SM) {
    pI2CHandle->pI2Cx->TIMINGR = 0x00402D41; // Hardcoded value for 12mhz clock which is the default clock for APB bus
  }
  else if (pI2CHandle->I2CConfig.I2C_SCLSpeed == I2C_SCL_SPEED_FM) {
    pI2CHandle->pI2Cx->TIMINGR = 0x00200410;
  }
  /*
  uint32_t tempreg = 0;
  tempreg |= pI2CHandle->I2CConfig.I2C_DeviceAddress << 1;
  tempreg |=  1 << 15;
  pI2CHandle->pI2Cx->OAR1 = tempreg;*/


}

void I2C_DeInit(I2C_RegDef_t *pI2Cx) {
  if (pI2Cx == I2C1) {
    I2C1_REG_RESET();
  }
  else if (pI2Cx == I2C2) {
    I2C2_REG_RESET();
  }
}

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr)
{
    /* Clear ADD10 — use 7-bit addressing */
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 11);

    /* Clear RD_WRN — write transfer */
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 10);

    /* Clear AUTOEND — we want software control over STOP/Repeated Start */
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 25);

    /* Clear RELOAD */
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 24);

    /* Clear SADD bits [9:0], then set 7-bit slave address */
    pI2CHandle->pI2Cx->CR2 &= ~(0x3FF << 0);
    pI2CHandle->pI2Cx->CR2 |= (SlaveAddr << 1);

    /* Set NBYTES */
    pI2CHandle->pI2Cx->CR2 &= ~(0xFF << 16);
    pI2CHandle->pI2Cx->CR2 |= (Len << 16);

    /* Generate START */
    pI2CHandle->pI2Cx->CR2 |= (1 << 13);

    /* Transmit data */
    while (Len > 0)
{
      while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TXIS_FLAG))
      {
          if (I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_NACK_FLAG))
          {
              pI2CHandle->pI2Cx->ICR = (1 << 4);
              return;
          }
      }

      pI2CHandle->pI2Cx->TXDR = *pTxBuffer;

      pTxBuffer++;
      Len--;
}

    /* Wait for Transfer Complete (TC) */
    while (!I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_TC_FLAG));

    /* If Repeated Start is DISABLED, generate a STOP condition */
    if (Sr == I2C_DISABLE_SR) {
        pI2CHandle->pI2Cx->CR2 |= (1 << 14); /* STOP */
    }
    
    /* If Sr == I2C_ENABLE_SR, we simply exit while TC is still set. 
       The next MasterSend or MasterReceive call will issue the next START. */
}


void I2C_MasterReceiveData(
    I2C_Handle_t *pI2CHandle,
    uint8_t *pRxBuffer,
    uint32_t Len,
    uint8_t SlaveAddr,
    uint8_t Sr)
{
    /* 7-bit addressing */
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 11);

    /* No reload */
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 24);

    /* Slave address */
    pI2CHandle->pI2Cx->CR2 &= ~(0x3FF << 0);
    pI2CHandle->pI2Cx->CR2 |= ((uint32_t)SlaveAddr << 1);

    /* Read direction */
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 10);
    pI2CHandle->pI2Cx->CR2 |=  (1 << 10);

    /*
     * AUTOEND:
     *
     * Sr = ENABLE  -> AUTOEND = 0
     *                 leave bus active
     *
     * Sr = DISABLE -> AUTOEND = 1
     *                 generate STOP automatically
     */
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 25);

    if (Sr == I2C_DISABLE_SR)
    {
        pI2CHandle->pI2Cx->CR2 |= (1 << 25);
    }

    /* NBYTES */
    pI2CHandle->pI2Cx->CR2 &= ~(0xFF << 16);
    pI2CHandle->pI2Cx->CR2 |= ((uint32_t)Len << 16);

    /* START */
    pI2CHandle->pI2Cx->CR2 |= (1 << 13);


    /* ================= RECEIVE ================= */

    while (Len > 0)
    {
        /*
         * Check for NACK instead of waiting forever.
         */
        if (I2C_GetFlagStatus(
                pI2CHandle->pI2Cx,
                I2C_NACK_FLAG))
        {
            /*
             * Clear NACKF
             */
            pI2CHandle->pI2Cx->ICR = (1 << 4);

            /*
             * End transaction
             */
            pI2CHandle->pI2Cx->CR2 |= (1 << 14);

            return;
        }

        /*
         * Wait for received byte.
         */
        while (!I2C_GetFlagStatus(
                    pI2CHandle->pI2Cx,
                    I2C_RXNE_FLAG))
        {
            if (I2C_GetFlagStatus(
                    pI2CHandle->pI2Cx,
                    I2C_NACK_FLAG))
            {
                pI2CHandle->pI2Cx->ICR = (1 << 4);
                pI2CHandle->pI2Cx->CR2 |= (1 << 14);
                return;
            }
        }

        /*
         * RXNE = 1
         * Read RXDR.
         */
        *pRxBuffer = pI2CHandle->pI2Cx->RXDR;

        pRxBuffer++;
        Len--;
    }


    /* ================= END TRANSFER ================= */

    if (Sr == I2C_DISABLE_SR)
    {
        /*
         * AUTOEND = 1.
         * Hardware generates STOP.
         */
        while (!I2C_GetFlagStatus(
                    pI2CHandle->pI2Cx,
                    I2C_STOP_FLAG))
        {
        }

        /*
         * Clear STOPF.
         */
        pI2CHandle->pI2Cx->ICR = (1 << 5);
    }
    else
    {
        /*
         * AUTOEND = 0.
         * Wait for TC.
         */
        while (!I2C_GetFlagStatus(
                    pI2CHandle->pI2Cx,
                    I2C_TC_FLAG))
        {
        }
    }
}

uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr) {
  uint8_t busystate = pI2CHandle->TxRxState;
  if ((busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX)) {
    pI2CHandle->pTxBuffer = pTxBuffer;
    pI2CHandle->TxLen = Len;
    pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
    pI2CHandle->DevAddr = SlaveAddr;
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 11);
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 10);
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 25);
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 24);
    pI2CHandle->pI2Cx->CR2 &= ~(0x3FF << 0);
    pI2CHandle->pI2Cx->CR2 |= (SlaveAddr << 1);
    pI2CHandle->pI2Cx->CR2 &= ~(0xFF << 16);
    pI2CHandle->pI2Cx->CR2 |= (Len << 16);
    pI2CHandle->pI2Cx->CR1 |= (1 << 1);
    pI2CHandle->pI2Cx->CR1 |= (1 << 4);
    pI2CHandle->pI2Cx->CR1 |= (1 << 5);
    pI2CHandle->pI2Cx->CR1 |= (1 << 6);
    pI2CHandle->pI2Cx->CR1 |= (1 << 7);
    pI2CHandle->pI2Cx->CR2 |= (1 << 13);
  }
  return busystate;
}


uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr) {
  uint8_t busystate = pI2CHandle->TxRxState;
  if ((busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX)) {
    pI2CHandle->pRxBuffer = pRxBuffer;
    pI2CHandle->RxLen = Len;
    pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
    pI2CHandle->DevAddr = SlaveAddr;
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 11);
    pI2CHandle->pI2Cx->CR2 |= (1 << 10);
    pI2CHandle->pI2Cx->CR2 &= ~(1 << 24);
    pI2CHandle->pI2Cx->CR2 |= (1 << 25);
    pI2CHandle->pI2Cx->CR2 &= ~(0x3FF << 0);
    pI2CHandle->pI2Cx->CR2 |= (SlaveAddr << 1);
    pI2CHandle->pI2Cx->CR2 &= ~(0xFF << 16);
    pI2CHandle->pI2Cx->CR2 |= (Len << 16);
    pI2CHandle->pI2Cx->CR1 |= (1 << 2);
    pI2CHandle->pI2Cx->CR1 |= (1 << 4);
    pI2CHandle->pI2Cx->CR1 |= (1 << 5);
    pI2CHandle->pI2Cx->CR1 |= (1 << 6);
    pI2CHandle->pI2Cx->CR1 |= (1 << 7);
    pI2CHandle->pI2Cx->CR2 |= (1 << 13);
  }
  return busystate;
}

void I2C_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi) {
  if (EnorDi == ENABLE) {
    *NVIC_ISER |= (1 << IRQNumber);
  }
  else {
    *NVIC_ICER |= (1 << IRQNumber);
  }
}

void I2C_PriorityConfig(uint8_t IRQPriority, uint8_t IRQNumber) {
  uint8_t iprx = IRQNumber / 4;
  uint8_t iprx_section = IRQNumber % 4;
  uint8_t shift_amount = (8 * iprx_section) + 6;
  NVIC_PR_BASEADDR[iprx] |= (IRQPriority << shift_amount);
}

void I2C_EV_IRQHandling(I2C_Handle_t *pHandle) {
  uint32_t txit, rxit, addrit, stopit, tcit, nackit, temp;

  txit   = pHandle->pI2Cx->CR1 & (1 << 1);   /* TXIE   */
  rxit   = pHandle->pI2Cx->CR1 & (1 << 2);   /* RXIE   */
  addrit = pHandle->pI2Cx->CR1 & (1 << 3);   /* ADDRIE */
  nackit = pHandle->pI2Cx->CR1 & (1 << 4);   /* NACKIE */
  stopit = pHandle->pI2Cx->CR1 & (1 << 5);   /* STOPIE */
  tcit   = pHandle->pI2Cx->CR1 & (1 << 6);   /* TCIE   */

  /* TXIS */
  temp = pHandle->pI2Cx->ISR & (1 << 1);
  if (temp && txit) {
    if (pHandle->TxRxState == I2C_BUSY_IN_TX && pHandle->TxLen > 0) {
      pHandle->pI2Cx->TXDR = *(pHandle->pTxBuffer);
      pHandle->TxLen--;
      pHandle->pTxBuffer++;
    }
  }

  /* RXNE */
  temp = pHandle->pI2Cx->ISR & (1 << 2);
  if (temp && rxit) {
    if (pHandle->TxRxState == I2C_BUSY_IN_RX && pHandle->RxLen > 0) {
      *(pHandle->pRxBuffer) = pHandle->pI2Cx->RXDR;
      pHandle->RxLen--;
      pHandle->pRxBuffer++;
    }
  }

  /* ADDR */
  temp = pHandle->pI2Cx->ISR & (1 << 3);
  if (temp && addrit) {
    pHandle->pI2Cx->ICR |= (1 << 3);
  }

  /* NACKF */
  temp = pHandle->pI2Cx->ISR & (1 << 4);
  if (temp && nackit) {
    pHandle->pI2Cx->ICR |= (1 << 4);

    pHandle->pI2Cx->CR2 |= (1 << 14);   /* STOP */

    pHandle->pI2Cx->CR1 &= ~((1 << 1) | (1 << 2) | (1 << 4) |
                             (1 << 5) | (1 << 6));

    pHandle->TxLen = 0;
    pHandle->RxLen = 0;
    pHandle->TxRxState = I2C_READY;

    I2C_ApplicationEventCallBack(pHandle, I2C_EV_NACK);
  }

  /* STOPF */
  temp = pHandle->pI2Cx->ISR & (1 << 5);
  if (temp && stopit) {
    pHandle->pI2Cx->ICR |= (1 << 5);

    if (pHandle->TxRxState == I2C_BUSY_IN_RX) {
      pHandle->pI2Cx->CR1 &= ~((1 << 2) | (1 << 4) |
                               (1 << 5) | (1 << 6));

      pHandle->RxLen = 0;
      pHandle->TxRxState = I2C_READY;
      pHandle->pRxBuffer = NULL;

      I2C_ApplicationEventCallBack(pHandle, I2C_EV_RX_CMPLT);
    }

    I2C_ApplicationEventCallBack(pHandle, I2C_EV_STOP);
  }

  /* TC */
  temp = pHandle->pI2Cx->ISR & (1 << 6);
  if (temp && tcit) {
    if (pHandle->TxRxState == I2C_BUSY_IN_TX &&
        pHandle->TxLen == 0) {

      pHandle->pI2Cx->CR2 |= (1 << 14);   /* STOP */

      /* Disable TX-related interrupts */
      pHandle->pI2Cx->CR1 &= ~((1 << 1) | (1 << 4) |
                               (1 << 5) | (1 << 6));

      pHandle->TxRxState = I2C_READY;
      pHandle->pTxBuffer = NULL;
      pHandle->TxLen = 0;

      I2C_ApplicationEventCallBack(pHandle, I2C_EV_TX_CMPLT);
    }
  }
}

void I2C_ER_IRQHandling(I2C_Handle_t *pHandle)
{
  uint32_t errit, temp;

  errit = pHandle->pI2Cx->CR1 & (1 << 7);   /* ERRIE */

  /* BERR */
  temp = pHandle->pI2Cx->ISR & (1 << 8);
  if (temp && errit) {
    pHandle->pI2Cx->ICR |= (1 << 8);

    pHandle->pI2Cx->CR1 &= ~((1 << 1) | (1 << 2) | (1 << 4) |
                             (1 << 5) | (1 << 6));

    pHandle->TxRxState = I2C_READY;
    pHandle->TxLen = 0;
    pHandle->RxLen = 0;

    I2C_ApplicationEventCallBack(pHandle, I2C_ERROR_BERR);
  }

  /* ARLO */
  temp = pHandle->pI2Cx->ISR & (1 << 9);
  if (temp && errit) {
    pHandle->pI2Cx->ICR |= (1 << 9);

    pHandle->pI2Cx->CR1 &= ~((1 << 1) | (1 << 2) | (1 << 4) |
                             (1 << 5) | (1 << 6));

    pHandle->TxRxState = I2C_READY;
    pHandle->TxLen = 0;
    pHandle->RxLen = 0;

    I2C_ApplicationEventCallBack(pHandle, I2C_ERROR_ARLO);
  }

  /* OVR */
  temp = pHandle->pI2Cx->ISR & (1 << 10);
  if (temp && errit) {
    pHandle->pI2Cx->ICR |= (1 << 10);

    pHandle->pI2Cx->CR1 &= ~((1 << 1) | (1 << 2) | (1 << 4) |
                             (1 << 5) | (1 << 6));

    pHandle->TxRxState = I2C_READY;
    pHandle->TxLen = 0;
    pHandle->RxLen = 0;

    I2C_ApplicationEventCallBack(pHandle, I2C_ERROR_OVR);
  }

  /* PECERR */
  temp = pHandle->pI2Cx->ISR & (1 << 11);
  if (temp && errit) {
    pHandle->pI2Cx->ICR |= (1 << 11);

    pHandle->pI2Cx->CR1 &= ~((1 << 1) | (1 << 2) | (1 << 4) |
                             (1 << 5) | (1 << 6));

    pHandle->TxRxState = I2C_READY;
    pHandle->TxLen = 0;
    pHandle->RxLen = 0;

    I2C_ApplicationEventCallBack(pHandle, I2C_ERROR_PECERR);
  }

  /* TIMEOUT */
  temp = pHandle->pI2Cx->ISR & (1 << 12);
  if (temp && errit) {
    pHandle->pI2Cx->ICR |= (1 << 12);

    pHandle->pI2Cx->CR1 &= ~((1 << 1) | (1 << 2) | (1 << 4) |
                             (1 << 5) | (1 << 6));

    pHandle->TxRxState = I2C_READY;
    pHandle->TxLen = 0;
    pHandle->RxLen = 0;

    I2C_ApplicationEventCallBack(pHandle, I2C_ERROR_TIMEOUT);
  }

  /* ALERT */
  temp = pHandle->pI2Cx->ISR & (1 << 13);
  if (temp && errit) {
    pHandle->pI2Cx->ICR |= (1 << 13);

    pHandle->pI2Cx->CR1 &= ~((1 << 1) | (1 << 2) | (1 << 4) |
                             (1 << 5) | (1 << 6));

    pHandle->TxRxState = I2C_READY;
    pHandle->TxLen = 0;
    pHandle->RxLen = 0;

    I2C_ApplicationEventCallBack(pHandle, I2C_ERROR_ALERT);
  }
}

__attribute__ ((weak)) void I2C_ApplicationEventCallBack(I2C_Handle_t *pI2CHandle, uint8_t AppEv) {

}
