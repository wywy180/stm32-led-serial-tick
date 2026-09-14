/******************************************************************************
 * @file    system_stm32f4xx.h
 * @brief   CMSIS 系统层接口声明（SystemInit / SystemCoreClock）
 *          本工程为裸机寄存器级，仅保留最小必需接口
 ******************************************************************************/
#ifndef __SYSTEM_STM32F4XX_H
#define __SYSTEM_STM32F4XX_H

#include <stdint.h>                 // 标准 uint32_t 类型

#ifdef __cplusplus
extern "C" {                        // 允许 C++ 调用
#endif

/* 系统主频，由 SystemInit() 配置后写入；bsp_delay_us 等依赖此值 */
extern uint32_t SystemCoreClock;

/* 启动时由 startup 调用一次，完成时钟树 8M->168M 配置 */
extern void SystemInit(void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_STM32F4XX_H */
