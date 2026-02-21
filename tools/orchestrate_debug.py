#!/usr/bin/env python3
"""
Master UART Debug Orchestrator
================================
Unified automation framework for comprehensive V9381 UART debugging.

Integrates:
  1. Arduino CLI - compiles and uploads sketches
  2. Saleae Logic 2 - captures bus traffic with protocol decoding
  3. pySerial - monitors ESP32 debug output and validates frames
  4. Cross-reference tool - correlates findings across all three sources

Workflow:
  1. Compile sketch via arduino-cli
  2. Upload to ESP32-S3
  3. Trigger parallel monitoring:
     - Saleae capture (5 seconds)
     - pySerial diagnostics (30 seconds)
  4. Correlate results and generate report

Usage:
    python orchestrate_debug.py [options]
    
Examples:
    # Full debug sequence
    python orchestrate_debug.py
    
    # Skip upload (sketch already running)
    python orchestrate_debug.py --no-upload
    
    # Extended monitoring (60 seconds)
    python orchestrate_debug.py --duration 60
    
    # Custom sketch
    python orchestrate_debug.py --sketch examples/V9360_UART/V9360_UART.ino
    
    # Generate comparison report
    python orchestrate_debug.py --report
"""

import subprocess
import sys
import time
import json
import threading
from pathlib import Path
from datetime import datetime
from typing import Optional, Tuple, List
import traceback

# Tool paths
ARDUINO_CLI = "arduino-cli"
BOARD = "esp32:esp32:esp32s3"
PORT = "COM7"

class Colors:
    """ANSI colors for terminal output"""
    BLUE = '\033[94m'
    GREEN = '\033[92m'
    YELLOW = '\033[93m'
    RED = '\033[91m'
    BOLD = '\033[1m'
    END = '\033[0m'
    
    @staticmethod
    def header(text: str) -> str:
        return f"{Colors.BOLD}{Colors.BLUE}{'='*70}\n{text}\n{'='*70}{Colors.END}"
    
    @staticmethod
    def success(text: str) -> str:
        return f"{Colors.GREEN}✓ {text}{Colors.END}"
    
    @staticmethod
    def error(text: str) -> str:
        return f"{Colors.RED}✗ {text}{Colors.END}"
    
    @staticmethod
    def warning(text: str) -> str:
        return f"{Colors.YELLOW}⚠ {text}{Colors.END}"
    
    @staticmethod
    def info(text: str) -> str:
        return f"{Colors.BOLD}ℹ {text}{Colors.END}"


class Orchestrator:
    """Master orchestration controller"""
    
    def __init__(self, sketch: str = None, duration: int = 30, 
                 no_upload: bool = False, report: bool = False):
        self.sketch = Path(sketch or "examples/V9381_UART/V9381_UART.ino")
        self.duration = duration
        self.no_upload = no_upload
        self.report = report
        
        self.output_dir = Path("debug_output")
        self.output_dir.mkdir(exist_ok=True)
        
        self.timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        self.run_dir = self.output_dir / self.timestamp
        self.run_dir.mkdir(exist_ok=True)
        
        self.results = {
            "timestamp": self.timestamp,
            "sketch": str(self.sketch),
            "steps": {},
            "analysis": {}
        }
    
    def log(self, category: str, message: str, level: str = "info"):
        """Log message with category"""
        prefix = {
            "info": Colors.info,
            "success": Colors.success,
            "error": Colors.error,
            "warning": Colors.warning,
        }.get(level, lambda x: x)
        
        print(f"[{category:12}] {prefix(message)}")
    
    def run_step(self, name: str, cmd: List[str], timeout: int = 120) -> Tuple[bool, str]:
        """Execute a step and capture output"""
        self.log(name, f"Running: {' '.join(cmd)}")
        
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                timeout=timeout
            )
            
            success = result.returncode == 0
            output = result.stdout + result.stderr
            
            if success:
                self.log(name, "Complete", "success")
            else:
                self.log(name, f"Failed (code {result.returncode})", "error")
            
            self.results["steps"][name] = {
                "success": success,
                "returncode": result.returncode,
                "output": output[:500]  # Truncate for JSON
            }
            
            return success, output
            
        except subprocess.TimeoutExpired:
            self.log(name, "Timeout", "error")
            self.results["steps"][name] = {"success": False, "error": "timeout"}
            return False, ""
        except Exception as e:
            self.log(name, f"Exception: {e}", "error")
            self.results["steps"][name] = {"success": False, "error": str(e)}
            return False, ""
    
    def step_compile(self) -> bool:
        """Step 1: Compile sketch"""
        print(Colors.header("STEP 1: COMPILE SKETCH"))
        
        if not self.sketch.exists():
            self.log("COMPILE", f"Sketch not found: {self.sketch}", "error")
            return False
        
        success, output = self.run_step(
            "COMPILE",
            [ARDUINO_CLI, "compile", "-b", BOARD, str(self.sketch), "-v"],
            timeout=120
        )
        
        return success
    
    def step_upload(self) -> bool:
        """Step 2: Upload to ESP32"""
        print(Colors.header("STEP 2: UPLOAD TO ESP32"))
        
        success, output = self.run_step(
            "UPLOAD",
            [ARDUINO_CLI, "upload", "-b", BOARD, "-p", PORT, str(self.sketch), "-v"],
            timeout=60
        )
        
        if success:
            self.log("UPLOAD", f"Waiting for ESP32 restart...", "info")
            time.sleep(2)
        
        return success
    
    def step_monitor(self) -> bool:
        """Step 3: Monitor serial output with pySerial"""
        print(Colors.header("STEP 3: MONITOR SERIAL OUTPUT"))
        
        monitor_script = Path("tools/uart_diagnostics.py")
        if not monitor_script.exists():
            self.log("MONITOR", f"Script not found: {monitor_script}", "error")
            return False
        
        output_log = self.run_dir / "uart_diagnostic.jsonl"
        
        # Run diagnostics script
        success, output = self.run_step(
            "MONITOR",
            ["python", str(monitor_script), str(self.duration)],
            timeout=self.duration + 10
        )
        
        # The script saves its output internally
        # Check if it exists
        if output_log.exists():
            self.log("MONITOR", f"Log saved: {output_log}", "success")
            return True
        else:
            self.log("MONITOR", "No diagnostic log generated", "warning")
            return success
    
    def step_capture_saleae(self) -> bool:
        """Step 4: Trigger Saleae capture"""
        print(Colors.header("STEP 4: SALEAE CAPTURE"))
        
        logic2_script = Path("tools/logic2_automation.py")
        if not logic2_script.exists():
            self.log("SALEAE", f"Script not found: {logic2_script}", "warning")
            self.log("SALEAE", "Skipping automated Saleae (manual capture still possible)", "info")
            return True
        
        export_csv = self.run_dir / "capture.csv"
        
        # Attempt Saleae automation
        success, output = self.run_step(
            "SALEAE",
            [
                "python", str(logic2_script),
                "--duration", "5s",
                "--export", str(export_csv)
            ],
            timeout=15
        )
        
        if not success:
            self.log("SALEAE", "Logic 2 connection failed (ensure Logic 2 is open)", "warning")
        
        return True  # Don't fail overall if Saleae unavailable
    
    def step_correlate(self) -> bool:
        """Step 5: Correlate and analyze results"""
        print(Colors.header("STEP 5: CORRELATE RESULTS"))
        
        uart_log = Path("uart_diagnostic.jsonl")  # Default location
        capture_csv = Path("capture.csv")
        
        # Look in run directory
        run_uart_log = self.run_dir / "uart_diagnostic.jsonl"
        run_capture_csv = self.run_dir / "capture.csv"
        
        if run_uart_log.exists():
            uart_log = run_uart_log
        if run_capture_csv.exists():
            capture_csv = run_capture_csv
        
        # Perform analysis
        self.analyze_results(uart_log)
        
        return True
    
    def analyze_results(self, log_path: Path):
        """Analyze monitoring results"""
        
        if not log_path.exists():
            self.log("ANALYZE", f"Log file not found: {log_path}", "warning")
            return
        
        try:
            frames = []
            with open(log_path) as f:
                for line in f:
                    if line.strip():
                        record = json.loads(line)
                        if record.get("type") == "frame":
                            frames.append(record)
            
            if not frames:
                self.log("ANALYZE", "No frames captured", "warning")
                return
            
            # Count by status
            valid = sum(1 for f in frames if f.get("decoded", {}).get("match"))
            invalid = len(frames) - valid
            
            self.log("ANALYZE", f"Total frames: {len(frames)}", "info")
            self.log("ANALYZE", f"Valid frames: {valid}", "success")
            if invalid > 0:
                self.log("ANALYZE", f"Checksum failures: {invalid}", "error")
            
            # Store analysis
            self.results["analysis"] = {
                "total_frames": len(frames),
                "valid_frames": valid,
                "invalid_frames": invalid,
                "frame_dump": frames[:5]  # Store first 5 for inspection
            }
            
            # Recommendations
            if invalid > 0:
                self.log("ANALYZE", "⚠️  Driver validation logic rejecting valid frames", "warning")
                self.log("ANALYZE", "Check: V93XX_Raccoon.cpp RegisterRead() checksum calculation", "info")
        
        except Exception as e:
            self.log("ANALYZE", f"Error: {e}", "error")
    
    def run(self) -> bool:
        """Execute full orchestration"""
        
        print(f"\n{Colors.header('V9381 UART DEBUG ORCHESTRATOR')}")
        print(f"Output directory: {self.run_dir}")
        
        try:
            # Step 1: Compile
            if not self.step_compile():
                return False
            
            # Step 2: Upload (unless skipped)
            if not self.no_upload:
                if not self.step_upload():
                    return False
            else:
                self.log("UPLOAD", "Skipped (--no-upload)", "info")
            
            # Step 3: Start parallel monitoring
            print(Colors.header("STEP 3-4: PARALLEL MONITORING & CAPTURE"))
            
            # Run monitoring (foreground)
            self.step_monitor()
            
            # Run Saleae capture (parallel)
            saleae_thread = threading.Thread(target=self.step_capture_saleae, daemon=True)
            saleae_thread.start()
            saleae_thread.join(timeout=15)
            
            # Step 5: Correlate and analyze
            self.step_correlate()
            
            # Save results
            results_file = self.run_dir / "results.json"
            with open(results_file, "w") as f:
                json.dump(self.results, f, indent=2)
            
            self.log("ORCHESTRATOR", f"Results saved: {results_file}", "success")
            
            return True
            
        except KeyboardInterrupt:
            self.log("ORCHESTRATOR", "Interrupted by user", "warning")
            return False
        except Exception as e:
            self.log("ORCHESTRATOR", f"Unexpected error: {e}", "error")
            traceback.print_exc()
            return False
    
    def print_summary(self):
        """Print execution summary"""
        print(Colors.header("EXECUTION SUMMARY"))
        
        for step, result in self.results["steps"].items():
            status = "SUCCESS" if result["success"] else "FAILED"
            color_status = Colors.success(status) if result["success"] else Colors.error(status)
            print(f"  {step:15} {color_status}")
        
        if self.results.get("analysis"):
            print(f"\nAnalysis:")
            for key, value in self.results["analysis"].items():
                if key != "frame_dump":
                    print(f"  {key}: {value}")
        
        print(f"\nOutput saved to: {self.run_dir}")


def main():
    import argparse
    
    parser = argparse.ArgumentParser(
        description="Unified V9381 UART debugging orchestrator",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument("-s", "--sketch", help="Sketch to debug")
    parser.add_argument("-d", "--duration", type=int, default=30, 
                       help="Monitoring duration (seconds)")
    parser.add_argument("--no-upload", action="store_true", help="Skip upload")
    parser.add_argument("--report", action="store_true", help="Generate comparison report")
    
    args = parser.parse_args()
    
    # Run orchestrator
    orchestrator = Orchestrator(
        sketch=args.sketch,
        duration=args.duration,
        no_upload=args.no_upload,
        report=args.report
    )
    
    success = orchestrator.run()
    orchestrator.print_summary()
    
    sys.exit(0 if success else 1)


if __name__ == "__main__":
    main()
