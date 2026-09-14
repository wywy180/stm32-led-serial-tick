/******************************************************************************
 * @file    system_stm32f4xx.c
 * @brief   CMSIS 系统层：SystemInit() 把系统时钟从 HSE 8MHz 推到 168MHz
 *          - 由启动文件 Reset_Handler 在进入 main 之前调用一次
 *          - 此时全局变量尚未初始化，故本函数只写寄存器、不依赖任何 C 状态
 *          - 真正的寄存器配置在 bsp_clk.c::bsp_clk_init() 中，本文件只做转发
 ******************************************************************************/
#include "stm32f4xx.h"              // 设备头（来自 Keil STM32F4xx_DFP）
#include "system_stm32f4xx.h"       // 本文件接口
#include "bsp_clk.h"                // 时钟树寄存器配置

/* 当前系统主频（Hz），bsp_delay_us 等模块读取此值计算延时 */
uint32_t SystemCoreClock = 168000000UL;

/**
 * @brief  系统初始化：配置时钟树
 *         startup 在调用 __main(运行时初始化) 之前先调用本函数，
 *         因此函数返回后系统已运行在 168MHz，main 里的外设初始化可直接按 168MHz 计算
 */
void SystemInit(void)
{
    /* 调 bsp_clk_init 完成具体 RCC 寄存器配置 */
    bsp_clk_init();

    /* 更新主频变量（裸机工程无 CMSIS SystemCoreClockUpdate，直接赋值） */
    SystemCoreClock = 168000000UL;

    /* 注意：FPU 未启用（本工程不使用浮点）；如需用浮点，在此处加
       SCB->CPACR |= (0xFUL << 20); __DSB(); __ISB(); */
}
