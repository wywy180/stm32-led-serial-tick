/******************************************************************************
 * @file    bsp_delay.c
 * @brief   延时实现
 *          bsp_delay_ms: SysTick 1ms 滴查差值，无累积漂移
 *          bsp_delay_us: DWT CYCCNT 周期计数
 *
 *  DWT 启用三步（Cortex-M4，168MHz 下每周期约 5.95ns）：
 *    CoreDebug->DEMCR |= (1<<24)   TRCENA  跟踪使能
 *    DWT->CYCCNT = 0                清零
 *    DWT->CTRL  |= (1<<0)           CYCCNTENA 计数使能
 ******************************************************************************/
#include "bsp_delay.h"
#include "bsp_tick.h"               // g_tick
#include "stm32f4xx.h"              // DWT / CoreDebug / SysTick
#include "system_stm32f4xx.h"       // SystemCoreClock（168MHz）

/* 每微秒所需的周期数（= 主频 MHz），在 init 时缓存 */
static uint32_t s_cycles_per_us = 168UL;

void bsp_delay_init(void)
{
    /* 缓存每微秒周期数（主频/1e6） */
    s_cycles_per_us = SystemCoreClock / 1000000UL;

    /* 使能 DWT 数据观察点/跟踪单元的周期计数器 */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;   /* TRCENA */
    DWT->CYCCNT = 0;                                   /* 清零计数器 */
    DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;             /* 使能 CYCCNT */
}

void bsp_delay_ms(uint32_t ms)
{
    uint32_t t0 = g_tick;                   /* 记录起始毫秒值 */
    /* 用差值比较，天然处理 32bit 回卷（无符号减法环绕） */
    while ((g_tick - t0) < ms) { }
}

void bsp_delay_us(uint32_t us)
{
    uint32_t start  = DWT->CYCCNT;          /* 起始周期计数 */
    uint32_t cycles = us * s_cycles_per_us; /* 目标周期数 = us * cycles/us */
    /* 同样用差值，处理 CYCCNT 32bit 回卷 */
    while ((DWT->CYCCNT - start) < cycles) { }
}
