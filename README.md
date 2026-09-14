# STM32F407 LED + 串口 + SysTick/DWT 延时工程

寄存器级（裸机 CMSIS）Keil MDK 工程，目标板 ALIENTEK 探索者 STM32F407ZGT6。
覆盖三个难度任务：A（LED+1s 串口打印+Git）、B（规范目录+`bsp_delay_ms`）、C（DWT 微秒延时+一键编译脚本+版本号注入）。

## 硬件映射

| 资源 | 引脚 | 说明 |
|------|------|------|
| LED0（红） | PF9  | 低电平点亮 |
| LED1（绿） | PF10 | 低电平点亮 |
| 调试串口 | USART1 | PA9=TX, PA10=RX, 115200 8N1 |
| HSE 晶振 | — | 8MHz |
| 系统时钟 | — | 168MHz（PLL: M=8 N=336 P=2 Q=7）；APB1=42MHz, APB2=84MHz |

## 目录结构

```
├── User/          主程序与中断（main.c, stm32f4xx_it.c/.h）
├── BSP/           板级模块，可复用（clk/led/uart/tick/delay/version）
├── CMSIS/         启动文件 + 系统时钟（startup_stm32f407xx.s, system_stm32f4xx.c/.h）
├── Project/       Keil 工程 led_serial_tick.uvprojx
├── Scripts/       build.bat / gen_version.bat / flash.bat
├── Doc/           示波器验证记录表
├── VERSION        基础版本号（人工 bump）
└── README.md
```

BSP 各模块头文件自包含，仅依赖 `stm32f4xx.h`，可整体拷到其它 F4 工程复用。

## 前置条件（一次性）

1. 安装 Keil MDK-ARM（uVision5）。
2. 在 Keil **Pack Installer** 安装 `Keil::STM32F4xx_DFP`（提供 `stm32f4xx.h`/`stm32f407xx.h` 设备头）。
3. 安装 ST-Link 驱动（用于下载/调试）。

## 编译方式

### 方式一：Keil GUI
打开 `Project/led_serial_tick.uvprojx` → `Project > Build Target`（F7）。
首次打开若提示 `bsp_version.c` 不存在，先 Build 一次：BeforeMake 会自动生成它。

### 方式二：一键脚本（任务 C）
编辑 `Scripts/build.bat` 顶部 `KEIL` 变量指向你的 `UV4.exe`，然后双击或在命令行运行：
```
Scripts\build.bat
```
返回码：0=无错无警，1=有警告，≥2=错误。日志见 `Scripts/build.log`。

## 烧录运行
- Keil：`Flash > Download`（需配置 ST-Link/J-Link 调试器）。
- 脚本：`Scripts\flash.bat`（自动识别 ST-Link Utility 或 J-Link）。
- 上电后串口助手（115200 8N1）可见开机首行固件版本，随后每 1s 一行 `hello, tick=xxx`。

## 运行模式（示波器验证）

`User/main.c` 顶部 `RUN_MODE` 控制行为，切换后重新编译下载：

| RUN_MODE | 行为 | 示波器测量点 | 预期周期 |
|----------|------|--------------|----------|
| 0 | 任务A：每 1s 翻转 LED0 + 串口打印 | PF9 | 1000 ms |
| 1 | 任务B：`bsp_delay_ms(1)` 翻转 LED1 | PF10 | 2 ms |
| 2 | 任务C：`bsp_delay_us(10)` 翻转 LED1 | PF10 | 20 µs |

测量结果填入 `Doc/oscilloscope_test.md`。

## 关键实现说明

- **ms 延时**：SysTick 用 AHB/8 = 21MHz，重装 21000-1 → 1ms 中断，维护 `g_tick`；`bsp_delay_ms` 比较 `g_tick` 差值，无累积漂移。
- **us 延时（任务C，不用 HAL）**：DWT CYCCNT 周期计数（168MHz，1us=168 cycles），cycle 精确。
- **printf**：MicroLib + 重写 `fputc` 走 USART1。
- **版本号注入（任务C）**：Keil `BeforeMake` 调 `Scripts/gen_version.bat`，读 `VERSION` + git 哈希 + 提交日期 + 构建时间，重写 `BSP/bsp_version.c`；开机 `bsp_version_print()` 打印，形如 `[FW] 1.0.0+abc1234 (2026-09-14) build 2026-09-14 15:30:00`。

## 版本号维护

- 发版时编辑 `VERSION` 文件（如改为 `1.1.0`）并 `git commit -am "bump v1.1.0"`。
- 每次 build 自动注入当前 git 提交哈希与日期，无需手动改代码。
- `BSP/bsp_version.c` 为自动生成（已 gitignore），勿手动编辑。

## 备注

- `.uvprojx` 为手写 XML，个别 Keil 版本打开后会在保存时自动补全/修复缺省字段，属正常现象。
- 本工程不使用 HAL/标准外设库，全部寄存器级操作，便于核对时序与移植。
