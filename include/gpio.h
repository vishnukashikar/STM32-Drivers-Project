#ifndef GPIO_H_
#define GPIO_H_

#include "stm32c031c6.h"

/**
 * @brief Configuration structure for a GPIO pin
 */
typedef struct {
  uint8_t GPIO_PinNumber;       /*!< Specifies the GPIO pin number. @ref GPIO_PIN_NUMBERS */
  uint8_t GPIO_PinMode;         /*!< Specifies the operating mode for the selected pin. @ref GPIO_PIN_MODES */
  uint8_t GPIO_PinSpeed;        /*!< Specifies the speed for the selected pin. @ref GPIO_PIN_SPEEDS */
  uint8_t GPIO_PinPuPdControl;  /*!< Specifies the Pull-up/Pull-down configuration. @ref GPIO_PIN_PUPD */
  uint8_t GPIO_PinOPType;       /*!< Specifies the output type (Push-Pull or Open-Drain). @ref GPIO_PIN_OP_TYPES */
  uint8_t GPIO_PinAltFunMode;   /*!< Specifies the alternate function mode (AF0 - AF7). */
} GPIO_PinConfig_t;

/**
 * @brief Handle structure for a GPIO pin
 */
typedef struct {
  GPIO_RegDef_t *pGPIOx;         /*!< Base address of the GPIO port to which the pin belongs */
  GPIO_PinConfig_t GPIO_PinConfig;/*!< Pin configuration settings */
} GPIO_Handle_t;

/*
 * ============================================================================
 *                          GPIO Pin Numbers
 * ============================================================================
 */
#define GPIO_PIN_NO_0   0
#define GPIO_PIN_NO_1   1
#define GPIO_PIN_NO_2   2
#define GPIO_PIN_NO_3   3
#define GPIO_PIN_NO_4   4
#define GPIO_PIN_NO_5   5
#define GPIO_PIN_NO_6   6
#define GPIO_PIN_NO_7   7
#define GPIO_PIN_NO_8   8
#define GPIO_PIN_NO_9   9
#define GPIO_PIN_NO_10  10
#define GPIO_PIN_NO_11  11
#define GPIO_PIN_NO_12  12
#define GPIO_PIN_NO_13  13
#define GPIO_PIN_NO_14  14
#define GPIO_PIN_NO_15  15

/*
 * ============================================================================
 *                          GPIO Pin Modes
 * ============================================================================
 */
#define GPIO_MODE_IN      0   /*!< Input mode (reset state) */
#define GPIO_MODE_OUT     1   /*!< General purpose output mode */
#define GPIO_MODE_ALTFN   2   /*!< Alternate function mode */
#define GPIO_MODE_ANALOG  3   /*!< Analog mode */
#define GPIO_MODE_IT_FT   4   /*!< Interrupt mode: Falling-edge trigger */
#define GPIO_MODE_IT_RT   5   /*!< Interrupt mode: Rising-edge trigger */
#define GPIO_MODE_IT_RFT  6   /*!< Interrupt mode: Rising/Falling-edge trigger */

/*
 * ============================================================================
 *                          GPIO Output Types
 * ============================================================================
 */
#define GPIO_OP_TYPE_PP   0   /*!< Output push-pull (reset state) */
#define GPIO_OP_TYPE_OD   1   /*!< Output open-drain */

/*
 * ============================================================================
 *                          GPIO Output Speeds
 * ============================================================================
 */
#define GPIO_SPEED_LOW    0   /*!< Low speed */
#define GPIO_SPEED_MEDIUM 1   /*!< Medium speed */
#define GPIO_SPEED_FAST   2   /*!< High / Fast speed */
#define GPIO_SPEED_HIGH   3   /*!< Very high speed */

/*
 * ============================================================================
 *                  GPIO Pull-Up / Pull-Down Configurations
 * ============================================================================
 */
#define GPIO_NO_PUPD      0   /*!< No pull-up, no pull-down */
#define GPIO_PU           1   /*!< Pull-up enabled */
#define GPIO_PD           2   /*!< Pull-down enabled */

/*
 * ============================================================================
 *                          Driver API Functions
 * ============================================================================
 */

/**
 * @brief  Enables or disables the peripheral clock for the given GPIO port.
 * @param  pGPIOx : Base address of the GPIO port (GPIOA, GPIOB, etc.)
 * @param  EnorDi : ENABLE or DISABLE macros
 */
void GPIO_PeriClkCtrl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi);

/**
 * @brief  Initializes the GPIO pin according to the specified parameters in pGPIOHandle.
 * @param  pGPIOHandle : Pointer to the GPIO handle structure containing configuration info
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);

/**
 * @brief  De-initializes the GPIO port by resetting its peripheral registers via RCC.
 * @param  pGPIOx : Base address of the GPIO port
 */
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/**
 * @brief  Reads the input value from a specific GPIO pin.
 * @param  pGPIOx     : Base address of the GPIO port
 * @param  PinNumber  : Pin number (0 to 15)
 * @return 0 or 1
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/**
 * @brief  Reads the value from the entire GPIO input port (all 16 pins).
 * @param  pGPIOx : Base address of the GPIO port
 * @return 16-bit port input status
 */
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);

/**
 * @brief  Writes a high (SET) or low (RESET) value to a specific GPIO output pin.
 * @param  pGPIOx     : Base address of the GPIO port
 * @param  PinNumber  : Pin number (0 to 15)
 * @param  Value      : GPIO_PIN_SET or GPIO_PIN_RESET
 */
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);

/**
 * @brief  Writes a 16-bit value to the entire GPIO output port.
 * @param  pGPIOx : Base address of the GPIO port
 * @param  Value  : 16-bit output value
 */
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);

/**
 * @brief  Toggles the output state of a specific GPIO pin.
 * @param  pGPIOx     : Base address of the GPIO port
 * @param  PinNumber  : Pin number (0 to 15)
 */
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

/**
 * @brief  Enables or disables an interrupt in the ARM Cortex-M0+ NVIC.
 * @param  IRQNumber : NVIC IRQ number
 * @param  EnorDi    : ENABLE or DISABLE
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);

/**
 * @brief  Configures the priority level of an interrupt in NVIC.
 * @param  IRQPriority : Priority value (0 to 3 on Cortex-M0+)
 * @param  IRQNumber   : NVIC IRQ number
 */
void GPIO_PriorityConfig(uint8_t IRQPriority, uint8_t IRQNumber);

/**
 * @brief  Clears the pending EXTI interrupt flag for the specified pin number.
 * @param  PinNumber : Pin number (0 to 15)
 */
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* GPIO_H_ */
