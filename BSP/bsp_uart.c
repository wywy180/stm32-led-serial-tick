/******************************************************************************
 * @file    bsp_uart.c
 * @brief   USART1 驱动 + printf 重定向（MicroLib）
 *          配置：PA9/PA10 复用 AF7，USART1 在 APB2(84MHz)，波特率 115200
 *          BRR = 84MHz / 115200 ≈ 729
 *          printf 重定向：重写 fputc，MicroLib 内部调用之
 ******************************************************************************/
#include "bsp_uart.h"
#include <stdio.h>                  /* FILE, printf 等（MicroLib） */

void bsp_uart_init(void)
{
    /* ---- 1. 开 GPIOA 与 USART1 时钟 ---- */
    RCC->AHB1ENR |= BSP_UART_GPIO_RCC;          /* GPIOA 时钟 */
    RCC->APB2ENR |= BSP_UART_RCC_BIT;           /* USART1 时钟 */

    /* ---- 2. PA9(TX) / PA10(RX) 配置为复用功能 AF7 ---- */
    /* MODER = 10（复用） */
    BSP_UART_GPIO->MODER  &= ~(3UL << (BSP_UART_TX_PIN * 2));
    BSP_UART_GPIO->MODER  |=  (2UL << (BSP_UART_TX_PIN * 2));
    BSP_UART_GPIO->MODER  &= ~(3UL << (BSP_UART_RX_PIN * 2));
    BSP_UART_GPIO->MODER  |=  (2UL << (BSP_UART_RX_PIN * 2));

    /* OTYPER = 0（推挽） */
    BSP_UART_GPIO->OTYPER &= ~((1UL << BSP_UART_TX_PIN) | (1UL << BSP_UART_RX_PIN));

    /* OSPEEDR = 11（高速） */
    BSP_UART_GPIO->OSPEEDR |= (3UL << (BSP_UART_TX_PIN * 2));
    BSP_UART_GPIO->OSPEEDR |= (3UL << (BSP_UART_RX_PIN * 2));

    /* PUPDR：TX 无上下拉(00)，RX 上拉(01) */
    BSP_UART_GPIO->PUPDR  &= ~(3UL << (BSP_UART_TX_PIN * 2));
    BSP_UART_GPIO->PUPDR  &= ~(3UL << (BSP_UART_RX_PIN * 2));
    BSP_UART_GPIO->PUPDR  |=  (1UL << (BSP_UART_RX_PIN * 2));

    /* AFR：AFR[0] 管 P0~P7，AFR[1] 管 P8~P15。PA9/PA10 都在 AFR[1] */
    BSP_UART_GPIO->AFR[1] &= ~(0xFUL << ((BSP_UART_TX_PIN - 8) * 4));
    BSP_UART_GPIO->AFR[1] |=  (BSP_UART_AF  << ((BSP_UART_TX_PIN - 8) * 4));
    BSP_UART_GPIO->AFR[1] &= ~(0xFUL << ((BSP_UART_RX_PIN - 8) * 4));
    BSP_UART_GPIO->AFR[1] |=  (BSP_UART_AF  << ((BSP_UART_RX_PIN - 8) * 4));

    /* ---- 3. USART1 配置 ---- */
    BSP_UART->CR1 = 0;                          /* 先禁能，清配置 */
    BSP_UART->CR2 = 0;                          /* 1 停止位 */
    BSP_UART->CR3 = 0;                          /* 无硬件流控 */

    /* 波特率：BRR = fPCLK / 波特率 = 84e6 / 115200 ≈ 729 */
    BSP_UART->BRR = 84000000UL / BSP_UART_BAUD;

    /* CR1：使能 UE(USART) + TE(发) + RE(收)，字长 8bit，无校验 */
    BSP_UART->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void bsp_uart_putc(uint8_t ch)
{
    /* 等待发送数据寄存器空 TXE（SR bit7） */
    while ((BSP_UART->SR & USART_SR_TXE) == 0U) { }
    BSP_UART->DR = (uint32_t)ch;                /* 写 DR 自动清 TXE */
}

void bsp_uart_puts(const char *s)
{
    while (*s) {                                /* 直到字符串结尾 */
        bsp_uart_putc((uint8_t)*s++);
    }
}

/* ============================================================================
 * printf 重定向（MicroLib）：重写 fputc，把字符送 USART1
 * 工程需在 Keil Options->Target 勾选 "Use MicroLib"（.uvprojx 已设）
 * ==========================================================================*/
int fputc(int ch, FILE *f)
{
    (void)f;                                    /* 文件指针不用 */
    if (ch == '\n') {                           /* 换行前补回车，便于串口助手显示 */
        bsp_uart_putc('\r');
    }
    bsp_uart_putc((uint8_t)ch);
    return ch;
}
