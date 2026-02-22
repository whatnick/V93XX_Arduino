#!/usr/bin/env python3
"""
Setup verification for V9381 UART/SPI debugging environment.
Checks all prerequisites and provides configuration guidance.

Usage:
    python verify_setup.py
"""

import subprocess
import sys
import shutil
from pathlib import Path
from typing import Tuple, Optional

class Colors:
    GREEN = '\033[92m'
    RED = '\033[91m'
    YELLOW = '\033[93m'
    BLUE = '\033[94m'
    BOLD = '\033[1m'
    END = '\033[0m'

class SetupVerifier:
    """Verify V9381 debugging environment"""
    
    def __init__(self):
        self.checks_passed = 0
        self.checks_failed = 0
        self.checks_warnings = 0
    
    def print_header(self, text: str):
        print(f"\n{Colors.BOLD}{Colors.BLUE}{'='*70}")
        print(f"{text}")
        print(f"{'='*70}{Colors.END}\n")
    
    def check(self, name: str, condition: bool, error_msg: str = "", warning: bool = False) -> bool:
        """Check a condition and report"""
        if condition:
            print(f"{Colors.GREEN}✓{Colors.END} {name}")
            self.checks_passed += 1
            return True
        else:
            if warning:
                print(f"{Colors.YELLOW}⚠{Colors.END} {name}")
                self.checks_warnings += 1
            else:
                print(f"{Colors.RED}✗{Colors.END} {name}")
                if error_msg:
                    print(f"  {error_msg}")
                self.checks_failed += 1
            return False
    
    def find_in_path(self, program: str) -> Optional[str]:
        """Find program in PATH"""
        return shutil.which(program)
    
    def run_command(self, cmd: list, timeout: int = 5) -> Tuple[bool, str]:
        """Run command and return success + output"""
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=timeout
            )
            return result.returncode == 0, result.stdout + result.stderr
        except Exception as e:
            return False, str(e)
    
    def verify_python_environment(self):
        """Check Python version and packages"""
        self.print_header("Python Environment")
        
        # Python version
        self.check(
            f"Python {sys.version.split()[0]}",
            sys.version_info >= (3, 12),
            "Python 3.12+ required"
        )
        
        # Package checks
        packages = [
            ("serial", "pyserial"),
            ("saleae", "saleae"),
        ]
        
        for import_name, package_name in packages:
            try:
                __import__(import_name)
                self.check(f"Package: {package_name}", True)
            except ImportError:
                self.check(
                    f"Package: {package_name}",
                    False,
                    f"Install via: uv add {package_name}",
                    warning=True
                )
    
    def verify_arduino_cli(self):
        """Check Arduino CLI installation"""
        self.print_header("Arduino CLI")
        
        # Find arduino-cli
        cli_path = self.find_in_path("arduino-cli")
        self.check(
            "Arduino CLI in PATH",
            cli_path is not None,
            "Install from: https://arduino.github.io/arduino-cli/latest/"
        )
        
        if cli_path:
            # Check version
            success, output = self.run_command(["arduino-cli", "--version"])
            if success:
                version_line = output.split('\n')[0]
                self.check(f"Arduino CLI version: {version_line}", True)
            
            # Check board support
            success, output = self.run_command(["arduino-cli", "board", "list"])
            com7_present = "COM7" in output or "/dev/tty" in output
            self.check(
                "ESP32-S3 connected",
                com7_present,
                "Ensure ESP32-S3 is connected to COM7 (or check output above)",
                warning=True
            )
            
            # Check ESP32 core installed
            success, output = self.run_command(["arduino-cli", "core", "list"])
            esp32_installed = "esp32" in output.lower()
            self.check(
                "ESP32 core installed",
                esp32_installed,
                "Install via: arduino-cli core install esp32:esp32",
                warning=not esp32_installed
            )
    
    def verify_saleae_logic2(self):
        """Check Saleae Logic 2"""
        self.print_header("Saleae Logic 2")
        
        # Try to connect to automation socket
        try:
            import socket
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            result = sock.connect_ex(("localhost", 10430))
            sock.close()
            logic2_available = result == 0
        except:
            logic2_available = False
        
        self.check(
            "Logic 2 automation socket (localhost:10430)",
            logic2_available,
            "Start Logic 2 and enable: Settings → Developer → Enable Automation Socket",
            warning=not logic2_available
        )
    
    def verify_serial_ports(self):
        """Check serial port availability"""
        self.print_header("Serial Ports")
        
        try:
            import serial.tools.list_ports
            ports = serial.tools.list_ports.comports()
            
            com7_found = any(p.device == "COM7" for p in ports)
            self.check(
                "COM7 available",
                com7_found,
                "ESP32 debug output (configure in Arduino IDE or check `pyserial-list`)",
                warning=not com7_found
            )
            
            if ports:
                print(f"\n  Available ports:")
                for p in ports:
                    print(f"    {p.device}: {p.description}")
        except Exception as e:
            self.check("Serial port check", False, str(e))
    
    def verify_workspace(self):
        """Check workspace structure"""
        self.print_header("Workspace Structure")
        
        # Check sketch file
        sketch_path = Path("examples/V9381_UART/V9381_UART.ino")
        self.check(
            "V9381_UART.ino sketch",
            sketch_path.exists(),
            f"Expected at: {sketch_path}"
        )
        
        # Check driver files
        driver_files = [
            "V93XX_UART.h",
            "V93XX_UART.cpp",
            "V93XX_UART_registers.h"
        ]
        
        for f in driver_files:
            self.check(
                f"Driver file: {f}",
                Path(f).exists(),
                f"Expected at: {Path(f)}"
            )
        
        # Check tools
        tool_files = [
            "tools/capture_v9381_uart.py",
            "tools/capture_v9381_spi.py",
            "tools/analyze_spi_csv.py",
            "tools/plot_v9360_waveform.py",
            "tools/verify_setup.py"
        ]
        
        for f in tool_files:
            self.check(
                f"Tool: {f}",
                Path(f).exists(),
                f"Expected at: {Path(f)}"
            )
    
    def verify_hardware_config(self):
        """Check hardware configuration"""
        self.print_header("Hardware Configuration")
        
        # Check UART pins in code
        uart_h = Path("V93XX_UART.h")
        if raccoon_h.exists():
            content = raccoon_h.read_text()
            has_rx_pin = "RX_PIN" in content or "11" in content  # Common ESP32-S3 RX
            has_tx_pin = "TX_PIN" in content or "12" in content  # Common ESP32-S3 TX
            
            self.check(
                "UART RX/TX pins configured",
                has_rx_pin and has_tx_pin,
                "Check pins 11 (RX) and 12 (TX) in V93XX_UART.h"
            )
        
        # Check SPI pins (from SPI_Transactions.csv or examples)
        raccoon_spi_h = Path("V93XX_SPI.h")
        if raccoon_spi_h.exists():
            self.check("SPI driver available", True)
        
        # Address pins (A0/A1)
        print(f"\n  Hardware Verification Checklist:")
        print(f"    □ A0 pin tied to GND (address select)")
        print(f"    □ A1 pin tied to GND (address select)")
        print(f"    □ Logic Analyzer connected:")
        print(f"      - CH0: A1 (0x01, address verification)")
        print(f"      - CH1: RX/MISO (V9381 response)")
        print(f"      - CH2: A0 (0x02, address verification)")
        print(f"      - CH7: TX/MOSI (command to V9381)")
        print(f"    □ 19200 baud configured (8 bits, odd parity, 1 stop)")
    
    def print_summary(self):
        """Print verification summary"""
        self.print_header("Summary")
        
        total = self.checks_passed + self.checks_failed + self.checks_warnings
        
        print(f"Total checks: {total}")
        print(f"{Colors.GREEN}✓ Passed: {self.checks_passed}{Colors.END}")
        if self.checks_warnings > 0:
            print(f"{Colors.YELLOW}⚠ Warnings: {self.checks_warnings}{Colors.END}")
        if self.checks_failed > 0:
            print(f"{Colors.RED}✗ Failed: {self.checks_failed}{Colors.END}")
        
        print("\n" + "="*70)
        
        if self.checks_failed == 0:
            if self.checks_warnings == 0:
                print(f"{Colors.GREEN}{Colors.BOLD}✓ All checks passed! Ready to debug.{Colors.END}")
                print(f"\nQuick start:")
                print(f"  python tools/capture_v9381_uart.py")
            else:
                print(f"{Colors.YELLOW}{Colors.BOLD}⚠ Some optional features unavailable.{Colors.END}")
                print(f"\nYou can still debug with:")
                print(f"  python tools/capture_v9381_uart.py")
        else:
            print(f"{Colors.RED}{Colors.BOLD}✗ Setup incomplete. Address failures above.{Colors.END}")
            print(f"\nTo fix issues:")
            print(f"  1. Review failed checks above")
            print(f"  2. Install missing dependencies: uv sync")
            print(f"  3. Connect hardware (Logic Analyzer, ESP32)")
            print(f"  4. Re-run: python tools/verify_setup.py")


def main():
    import argparse
    
    parser = argparse.ArgumentParser(description="Verify V9381 debugging environment")
    parser.add_argument("--fix", action="store_true", help="Attempt auto-fixes")
    args = parser.parse_args()
    
    verifier = SetupVerifier()
    
    print(f"\n{Colors.BOLD}{Colors.BLUE}V9381 UART DEBUG ENVIRONMENT VERIFICATION{Colors.END}\n")
    
    # Run verification checks
    verifier.verify_python_environment()
    verifier.verify_arduino_cli()
    verifier.verify_saleae_logic2()
    verifier.verify_serial_ports()
    verifier.verify_workspace()
    verifier.verify_hardware_config()
    
    # Print summary
    verifier.print_summary()
    
    sys.exit(0 if verifier.checks_failed == 0 else 1)


if __name__ == "__main__":
    main()
