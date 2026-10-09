/**
 ******************************************************************************
 * @file    stm32c031c6.h
 * @brief   MCU device header file for STM32C031C6 (ARM Cortex-M0+).
 *          Contains base addresses of memories, buses, and peripherals,
 *          peripheral register structures, clock control macros, and
 *          reset macros.
 ******************************************************************************
 */

#ifndef INC_STM32C031C6_H_
#define INC_STM32C031C6_H_

#include <stdint.h>
#include <stddef.h>

/*
 * ============================================================================
 *                       ARM Cortex-M0+ NVIC Macros
 * ============================================================================
 */
#define NVIC_ISER                   ((volatile uint32_t*)0xE000E100)  /*!< Interrupt Set-Enable Register */
#define NVIC_ICER                   ((volatile uint32_t*)0xE000E180)  /*!< Interrupt Clear-Enable Register */
#define NVIC_PR_BASEADDR            ((volatile uint32_t*)0xE000E400)  /*!< Interrupt Priority Register Base Address */

/*
 * ============================================================================
 *                     Base Addresses of Memories
 * ============================================================================
 */
#define FLASH_BASEADDR              0x08000000U  /*!< Main Flash memory base address (up to 32 KB) */
#define SRAM_BASEADDR               0x20000000U  /*!< Embedded SRAM base address (12 KB) */
#define SRAM                        SRAM_BASEADDR
#define ROM_BASEADDR                0x1FFF0000U  /*!< System memory (bootloader ROM) */

/*
 * ============================================================================
 *                 Base Addresses of Peripheral Buses
 * ============================================================================
 */
#define PERIPH_BASE                 0x40000000U  /*!< Peripheral base address */
#define APB1_BASEADDR               PERIPH_BASE  /*!< APB1 peripheral bus base address */
#define APB2_BASEADDR               0x40010000U  /*!< APB2 peripheral bus base address */
#define AHB_BASEADDR                0x40020000U  /*!< AHB peripheral bus base address */
#define IOPORT_BASEADDR             0x50000000U  /*!< IO Port bus base address (single-cycle GPIO access) */

/*
 * ============================================================================
 *                     Base Addresses of GPIO Ports
 * ============================================================================
 */
#define GPIOA_BASEADDR              (IOPORT_BASEADDR + 0x0000)
#define GPIOB_BASEADDR              (IOPORT_BASEADDR + 0x0400)
#define GPIOC_BASEADDR              (IOPORT_BASEADDR + 0x0800)
#define GPIOD_BASEADDR              (IOPORT_BASEADDR + 0x0C00)
#define GPIOF_BASEADDR              (IOPORT_BASEADDR + 0x1400)

/*
 * ============================================================================
 *               Base Addresses of Peripherals on APB Buses
 * ============================================================================
 */
#define SYSCFG_BASEADDR             (APB1_BASEADDR + 0x0000)
#define I2C1_BASEADDR               (APB1_BASEADDR + 0x5400)
#define I2C2_BASEADDR               (APB1_BASEADDR + 0x5800)
#define USART2_BASEADDR             (APB1_BASEADDR + 0x4400)
#define USART3_BASEADDR             (APB1_BASEADDR + 0x4800)
#define USART4_BASEADDR             (APB1_BASEADDR + 0x4C00)
#define SPI2_BASEADDR               (APB1_BASEADDR + 0x3800)

#define SPI1_BASEADDR               (APB2_BASEADDR + 0x3000)
#define USART1_BASEADDR             (APB2_BASEADDR + 0x3800)

/*
 * ============================================================================
 *               Base Addresses of Peripherals on AHB Bus
 * ============================================================================
 */
#define RCC_BASEADDR                (AHB_BASEADDR + 0x1000)
#define EXTI_BASEADDR               (AHB_BASEADDR + 0x1800)

/*
 * ============================================================================
 *                   Peripheral Register Definition Structures
 * ============================================================================
 */

/**
 * @brief General Purpose I/O (GPIO) Register Structure
 */
typedef struct {
  volatile uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00 */
  volatile uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04 */
  volatile uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08 */
  volatile uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C */
  volatile uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10 */
  volatile uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14 */
  volatile uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18 */
  volatile uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C */
  volatile uint32_t AFRL;     /*!< GPIO alternate function low register,  Address offset: 0x20 */
  volatile uint32_t AFRH;     /*!< GPIO alternate function high register, Address offset: 0x24 */
  volatile uint32_t BRR;      /*!< GPIO port bit reset register,          Address offset: 0x28 */
} GPIO_RegDef_t;

/**
 * @brief Reset and Clock Control (RCC) Register Structure
 */
typedef struct {
  volatile uint32_t CR;         /*!< Clock control register,                    Address offset: 0x00 */
  volatile uint32_t ICSCR;      /*!< Internal clock sources calibration,        Address offset: 0x04 */
  volatile uint32_t CFGR;       /*!< Clock configuration register,              Address offset: 0x08 */
  uint32_t Reserved1;           /*!< Reserved,                                  Address offset: 0x0C */
  uint32_t Reserved2;           /*!< Reserved,                                  Address offset: 0x10 */
  volatile uint32_t CRRCR;      /*!< Clock recovery RC register,                Address offset: 0x14 */
  volatile uint32_t CIER;       /*!< Clock interrupt enable register,           Address offset: 0x18 */
  volatile uint32_t CIFR;       /*!< Clock interrupt flag register,             Address offset: 0x1C */
  volatile uint32_t CICR;       /*!< Clock interrupt clear register,            Address offset: 0x20 */
  volatile uint32_t IOPRSTR;    /*!< I/O port reset register,                   Address offset: 0x24 */
  volatile uint32_t AHBRSTR;    /*!< AHB peripheral reset register,             Address offset: 0x28 */
  volatile uint32_t APBRSTR1;   /*!< APB peripheral reset register 1,           Address offset: 0x2C */
  volatile uint32_t APBRSTR2;   /*!< APB peripheral reset register 2,           Address offset: 0x30 */
  volatile uint32_t IOPENR;     /*!< I/O port clock enable register,            Address offset: 0x34 */
  volatile uint32_t AHBENR;     /*!< AHB peripheral clock enable register,      Address offset: 0x38 */
  volatile uint32_t APBENR1;    /*!< APB peripheral clock enable register 1,    Address offset: 0x3C */
  volatile uint32_t APBENR2;    /*!< APB peripheral clock enable register 2,    Address offset: 0x40 */
  volatile uint32_t IOPSMENR;   /*!< I/O port in Sleep mode clock enable,       Address offset: 0x44 */
  volatile uint32_t AHBSMENR;   /*!< AHB in Sleep mode clock enable,            Address offset: 0x48 */
  volatile uint32_t APBSMENR1;  /*!< APB1 in Sleep mode clock enable,           Address offset: 0x4C */
  volatile uint32_t APBSMENR2;  /*!< APB2 in Sleep mode clock enable,           Address offset: 0x50 */
  volatile uint32_t CCIPR;      /*!< Peripherals independent clock config 1,    Address offset: 0x54 */
  volatile uint32_t CCIPR2;     /*!< Peripherals independent clock config 2,    Address offset: 0x58 */
  volatile uint32_t CSR1;       /*!< Control/status register 1,                 Address offset: 0x5C */
  volatile uint32_t CSR2;       /*!< Control/status register 2,                 Address offset: 0x60 */
} RCC_RegDef_t;

/**
 * @brief Extended Interrupt and Event Controller (EXTI) Register Structure
 */
typedef struct {
  volatile uint32_t RTSR;             /*!< Rising trigger selection register,   Address offset: 0x00 */
  volatile uint32_t FTSR;             /*!< Falling trigger selection register,  Address offset: 0x04 */
  volatile uint32_t SWIER;            /*!< Software interrupt event register,   Address offset: 0x08 */
  volatile uint32_t RPR;              /*!< Rising edge pending register,        Address offset: 0x0C */
  volatile uint32_t FPR;              /*!< Falling edge pending register,       Address offset: 0x10 */
  volatile uint32_t Reserved1[19];    /*!< Reserved,                            Address offset: 0x14-0x5C */
  volatile uint32_t EXTICR[4];        /*!< External interrupt selection,        Address offset: 0x60-0x6C */
  volatile uint32_t Reserved2[4];     /*!< Reserved,                            Address offset: 0x70-0x7C */
  volatile uint32_t IMR;              /*!< CPU wakeup with interrupt mask,      Address offset: 0x80 */
  volatile uint32_t EMR;              /*!< CPU wakeup with event mask,          Address offset: 0x84 */
} EXTI_RegDef_t;

/**
 * @brief System Configuration (SYSCFG) Register Structure
 */
typedef struct {
  volatile uint32_t CFGR1;            /*!< SYSCFG configuration register 1,     Address offset: 0x00 */
  volatile uint32_t Reserved1[5];     /*!< Reserved,                            Address offset: 0x04-0x14 */
  volatile uint32_t CFGR2;            /*!< SYSCFG configuration register 2,     Address offset: 0x18 */
  volatile uint32_t Reserved2[8];     /*!< Reserved,                            Address offset: 0x1C-0x3B */
  volatile uint32_t CFGR3;            /*!< SYSCFG configuration register 3,     Address offset: 0x3C */
} SYSCFG_RegDef_t;

/**
 * @brief Serial Peripheral Interface (SPI) Register Structure
 */
typedef struct {
  volatile uint32_t CR1;       /*!< SPI control register 1,             Address offset: 0x00 */
  volatile uint32_t CR2;       /*!< SPI control register 2,             Address offset: 0x04 */
  volatile uint32_t SR;        /*!< SPI status register,                Address offset: 0x08 */
  volatile uint32_t DR;        /*!< SPI data register,                  Address offset: 0x0C */
  volatile uint32_t CRCPR;     /*!< SPI CRC polynomial register,        Address offset: 0x10 */
  volatile uint32_t RXCRCR;    /*!< SPI RX CRC register,                Address offset: 0x14 */
  volatile uint32_t TXCRCR;    /*!< SPI TX CRC register,                Address offset: 0x18 */
  volatile uint32_t I2SCFGR;   /*!< SPI_I2S configuration register,     Address offset: 0x1C */
  volatile uint32_t I2SPR;     /*!< SPI_I2S prescaler register,         Address offset: 0x20 */
} SPI_RegDef_t;

/**
 * @brief Inter-Integrated Circuit (I2C) Register Structure
 */
typedef struct {
  volatile uint32_t CR1;       /*!< I2C control register 1,             Address offset: 0x00 */
  volatile uint32_t CR2;       /*!< I2C control register 2,             Address offset: 0x04 */
  volatile uint32_t OAR1;      /*!< I2C own address 1 register,         Address offset: 0x08 */
  volatile uint32_t OAR2;      /*!< I2C own address 2 register,         Address offset: 0x0C */
  volatile uint32_t TIMINGR;   /*!< I2C timing register,                Address offset: 0x10 */
  volatile uint32_t TIMEOUTR;  /*!< I2C timeout register,               Address offset: 0x14 */
  volatile uint32_t ISR;       /*!< I2C interrupt and status register,  Address offset: 0x18 */
  volatile uint32_t ICR;       /*!< I2C interrupt clear register,       Address offset: 0x1C */
  volatile uint32_t PECR;      /*!< I2C PEC register,                   Address offset: 0x20 */
  volatile uint32_t RXDR;      /*!< I2C receive data register,          Address offset: 0x24 */
  volatile uint32_t TXDR;      /*!< I2C transmit data register,         Address offset: 0x28 */
} I2C_RegDef_t;

/**
 * @brief Universal Synchronous/Asynchronous Receiver Transmitter (USART) Register Structure
 */
typedef struct {
  volatile uint32_t CR1;       /*!< USART control register 1,           Address offset: 0x00 */
  volatile uint32_t CR2;       /*!< USART control register 2,           Address offset: 0x04 */
  volatile uint32_t CR3;       /*!< USART control register 3,           Address offset: 0x08 */
  volatile uint32_t BRR;       /*!< USART baud rate register,           Address offset: 0x0C */
  volatile uint32_t GTPR;      /*!< USART guard time and prescaler,     Address offset: 0x10 */
  volatile uint32_t RTOR;      /*!< USART receiver timeout register,    Address offset: 0x14 */
  volatile uint32_t RQR;       /*!< USART request register,             Address offset: 0x18 */
  volatile uint32_t ISR;       /*!< USART interrupt and status register,Address offset: 0x1C */
  volatile uint32_t ICR;       /*!< USART interrupt flag clear register,Address offset: 0x20 */
  volatile uint32_t RDR;       /*!< USART receive data register,        Address offset: 0x24 */
  volatile uint32_t TDR;       /*!< USART transmit data register,       Address offset: 0x28 */
  volatile uint32_t PRESC;     /*!< USART prescaler register,           Address offset: 0x2C */
} USART_RegDef_t;

/*
 * ============================================================================
 *              Peripheral Definitions (Typecasted Base Addresses)
 * ============================================================================
 */
#define GPIOA                       ((GPIO_RegDef_t*) GPIOA_BASEADDR)
#define GPIOB                       ((GPIO_RegDef_t*) GPIOB_BASEADDR)
#define GPIOC                       ((GPIO_RegDef_t*) GPIOC_BASEADDR)
#define GPIOD                       ((GPIO_RegDef_t*) GPIOD_BASEADDR)
#define GPIOF                       ((GPIO_RegDef_t*) GPIOF_BASEADDR)

#define RCC                         ((RCC_RegDef_t*) RCC_BASEADDR)
#define EXTI                        ((EXTI_RegDef_t*) EXTI_BASEADDR)
#define SYSCFG                      ((SYSCFG_RegDef_t*) SYSCFG_BASEADDR)

#define SPI1                        ((SPI_RegDef_t*) SPI1_BASEADDR)
#define SPI2                        ((SPI_RegDef_t*) SPI2_BASEADDR)

#define I2C1                        ((I2C_RegDef_t*) I2C1_BASEADDR)
#define I2C2                        ((I2C_RegDef_t*) I2C2_BASEADDR)

#define USART1                      ((USART_RegDef_t*) USART1_BASEADDR)
#define USART2                      ((USART_RegDef_t*) USART2_BASEADDR)
#define USART3                      ((USART_RegDef_t*) USART3_BASEADDR)
#define USART4                      ((USART_RegDef_t*) USART4_BASEADDR)

/*
 * ============================================================================
 *                    Clock Enable Macros for Peripherals
 * ============================================================================
 */

// GPIO Ports
#define GPIOA_PCLK_EN()             (RCC->IOPENR |= (1 << 0))
#define GPIOB_PCLK_EN()             (RCC->IOPENR |= (1 << 1))
#define GPIOC_PCLK_EN()             (RCC->IOPENR |= (1 << 2))
#define GPIOD_PCLK_EN()             (RCC->IOPENR |= (1 << 3))
#define GPIOF_PCLK_EN()             (RCC->IOPENR |= (1 << 5))

// I2C
#define I2C1_PCLK_EN()              (RCC->APBENR1 |= (1 << 21))
#define I2C2_PCLK_EN()              (RCC->APBENR1 |= (1 << 22))

// SPI
#define SPI1_PCLK_EN()              (RCC->APBENR2 |= (1 << 12))
#define SPI2_PCLK_EN()              (RCC->APBENR1 |= (1 << 14))

// USART
#define USART1_PCLK_EN()            (RCC->APBENR2 |= (1 << 14))
#define USART2_PCLK_EN()            (RCC->APBENR1 |= (1 << 17))
#define USART3_PCLK_EN()            (RCC->APBENR1 |= (1 << 18))
#define USART4_PCLK_EN()            (RCC->APBENR1 |= (1 << 19))

// SYSCFG
#define SYSCFG_PCLK_EN()            (RCC->APBENR2 |= (1 << 0))

/*
 * ============================================================================
 *                    Clock Disable Macros for Peripherals
 * ============================================================================
 */

// GPIO Ports
#define GPIOA_PCLK_DI()             (RCC->IOPENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()             (RCC->IOPENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()             (RCC->IOPENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()             (RCC->IOPENR &= ~(1 << 3))
#define GPIOF_PCLK_DI()             (RCC->IOPENR &= ~(1 << 5))

// I2C
#define I2C1_PCLK_DI()              (RCC->APBENR1 &= ~(1 << 21))
#define I2C2_PCLK_DI()              (RCC->APBENR1 &= ~(1 << 22))

// SPI
#define SPI1_PCLK_DI()              (RCC->APBENR2 &= ~(1 << 12))
#define SPI2_PCLK_DI()              (RCC->APBENR1 &= ~(1 << 14))

// USART
#define USART1_PCLK_DI()            (RCC->APBENR2 &= ~(1 << 14))
#define USART2_PCLK_DI()            (RCC->APBENR1 &= ~(1 << 17))
#define USART3_PCLK_DI()            (RCC->APBENR1 &= ~(1 << 18))
#define USART4_PCLK_DI()            (RCC->APBENR1 &= ~(1 << 19))

// SYSCFG
#define SYSCFG_PCLK_DI()            (RCC->APBENR2 &= ~(1 << 0))

/*
 * ============================================================================
 *                    Peripheral Reset Macros
 * ============================================================================
 */

// GPIO Ports Reset
#define GPIOA_REG_RESET()           do {(RCC->IOPRSTR |= (1 << 0)); (RCC->IOPRSTR &= ~(1 << 0));} while(0)
#define GPIOB_REG_RESET()           do {(RCC->IOPRSTR |= (1 << 1)); (RCC->IOPRSTR &= ~(1 << 1));} while(0)
#define GPIOC_REG_RESET()           do {(RCC->IOPRSTR |= (1 << 2)); (RCC->IOPRSTR &= ~(1 << 2));} while(0)
#define GPIOD_REG_RESET()           do {(RCC->IOPRSTR |= (1 << 3)); (RCC->IOPRSTR &= ~(1 << 3));} while(0)
#define GPIOF_REG_RESET()           do {(RCC->IOPRSTR |= (1 << 5)); (RCC->IOPRSTR &= ~(1 << 5));} while(0)

// I2C Reset
#define I2C1_REG_RESET()            do {(RCC->APBRSTR1 |= (1 << 21)); (RCC->APBRSTR1 &= ~(1 << 21));} while(0)
#define I2C2_REG_RESET()            do {(RCC->APBRSTR1 |= (1 << 22)); (RCC->APBRSTR1 &= ~(1 << 22));} while(0)

// SPI Reset
#define SPI1_REG_RESET()            do {(RCC->APBRSTR2 |= (1 << 12)); (RCC->APBRSTR2 &= ~(1 << 12));} while(0)
#define SPI2_REG_RESET()            do {(RCC->APBRSTR1 |= (1 << 14)); (RCC->APBRSTR1 &= ~(1 << 14));} while(0)

// USART Reset
#define USART1_REG_RESET()          do {(RCC->APBRSTR2 |= (1 << 14)); (RCC->APBRSTR2 &= ~(1 << 14));} while(0)
#define USART2_REG_RESET()          do {(RCC->APBRSTR1 |= (1 << 17)); (RCC->APBRSTR1 &= ~(1 << 17));} while(0)
#define USART3_REG_RESET()          do {(RCC->APBRSTR1 |= (1 << 18)); (RCC->APBRSTR1 &= ~(1 << 18));} while(0)
#define USART4_REG_RESET()          do {(RCC->APBRSTR1 |= (1 << 19)); (RCC->APBRSTR1 &= ~(1 << 19));} while(0)

/*
 * ============================================================================
 *                    Helper Macros & Interrupt Numbers
 * ============================================================================
 */
#define GPIO_BASEADDR_TO_CODE(x)    ((x == GPIOA) ? 0 : \
                                     (x == GPIOB) ? 1 : \
                                     (x == GPIOC) ? 2 : \
                                     (x == GPIOD) ? 3 : \
                                     (x == GPIOF) ? 5 : 0)

// NVIC IRQ Numbers on STM32C031
#define IRQ_NO_EXTI0_1              5
#define IRQ_NO_EXTI2_3              6
#define IRQ_NO_EXTI4_15             7
#define IRQ_NO_I2C1                 23
#define IRQ_NO_I2C2                 24
#define IRQ_NO_SPI1                 25
#define IRQ_NO_SPI2                 26
#define IRQ_NO_USART1               27
#define IRQ_NO_USART2               28
#define IRQ_NO_USART3_4             29

/*
 * ============================================================================
 *                             Generic Macros
 * ============================================================================
 */
#define ENABLE                      1
#define DISABLE                     0
#define SET                         ENABLE
#define RESET                       DISABLE
#define GPIO_PIN_SET                ENABLE
#define GPIO_PIN_RESET              DISABLE
#define FLAG_RESET                  RESET
#define FLAG_SET                    SET

/*
 * ============================================================================
 *                        Peripheral Driver Includes
 * ============================================================================
 */
#include "gpio.h"
#include "spi.h"
#include "i2c.h"
#include "usart.h"

#endif /* INC_STM32C031C6_H_ */
