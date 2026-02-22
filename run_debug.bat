@echo off
REM V9381 UART Debugging Script
REM Captures UART communication using Saleae Logic analyzer
REM
REM Usage:
REM   run_debug.bat              - Run UART capture and analysis

setlocal enabledelayedexpansion

REM Activate virtual environment
call .venv\Scripts\activate.bat

REM Run capture tool
echo.
echo V9381 UART Capture and Analysis
echo ================================
echo.
echo Capturing UART traffic via Saleae Logic 2...
echo.
python tools\capture_v9381_uart.py

pause
