#include "stm32c031c6.h"
#include "gpio.h"

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

void GPIO_Init(GPIO_Handle_t *pGPIOHandle){
  uint32_t temp = 0;
  if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->MODER &= ~(0x03 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
    pGPIOHandle->pGPIOx->MODER |= temp;
    temp = 0;
  }
  else{
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

    uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
    uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
    uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
    EXTI->EXTICR[temp1] &= ~(0xFF << (temp2 * 8));
    EXTI->EXTICR[temp1] = portcode << (temp2 * 8);


    EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
  }

  temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
  pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x03 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
  pGPIOHandle->pGPIOx->OSPEEDR |= temp;
  temp = 0;

  temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
  pGPIOHandle->pGPIOx->PUPDR &= ~(0x03 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
  pGPIOHandle->pGPIOx->PUPDR |= temp;
  temp = 0;

  temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
  pGPIOHandle->pGPIOx->OTYPER &= ~(0x01 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
  pGPIOHandle->pGPIOx->OTYPER |= temp;
  temp = 0;

  if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber <= 7){
      temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
      pGPIOHandle->pGPIOx->AFRL &= ~(0x0F << (4 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
      pGPIOHandle->pGPIOx->AFRL |= temp;
      temp = 0;
    }
    else{
      temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber - 8));
      pGPIOHandle->pGPIOx->AFRH &= ~(0x0F << (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber - 8)));
      pGPIOHandle->pGPIOx->AFRH |= temp;
      temp = 0;
    }
  
  }

}

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

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
   uint8_t value = (uint8_t) ((pGPIOx->IDR >> PinNumber) & 0x00000001);
   return value;
}
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){
  uint16_t value = (uint16_t) ((pGPIOx->IDR));
  return value;
}

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value){
  if(Value == GPIO_PIN_SET){
    pGPIOx->ODR |= (1<<PinNumber);
  }
  else{
    pGPIOx->ODR &= ~(1<<PinNumber);
  }
}
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value){
  pGPIOx->ODR = Value;
}
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber){
  pGPIOx->ODR ^= (1 << PinNumber);
}

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi){
  if(EnorDi == ENABLE){
    *NVIC_ISER |= (1<<IRQNumber);
  }
  else{
    *NVIC_ICER |= (1<<IRQNumber);
  }
}

void GPIO_PriorityConfig(uint8_t IRQPriority, uint8_t IRQNumber){
  uint8_t iprx = IRQNumber / 4;
  uint8_t iprx_section = IRQNumber % 4;
  uint8_t shift_amount = (8 * iprx_section) + 6;
  NVIC_PR_BASEADDR[iprx] |= (IRQPriority << shift_amount);
}
void GPIO_IRQHandling(uint8_t PinNumber){
  if(EXTI->RPR & (1<<PinNumber)){
    EXTI->RPR |= (1<<PinNumber);
  }
  if(EXTI->FPR & (1<<PinNumber)){
    EXTI->FPR |= (1<<PinNumber);
  }
}
