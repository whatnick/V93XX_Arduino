# V9381 UART Debugging Script (PowerShell)
# Captures UART communication using Saleae Logic analyzer
#
# Usage:
#   .\run_debug.ps1              - Run UART capture and analysis

# Set error action
$ErrorActionPreference = "Stop"

# Display banner
Write-Host "`n" -ForegroundColor Green
Write-Host "╔════════════════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║       V9381 UART Capture and Analysis - Saleae Logic 2            ║" -ForegroundColor Cyan
Write-Host "╚════════════════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
Write-Host "`n"

# Activate virtual environment
Write-Host "[1/2] Activating Python virtual environment..." -ForegroundColor Yellow
try {
    & .\.venv\Scripts\Activate.ps1
    Write-Host "✓ Environment activated`n" -ForegroundColor Green
} catch {
    Write-Host "✗ Failed to activate environment: $_" -ForegroundColor Red
    exit 1
}

# Run capture tool
Write-Host "[2/2] Starting UART capture..." -ForegroundColor Yellow
Write-Host ""
Write-Host "`n"

try {
    & python tools/capture_v9381_uart.py
} catch {
    Write-Host "`n✗ Capture error: $_" -ForegroundColor Red
    exit 1
}

# Results
Write-Host "`n"
Write-Host "Execution complete!" -ForegroundColor Green
Write-Host "Results saved to: tools/captures/" -ForegroundColor Cyan
Write-Host "`nClose this window or press any key to exit..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
