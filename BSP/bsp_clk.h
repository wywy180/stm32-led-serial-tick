/******************************************************************************
 * @file    bsp_clk.h
 * @brief   时钟树配置模块：HSE 8MHz -> SYSCLK 168MHz
 *          依赖：stm32f4xx.h（设备头）；可移植到其它 F4 工程
 ******************************************************************************/
#ifndef __BSP_CLK_H
#define __BSP_CLK_H

#include <stdint.h>                 // 标准 uint 类型

#ifdef __cplusplus
extern "C" {
#endif

/* 配置时钟树：HSE 8M -> PLL -> 168MHz，AHB=168, APB1=42, APB2=84 */
void bsp_clk_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_CLK_H */
