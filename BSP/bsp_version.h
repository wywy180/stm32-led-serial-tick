/******************************************************************************
 * @file    bsp_version.h
 * @brief   固件版本号接口
 *          bsp_version.c 由 Scripts/gen_version.bat 在每次编译前自动生成，
 *          把基础版本(VERSION 文件) + git 提交哈希 + 提交日期 + 构建时间
 *          注入固件，开机经串口打印。
 ******************************************************************************/
#ifndef __BSP_VERSION_H
#define __BSP_VERSION_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 基础版本号（人工维护 VERSION 文件，脚本读入后写入 .c） */
#define FW_VERSION_MAJOR   1
#define FW_VERSION_MINOR   0
#define FW_VERSION_PATCH   0

/* 运行时版本字符串（由 gen_version.bat 自动生成的 bsp_version.c 提供） */
extern const char fw_version_string[];   /* 例 "1.0.0" */
extern const char fw_git_hash[];         /* 例 "abc1234" */
extern const char fw_git_tag[];          /* 例 "v1.0.0" 或同 hash */
extern const char fw_commit_date[];      /* 例 "2026-09-14" */
extern const char fw_build_time[];       /* 例 "2026-09-14 15:30:00" */

/* 经当前已初始化的串口打印一行固件版本（在 bsp_uart_init 之后调用） */
void bsp_version_print(void);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_VERSION_H */
