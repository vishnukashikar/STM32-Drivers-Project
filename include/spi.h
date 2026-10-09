#ifndef SPI_H_
#define SPI_H_

#include "stm32c031c6.h"

/**
 * @brief Configuration structure for SPI peripheral
 */
typedef struct {
  uint8_t SPI_DeviceMode;  /*!< Master or Slave mode. @ref SPI_DEVICE_MODES */
  uint8_t SPI_BusConfig;   /*!< Full-duplex, Half-duplex, or Simplex. @ref SPI_BUS_CONFIGS */
  uint8_t SPI_SClkSpeed;   /*!< Serial clock baud rate prescaler. @ref SPI_SCLK_SPEEDS */
  uint8_t SPI_DataSize;    /*!< Data frame format size (8-bit or 16-bit). @ref SPI_DATA_SIZES */
  uint8_t SPI_CRCL;        /*!< CRC length (8-bit or 16-bit). */
  uint8_t SPI_CPOL;        /*!< Clock polarity (Idle Low or Idle High). @ref SPI_CPOL_MODES */
  uint8_t SPI_CPHA;        /*!< Clock phase (1st edge or 2nd edge sampling). @ref SPI_CPHA_MODES */
  uint8_t SPI_SSM;         /*!< Software Slave Management (Hardware or Software NSS). @ref SPI_SSM_MODES */
} SPI_Config_t;

/**
 * @brief Handle structure for SPI peripheral
 */
typedef struct {
  SPI_RegDef_t *pSPIx;     /*!< Base address of SPI peripheral (SPI1, SPI2) */
  SPI_Config_t SPIConfig;  /*!< SPI configuration settings */
  uint8_t *pTxBuffer;      /*!< Pointer to transmit buffer (for interrupt-driven TX) */
  uint8_t *pRxBuffer;      /*!< Pointer to receive buffer (for interrupt-driven RX) */
  uint32_t TxLen;          /*!< Length of transmit data remaining */
  uint32_t RxLen;          /*!< Length of receive data remaining */
  uint8_t TxState;         /*!< Transmission state: @ref SPI_STATES */
  uint8_t RxState;         /*!< Reception state: @ref SPI_STATES */
} SPI_Handle_t;

/*
 * ============================================================================
 *                          SPI Device Modes
 * ============================================================================
 */
#define SPI_DEVICE_MODE_MASTER        1  /*!< Master configuration (CR1 MSTR = 1) */
#define SPI_DEVICE_MODE_SLAVE         0  /*!< Slave configuration (CR1 MSTR = 0) */

/*
 * ============================================================================
 *                          SPI Bus Configurations
 * ============================================================================
 */
#define SPI_BUS_CONFIG_FD             1  /*!< 2-line Full-Duplex (BIDIMODE = 0, RXONLY = 0) */
#define SPI_BUS_CONFIG_HD             2  /*!< 1-line Half-Duplex bidirectional (BIDIMODE = 1) */
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY 3  /*!< 2-line Simplex Receive-Only (BIDIMODE = 0, RXONLY = 1) */

/*
 * ============================================================================
 *                    SPI Clock Baud Rate Prescalers (CR1 BR[2:0])
 * ============================================================================
 */
#define SPI_SCLK_SPEED_DIV2           0  /*!< f_PCLK / 2 */
#define SPI_SCLK_SPEED_DIV4           1  /*!< f_PCLK / 4 */
#define SPI_SCLK_SPEED_DIV8           2  /*!< f_PCLK / 8 */
#define SPI_SCLK_SPEED_DIV16          3  /*!< f_PCLK / 16 */
#define SPI_SCLK_SPEED_DIV32          4  /*!< f_PCLK / 32 */
#define SPI_SCLK_SPEED_DIV64          5  /*!< f_PCLK / 64 */
#define SPI_SCLK_SPEED_DIV128         6  /*!< f_PCLK / 128 */
#define SPI_SCLK_SPEED_DIV256         7  /*!< f_PCLK / 256 */

/*
 * ============================================================================
 *                          SPI CRC Length
 * ============================================================================
 */
#define SPI_CRCL_8BITS                0  /*!< 8-bit CRC */
#define SPI_CRCL_16BITS               1  /*!< 16-bit CRC */

/*
 * ============================================================================
 *                      SPI Clock Phase & Polarity
 * ============================================================================
 */
#define SPI_CPHA_LOW                  0  /*!< 1st clock transition is first data capture edge */
#define SPI_CPHA_HIGH                 1  /*!< 2nd clock transition is first data capture edge */

#define SPI_CPOL_LOW                  0  /*!< SCK to 0 when idle */
#define SPI_CPOL_HIGH                 1  /*!< SCK to 1 when idle */

/*
 * ============================================================================
 *                  Software Slave Management (SSM)
 * ============================================================================
 */
#define SPI_SSM_DI                    0  /*!< Hardware NSS management (SSM = 0) */
#define SPI_SSM_EN                    1  /*!< Software NSS management (SSM = 1, controlled via SSI bit) */

/*
 * ============================================================================
 *                          SPI Status Flags (SR register)
 * ============================================================================
 */
#define SPI_RXNE_FLAG                 (1 << 0)  /*!< Receive buffer not empty */
#define SPI_TXE_FLAG                  (1 << 1)  /*!< Transmit buffer empty */
#define SPI_BSY_FLAG                  (1 << 7)  /*!< Busy flag */

/*
 * ============================================================================
 *                          SPI State Machine States
 * ============================================================================
 */
#define SPI_READY                     0
#define SPI_BUSY_IN_RX                1
#define SPI_BUSY_IN_TX                2

/*
 * ============================================================================
 *                          SPI Application Events
 * ============================================================================
 */
#define SPI_EVENT_TX_CMPLT            1  /*!< Transmission complete event */
#define SPI_EVENT_RX_CMPLT            2  /*!< Reception complete event */
#define SPI_EVENT_OVR_ERR             3  /*!< Overrun error event */

/*
 * ============================================================================
 *                    Data Frame Size (CR2 DS[3:0] on STM32)
 * ============================================================================
 */
#define SPI_DATASIZE_8BIT             7   /*!< 8-bit data size: DS[3:0] = 0111 */
#define SPI_DATASIZE_16BIT            15  /*!< 16-bit data size: DS[3:0] = 1111 */

/*
 * ============================================================================
 *                          Driver API Functions
 * ============================================================================
 */

/**
 * @brief  Controls the peripheral clock for the given SPI.
 */
void SPI_PeriClkCtrl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/**
 * @brief  Enables or disables the SPI peripheral (CR1 SPE bit).
 */
void SPI_PeriCtrl(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/**
 * @brief  Configures the internal NSS state when Software Slave Management is active (CR1 SSI bit).
 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/**
 * @brief  Enables or disables SS output for Master mode (CR2 SSOE bit).
 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi);

/**
 * @brief  Reads the status of a specific flag in the SPI Status Register (SR).
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName);

/**
 * @brief  Clears the Overrun (OVR) error flag by reading DR followed by SR.
 */
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);

/**
 * @brief  Terminates an ongoing non-blocking transmission.
 */
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);

/**
 * @brief  Terminates an ongoing non-blocking reception.
 */
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

/**
 * @brief  Initializes the SPI peripheral according to parameters in pSPIHandle.
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);

/**
 * @brief  Resets SPI registers to their default reset values via RCC.
 */
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/**
 * @brief  Blocking data transmission over SPI.
 */
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *pTxBuffer, uint32_t Len);

/**
 * @brief  Blocking data reception over SPI.
 */
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *pRxBuffer, uint32_t Len);

/**
 * @brief  Non-blocking interrupt-driven data transmission over SPI.
 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pTxBuffer, uint32_t Len);

/**
 * @brief  Non-blocking interrupt-driven data reception over SPI.
 */
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle, uint8_t *pRxBuffer, uint32_t Len);

/**
 * @brief  Configures NVIC interrupt enable/disable for SPI.
 */
void SPI_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);

/**
 * @brief  Configures NVIC priority for SPI interrupt line.
 */
void SPI_PriorityConfig(uint8_t IRQPriority, uint8_t IRQNumber);

/**
 * @brief  SPI Interrupt Service Routine handler (dispatches TXE, RXNE, and OVR events).
 */
void SPI_IRQHandling(SPI_Handle_t *pHandle);

/**
 * @brief  Weak callback implemented by application to handle SPI events.
 */
void SPI_ApplicationEventCallBack(SPI_Handle_t *pSPIHandle, uint8_t AppEv);

#endif /* SPI_H_ */
