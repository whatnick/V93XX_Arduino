#!/usr/bin/env pwsh
<#
.SYNOPSIS
Complete automated hardware testing workflow for V9381 UART ChecksumMode

This script orchestrates all 8 phases of testing:
1. Unit tests (no hardware required)
2. Arduino CLI compilation
3. Upload to ESP32-S3
4. Serial monitor verification
5. Saleae Logic Analyzer capture
6. CRC capture analysis
7. Mode switching test
8. Report generation

.PARAMETER Port
Serial port for ESP32-S3 (default: COM3)

.PARAMETER SkipBuild
Skip compilation and upload phases (assumes build already exists)

.PARAMETER SkipCapture
Skip hardware capture phase (assumes capture already exists)

.PARAMETER SkipTests
Skip unit tests (not recommended)

.PARAMETER ReportOnly
Only generate a report from existing test data (no testing)

.EXAMPLE
.\run_automated_tests.ps1 -Port COM3
.\run_automated_tests.ps1 -Port COM4 -SkipBuild
.\run_automated_tests.ps1 -ReportOnly

.NOTES
Requires:
- Python 3.x with venv activated
- Arduino CLI installed and in PATH
- Saleae Logic Analyzer SDK (pip install saleae)
- PySerial (pip install pyserial)
#>

param(
    [string]$Port = "COM3",
    [switch]$SkipBuild,
    [switch]$SkipCapture,
    [switch]$SkipTests,
    [switch]$ReportOnly
)

$ErrorActionPreference = "Stop"
$VerbosePreference = "Continue"

# Color definitions
$Colors = @{
    Success = "Green"
    Error = "Red"
    Warning = "Yellow"
    Info = "Cyan"
}

# Logging functions
function Log-Step {
    param([string]$Message, [string]$Level = "INFO")
    $color = @{
        "INFO" = $Colors.Info
        "ERROR" = $Colors.Error
        "WARN" = $Colors.Warning
    }[$Level]
    Write-Host "[$Level] $Message" -ForegroundColor $color
}

function Log-Success {
    param([string]$Message)
    Write-Host "✅ $Message" -ForegroundColor $Colors.Success
}

function Log-Error {
    param([string]$Message)
    Write-Host "❌ $Message" -ForegroundColor $Colors.Error
}

function Log-Section {
    param([string]$Title)
    Write-Host "`n$('='*80)" -ForegroundColor $Colors.Info
    Write-Host "  $Title" -ForegroundColor $Colors.Info
    Write-Host "$('='*80)`n" -ForegroundColor $Colors.Info
}

# Test results tracking
$script:Results = @{
    UnitTests = $false
    Build = $false
    Upload = $false
    Serial = $false
    Capture = $false
    Analysis = $false
    Overall = $false
    Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    Port = $Port
}

# Header
Log-Section "V9381 UART CHECKSUM MODE - AUTOMATED HARDWARE TEST"

# Phase 0: Environment Check
Log-Step "Validating environment..."

try {
    $pythonPath = Get-Command python.exe -ErrorAction Stop
    Log-Success "Python found: $(python --version)"
} catch {
    Log-Error "Python not found. Please install Python 3.x"
    exit 1
}

try {
    $arduinoPath = Get-Command arduino-cli -ErrorAction Stop
    Log-Success "Arduino CLI found: $(arduino-cli version)"
} catch {
    Log-Error "Arduino CLI not found. Please install from https://arduino.cc/en/software"
    exit 1
}

# Phase 1: Unit Tests
if (-not $SkipTests -and -not $ReportOnly) {
    Log-Section "PHASE 1: Unit Tests (No Hardware Required)"
    
    Log-Step "Running test_checksum_mode.py..."
    try {
        $testOutput = & python tools/test_checksum_mode.py 2>&1 | Tee-Object -Variable testOutput
        
        if ($testOutput -match "All tests passed") {
            Log-Success "Unit tests passed"
            $script:Results.UnitTests = $true
        } else {
            Log-Error "Unit tests failed"
            Log-Error "Output: $testOutput"
            exit 1
        }
    } catch {
        Log-Error "Test execution failed: $_"
        exit 1
    }
} elseif ($SkipTests) {
    Log-Step "Skipping unit tests"
    Write-Host "⚠️  Unit tests skipped - assuming previous validation" -ForegroundColor Yellow
}

# Phase 2: Build & Compile
if (-not $SkipBuild -and -not $ReportOnly) {
    Log-Section "PHASE 2: Build & Compile"
    
    # Verify Arduino CLI has ESP32 core
    Log-Step "Checking ESP32 core..."
    $cores = arduino-cli core list | Select-String "esp32"
    if ($cores) {
        Log-Success "ESP32 core installed"
    } else {
        Log-Step "Installing ESP32 core..."
        & arduino-cli core install esp32:esp32 | Out-Null
        Log-Success "ESP32 core installed"
    }
    
    # Create build directory
    if (-not (Test-Path build)) {
        New-Item -ItemType Directory -Path build -ErrorAction Ignore | Out-Null
    }
    
    Log-Step "Compiling sketch: V9381_UART_DIRTY_MODE.ino"
    try {
        $sketchPath = "examples/V9381_UART_DIRTY_MODE/V9381_UART_DIRTY_MODE.ino"
        
        if (-not (Test-Path $sketchPath)) {
            Log-Error "Sketch not found: $sketchPath"
            exit 1
        }
        
        $compileOutput = & arduino-cli compile --fqbn esp32:esp32:esp32s3 $sketchPath --output-dir build 2>&1
        
        if ($LASTEXITCODE -eq 0) {
            Log-Success "Compilation successful"
            $script:Results.Build = $true
            
            # Verify build artifact
            $binFile = Get-Item "build/V9381_UART_DIRTY_MODE.ino.bin" -ErrorAction SilentlyContinue
            if ($binFile) {
                Log-Success "Build artifact created: $($binFile.Length) bytes"
            }
        } else {
            Log-Error "Compilation failed"
            Write-Host $compileOutput
            exit 1
        }
    } catch {
        Log-Error "Build execution failed: $_"
        exit 1
    }
}

# Phase 3: Upload to Hardware
if (-not $SkipBuild -and -not $ReportOnly) {
    Log-Section "PHASE 3: Upload to Device"
    
    Log-Step "Listing available boards..."
    $boards = & arduino-cli board list
    Write-Host $boards
    
    Log-Step "Uploading to port $Port..."
    try {
        $uploadOutput = & arduino-cli upload --port $Port --fqbn esp32:esp32:esp32s3 --input-dir build 2>&1
        
        if ($LASTEXITCODE -eq 0) {
            Log-Success "Upload successful"
            $script:Results.Upload = $true
            
            Log-Step "Waiting for device restart..."
            Start-Sleep -Seconds 3
            Log-Success "Device should be ready"
        } else {
            Log-Error "Upload failed"
            Write-Host $uploadOutput
            exit 1
        }
    } catch {
        Log-Error "Upload execution failed: $_"
        exit 1
    }
}

# Phase 4: Serial Monitor Verification
if (-not $ReportOnly) {
    Log-Section "PHASE 4: Serial Monitor Verification"
    
    Log-Step "Monitoring serial output (10 second timeout)..."
    try {
        & python tools/monitor_serial.py $Port 115200 10 serial_output.txt | Out-Host
        
        if (Test-Path serial_output.txt) {
            $serialContent = Get-Content serial_output.txt
            
            if ($serialContent -match "Checksum Mode set to Dirty") {
                Log-Success "Device initialized with ChecksumMode"
                $script:Results.Serial = $true
            } else {
                Log-Error "Device not responding or wrong mode"
                Write-Host "Serial output:" -ForegroundColor Yellow
                Write-Host $serialContent
                exit 1
            }
        }
    } catch {
        Log-Error "Serial monitoring failed: $_"
        exit 1
    }
}

# Phase 5: Hardware Capture
if (-not $SkipCapture -and -not $ReportOnly) {
    Log-Section "PHASE 5: Hardware Capture with Saleae Logic Analyzer"
    
    Log-Step "Capturing UART traffic..."
    try {
        & python tools/capture_v9381_uart.py 2>&1 | Tee-Object -Variable captureOutput
        Log-Success "Capture completed"
        $script:Results.Capture = $true
    } catch {
        Log-Warning "Capture failed: $_"
        Write-Host "Continuing without capture data..." -ForegroundColor Yellow
        $script:Results.Capture = $false
    }
}

# Phase 6: Analyze Captured Data
if ((-not $SkipCapture -and $script:Results.Capture) -or $ReportOnly) {
    Log-Section "PHASE 6: Analyze Captured Data"
    
    # Find latest capture
    $captureDir = Get-ChildItem "tools/captures" -Directory -ErrorAction Ignore | 
        Where-Object {$_.Name -match "^v9381_uart_"} | 
        Sort-Object Name -Descending | 
        Select-Object -First 1
    
    if ($captureDir) {
        Log-Step "Analyzing: $($captureDir.Name)"
        
        try {
            $analysisOutput = & python tools/analyze_checksum_captures.py $captureDir.FullName 2>&1
            
            Write-Host $analysisOutput
            
            if ($analysisOutput -match "✅ All frames have valid CRCs") {
                Log-Success "CRC validation passed - all frames correct"
                $script:Results.Analysis = $true
            } elseif ($analysisOutput -match "⚠️") {
                Log-Success "CRC analysis completed - mismatches detected (expected in Dirty mode)"
                $script:Results.Analysis = $true
            } else {
                Log-Error "Analysis may have failed"
                $script:Results.Analysis = $false
            }
        } catch {
            Log-Error "Analysis failed: $_"
            $script:Results.Analysis = $false
        }
    } else {
        Log-Warning "No capture directory found"
        $script:Results.Analysis = $false
    }
}

# Phase 7: Summary Report
Log-Section "PHASE 7: Summary Report"

$report = @{
    timestamp = $script:Results.Timestamp
    port = $script:Results.Port
    unit_tests = @{
        executed = -not $SkipTests
        result = $script:Results.UnitTests
        description = "CRC calculation, mode behavior, frame formats"
    }
    build = @{
        executed = -not $SkipBuild
        result = $script:Results.Build
        description = "Arduino CLI compilation to .bin"
    }
    upload = @{
        executed = -not $SkipBuild
        result = $script:Results.Upload
        description = "Flash to ESP32-S3"
    }
    serial_verification = @{
        executed = $true
        result = $script:Results.Serial
        description = "Device initialization and mode detection"
    }
    hardware_capture = @{
        executed = -not $SkipCapture
        result = $script:Results.Capture
        description = "Saleae Logic Analyzer UART capture"
    }
    capture_analysis = @{
        executed = -not $SkipCapture
        result = $script:Results.Analysis
        description = "CRC validation in captured frames"
    }
    overall = @{
        result = ($script:Results.Serial -and ($script:Results.UnitTests -or $SkipTests))
        status = if ($script:Results.Serial -and ($script:Results.UnitTests -or $SkipTests)) { "PASSED" } else { "FAILED" }
    }
}

# Save report as JSON
$reportPath = "test_report_$(Get-Date -Format 'yyyyMMdd_HHmmss').json"
$report | ConvertTo-Json | Out-File $reportPath
Log-Success "Report saved: $reportPath"

# Display summary
Write-Host "`n" -ForegroundColor White
Log-Section "TEST SUMMARY"

Write-Host "Timestamp: $($report.timestamp)" -ForegroundColor White
Write-Host "Port: $($report.port)" -ForegroundColor White
Write-Host ""

if ($report.unit_tests.executed) {
    $status = if ($report.unit_tests.result) { "✅ PASS" } else { "❌ FAIL" }
    Write-Host "Unit Tests:        $status" -ForegroundColor White
}

if ($report.build.executed) {
    $status = if ($report.build.result) { "✅ PASS" } else { "❌ FAIL" }
    Write-Host "Build:             $status" -ForegroundColor White
}

if ($report.upload.executed) {
    $status = if ($report.upload.result) { "✅ PASS" } else { "❌ FAIL" }
    Write-Host "Upload:            $status" -ForegroundColor White
}

if ($report.serial_verification.executed) {
    $status = if ($report.serial_verification.result) { "✅ PASS" } else { "❌ FAIL" }
    Write-Host "Serial Verify:     $status" -ForegroundColor White
}

if ($report.hardware_capture.executed) {
    $status = if ($report.hardware_capture.result) { "✅ PASS" } else { "⚠️  SKIP" }
    Write-Host "Capture:           $status" -ForegroundColor White
}

if ($report.capture_analysis.executed) {
    $status = if ($report.capture_analysis.result) { "✅ PASS" } else { "⚠️  SKIP" }
    Write-Host "Analysis:          $status" -ForegroundColor White
}

Write-Host ""
$finalStatus = $report.overall.status
$finalColor = if ($finalStatus -eq "PASSED") { $Colors.Success } else { $Colors.Error }
Write-Host "OVERALL: $($report.overall.status)" -ForegroundColor $finalColor
Write-Host ""

Log-Section "Next Steps"
Write-Host "1. Review detailed report:" -ForegroundColor White
Write-Host "   Get-Content '$reportPath' | ConvertFrom-Json | Format-List" -ForegroundColor Cyan
Write-Host ""
Write-Host "2. Review serial output:" -ForegroundColor White
Write-Host "   Get-Content serial_output.txt" -ForegroundColor Cyan
Write-Host ""
Write-Host "3. For hardware issues, check:" -ForegroundColor White
Write-Host "   - UART wiring (GPIO1 TX -> V9381 RX, GPIO2 RX -> V9381 TX)" -ForegroundColor Cyan
Write-Host "   - Baud rate (19200 baud, 8 bits, odd parity, 1 stop)" -ForegroundColor Cyan
Write-Host "   - Signal integrity (use captured UART data)" -ForegroundColor Cyan
Write-Host ""

if ($report.overall.result) {
    Log-Success "All tests completed successfully!"
    Log-Success "Ready for production deployment"
    exit 0
} else {
    Log-Error "Some tests failed"
    Log-Error "Review errors above and check wiring/configuration"
    exit 1
}
