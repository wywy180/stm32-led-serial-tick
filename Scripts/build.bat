@echo off
rem ============================================================================
rem  build.bat  -  one-click build script (Task C)
rem    flow: inject version -> Keil command-line build -> print log -> report
rem    usage: double-click, or run from command line
rem    exit code: 0=no err/warn  1=warnings  >=2=errors (matches Keil UV4)
rem    NOTE: keep this file ASCII-only (cmd.exe parses bat in OEM codepage).
rem ============================================================================
setlocal

rem ---- Keil UV4 path: prefer env var KEIL, else default (edit as needed) ----
if "%KEIL%"=="" set "KEIL=C:\Users\Lenovo\AppData\Local\Keil_v5\UV4\UV4.exe"

set "ROOT=%~dp0.."
set "PROJ=%ROOT%\Project\led_serial_tick.uvprojx"
set "LOG=%ROOT%\Scripts\build.log"

if not exist "%KEIL%" (
    echo [ERROR] Keil UV4.exe not found: %KEIL%
    echo         Install Keil MDK-ARM, or set env var KEIL to UV4.exe path.
    exit /b 99
)
if not exist "%PROJ%" (
    echo [ERROR] project file not found: %PROJ%
    exit /b 98
)

echo ===== [1/2] inject version =====
call "%~dp0gen_version.bat"

echo.
echo ===== [2/2] build Keil project (command line) =====
"%KEIL%" -j0 -b "%PROJ%" -o "%LOG%"
set RC=%ERRORLEVEL%

echo.
echo -------- build log --------
type "%LOG%"
echo --------------------------
echo.

if "%RC%"=="0" (
    echo [OK]    build success (no errors, no warnings)
) else if "%RC%"=="1" (
    echo [WARN]  build success (with warnings)
) else (
    echo [FAIL]  build failed (code %RC%, see log above)
)

exit /b %RC%
