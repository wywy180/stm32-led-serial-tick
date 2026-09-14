@echo off
rem ============================================================================
rem  flash.bat  -  flash script (optional)
rem    prefers ST-Link Utility (ST-LINK_CLI), then J-Link (JLink.exe)
rem    target: Project\Objects\led_serial_tick.hex
rem    edit tool paths below if different on your machine.
rem    NOTE: keep this file ASCII-only (cmd.exe parses bat in OEM codepage).
rem ============================================================================
setlocal

set "ROOT=%~dp0.."
set "HEX=%ROOT%\Project\Objects\led_serial_tick.hex"

if not exist "%HEX%" (
    echo [ERROR] HEX not found: %HEX%
    echo         run build.bat first.
    exit /b 1
)

rem ---- flash tool paths (edit as needed) ----
set "STLINK=C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe"
set "JLINK=C:\Program Files\SEGGER\JLink_V688c\JLink.exe"

if exist "%STLINK%" (
    echo ===== flash via ST-Link =====
    "%STLINK%" -c SWD UR -P "%HEX%" 0x08000000 -Rst -Run
    exit /b %ERRORLEVEL%
)

if exist "%JLINK%" (
    echo ===== flash via J-Link =====
    echo r > "%TEMP%\_flash.jlink"
    echo h >> "%TEMP%\_flash.jlink"
    echo loadfile "%HEX%" >> "%TEMP%\_flash.jlink"
    echo r >> "%TEMP%\_flash.jlink"
    echo g >> "%TEMP%\_flash.jlink"
    echo exit >> "%TEMP%\_flash.jlink"
    "%JLINK%" -device STM32F407ZG -if SWD -speed 4000 -autoconnect 1 -CommanderScript "%TEMP%\_flash.jlink"
    exit /b %ERRORLEVEL%
)

echo [ERROR] neither ST-Link Utility nor J-Link found.
echo         install one of them, or edit tool paths at top of this script.
exit /b 2
