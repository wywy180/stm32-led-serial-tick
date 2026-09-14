/******************************************************************************
 * @file    bsp_led.h
 * @brief   LED 模块（ALIENTEK 探索者 F407）
 *          LED0=PF9(红)  LED1=PF10(绿)，低电平点亮
 *          可复用到其他 F4 工程：修改 BSP_LED0_PIN 等宏即可
 ******************************************************************************/
#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f4xx.h"              // 设备头：GPIO_TypeDef 等

#ifdef __cplusplus
extern "C" {
#endif

/* LED 引脚映射（移植时改这里即可） */
#define BSP_LED0_PORT       GPIOF
#define BSP_LED0_PIN        (9U)       /* PF9  红 */
#define BSP_LED0_RCC_BIT    (RCC_AHB1ENR_GPIOFEN)

#define BSP_LED1_PORT       GPIOF
#define BSP_LED1_PIN        (10U)      /* PF10 绿 */
#define BSP_LED1_RCC_BIT    (RCC_AHB1ENR_GPIOFEN)

typedef enum {
    BSP_LED0 = 0,                    /* 红灯 PF9  */
    BSP_LED1 = 1,                    /* 绿灯 PF10 */
} bsp_led_t;

/* 初始化 LED 引脚为推挽输出（默认熄灭，因低电平点亮） */
void bsp_led_init(void);

/* 点亮（输出低） */
void bsp_led_on(bsp_led_t led);

/* 熄灭（输出高） */
void bsp_led_off(bsp_led_t led);

/* 翻转状态 */
void bsp_led_toggle(bsp_led_t led);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_LED_H */
