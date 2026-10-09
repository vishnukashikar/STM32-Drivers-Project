#include "stm32c031c6.h"
#include "gpio.h"

/**
 * @brief  Enables or disables peripheral clock for the given GPIO port.
 * @param  pGPIOx : Base address of the GPIO port (GPIOA, GPIOB, etc.)
 * @param  EnorDi : ENABLE or DISABLE
 */
void GPIO_PeriClkCtrl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    if (pGPIOx == GPIOA){
      GPIOA_PCLK_EN();
    }
    else if (pGPIOx == GPIOB){
      GPIOB_PCLK_EN();
    }
    else if (pGPIOx == GPIOC){
      GPIOC_PCLK_EN();
    }
    else if (pGPIOx == GPIOD){
      GPIOD_PCLK_EN();
    }
    else if (pGPIOx == GPIOF){
      GPIOF_PCLK_EN();
    }
  }
  else{
    if (pGPIOx == GPIOA){
      GPIOA_PCLK_DI();
    }
    else if (pGPIOx == GPIOB){
      GPIOB_PCLK_DI();
    }
    else if (pGPIOx == GPIOC){
      GPIOC_PCLK_DI();
    }
    else if (pGPIOx == GPIOD){
      GPIOD_PCLK_DI();
    }
    else if (pGPIOx == GPIOF){
      GPIOF_PCLK_DI();
    }
  }
}

/**
 * @brief  Initializes the GPIO pin based on the settings in pGPIOHandle.
 *         Configures MODER, OSPEEDR, PUPDR, OTYPER, AFR, and EXTI registers.
 * @param  pGPIOHandle : Pointer to the GPIO handle structure
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle){
  uint32_t temp = 0;

  // 1. Configure the Pin Mode (Input, Output, Alternate Function, Analog, or Interrupt)
  if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
    // Non-interrupt modes: configure MODER (2 bits per pin)
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->MODER &= ~(0x03 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)); // Clear bitfield
    pGPIOHandle->pGPIOx->MODER |= temp;                                                         // Set bitfield
    temp = 0;
  }
  else{
    // Interrupt modes: configure EXTI trigger and SYSCFG/EXTICR port routing
    // Set pin mode as Input in MODER
    pGPIOHandle->pGPIOx->MODER &= ~(0x03 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

    // Configure falling edge trigger (FTSR) and rising edge trigger (RTSR)
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT){
      EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
      EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    }
    else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT){
      EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
      EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    }
    else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT){
      EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
      EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    }

    // Configure GPIO port selection in EXTI_EXTICR (4 pins per register, 8 bits per field on STM32C0)
    uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
    uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
    uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
    EXTI->EXTICR[temp1] &= ~(0xFF << (temp2 * 8));
    EXTI->EXTICR[temp1] |= (portcode << (temp2 * 8));

    // Enable EXTI interrupt delivery using Interrupt Mask Register (IMR)
    EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
  }

  // 2. Configure Output Speed in OSPEEDR (2 bits per pin)
  temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
  pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x03 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
  pGPIOHandle->pGPIOx->OSPEEDR |= temp;
  temp = 0;

  // 3. Configure Pull-Up / Pull-Down in PUPDR (2 bits per pin)
  temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
  pGPIOHandle->pGPIOx->PUPDR &= ~(0x03 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
  pGPIOHandle->pGPIOx->PUPDR |= temp;
  temp = 0;

  // 4. Configure Output Type in OTYPER (1 bit per pin: Push-Pull vs Open-Drain)
  temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
  pGPIOHandle->pGPIOx->OTYPER &= ~(0x01 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
  pGPIOHandle->pGPIOx->OTYPER |= temp;
  temp = 0;

  // 5. Configure Alternate Function (AFR: 4 bits per pin)
  if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber <= 7){
      // Pins 0-7 use AFRL
      temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
      pGPIOHandle->pGPIOx->AFRL &= ~(0x0F << (4 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
      pGPIOHandle->pGPIOx->AFRL |= temp;
      temp = 0;
    }
    else{
      // Pins 8-15 use AFRH
      temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber - 8)));
      pGPIOHandle->pGPIOx->AFRH &= ~(0x0F << (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber - 8)));
      pGPIOHandle->pGPIOx->AFRH |= temp;
      temp = 0;
    }
  }
}

/**
 * @brief  Resets the GPIO port registers using RCC reset registers.
 * @param  pGPIOx : Base address of the GPIO port
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
  if (pGPIOx == GPIOA){
    GPIOA_REG_RESET();
  }
  else if (pGPIOx == GPIOB){
    GPIOB_REG_RESET();
  }
  else if (pGPIOx == GPIOC){
    GPIOC_REG_RESET();
  }
  else if (pGPIOx == GPIOD){
    GPIOD_REG_RESET();
  }
  else if (pGPIOx == GPIOF){
    GPIOF_REG_RESET();
  }
}

/**
 * @brief  Reads the logic state of an individual GPIO input pin from the IDR register.
 * @param  pGPIOx    : Base address of the GPIO port
 * @param  PinNumber : Pin index (0 to 15)
 * @return 0 or 1
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
   uint8_t value = (uint8_t) ((pGPIOx->IDR >> PinNumber) & 0x00000001);
   return value;
}

/**
 * @brief  Reads the complete 16-bit input port value from IDR.
 * @param  pGPIOx : Base address of the GPIO port
 * @return 16-bit port status
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){
  uint16_t value = (uint16_t) (pGPIOx->IDR);
  return value;
}

/**
 * @brief  Writes logic HIGH or LOW to a GPIO output pin via ODR.
 * @param  pGPIOx    : Base address of the GPIO port
 * @param  PinNumber : Pin index (0 to 15)
 * @param  Value     : GPIO_PIN_SET or GPIO_PIN_RESET
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value){
  if(Value == GPIO_PIN_SET){
    pGPIOx->ODR |= (1 << PinNumber);
  }
  else{
    pGPIOx->ODR &= ~(1 << PinNumber);
  }
}

/**
 * @brief  Writes a 16-bit value to the entire GPIO port via ODR.
 * @param  pGPIOx : Base address of the GPIO port
 * @param  Value  : 16-bit value
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value){
  pGPIOx->ODR = Value;
}

/**
 * @brief  Toggles the state of a GPIO output pin by XORing the corresponding bit in ODR.
 * @param  pGPIOx    : Base address of the GPIO port
 * @param  PinNumber : Pin index (0 to 15)
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
  pGPIOx->ODR ^= (1 << PinNumber);
}

/**
 * @brief  Enables or disables an interrupt in the ARM Cortex-M0+ NVIC ISER/ICER registers.
 * @param  IRQNumber : IRQ line number
 * @param  EnorDi    : ENABLE or DISABLE
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    *NVIC_ISER |= (1 << IRQNumber);
  }
  else{
    *NVIC_ICER |= (1 << IRQNumber);
  }
}

/**
 * @brief  Sets priority for an IRQ number in the ARM Cortex-M0+ NVIC IPR registers.
 *         On Cortex-M0+, priority bits are stored in the top 2 bits of each 8-bit priority field.
 * @param  IRQPriority : Priority value (0 to 3)
 * @param  IRQNumber   : IRQ line number
 */
void GPIO_PriorityConfig(uint8_t IRQPriority, uint8_t IRQNumber){
  uint8_t iprx = IRQNumber / 4;
  uint8_t iprx_section = IRQNumber % 4;
  uint8_t shift_amount = (8 * iprx_section) + 6; // On Cortex-M0+, only upper 2 bits [7:6] are implemented
  NVIC_PR_BASEADDR[iprx] |= (IRQPriority << shift_amount);
}

/**
 * @brief  Clears the pending EXTI interrupt flag in RPR (Rising Pending Register)
 *         and FPR (Falling Pending Register) by writing 1 (rc_w1).
 * @param  PinNumber : Pin index (0 to 15)
 */
void GPIO_IRQHandling(uint8_t PinNumber){
  if(EXTI->RPR & (1 << PinNumber)){
    EXTI->RPR |= (1 << PinNumber); // Clear by writing 1
  }
  if(EXTI->FPR & (1 << PinNumber)){
    EXTI->FPR |= (1 << PinNumber); // Clear by writing 1
  }
}
