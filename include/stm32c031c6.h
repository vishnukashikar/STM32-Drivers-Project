#ifndef INC_STM32C031C6_H_
#define INC_STM32C031C6_H_
#include <stdint.h>
#include <stddef.h>




// ARM specific macros
#define NVIC_ISER                   ((volatile uint32_t*)0xE000E100)
#define NVIC_ICER                   ((volatile uint32_t*)0xE000E180)
#define NVIC_PR_BASEADDR            ((volatile uint32_t*)0xE000E400)



// Base addresses of flash and SRAM memory

#define FLASH_BASEADDR              0x08000000U
#define SRAM_BASEADDR               0x20000000U
#define SRAM                        SRAM_BASEADDR
#define ROM_BASEADDR                0x1FFF0000U

// Base addresses of peripheral buses
#define PERIPH_BASE                 0x40000000U
#define APB1_BASEADDR               PERIPH_BASE
#define APB2_BASEADDR               0x40010000U //Breaking down APB bus into 2 parts to make it easier to code.
#define AHB_BASEADDR                0x40020000U
#define IOPORT_BASEADDR             0x50000000U

// Base addresses of IO PORTS
#define GPIOA_BASEADDR              (IOPORT_BASEADDR + 0x0000) // Base address + offset
#define GPIOB_BASEADDR              (IOPORT_BASEADDR + 0x0400)
#define GPIOC_BASEADDR              (IOPORT_BASEADDR + 0x0800)
#define GPIOD_BASEADDR              (IOPORT_BASEADDR + 0x0C00)
#define GPIOF_BASEADDR              (IOPORT_BASEADDR + 0x1400)

// Base addresses of peripherals on APB bus
#define I2C1_BASEADDR               (APB1_BASEADDR + 0x5400)
#define I2C2_BASEADDR               (APB1_BASEADDR + 0x5800)
#define USART1_BASEADDR             (APB2_BASEADDR + 0x3800)
#define USART2_BASEADDR             (APB1_BASEADDR + 0x4400)
#define USART3_BASEADDR             (APB1_BASEADDR + 0x4800)
#define USART4_BASEADDR             (APB1_BASEADDR + 0x4C00)
#define SPI1_BASEADDR               (APB2_BASEADDR + 0x3000)
#define SPI2_BASEADDR               (APB1_BASEADDR + 0x3800)
#define SYSCFG_BASEADDR             (APB1_BASEADDR + 0x0000)

//Base addresses for peripherals on AHB bus
#define EXTI_BASEADDR               (AHB_BASEADDR + 0x1800)
#define RCC_BASEADDR                (AHB_BASEADDR + 0x1000)


// Peripheral Register definition structures
typedef struct{
  volatile uint32_t MODER;
  volatile uint32_t OTYPER;
  volatile uint32_t OSPEEDR;
  volatile uint32_t PUPDR;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t LCKR;
  volatile uint32_t AFRL;
  volatile uint32_t AFRH;
  volatile uint32_t BRR;
}GPIO_RegDef_t;

// RCC Register structure
typedef struct{
  volatile uint32_t CR;
  volatile uint32_t ICSCR;
  volatile uint32_t CFGR;
  uint32_t Reserved1;
  uint32_t Reserved2;
  volatile uint32_t CRRCR;
  volatile uint32_t CIER;
  volatile uint32_t CIFR;
  volatile uint32_t CICR;
  volatile uint32_t IOPRSTR;
  volatile uint32_t AHBRSTR;
  volatile uint32_t APBRSTR1;
  volatile uint32_t APBRSTR2;
  volatile uint32_t IOPENR;
  volatile uint32_t AHBENR;
  volatile uint32_t APBENR1;
  volatile uint32_t APBENR2;
  volatile uint32_t IOPSMENR;
  volatile uint32_t AHBSMENR;
  volatile uint32_t APBSMENR1;
  volatile uint32_t APBSMENR2;
  volatile uint32_t CCIPR;
  volatile uint32_t CCIPR2;
  volatile uint32_t CSR1;
  volatile uint32_t CSR2;
}RCC_RegDef_t;

// EXTI Register structure
typedef struct{
  volatile uint32_t RTSR;
  volatile uint32_t FTSR;
  volatile uint32_t SWIER;
  volatile uint32_t RPR;
  volatile uint32_t FPR;
  volatile uint32_t Reserved1[19];
  volatile uint32_t EXTICR[4];
  volatile uint32_t Reserved2[4];
  volatile uint32_t IMR;
  volatile uint32_t EMR;
}EXTI_RegDef_t;

// SYSCFG Register Structure
typedef struct{
    volatile uint32_t CFGR1;          // 0x00
    volatile uint32_t Reserved1[5];   // 0x04 - 0x14
    volatile uint32_t CFGR2;          // 0x18
    volatile uint32_t Reserved2[8];   // 0x1C - 0x3B
    volatile uint32_t CFGR3;          // 0x3C
} SYSCFG_RegDef_t;

// SPI Register structure
typedef struct{
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t SR;
  volatile uint32_t DR;
  volatile uint32_t CRCPR;
  volatile uint32_t RXCRCR;
  volatile uint32_t TXCRCR;
  volatile uint32_t I2SCFGR;
  volatile uint32_t I2SPR;
} SPI_RegDef_t;

typedef struct{
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t OAR1;
  volatile uint32_t OAR2;
  volatile uint32_t TIMINGR;
  volatile uint32_t TIMEOUTR;
  volatile uint32_t ISR;
  volatile uint32_t ICR;
  volatile uint32_t PECR;
  volatile uint32_t RXDR;
  volatile uint32_t TXDR;
} I2C_RegDef_t;



// Typecasted base addressses for GPIO
#define GPIOA                       ((GPIO_RegDef_t*) GPIOA_BASEADDR)             
#define GPIOB                       ((GPIO_RegDef_t*) GPIOB_BASEADDR) 
#define GPIOC                       ((GPIO_RegDef_t*) GPIOC_BASEADDR) 
#define GPIOD                       ((GPIO_RegDef_t*) GPIOD_BASEADDR) 
#define GPIOF                       ((GPIO_RegDef_t*) GPIOF_BASEADDR) 

//Typecasted base address for RCC
#define RCC                         ((RCC_RegDef_t*) RCC_BASEADDR) 

// Typecasted base address for EXTI
#define EXTI                        ((EXTI_RegDef_t*) EXTI_BASEADDR)

// Typecasted base address for SYSCFG
#define SYSCFG                      ((SYSCFG_RegDef_t*) SYSCFG_BASEADDR)

// Typecasted base address for SPI
#define SPI1                        ((SPI_RegDef_t*) SPI1_BASEADDR)
#define SPI2                        ((SPI_RegDef_t*) SPI2_BASEADDR)

//Typecasted baseaddress for I2C
#define I2C1                        ((I2C_RegDef_t*) I2C1_BASEADDR)
#define I2C2                        ((I2C_RegDef_t*) I2C2_BASEADDR)


// Clock enable macros for GPIO
#define GPIOA_PCLK_EN()             (RCC->IOPENR |= (1<<0))
#define GPIOB_PCLK_EN()             (RCC->IOPENR |= (1<<1))
#define GPIOC_PCLK_EN()             (RCC->IOPENR |= (1<<2))
#define GPIOD_PCLK_EN()             (RCC->IOPENR |= (1<<3))
#define GPIOF_PCLK_EN()             (RCC->IOPENR |= (1<<5))

// Clock enable macros for I2C
#define I2C1_PCLK_EN()              (RCC->APBENR1 |= (1<<21))
#define I2C2_PCLK_EN()              (RCC->APBENR1 |= (1<<22))

// Clock enable macros for SPI
#define SPI1_PCLK_EN()              (RCC->APBENR2 |= (1<<12)) 
#define SPI2_PCLK_EN()              (RCC->APBENR1 |= (1<<14)) 

// Clock enable macros for USART
#define USART1_PCLK_EN()            (RCC->APBENR2 |= (1<<14))
#define USART2_PCLK_EN()            (RCC->APBENR1 |= (1<<17)) 
#define USART3_PCLK_EN()            (RCC->APBENR1 |= (1<<18)) 
#define USART4_PCLK_EN()            (RCC->APBENR1 |= (1<<19)) 

// Clock enable macros for SYSCFG
#define SYSCFG_PCLK_EN()            (RCC->APBENR2 |= (1<<0)) 


#define GPIO_BASEADDR_TO_CODE(x)    ((x == GPIOA)?0:\
                                     (x == GPIOB)?1:\
                                     (x == GPIOC)?2:\
                                     (x == GPIOD)?3:\
                                     (x == GPIOF)?5:0)


#define IRQ_NO_EXTI0_1    5
#define IRQ_NO_EXTI2_3    6
#define IRQ_NO_EXTI4_15   7
#define IRQ_NO_SPI1      25
#define IRQ_NO_SPI2      26
#define IRQ_NO_I2C1      23
#define IRQ_NO_I2C2      24


// Clock diable macros for GPIO
#define GPIOA_PCLK_DI()             (RCC->IOPENR &= ~(1<<0))
#define GPIOB_PCLK_DI()             (RCC->IOPENR &= ~(1<<1))
#define GPIOC_PCLK_DI()             (RCC->IOPENR &= ~(1<<2))
#define GPIOD_PCLK_DI()             (RCC->IOPENR &= ~(1<<3))
#define GPIOF_PCLK_DI()             (RCC->IOPENR &= ~(1<<5))


// GPIO reset macros
#define GPIOA_REG_RESET()           do {(RCC->IOPRSTR |= (1<<0)); (RCC->IOPRSTR &= ~(1<<0));}while(0)
#define GPIOB_REG_RESET()           do {(RCC->IOPRSTR |= (1<<1)); (RCC->IOPRSTR &= ~(1<<1));}while(0)
#define GPIOC_REG_RESET()           do {(RCC->IOPRSTR |= (1<<2)); (RCC->IOPRSTR &= ~(1<<2));}while(0)
#define GPIOD_REG_RESET()           do {(RCC->IOPRSTR |= (1<<3)); (RCC->IOPRSTR &= ~(1<<3));}while(0)
#define GPIOF_REG_RESET()           do {(RCC->IOPRSTR |= (1<<5)); (RCC->IOPRSTR &= ~(1<<5));}while(0)

// Clock disable macros for I2C
#define I2C1_PCLK_DI()              (RCC->APBENR1 &= ~(1<<21))
#define I2C2_PCLK_DI()              (RCC->APBENR1 &= ~(1<<22))

// I2C reset macros
#define I2C1_REG_RESET()           do {(RCC->APBRSTR1 |= (1<<21)); (RCC->APBRSTR1 &= ~(1<<21));}while(0)
#define I2C2_REG_RESET()           do {(RCC->APBRSTR1 |= (1<<22)); (RCC->APBRSTR1 &= ~(1<<22));}while(0)

// Clock disable macros for SPI
#define SPI1_PCLK_DI()              (RCC->APBENR2 &= ~(1<<12)) 
#define SPI2_PCLK_DI()              (RCC->APBENR1 &= ~(1<<14)) 

// SPI reset macros
#define SPI1_REG_RESET()           do {(RCC->APBRSTR2 |= (1<<12)); (RCC->APBRSTR2 &= ~(1<<12));}while(0)
#define SPI2_REG_RESET()           do {(RCC->APBRSTR1 |= (1<<14)); (RCC->APBRSTR1 &= ~(1<<14));}while(0)

// Clock disable macros for USART
#define USART1_PCLK_DI()            (RCC->APBENR2 &= ~(1<<14))
#define USART2_PCLK_DI()            (RCC->APBENR1 &= ~(1<<17)) 
#define USART3_PCLK_DI()            (RCC->APBENR1 &= ~(1<<18)) 
#define USART4_PCLK_DI()            (RCC->APBENR1 &= ~(1<<19)) 

// Clock disable macros for SYSCFG
#define SYSCFG_PCLK_DI()            (RCC->APBENR2 |= ~(1<<0)) 



//generic macros
#define ENABLE 1
#define DISABLE 0
#define SET ENABLE
#define RESET DISABLE
#define GPIO_PIN_SET ENABLE
#define GPIO_PIN_RESET DISABLE
#define FLAG_RESET RESET 
#define FLAG_SET SET

#include "gpio.h"
#include "spi.h"
#include "i2c.h"


#endif /*INC_STM32C031C6_H_*/
