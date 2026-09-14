/******************************************************************************
 * @file    bsp_uart.h
 * @brief   USART1 串口模块（PA9=TX, PA10=RX, 115200 8N1, APB2=84MHz）
 *          配合 MicroLib 重定向 printf，开机即可 printf 调试
 ******************************************************************************/
#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "stm32f4xx.h"              // 设备头

#ifdef __cplusplus
extern "C" {
#endif

/* 串口参数（移植时改这里） */
#define BSP_UART            USART1
#define BSP_UART_RCC_BIT    (RCC_APB2ENR_USART1EN)
#define BSP_UART_GPIO       GPIOA
#define BSP_UART_GPIO_RCC   (RCC_AHB1ENR_GPIOAEN)
#define BSP_UART_TX_PIN     (9U)
#define BSP_UART_RX_PIN     (10U)
#define BSP_UART_AF         (7U)       /* USART1 复用功能号 AF7 */
#define BSP_UART_BAUD       (115200UL)

/* 初始化 USART1：GPIO 复用 + 波特率 + 使能收发 */
void bsp_uart_init(void);

/* 轮询发送一个字节 */
void bsp_uart_putc(uint8_t ch);

/* 轮询发送字符串 */
void bsp_uart_puts(const char *s);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_UART_H */
