# STM32C031C6 Register-Level Peripheral Drivers

A register-level peripheral driver project for the STM32C031C6 microcontroller, developed as a learning and experimentation project.

> **Note:** This project is currently being developed and tested in a **simulation environment**.
[text](https://wokwi.com/projects/473951649788058625)

## Overview

This repository contains custom peripheral drivers for the STM32C031C6, written using direct register-level programming rather than STM32 HAL or LL libraries.

The purpose of the project is to develop a practical understanding of STM32 peripherals, memory-mapped registers, embedded C, interrupts, and low-level driver development.

Current peripheral development focuses on:

- GPIO
- EXTI / GPIO interrupts
- SPI
- I2C
- NVIC configuration within peripheral drivers

## Current Status

| Peripheral | Status |
|---|---|
| GPIO | Implemented |
| GPIO interrupts / EXTI | Implemented |
| SPI polling | Implemented |
| SPI interrupts | Implemented |
| I2C polling | Implemented, debugging |
| I2C interrupts | Implemented, debugging |
| USART | Not yet implemented |

## Project Structure

```text
stm32c031c6-register-level-drivers/
│
├── include/
│   ├── stm32c031c6.h
│   ├── gpio.h
│   ├── spi.h
│   └── i2c.h
│
├── src/
│   ├── gpio.c
│   ├── spi.c
│   ├── i2c.c
│   └── main.c
│
├── README.md
├── LICENSE
└── .gitignore
```

## Driver Architecture

Each peripheral contains its own peripheral-specific functionality, including:

- Peripheral clock control
- Initialization / deinitialization
- Configuration
- Data transfer functions
- Interrupt configuration
- Interrupt handling
- Peripheral-specific callbacks

Interrupt and NVIC handling is kept within the relevant peripheral driver rather than creating a separate generic interrupt-driver layer.

## GPIO

The GPIO driver supports:

- GPIO initialization / deinitialization
- Pin read / write
- Pin toggle
- Interrupt configuration / handling
- NVIC configuration

## SPI

The SPI driver supports:

- SPI initialization / deinitialization
- Data transmit / receive
- Polling / interrupt-driven transfers
- 8-bit / 16-bit data transfers
- Interrupt handling
- Callback handling

## I2C

The I2C driver supports:

- I2C initialization / deinitialization
- Data transmit / receive
- Polling / interrupt-driven transfers
- Interrupt / error handling
- Repeated-start / STOP handling

> **Known issue:** The I2C transmit and receive functionality currently has bugs that are still being investigated and fixed.

## MCU Register Definitions

`stm32c031c6.h` contains custom register definitions and peripheral base addresses for the STM32C031C6.

Currently mapped peripherals include:

- GPIOA/B/C/D/F
- RCC
- EXTI
- SYSCFG
- SPI1/SPI2
- I2C1/I2C2
- USART1/2/3/4

The header also contains peripheral clock-enable macros and interrupt numbers.

## Development Environment

This project is being developed and tested in a **simulation environment**.

The simulation allows the peripheral drivers and register-level code to be developed and debugged without initially requiring physical STM32 hardware.

## Learning Objectives

This project is intended to develop practical understanding of:

- ARM Cortex-M architecture
- STM32 peripheral registers
- Memory-mapped I/O
- Embedded C
- GPIO configuration
- Interrupts
- NVIC
- EXTI
- SPI communication
- I2C communication
- USART communication
- Peripheral clock management
- Low-level driver architecture
- Embedded debugging

## Roadmap

- [ ] Fix I2C bugs
- [ ] Add USART driver
- [ ] Build a project after getting physical hardware

## License

See [`LICENSE`](LICENSE) for the terms under which this project is distributed.
