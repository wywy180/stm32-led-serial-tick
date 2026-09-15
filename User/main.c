/******************************************************************************
 * @file    main.c
 * @brief   主程序
 *          任务A：每 1s 翻转 LED0(PF9) 并串口打印 "hello, tick=xxx"
 *          任务B：示波器测 bsp_delay_ms(1) -> LED1(PF10) 周期 2ms
 *          任务C：示波器测 bsp_delay_us(10) -> LED1(PF10) 周期 20us
 *          开机首行打印固件版本（gen_version.bat 自动注入）
 *
 *  运行模式 RUN_MODE：
 *    0 = 正常模式（任务A）：1s 打印 + LED0 翻转；示波器测 PF9 周期 1s
 *    1 = 毫秒延时测试（任务B）：LED1 每 bsp_delay_ms(1) 翻转；测 PF10 周期 2ms
 *    2 = 微秒延时测试（任务C）：LED1 每 bsp_delay_us(10) 翻转；测 PF10 周期 20us
 *  切换模式后重新编译下载即可。
 ******************************************************************************/
#include "stm32f4xx.h"              // 设备头
#include <stdio.h>                  /* printf */
#include "bsp_led.h"
#include "bsp_uart.h"
#include "bsp_tick.h"
#include "bsp_delay.h"
#include "bsp_version.h"

#define RUN_MODE   0                /* 0=任务A  1=任务B(ms)  2=任务C(us) */

int main(void)
{
    /* ---- 外设初始化（SystemInit 已把时钟配到 168MHz） ---- */
    bsp_led_init();                 /* LED0/LED1 */
    bsp_uart_init();                /* USART1 115200 用于 printf */
    bsp_delay_init();               /* DWT CYCCNT（bsp_delay_us 依赖） */
    bsp_tick_init();                /* SysTick 1ms 滴答（bsp_delay_ms/g_tick 依赖） */

    /* ---- 开机打印固件版本（任务C：每次下载可见版本） ---- */
    bsp_version_print();

#if RUN_MODE == 0
    /* ================= 任务 A：1s 打印 + LED0 翻转 ================= */
    {
        uint32_t last = 0;          /* 上次打印的 tick */
        bsp_led_off(BSP_LED1);      /* 任务A 不用 LED1，熄灭 */
        while (1) {
            uint32_t now = bsp_tick_get();
            if ((now - last) >= 1000UL) {        /* 每 1000ms */
                last = now;
                bsp_led_toggle(BSP_LED0);        /* PF9 翻转（示波器测周期=1s） */
                printf("hello, tick=%lu\n", (unsigned long)now);  /* 串口打印 */
            }
            /* 其余时间空转，可放低优先级工作 */
        }
    }
#elif RUN_MODE == 1
    /* ================= 任务 B：bsp_delay_ms 误差测试 ================= */
    /* LED1 每 bsp_delay_ms(1) 翻转一次 -> 周期 2ms，示波器测 PF10 */
    bsp_led_off(BSP_LED0);
    while (1) {
        bsp_led_toggle(BSP_LED1);
        bsp_delay_ms(1);            /* 毫秒延时（SysTick 滴答） */
    }
#elif RUN_MODE == 2
    /* ================= 任务 C：bsp_delay_us 精度测试 ================= */
    /* LED1 每 bsp_delay_us(10) 翻转一次 -> 周期 20us，示波器测 PF10 */
    bsp_led_off(BSP_LED0);
    while (1) {
        bsp_led_toggle(BSP_LED1);
        bsp_delay_us(10);           /* 微秒延时（DWT CYCCNT） */
    }
#else
    #error "RUN_MODE 必须为 0/1/2"
#endif
}

#ifdef  USE_FULL_ASSERT            /* 本工程未启用断言，保留占位 */
void assert_failed(uint8_t *file, uint32_t line) { (void)file; (void)line; while (1) {} }
#endif
