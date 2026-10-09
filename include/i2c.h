#ifndef I2C_H_
#define I2C_H_

#include "stm32c031c6.h"

/**
 * @brief Configuration structure for I2C peripheral
 */
typedef struct {
  uint32_t I2C_SCLSpeed;       /*!< SCL clock speed (100kHz or 400kHz). @ref I2C_SCL_SPEEDS */
  uint8_t  I2C_DeviceAddress;  /*!< Own 7-bit device address (when configured in slave mode) */
} I2C_Config_t;

/**
 * @brief Handle structure for I2C peripheral
 */
typedef struct {
  I2C_RegDef_t *pI2Cx;         /*!< Base address of I2C peripheral (I2C1, I2C2) */
  I2C_Config_t I2CConfig;      /*!< I2C configuration settings */
  uint8_t *pTxBuffer;          /*!< Pointer to TX buffer (for interrupt-driven transmission) */
  uint8_t *pRxBuffer;          /*!< Pointer to RX buffer (for interrupt-driven reception) */
  uint32_t TxLen;              /*!< Transmit length in bytes */
  uint32_t RxLen;              /*!< Receive length in bytes */
  uint8_t TxRxState;           /*!< Transmission/Reception communication state. @ref I2C_STATES */
  uint8_t DevAddr;             /*!< Target slave address */
  uint32_t RxSize;             /*!< Total size of reception data */
  uint8_t Sr;                  /*!< Repeated Start enable/disable. @ref I2C_REPEATED_START */
} I2C_Handle_t;

/*
 * ============================================================================
 *                          I2C SCL Clock Speeds
 * ============================================================================
 */
#define I2C_SCL_SPEED_SM        100000  /*!< Standard Mode (up to 100 kHz) */
#define I2C_SCL_SPEED_FM        400000  /*!< Fast Mode (up to 400 kHz) */

/*
 * ============================================================================
 *                          I2C Status Flags (ISR Register)
 * ============================================================================
 */
#define I2C_TXE_FLAG            (1 << 0)   /*!< Transmit data register empty */
#define I2C_TXIS_FLAG           (1 << 1)   /*!< Transmit interrupt status (TXDR ready for next byte) */
#define I2C_RXNE_FLAG           (1 << 2)   /*!< Receive data register not empty */
#define I2C_ADDR_FLAG           (1 << 3)   /*!< Address matched (slave mode) */
#define I2C_NACK_FLAG           (1 << 4)   /*!< Not Acknowledge received flag */
#define I2C_STOP_FLAG           (1 << 5)   /*!< Stop detection flag */
#define I2C_TC_FLAG             (1 << 6)   /*!< Transfer Complete (NBYTES transferred, AUTOEND = 0) */
#define I2C_TCR_FLAG            (1 << 7)   /*!< Transfer Complete Reload (RELOAD = 1) */
#define I2C_BERR_FLAG           (1 << 8)   /*!< Bus error */
#define I2C_ARLO_FLAG           (1 << 9)   /*!< Arbitration lost */
#define I2C_OVR_FLAG            (1 << 10)  /*!< Overrun / Underrun */
#define I2C_TIMEOUT_FLAG        (1 << 12)  /*!< Timeout or tLOW detection flag */

/*
 * ============================================================================
 *                          I2C Communication States
 * ============================================================================
 */
#define I2C_READY               0
#define I2C_BUSY_IN_RX          1
#define I2C_BUSY_IN_TX          2

/*
 * ============================================================================
 *                          I2C Application Events & Errors
 * ============================================================================
 */
#define I2C_EV_TX_CMPLT         0  /*!< Transmission complete event */
#define I2C_EV_RX_CMPLT         1  /*!< Reception complete event */
#define I2C_EV_STOP             2  /*!< Stop condition detected event */
#define I2C_ERROR_BERR          3  /*!< Bus error event */
#define I2C_ERROR_ARLO          4  /*!< Arbitration lost event */
#define I2C_ERROR_OVR           5  /*!< Overrun/underrun event */
#define I2C_ERROR_PECERR        6  /*!< PEC error event */
#define I2C_ERROR_TIMEOUT       7  /*!< Timeout error event */
#define I2C_ERROR_ALERT         8  /*!< SMBus alert event */
#define I2C_EV_DATA_RCV         9  /*!< Data received event */
#define I2C_EV_NACK             10 /*!< NACK received event */

/*
 * ============================================================================
 *                          Repeated Start Control
 * ============================================================================
 */
#define I2C_DISABLE_SR          0  /*!< Repeated Start disabled: generate STOP after transfer */
#define I2C_ENABLE_SR           1  /*!< Repeated Start enabled: keep bus active for next transfer */

/*
 * ============================================================================
 *                          Driver API Functions
 * ============================================================================
 */

/**
 * @brief  Controls peripheral clock for I2C1 or I2C2.
 */
void I2C_PeriClkCtrl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/**
 * @brief  Enables or disables the I2C peripheral (CR1 PE bit).
 */
void I2C_PeriCtrl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);

/**
 * @brief  Returns the status of a specific flag in the I2C Interrupt & Status Register (ISR).
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);

/**
 * @brief  Initializes the I2C peripheral (configures TIMINGR, address, etc.).
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);

/**
 * @brief  Resets the I2C peripheral registers via RCC reset lines.
 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/**
 * @brief  Master blocking data transmission.
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

/**
 * @brief  Master blocking data reception.
 */
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

/**
 * @brief  Master non-blocking interrupt-driven data transmission.
 */
uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr);

/**
 * @brief  Master non-blocking interrupt-driven data reception.
 */
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr);

/**
 * @brief  Configures NVIC interrupt line for I2C.
 */
void I2C_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);

/**
 * @brief  Configures NVIC priority for I2C interrupt line.
 */
void I2C_PriorityConfig(uint8_t IRQPriority, uint8_t IRQNumber);

/**
 * @brief  I2C Event Interrupt Service Routine handler.
 */
void I2C_EV_IRQHandling(I2C_Handle_t *pHandle);

/**
 * @brief  I2C Error Interrupt Service Routine handler.
 */
void I2C_ER_IRQHandling(I2C_Handle_t *pHandle);

/**
 * @brief  Weak callback implemented by application to receive I2C events and errors.
 */
void I2C_ApplicationEventCallBack(I2C_Handle_t *pI2CHandle, uint8_t AppEv);

#endif /* I2C_H_ */
