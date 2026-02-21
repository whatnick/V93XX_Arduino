#!/usr/bin/env python3
"""
Comprehensive UART debugging automation:
- Compiles and uploads sketch via Arduino CLI
- Captures bus traffic via Saleae Logic 2
- Monitors ESP32 serial output via pySerial
- Correlates findings to identify driver issues

Usage:
    python automated_uart_debug.py [sketch] [duration] [--no-upload]
    
Example:
    python automated_uart_debug.py examples/V9381_UART/V9381_UART.ino 30
"""

import subprocess
import sys
import json
import time
import threading
import serial
from pathlib import Path
from datetime import datetime, timedelta
from typing import Optional, Dict, List, Tuple
from dataclasses import dataclass, asdict
import os
import re

# Configuration
ARDUINO_CLI = "arduino-cli"
BOARD = "esp32:esp32:esp32s3"
PORT = "COM7"
BAUD = 115200
SALEAE_SAMPLE_RATE = "24MHz"
SALEAE_DURATION = "5s"
SALEAE_CHANNELS = [0, 1, 2, 7]  # A1, RX/MISO, A0, TX/MOSI

@dataclass
class SerialLogEntry:
    """Serial output log entry"""
    timestamp: float
    type: str  # "text", "frame", "noise"
    content: str
    decoded: Optional[Dict] = None


class ArduinoCLIManager:
    """Manage Arduino CLI compilation and upload"""
    
    def __init__(self, sketch_path: str):
        self.sketch_path = Path(sketch_path)
        self.sketch_name = self.sketch_path.stem
        
    def compile(self) -> bool:
        """Compile sketch and return success status"""
        print(f"\n{'='*70}")
        print(f"[ARDUINO CLI] Compiling {self.sketch_name}...")
        print(f"{'='*70}")
        
        try:
            result = subprocess.run(
                [ARDUINO_CLI, "compile", "-b", BOARD, str(self.sketch_path), "-v"],
                capture_output=False,
                timeout=120
            )
            success = result.returncode == 0
            print(f"[ARDUINO CLI] Compile {'✓ SUCCESS' if success else '✗ FAILED'}")
            return success
        except Exception as e:
            print(f"[ARDUINO CLI] Error: {e}")
            return False
    
    def upload(self) -> bool:
        """Upload compiled sketch and return success status"""
        print(f"\n{'='*70}")
        print(f"[ARDUINO CLI] Uploading to {PORT}...")
        print(f"{'='*70}")
        
        try:
            result = subprocess.run(
                [ARDUINO_CLI, "upload", "-b", BOARD, "-p", PORT, str(self.sketch_path), "-v"],
                capture_output=False,
                timeout=60
            )
            success = result.returncode == 0
            print(f"[ARDUINO CLI] Upload {'✓ SUCCESS' if success else '✗ FAILED'}")
            return success
        except Exception as e:
            print(f"[ARDUINO CLI] Error: {e}")
            return False


class SerialMonitor:
    """Capture and decode serial output from ESP32"""
    
    FRAME_MARKER = 0x7D
    
    def __init__(self, port: str, baud: int, timeout: float = 0.05):
        self.port = port
        self.baud = baud
        self.timeout = timeout
        self.entries: List[SerialLogEntry] = []
        self.running = False
        self.start_time = None
        
    def calculate_checksum(self, data: bytes) -> int:
        """Calculate V9381 odd-parity checksum"""
        xor_result = 0
        for byte in data:
            xor_result ^= byte
        return xor_result
    
    def run(self, duration: int):
        """Monitor serial for specified duration"""
        print(f"\n{'='*70}")
        print(f"[PYSERIAL] Monitoring {self.port} at {self.baud} baud...")
        print(f"{'='*70}")
        
        self.running = True
        self.start_time = time.time()
        timeout_at = self.start_time + duration
        
        try:
            with serial.Serial(self.port, self.baud, timeout=self.timeout) as ser:
                buffer = bytearray()
                line_buffer = ""
                frame_count = 0
                
                while time.time() < timeout_at and self.running:
                    byte_data = ser.read(256)
                    if not byte_data:
                        continue
                    
                    elapsed = time.time() - self.start_time
                    
                    for byte in byte_data:
                        buffer.append(byte)
                        
                        if 32 <= byte < 127 or byte in (9, 10, 13):
                            # Printable/whitespace
                            line_buffer += chr(byte)
                            
                            if byte == 10:  # Newline
                                if line_buffer.strip():
                                    self.entries.append(SerialLogEntry(
                                        timestamp=elapsed,
                                        type="text",
                                        content=line_buffer.strip()
                                    ))
                                    print(f"[{elapsed:7.3f}s] {line_buffer.strip()}")
                                line_buffer = ""
                        else:
                            # Non-printable
                            if line_buffer.strip():
                                self.entries.append(SerialLogEntry(
                                    timestamp=elapsed,
                                    type="text",
                                    content=line_buffer.strip()
                                ))
                                print(f"[{elapsed:7.3f}s] {line_buffer.strip()}")
                                line_buffer = ""
                            
                            # Frame decode attempt
                            if len(buffer) >= 5 and buffer[0] == self.FRAME_MARKER:
                                frame = bytes(buffer[:5])
                                marker, addr, cmd, hi, lo = frame
                                checksum_calc = self.calculate_checksum(frame[:4])
                                
                                decoded = {
                                    "marker": f"0x{marker:02X}",
                                    "addr": f"0x{addr:02X}",
                                    "cmd": f"0x{cmd:02X}",
                                    "value": f"0x{(hi << 8) | lo:04X}",
                                    "checksum_expected": f"0x{lo:02X}",
                                    "checksum_calculated": f"0x{checksum_calc:02X}",
                                    "match": checksum_calc == lo
                                }
                                
                                frame_count += 1
                                status = "✓" if decoded["match"] else "✗"
                                self.entries.append(SerialLogEntry(
                                    timestamp=elapsed,
                                    type="frame",
                                    content=" ".join(f"{b:02X}" for b in frame),
                                    decoded=decoded
                                ))
                                print(f"[{elapsed:7.3f}s] {status} Frame: {decoded}")
                                del buffer[:5]
                            elif len(buffer) > 5:
                                bad = buffer.pop(0)
                                self.entries.append(SerialLogEntry(
                                    timestamp=elapsed,
                                    type="noise",
                                    content=f"0x{bad:02X}"
                                ))
                
        except serial.SerialException as e:
            print(f"[PYSERIAL] Error: {e}")
        except Exception as e:
            print(f"[PYSERIAL] Unexpected error: {e}")
        finally:
            self.running = False
            print(f"[PYSERIAL] Capture complete")
    
    def save_log(self, path: str):
        """Save serial log to file"""
        with open(path, "w") as f:
            for entry in self.entries:
                record = {
                    "timestamp": entry.timestamp,
                    "type": entry.type,
                    "content": entry.content,
                }
                if entry.decoded:
                    record["decoded"] = entry.decoded
                f.write(json.dumps(record) + "\n")


class SaleaeAutomation:
    """Control Saleae Logic 2 capture"""
    
    def __init__(self):
        self.capture_dir = Path("captures")
        self.capture_dir.mkdir(exist_ok=True)
        
    def trigger_capture(self, duration: str = "5s") -> Optional[Path]:
        """
        Trigger Saleae capture.
        Note: This requires Saleae Logic 2 to be running with automation enabled.
        """
        print(f"\n{'='*70}")
        print(f"[SALEAE] Starting capture ({duration})...")
        print(f"{'='*70}")
        print("[SALEAE] Ensure Logic 2 is open and automation is enabled!")
        print(f"[SALEAE] Channels: {SALEAE_CHANNELS}, Sample rate: {SALEAE_SAMPLE_RATE}")
        
        # Attempt to use Saleae Python CLI if available
        try:
            from logic2_automation import LogicCapture
            
            timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
            capture_file = self.capture_dir / f"capture_{timestamp}.logic2"
            
            # Create and run capture
            capture = LogicCapture()
            # Configuration would go here based on Logic 2 Python API
            print(f"[SALEAE] Would save to: {capture_file}")
            print("[SALEAE] Manual trigger: Use Logic 2 GUI or wait for capture.logic2 file")
            
            return capture_file
            
        except ImportError:
            print("[SALEAE] logic2-automation not available; skipping automated capture")
            print("[SALEAE] To capture manually:")
            print("  1. Open Logic 2")
            print("  2. Set up UART decoders on channels 1 (RX) and 7 (TX)")
            print("  3. Click 'Capture' and run the sketch")
            print("  4. Save capture as captures/capture_*.logic2")
            return None


class DebugCoordinator:
    """Orchestrate compilation, upload, and monitoring"""
    
    def __init__(self, sketch_path: str, duration: int = 30, no_upload: bool = False):
        self.sketch_path = sketch_path
        self.duration = duration
        self.no_upload = no_upload
        self.arduino = ArduinoCLIManager(sketch_path)
        self.monitor = SerialMonitor(PORT, BAUD)
        self.saleae = SaleaeAutomation()
        self.results: Dict = {}
        
    def run(self):
        """Execute full debug sequence"""
        print(f"\n{'='*70}")
        print(f"V9381 UART AUTOMATED DEBUG")
        print(f"Sketch: {self.sketch_path}")
        print(f"Duration: {self.duration}s")
        print(f"{'='*70}")
        
        # Step 1: Compile
        if not self.arduino.compile():
            print("[ERROR] Compilation failed!")
            return False
        
        # Step 2: Upload (if not skipped)
        if not self.no_upload:
            if not self.arduino.upload():
                print("[ERROR] Upload failed!")
                return False
            print("[INFO] Waiting for ESP32 to restart...")
            time.sleep(2)
        
        # Step 3: Start monitoring and Saleae capture
        print("\n[INFO] Starting parallel monitoring...")
        
        # Start serial monitor in background
        monitor_thread = threading.Thread(
            target=self.monitor.run,
            args=(self.duration,),
            daemon=False
        )
        monitor_thread.start()
        
        # Trigger Saleae capture
        capture_file = self.saleae.trigger_capture()
        
        # Wait for serial monitoring to complete
        monitor_thread.join(timeout=self.duration + 5)
        
        # Step 4: Save logs
        log_path = Path("uart_debug.jsonl")
        self.monitor.save_log(str(log_path))
        print(f"\n[INFO] Serial log saved to: {log_path}")
        
        # Step 5: Analyze results
        self.analyze_results()
        
        return True
    
    def analyze_results(self):
        """Analyze collected data and report findings"""
        print(f"\n{'='*70}")
        print("ANALYSIS RESULTS")
        print(f"{'='*70}")
        
        text_entries = [e for e in self.monitor.entries if e.type == "text"]
        frame_entries = [e for e in self.monitor.entries if e.type == "frame"]
        noise_entries = [e for e in self.monitor.entries if e.type == "noise"]
        
        print(f"\nSerial Output: {len(text_entries)} lines")
        print(f"Frame Decodes: {len(frame_entries)}")
        print(f"Noise Bytes: {len(noise_entries)}")
        
        if frame_entries:
            checksum_passes = sum(
                1 for e in frame_entries 
                if e.decoded and e.decoded.get("match")
            )
            checksum_fails = len(frame_entries) - checksum_passes
            print(f"  ✓ Checksum OK: {checksum_passes}")
            print(f"  ✗ Checksum BAD: {checksum_fails}")
            
            if checksum_fails > 0:
                print("\n⚠️  CHECKSUM MISMATCH DETECTED!")
                print("First few failures:")
                count = 0
                for e in frame_entries:
                    if e.decoded and not e.decoded.get("match"):
                        print(f"  {e.content}")
                        print(f"    Expected: {e.decoded.get('checksum_expected')}, "
                              f"Calculated: {e.decoded.get('checksum_calculated')}")
                        count += 1
                        if count >= 3:
                            break
        
        # Check for timeout/error messages in serial output
        error_msgs = [e.content for e in text_entries if any(
            word in e.content.lower() 
            for word in ["timeout", "error", "failed", "invalid", "bad"]
        )]
        
        if error_msgs:
            print(f"\n⚠️  ERROR MESSAGES IN SERIAL OUTPUT ({len(error_msgs)}):")
            for msg in error_msgs[:5]:
                print(f"  {msg}")
        
        # Generate actionable recommendations
        print("\n" + "="*70)
        print("RECOMMENDATIONS:")
        print("="*70)
        
        if not frame_entries:
            print("❌ No frames captured. Possible issues:")
            print("   - Wrong UART configuration (baud/parity/stopbits)")
            print("   - V9381 not responding (address mismatch, power issue)")
            print("   - Serial connection lost")
        elif checksum_fails > 0:
            print("❌ Checksum validation failing in driver. Check:")
            print("   - RegisterRead() checksum calculation in V93XX_Raccoon.cpp")
            print("   - Byte order or frame format misunderstanding")
            print("   - Parity bit handling (odd vs even)")
        else:
            print("✓ All frames validate correctly!")
            print("  Issue may be in frame timing, buffering, or higher-level logic")
        
        print("\nNext steps:")
        print("  1. Compare captured frames with Logic 2 decode side-by-side")
        print("  2. Inspect V93XX_Raccoon.cpp RegisterRead() checksum logic")
        print("  3. Check uart_debug.jsonl for detailed frame data")


def main():
    import argparse
    
    parser = argparse.ArgumentParser(
        description="Automate UART debugging with Arduino CLI, Saleae, and pySerial"
    )
    parser.add_argument(
        "sketch",
        default="examples/V9381_UART/V9381_UART.ino",
        nargs="?",
        help="Sketch to compile and upload"
    )
    parser.add_argument(
        "-d", "--duration",
        type=int,
        default=30,
        help="Monitoring duration (seconds)"
    )
    parser.add_argument(
        "--no-upload",
        action="store_true",
        help="Skip upload (use if sketch already running)"
    )
    
    args = parser.parse_args()
    
    coordinator = DebugCoordinator(args.sketch, args.duration, args.no_upload)
    success = coordinator.run()
    
    sys.exit(0 if success else 1)


if __name__ == "__main__":
    main()
