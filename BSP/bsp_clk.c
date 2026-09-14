/******************************************************************************
 * @file    bsp_clk.c
 * @brief   STM32F407 时钟树寄存器级配置
 *          HSE 8MHz -> PLL(M=8,N=336,P=2,Q=7) -> SYSCLK 168MHz
 *          AHB=168MHz(/1)  APB1=42MHz(/4)  APB2=84MHz(/2)
 *          Flash 等待周期 = 5 WS（168MHz @ 2.7~3.6V）
 *
 *  寄存器速查（F4）：
 *    RCC->CR        HSEON/HSEON/HSERDY/PLLON/PLLRDY
 *    RCC->PLLCFGR   PLLM[5:0] | PLLN[14:6] | PLLP[17:16] | PLLSRC[22] | PLLQ[27:24]
 *    RCC->CFGR      SW[1:0] HPRE[7:4] PPRE1[12:10] PPRE2[15:13]
 *    FLASH->ACR     LATENCY[2:0] | PRFTEN[8] | ICEN[9] | DCEN[10]
 *    PWR->CR        VOS[15:14]  (11=Scale1, 168MHz 必需)
 ******************************************************************************/
#include "stm32f4xx.h"              // 设备头
#include "bsp_clk.h"

void bsp_clk_init(void)
{
    uint32_t tmp;

    /* ---- 1. 开 HSE 外部 8MHz 晶振，等待就绪 ---- */
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);                 // 使能 HSE
    while ((RCC->CR & RCC_CR_HSERDY) == 0U) { }          // 等待 HSE 就绪

    /* ---- 2. 使能 PWR 时钟，设 VOS=Scale1（168MHz 必需的内核电压） ---- */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;                   // 开 PWR 时钟
    PWR->CR |= (3U << 14);                               // VOS=11 -> Scale1

    /* ---- 3. Flash 等待周期 5 WS + 预取 + I/D Cache ---- */
    FLASH->ACR = FLASH_ACR_LATENCY_5W                    // 5 个等待周期
               | FLASH_ACR_PRFTEN                        // 预取使能
               | FLASH_ACR_ICEN                          // 指令 Cache
               | FLASH_ACR_DCEN;                         // 数据 Cache

    /* ---- 4. 总线分频：AHB=/1, APB1=/4, APB2=/2 ---- */
    tmp  = RCC->CFGR;
    tmp &= ~((uint32_t)(RCC_CFGR_HPRE  | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2));
    tmp |=  RCC_CFGR_HPRE_DIV1;                          // AHB=/1 -> 168MHz
    tmp |=  RCC_CFGR_PPRE1_DIV4;                         // APB1=/4 -> 42MHz
    tmp |=  RCC_CFGR_PPRE2_DIV2;                         // APB2=/2 -> 84MHz
    RCC->CFGR = tmp;

    /* ---- 5. 配置 PLL：M=8 N=336 P=2 Q=7，源=HSE ----
     * PLLCFGR 字段拼装（避免依赖具体宏名，用位移表达更直观）：
     *   PLLM = 8    -> bits[5:0]
     *   PLLN = 336  -> bits[14:6]
     *   PLLP = 0(/2)-> bits[17:16]
     *   PLLSRC=1    -> bit[22]   (1=HSE)
     *   PLLQ = 7    -> bits[27:24]
     */
    RCC->PLLCFGR = (8UL << 0)
                 | (336UL << 6)
                 | (0UL << 16)
                 | (1UL << 22)
                 | (7UL << 24);

    /* ---- 6. 开 PLL，等待就绪 ---- */
    RCC->CR |= RCC_CR_PLLON;                             // 使能 PLL
    while ((RCC->CR & RCC_CR_PLLRDY) == 0U) { }          // 等待 PLL 锁定

    /* ---- 7. 切换 SYSCLK 源到 PLL，等待切换完成 ---- */
    tmp  = RCC->CFGR;
    tmp &= ~RCC_CFGR_SW;                                 // 清 SW
    tmp |=  RCC_CFGR_SW_PLL;                             // SW=10 选 PLL
    RCC->CFGR = tmp;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) { } // 等待 SWS=PLL
}
