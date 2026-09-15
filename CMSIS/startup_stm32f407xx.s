;******************************************************************************
; @file    startup_stm32f407xx.s
; @brief   STM32F407xx 启动文件（Keil armasm / AC5 语法）
;          - 提供中断向量表（16 系统异常 + 82 外部中断）
;          - Reset_Handler：先调 SystemInit() 配置时钟，再跳 __main(C 运行时)
;          - 其余异常/中断均 [WEAK] 弱定义，可被用户 C 代码覆盖
;          SysTick_Handler 即在本文件中弱定义，被 stm32f4xx_it.c 中的强定义覆盖
;******************************************************************************
; 堆栈大小：1KB（裸机工程够用，按需调大）
Stack_Size      EQU     0x00000400
                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

; 堆大小：512B（MicroLib 下基本不用，保留以备 malloc）
Heap_Size       EQU     0x00000200
                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

;================ 中断向量表，映射到复位地址 0x08000000 ================
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; 栈顶
                DCD     Reset_Handler             ; 复位
                DCD     NMI_Handler               ; NMI
                DCD     HardFault_Handler         ; 硬件错误
                DCD     MemManage_Handler         ; 内存管理错误
                DCD     BusFault_Handler          ; 总线错误
                DCD     UsageFault_Handler        ; 用法错误
                DCD     0                         ; 保留
                DCD     0                         ; 保留
                DCD     0                         ; 保留
                DCD     0                         ; 保留
                DCD     SVC_Handler               ; SVCall
                DCD     DebugMon_Handler          ; 调试监视器
                DCD     0                         ; 保留
                DCD     PendSV_Handler            ; PendSV
                DCD     SysTick_Handler           ; SysTick（弱定义，C 中覆盖）

; ---- 外部中断（F407 共 82 个，序号 0~81）----
                DCD     WWDG_IRQHandler           ; 0  窗口看门狗
                DCD     PVD_IRQHandler            ; 1  PVD
                DCD     TAMP_STAMP_IRQHandler     ; 2  侵入/时间戳
                DCD     RTC_WKUP_IRQHandler       ; 3  RTC 唤醒
                DCD     FLASH_IRQHandler          ; 4  Flash
                DCD     RCC_IRQHandler            ; 5  RCC
                DCD     EXTI0_IRQHandler          ; 6  EXTI0
                DCD     EXTI1_IRQHandler          ; 7  EXTI1
                DCD     EXTI2_IRQHandler          ; 8  EXTI2
                DCD     EXTI3_IRQHandler          ; 9  EXTI3
                DCD     EXTI4_IRQHandler          ; 10 EXTI4
                DCD     DMA1_Stream0_IRQHandler   ; 11
                DCD     DMA1_Stream1_IRQHandler   ; 12
                DCD     DMA1_Stream2_IRQHandler   ; 13
                DCD     DMA1_Stream3_IRQHandler   ; 14
                DCD     DMA1_Stream4_IRQHandler   ; 15
                DCD     DMA1_Stream5_IRQHandler   ; 16
                DCD     DMA1_Stream6_IRQHandler   ; 17
                DCD     ADC_IRQHandler            ; 18
                DCD     CAN1_TX_IRQHandler        ; 19
                DCD     CAN1_RX0_IRQHandler       ; 20
                DCD     CAN1_RX1_IRQHandler       ; 21
                DCD     CAN1_SCE_IRQHandler       ; 22
                DCD     EXTI9_5_IRQHandler        ; 23 EXTI[9:5]
                DCD     TIM1_BRK_TIM9_IRQHandler  ; 24
                DCD     TIM1_UP_TIM10_IRQHandler  ; 25
                DCD     TIM1_TRG_COM_TIM11_IRQHandler ; 26
                DCD     TIM1_CC_IRQHandler        ; 27
                DCD     TIM2_IRQHandler           ; 28
                DCD     TIM3_IRQHandler           ; 29
                DCD     TIM4_IRQHandler           ; 30
                DCD     I2C1_EV_IRQHandler        ; 31
                DCD     I2C1_ER_IRQHandler        ; 32
                DCD     I2C2_EV_IRQHandler        ; 33
                DCD     I2C2_ER_IRQHandler        ; 34
                DCD     SPI1_IRQHandler           ; 35
                DCD     SPI2_IRQHandler           ; 36
                DCD     USART1_IRQHandler         ; 37
                DCD     USART2_IRQHandler         ; 38
                DCD     USART3_IRQHandler         ; 39
                DCD     EXTI15_10_IRQHandler      ; 40 EXTI[15:10]
                DCD     RTC_Alarm_IRQHandler      ; 41 RTC 闹钟
                DCD     OTG_FS_WKUP_IRQHandler    ; 42 USB OTG FS 唤醒
                DCD     TIM8_BRK_TIM12_IRQHandler ; 43
                DCD     TIM8_UP_TIM13_IRQHandler  ; 44
                DCD     TIM8_TRG_COM_TIM14_IRQHandler ; 45
                DCD     TIM8_CC_IRQHandler        ; 46
                DCD     DMA1_Stream7_IRQHandler   ; 47
                DCD     FMC_IRQHandler            ; 48
                DCD     SDIO_IRQHandler           ; 49
                DCD     TIM5_IRQHandler           ; 50
                DCD     SPI3_IRQHandler           ; 51
                DCD     UART4_IRQHandler          ; 52
                DCD     UART5_IRQHandler          ; 53
                DCD     TIM6_DAC_IRQHandler       ; 54
                DCD     TIM7_IRQHandler           ; 55
                DCD     DMA2_Stream0_IRQHandler   ; 56
                DCD     DMA2_Stream1_IRQHandler   ; 57
                DCD     DMA2_Stream2_IRQHandler   ; 58
                DCD     DMA2_Stream3_IRQHandler   ; 59
                DCD     DMA2_Stream4_IRQHandler   ; 60
                DCD     ETH_IRQHandler            ; 61 以太网
                DCD     ETH_WKUP_IRQHandler       ; 62
                DCD     CAN2_TX_IRQHandler        ; 63
                DCD     CAN2_RX0_IRQHandler       ; 64
                DCD     CAN2_RX1_IRQHandler       ; 65
                DCD     CAN2_SCE_IRQHandler       ; 66
                DCD     OTG_FS_IRQHandler         ; 67
                DCD     DMA2_Stream5_IRQHandler   ; 68
                DCD     DMA2_Stream6_IRQHandler   ; 69
                DCD     DMA2_Stream7_IRQHandler   ; 70
                DCD     USART6_IRQHandler         ; 71
                DCD     I2C3_EV_IRQHandler        ; 72
                DCD     I2C3_ER_IRQHandler        ; 73
                DCD     OTG_HS_EP1_OUT_IRQHandler ; 74
                DCD     OTG_HS_EP1_IN_IRQHandler  ; 75
                DCD     OTG_HS_WKUP_IRQHandler    ; 76
                DCD     OTG_HS_IRQHandler         ; 77
                DCD     DCMI_IRQHandler           ; 78
                DCD     0                         ; 79 保留（F417 为 CRYP）
                DCD     HASH_RNG_IRQHandler       ; 80
                DCD     FPU_IRQHandler            ; 81
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

;================ 代码段 ================
                AREA    |.text|, CODE, READONLY

;----- 复位处理：SystemInit 配时钟 -> __main(C 初始化 + main) -----
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                IMPORT  SystemInit
                LDR     R0, =SystemInit           ; 配置时钟到 168MHz
                BLX     R0
                LDR     R0, =__main                ; 跳转 C 运行时
                BX      R0
                ENDP

;----- 系统异常弱定义（未实现则死循环）-----
NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler PROC
                EXPORT  UsageFault_Handler       [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]   ; 被 stm32f4xx_it.c 覆盖
                B       .
                ENDP

;----- 外部中断统一弱定义到 Default_Handler -----
Default_Handler PROC
                EXPORT  WWDG_IRQHandler           [WEAK]
                EXPORT  PVD_IRQHandler            [WEAK]
                EXPORT  TAMP_STAMP_IRQHandler     [WEAK]
                EXPORT  RTC_WKUP_IRQHandler       [WEAK]
                EXPORT  FLASH_IRQHandler          [WEAK]
                EXPORT  RCC_IRQHandler            [WEAK]
                EXPORT  EXTI0_IRQHandler          [WEAK]
                EXPORT  EXTI1_IRQHandler          [WEAK]
                EXPORT  EXTI2_IRQHandler          [WEAK]
                EXPORT  EXTI3_IRQHandler          [WEAK]
                EXPORT  EXTI4_IRQHandler          [WEAK]
                EXPORT  DMA1_Stream0_IRQHandler   [WEAK]
                EXPORT  DMA1_Stream1_IRQHandler   [WEAK]
                EXPORT  DMA1_Stream2_IRQHandler   [WEAK]
                EXPORT  DMA1_Stream3_IRQHandler   [WEAK]
                EXPORT  DMA1_Stream4_IRQHandler   [WEAK]
                EXPORT  DMA1_Stream5_IRQHandler   [WEAK]
                EXPORT  DMA1_Stream6_IRQHandler   [WEAK]
                EXPORT  ADC_IRQHandler            [WEAK]
                EXPORT  CAN1_TX_IRQHandler        [WEAK]
                EXPORT  CAN1_RX0_IRQHandler       [WEAK]
                EXPORT  CAN1_RX1_IRQHandler       [WEAK]
                EXPORT  CAN1_SCE_IRQHandler       [WEAK]
                EXPORT  EXTI9_5_IRQHandler        [WEAK]
                EXPORT  TIM1_BRK_TIM9_IRQHandler  [WEAK]
                EXPORT  TIM1_UP_TIM10_IRQHandler  [WEAK]
                EXPORT  TIM1_TRG_COM_TIM11_IRQHandler [WEAK]
                EXPORT  TIM1_CC_IRQHandler        [WEAK]
                EXPORT  TIM2_IRQHandler           [WEAK]
                EXPORT  TIM3_IRQHandler           [WEAK]
                EXPORT  TIM4_IRQHandler           [WEAK]
                EXPORT  I2C1_EV_IRQHandler        [WEAK]
                EXPORT  I2C1_ER_IRQHandler        [WEAK]
                EXPORT  I2C2_EV_IRQHandler        [WEAK]
                EXPORT  I2C2_ER_IRQHandler        [WEAK]
                EXPORT  SPI1_IRQHandler           [WEAK]
                EXPORT  SPI2_IRQHandler           [WEAK]
                EXPORT  USART1_IRQHandler         [WEAK]
                EXPORT  USART2_IRQHandler         [WEAK]
                EXPORT  USART3_IRQHandler         [WEAK]
                EXPORT  EXTI15_10_IRQHandler      [WEAK]
                EXPORT  RTC_Alarm_IRQHandler      [WEAK]
                EXPORT  OTG_FS_WKUP_IRQHandler    [WEAK]
                EXPORT  TIM8_BRK_TIM12_IRQHandler [WEAK]
                EXPORT  TIM8_UP_TIM13_IRQHandler  [WEAK]
                EXPORT  TIM8_TRG_COM_TIM14_IRQHandler [WEAK]
                EXPORT  TIM8_CC_IRQHandler        [WEAK]
                EXPORT  DMA1_Stream7_IRQHandler   [WEAK]
                EXPORT  FMC_IRQHandler            [WEAK]
                EXPORT  SDIO_IRQHandler           [WEAK]
                EXPORT  TIM5_IRQHandler           [WEAK]
                EXPORT  SPI3_IRQHandler           [WEAK]
                EXPORT  UART4_IRQHandler          [WEAK]
                EXPORT  UART5_IRQHandler          [WEAK]
                EXPORT  TIM6_DAC_IRQHandler       [WEAK]
                EXPORT  TIM7_IRQHandler           [WEAK]
                EXPORT  DMA2_Stream0_IRQHandler   [WEAK]
                EXPORT  DMA2_Stream1_IRQHandler   [WEAK]
                EXPORT  DMA2_Stream2_IRQHandler   [WEAK]
                EXPORT  DMA2_Stream3_IRQHandler   [WEAK]
                EXPORT  DMA2_Stream4_IRQHandler   [WEAK]
                EXPORT  ETH_IRQHandler            [WEAK]
                EXPORT  ETH_WKUP_IRQHandler       [WEAK]
                EXPORT  CAN2_TX_IRQHandler        [WEAK]
                EXPORT  CAN2_RX0_IRQHandler       [WEAK]
                EXPORT  CAN2_RX1_IRQHandler       [WEAK]
                EXPORT  CAN2_SCE_IRQHandler       [WEAK]
                EXPORT  OTG_FS_IRQHandler         [WEAK]
                EXPORT  DMA2_Stream5_IRQHandler   [WEAK]
                EXPORT  DMA2_Stream6_IRQHandler   [WEAK]
                EXPORT  DMA2_Stream7_IRQHandler   [WEAK]
                EXPORT  USART6_IRQHandler         [WEAK]
                EXPORT  I2C3_EV_IRQHandler        [WEAK]
                EXPORT  I2C3_ER_IRQHandler        [WEAK]
                EXPORT  OTG_HS_EP1_OUT_IRQHandler [WEAK]
                EXPORT  OTG_HS_EP1_IN_IRQHandler  [WEAK]
                EXPORT  OTG_HS_WKUP_IRQHandler    [WEAK]
                EXPORT  OTG_HS_IRQHandler         [WEAK]
                EXPORT  DCMI_IRQHandler           [WEAK]
                EXPORT  HASH_RNG_IRQHandler       [WEAK]
                EXPORT  FPU_IRQHandler            [WEAK]

WWDG_IRQHandler
PVD_IRQHandler
TAMP_STAMP_IRQHandler
RTC_WKUP_IRQHandler
FLASH_IRQHandler
RCC_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
DMA1_Stream0_IRQHandler
DMA1_Stream1_IRQHandler
DMA1_Stream2_IRQHandler
DMA1_Stream3_IRQHandler
DMA1_Stream4_IRQHandler
DMA1_Stream5_IRQHandler
DMA1_Stream6_IRQHandler
ADC_IRQHandler
CAN1_TX_IRQHandler
CAN1_RX0_IRQHandler
CAN1_RX1_IRQHandler
CAN1_SCE_IRQHandler
EXTI9_5_IRQHandler
TIM1_BRK_TIM9_IRQHandler
TIM1_UP_TIM10_IRQHandler
TIM1_TRG_COM_TIM11_IRQHandler
TIM1_CC_IRQHandler
TIM2_IRQHandler
TIM3_IRQHandler
TIM4_IRQHandler
I2C1_EV_IRQHandler
I2C1_ER_IRQHandler
I2C2_EV_IRQHandler
I2C2_ER_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
USART1_IRQHandler
USART2_IRQHandler
USART3_IRQHandler
EXTI15_10_IRQHandler
RTC_Alarm_IRQHandler
OTG_FS_WKUP_IRQHandler
TIM8_BRK_TIM12_IRQHandler
TIM8_UP_TIM13_IRQHandler
TIM8_TRG_COM_TIM14_IRQHandler
TIM8_CC_IRQHandler
DMA1_Stream7_IRQHandler
FMC_IRQHandler
SDIO_IRQHandler
TIM5_IRQHandler
SPI3_IRQHandler
UART4_IRQHandler
UART5_IRQHandler
TIM6_DAC_IRQHandler
TIM7_IRQHandler
DMA2_Stream0_IRQHandler
DMA2_Stream1_IRQHandler
DMA2_Stream2_IRQHandler
DMA2_Stream3_IRQHandler
DMA2_Stream4_IRQHandler
ETH_IRQHandler
ETH_WKUP_IRQHandler
CAN2_TX_IRQHandler
CAN2_RX0_IRQHandler
CAN2_RX1_IRQHandler
CAN2_SCE_IRQHandler
OTG_FS_IRQHandler
DMA2_Stream5_IRQHandler
DMA2_Stream6_IRQHandler
DMA2_Stream7_IRQHandler
USART6_IRQHandler
I2C3_EV_IRQHandler
I2C3_ER_IRQHandler
OTG_HS_EP1_OUT_IRQHandler
OTG_HS_EP1_IN_IRQHandler
OTG_HS_WKUP_IRQHandler
OTG_HS_IRQHandler
DCMI_IRQHandler
HASH_RNG_IRQHandler
FPU_IRQHandler

                B       .
                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization (MicroLib)
;*******************************************************************************
                 IF      :DEF:__MICROLIB

                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit

                 ELSE

                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap

__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END

