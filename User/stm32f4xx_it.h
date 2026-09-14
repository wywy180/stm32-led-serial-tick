/******************************************************************************
 * @file    stm32f4xx_it.h
 * @brief   中断服务程序声明
 ******************************************************************************/
#ifndef __STM32F4XX_IT_H
#define __STM32F4XX_IT_H

#include "stm32f4xx.h"              // 设备头

#ifdef __cplusplus
extern "C" {
#endif

/* SysTick 中断服务（1ms，维护 g_tick） */
void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4XX_IT_H */
