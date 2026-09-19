#ifndef I2C_H_
#define I2C_H_
#include "stm32c031c6.h"

typedef struct {
  uint32_t I2C_SCLSpeed;
  uint8_t I2C_DeviceAddress;
} I2C_Config_t;



typedef struct {
  I2C_RegDef_t *pI2Cx;
  I2C_Config_t I2CConfig;
  uint8_t *pTxBuffer;
  uint8_t *pRxBuffer;
  uint32_t TxLen;
  uint32_t RxLen;
  uint8_t TxRxState;
  uint8_t DevAddr;
  uint32_t RxSize;
  uint8_t Sr;
} I2C_Handle_t;


#define I2C_SCL_SPEED_SM 100000
#define I2C_SCL_SPEED_FM 400000

#define I2C_TXE_FLAG  (1<<0)
#define I2C_TXIS_FLAG  (1<<1)
#define I2C_RXNE_FLAG (1<<2)
#define I2C_ADDR_FLAG (1<<3)
#define I2C_NACK_FLAG (1 << 4)  
#define I2C_STOP_FLAG (1<<5)
#define I2C_TC_FLAG (1<<6)
#define I2C_TCR_FLAG (1<<7)
#define I2C_BERR_FLAG (1<<8)
#define I2C_ARLO_FLAG (1<<9)
#define I2C_OVR_FLAG  (1<<10)
#define I2C_TIMEOUT_FLAG (1<<12)

#define I2C_READY					0
#define I2C_BUSY_IN_RX 				1
#define I2C_BUSY_IN_TX 				2

#define I2C_EV_TX_CMPLT  	 	0
#define I2C_EV_RX_CMPLT  	 	1
#define I2C_EV_STOP       		2
#define I2C_ERROR_BERR 	 		3
#define I2C_ERROR_ARLO  		4
#define I2C_ERROR_OVR   		5
#define I2C_ERROR_PECERR   		6
#define I2C_ERROR_TIMEOUT 		7
#define I2C_ERROR_ALERT        8
#define I2C_EV_DATA_RCV         9
#define I2C_EV_NACK           10

#define I2C_DISABLE_SR  0
#define I2C_ENABLE_SR   1

void I2C_PeriClkCtrl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);
void I2C_PeriCtrl(I2C_RegDef_t *pI2Cx, uint8_t EnorDi);
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t FlagName);

void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

void I2C_MasterSendData(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr, uint8_t Sr);

uint8_t I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pTxBuffer, uint32_t Len, uint8_t SlaveAddr);
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle, uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr);


void I2C_IRQConfig(uint8_t IRQNumber, uint8_t EnorDi);
void I2C_PriorityConfig(uint8_t IRQPriority, uint8_t IRQNumber);
void I2C_EV_IRQHandling(I2C_Handle_t *pHandle);
void I2C_ER_IRQHandling(I2C_Handle_t *pHandle);

void I2C_ApplicationEventCallBack(I2C_Handle_t *pI2CHandle, uint8_t AppEv);





#endif /*I2C_H_*/
