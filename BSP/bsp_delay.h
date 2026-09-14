/******************************************************************************
 * @file    bsp_delay.h
 * @brief   延时模块
 *          - bsp_delay_ms : 基于 SysTick 1ms 滴答，毫秒级，无累积漂移
 *          - bsp_delay_us : 基于 DWT CYCCNT 周期计数，微秒级，cycle 精确
 *          不使用 HAL_Delay，满足"不用 HAL 延时"的 C 级任务要求
 ******************************************************************************/
#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include <stdint.h>                 // uint32_t

#ifdef __cplusplus
extern "C" {
#endif

/* 初始化 DWT 周期计数器（bsp_delay_us 依赖；需在时钟配好后调用） */
void bsp_delay_init(void);

/* 毫秒延时：阻塞 n 毫秒（基于 g_tick 差值，回卷安全） */
void bsp_delay_ms(uint32_t ms);

/* 微秒延时：阻塞 n 微秒（基于 DWT CYCCNT，168MHz 下 1us=168 cycles） */
void bsp_delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_DELAY_H */
