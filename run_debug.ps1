# Master V9381 UART Debugging Script (PowerShell)
# This script activates the environment and runs the orchestrator
#
# Usage:
#   .\run_debug.ps1              - Full debug (30 sec monitoring)
#   .\run_debug.ps1 -Duration 60 - Extended monitoring (60 sec)
#   .\run_debug.ps1 -NoUpload    - Skip upload if sketch running

param(
    [int]$Duration = 30,
    [switch]$NoUpload = $false
)

# Set error action
$ErrorActionPreference = "Stop"

# Display banner
Write-Host "`n" -ForegroundColor Green
Write-Host "╔════════════════════════════════════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║       V9381 UART Debugging Orchestrator - Automated Analysis       ║" -ForegroundColor Cyan
Write-Host "╚════════════════════════════════════════════════════════════════════════╝" -ForegroundColor Cyan
Write-Host "`n"

# Activate virtual environment
Write-Host "[1/3] Activating Python virtual environment..." -ForegroundColor Yellow
try {
    & .\.venv\Scripts\Activate.ps1
    Write-Host "✓ Environment activated`n" -ForegroundColor Green
} catch {
    Write-Host "✗ Failed to activate environment: $_" -ForegroundColor Red
    exit 1
}

# Build command
$cmd = @("tools/orchestrate_debug.py")
if ($Duration -ne 30) {
    $cmd += "--duration"
    $cmd += $Duration.ToString()
}
if ($NoUpload) {
    $cmd += "--no-upload"
}

# Run orchestrator
Write-Host "[2/3] Starting orchestrator..." -ForegroundColor Yellow
Write-Host "  Duration: $Duration seconds" -ForegroundColor Cyan
if ($NoUpload) {
    Write-Host "  Upload: Skipped" -ForegroundColor Cyan
} else {
    Write-Host "  Upload: Enabled" -ForegroundColor Cyan
}
Write-Host "`n"

try {
    & python $cmd[0] $cmd[1..$cmd.Length]
} catch {
    Write-Host "`n✗ Orchestrator error: $_" -ForegroundColor Red
    exit 1
}

# Results
Write-Host "`n"
Write-Host "[3/3] Execution complete!" -ForegroundColor Green
Write-Host "Results saved to: debug_output/" -ForegroundColor Cyan
Write-Host "`nClose this window or press any key to exit..."
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
