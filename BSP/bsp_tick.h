/******************************************************************************
 * @file    bsp_tick.h
 * @brief   SysTick 1ms 系统滴答模块
 *          时钟源 AHB/8 = 168MHz/8 = 21MHz，重装值 21000-1 -> 1ms 中断
 *          维护全局毫秒计数 g_tick，供 bsp_delay_ms / 主循环时间片使用
 ******************************************************************************/
#ifndef __BSP_TICK_H
#define __BSP_TICK_H

#include <stdint.h>                 // uint32_t

#ifdef __cplusplus
extern "C" {
#endif

/* 全局毫秒计数（在中断里自增；读取方用差值比较以避免回卷问题） */
extern volatile uint32_t g_tick;

/* 初始化 SysTick 为 1ms 中断（需在时钟配到 168MHz 后调用） */
void bsp_tick_init(void);

/* 在 SysTick_Handler 中调用：计数 +1 */
void bsp_tick_inc(void);

/* 取当前毫秒计数值 */
uint32_t bsp_tick_get(void);

/* 当前从开机起的毫秒数（= g_tick） */
#define bsp_tick_millis()  (g_tick)

#ifdef __cplusplus
}
#endif

#endif /* __BSP_TICK_H */
