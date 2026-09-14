/******************************************************************************
 * @file    bsp_led.c
 * @brief   LED 驱动实现（寄存器级）
 *          F4 GPIO 配置流程：开 AHB1 时钟 -> MODER 输出 -> OTYPER 推挽
 *                            -> OSPEEDR -> PUPDR -> ODR 初值
 *          低电平点亮：ODR=1 熄灭，ODR=0 点亮
 ******************************************************************************/
#include "bsp_led.h"

/* 内部：根据枚举取端口/引脚/时钟位 */
static void led_get(bsp_led_t led, GPIO_TypeDef **port, uint16_t *pin, uint32_t *rcc_bit)
{
    if (led == BSP_LED0) {
        *port    = BSP_LED0_PORT;
        *pin     = BSP_LED0_PIN;
        *rcc_bit = BSP_LED0_RCC_BIT;
    } else {
        *port    = BSP_LED1_PORT;
        *pin     = BSP_LED1_PIN;
        *rcc_bit = BSP_LED1_RCC_BIT;
    }
}

void bsp_led_init(void)
{
    GPIO_TypeDef *port;
    uint16_t pin;
    uint32_t rcc_bit;
    uint32_t i;

    /* 两个 LED 都初始化；LED0 与 LED1 同在 GPIOF，时钟只需开一次 */
    for (i = 0; i < 2; i++) {
        led_get((bsp_led_t)i, &port, &pin, &rcc_bit);

        RCC->AHB1ENR |= rcc_bit;                 /* 开 GPIO 时钟 */

        /* MODER：对应引脚 2bit 置 01（通用输出） */
        port->MODER  &= ~(3UL << (pin * 2));
        port->MODER  |=  (1UL << (pin * 2));

        /* OTYPER：对应位 0（推挽输出） */
        port->OTYPER &= ~(1UL << pin);

        /* OSPEEDR：50MHz（10） */
        port->OSPEEDR &= ~(3UL << (pin * 2));
        port->OSPEEDR |=  (2UL << (pin * 2));

        /* PUPDR：无上下拉（00） */
        port->PUPDR   &= ~(3UL << (pin * 2));

        /* ODR：初值 1（熄灭，低电平点亮） */
        port->BSRR = (1UL << (pin + 16));        /* 置位 ODR -> 输出高 -> 灭 */
    }
}

void bsp_led_on(bsp_led_t led)
{
    GPIO_TypeDef *port;
    uint16_t pin;
    uint32_t rcc_bit;
    led_get(led, &port, &pin, &rcc_bit);
    port->BSRR = (1UL << pin);                   /* 复位 ODR -> 输出低 -> 亮 */
}

void bsp_led_off(bsp_led_t led)
{
    GPIO_TypeDef *port;
    uint16_t pin;
    uint32_t rcc_bit;
    led_get(led, &port, &pin, &rcc_bit);
    port->BSRR = (1UL << (pin + 16));            /* 置位 ODR -> 输出高 -> 灭 */
}

void bsp_led_toggle(bsp_led_t led)
{
    GPIO_TypeDef *port;
    uint16_t pin;
    uint32_t rcc_bit;
    led_get(led, &port, &pin, &rcc_bit);
    port->ODR ^= (1UL << pin);                   /* 异或翻转 */
}
