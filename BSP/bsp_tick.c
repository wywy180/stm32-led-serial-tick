/******************************************************************************
 * @file    bsp_tick.c
 * @brief   SysTick 1ms 滴答实现（寄存器级）
 *          SysTick->CTRL: bit0=ENABLE, bit1=TICKINT, bit2=CLKSOURCE
 *          本工程用 AHB/8(21MHz) 源：CLKSOURCE=0，故 1ms 需计 21000 个周期
 ******************************************************************************/
#include "bsp_tick.h"
<<<<<<< HEAD
=======
#include "stm32f4xx.h"              // SysTick 寄存器定义
>>>>>>> e4158175f9ab30d24f7d567946e59df59350963a

/* 全局毫秒计数，中断里自增；声明为 volatile 防止编译器优化掉循环读 */
volatile uint32_t g_tick = 0;

void bsp_tick_init(void)
{
    /* 重装值 = 21000-1（21MHz / 1000 = 21000 计数/1ms） */
    SysTick->LOAD = 21000UL - 1UL;

    /* 清当前值 */
    SysTick->VAL = 0;

    /* CTRL: ENABLE + TICKINT，CLKSOURCE=0(AHB/8)；不写 bit2 即用 AHB/8 */
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;

    /* 注意：NVIC 优先级用默认即可；如需调优先级可加
       NVIC_SetPriority(SysTick_IRQn, 15); */
}

void bsp_tick_inc(void)
{
    g_tick++;                                    /* 毫秒计数 +1 */
}

uint32_t bsp_tick_get(void)
{
    return g_tick;                               /* 原子读 32bit 在 M4 上单指令 */
}
