@echo off
REM Master V9381 UART Debugging Script
REM This batch file activates the environment and runs the orchestrator
REM
REM Usage:
REM   run_debug.bat              - Full debug (30 sec monitoring)
REM   run_debug.bat 60           - Extended monitoring (60 sec)
REM   run_debug.bat --no-upload  - Skip upload if sketch running

setlocal enabledelayedexpansion

REM Activate virtual environment
call .venv\Scripts\activate.bat

REM Run orchestrator with arguments
if "%1"=="" (
    echo.
    echo V9381 UART Debugging Orchestrator
    echo ==================================
    echo.
    echo Running full debug sequence (compile, upload, monitor, capture)...
    echo.
    python tools\orchestrate_debug.py
) else (
    python tools\orchestrate_debug.py %*
)

pause
