/**
 ******************************************************************************
 * @file    usart.h
 * @brief   USART peripheral driver header file for STM32C031C6.
 *          Contains configuration structures, register bit definitions,
 *          baud rate macros, status flags, and driver API prototypes.
 ******************************************************************************
 */

#ifndef USART_H_
#define USART_H_

#include "stm32c031c6.h"

/**
 * @brief Configuration structure for USART peripheral
 */
typedef struct {
  uint8_t  USART_Mode;            /*!< Communication mode (TX only, RX only, TX and RX). @ref USART_MODES */
  uint32_t USART_BaudRate;        /*!< Baud rate value (e.g. 9600, 115200). @ref USART_BAUD_RATES */
  uint8_t  USART_StopBits;        /*!< Number of stop bits (1, 0.5, 2, 1.5). @ref USART_STOP_BITS */
  uint8_t  USART_WordLength;      /*!< Word length: 8, 9, or 7 data bits. @ref USART_WORD_LENGTHS */
  uint8_t  USART_ParityControl;   /*!< Parity control enable/disable. @ref USART_PARITY_CONTROL */
  uint8_t  USART_ParitySelection; /*!< Parity selection (Even or Odd). @ref USART_PARITY_SELECTION */
  uint8_t  USART_Oversampling;    /*!< Oversampling mode (16x or 8x). @ref USART_OVERSAMPLING */
  uint8_t  USART_FIFO;            /*!< FIFO mode enable/disable. @ref USART_FIFO_MODES */
} USART_Config_t;

/**
 * @brief Handle structure for USART peripheral
 */
typedef struct {
  USART_RegDef_t *pUSARTx;        /*!< Base address of USART peripheral (USART1 - USART4) */
  USART_Config_t USARTConfig;     /*!< USART configuration settings */
} USART_Handle_t;


/*
 * USART Mode macros
 */
#define USART_MODE_ONLY_TX    0
#define USART_MODE_ONLY_RX    1
#define USART_MODE_TXRX       2

/*
 * Standard Baud Rate macros
 */
#define USART_STD_BAUD_1200       1200
#define USART_STD_BAUD_2400       2400
#define USART_STD_BAUD_9600       9600
#define USART_STD_BAUD_19200      19200
#define USART_STD_BAUD_38400      38400
#define USART_STD_BAUD_57600      57600
#define USART_STD_BAUD_115200     115200
#define USART_STD_BAUD_230400     230400
#define USART_STD_BAUD_460800     460800
#define USART_STD_BAUD_921600     921600
#define USART_STD_BAUD_2M         2000000

/*
 * Word Length macros (CR1 M0 bit 12, M1 bit 28)
 */
#define USART_WORDLEN_8BITS   0
#define USART_WORDLEN_9BITS   1
#define USART_WORDLEN_7BITS   2

/*
 * Stop Bits macros (CR2 STOP bits 13:12)
 */
#define USART_STOPBITS_1      0
#define USART_STOPBITS_0_5    1
#define USART_STOPBITS_2      2
#define USART_STOPBITS_1_5    3

/*
 * Parity Control macros (CR1 PCE bit 10)
 */
#define USART_PARITY_DISABLE  0
#define USART_PARITY_ENABLE   1

/*
 * Parity Selection macros (CR1 PS bit 9)
 */
#define USART_PARITY_EVEN     0
#define USART_PARITY_ODD      1

/*
 * Oversampling macros (CR1 OVER8 bit 15)
 */
#define USART_OVERSAMPLING_16 0
#define USART_OVERSAMPLING_8  1

/*
 * FIFO Enable macros (CR1 FIFOEN bit 29)
 */
#define USART_FIFO_DISABLE    0
#define USART_FIFO_ENABLE     1

/*
 * USART Status Flags (ISR register)
 */
#define USART_FLAG_PE         (1 << 0)
#define USART_FLAG_FE         (1 << 1)
#define USART_FLAG_NF         (1 << 2)
#define USART_FLAG_ORE        (1 << 3)
#define USART_FLAG_IDLE       (1 << 4)
#define USART_FLAG_RXNE       (1 << 5)
#define USART_FLAG_TC         (1 << 6)
#define USART_FLAG_TXE        (1 << 7)
#define USART_FLAG_BUSY       (1 << 16)

/*
 * Peripheral Clock setup
 */
void USART_PeriClkCtrl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);
void USART_PeriCtrl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_RegDef_t *pUSARTx);

/*
 * Data Send and Receive
 */
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len);
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/*
 * Flag status and Clear
 */
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint32_t FlagName);

/*
 * Baud rate configuration helper
 */
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate, uint8_t Oversampling);

#endif /*USART_H_*/
