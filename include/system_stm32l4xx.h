/**
  ******************************************************************************
  * @file    system_stm32l4xx.h
  * @author  MCD Application Team
  * @brief   CMSIS Cortex-M4 Device System Source File for STM32L4xx devices.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32l4xx_system
  * @{
  */

/**
  * @brief Define to prevent recursive inclusion
  */
#ifndef __SYSTEM_STM32L4XX_H
#define __SYSTEM_STM32L4XX_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l4s5xx.h"

// Hardware clock speeds
#define HSI_VALUE ((uint32_t) 16000000)
#define HSE_VALUE ((uint32_t) 48000000)
#define MSI_VALUE ((uint32_t) 48000000)

// PLL Values for using 48Mhz HSE clock source
// System clock = HSE_VALUE / PLL_M * PLL_N / PLL_R
// 120Mhz = 48 Mhz / 1 * 20 / 8
#define PLL_M ((uint32_t) 1) // Actual bit value is this minus 1
#define PLL_N ((uint32_t) 20)
#define PLL_R ((uint32_t) 8) // Actual bit is PLL_R/2 - 1

extern uint32_t SystemCoreClock;            /*!< System Clock Frequency (Core Clock) */

extern const uint8_t  AHBPrescTable[16];    /*!< AHB prescalers table values */
extern const uint8_t  APBPrescTable[8];     /*!< APB prescalers table values */
extern const uint32_t MSIRangeTable[12];    /*!< MSI ranges table values     */


extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
void set_sysclk_to_120(void);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_STM32L4XX_H */
