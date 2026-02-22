#!/usr/bin/env python3
"""
Serial Monitor for V9381 UART Testing
Captures serial output from ESP32-S3 device with timestamps
"""

import serial
import sys
import time
from datetime import datetime
from pathlib import Path


def monitor_serial(port, baud, timeout=10, capture_file=None):
    """
    Monitor serial port and capture output.
    
    Args:
        port: Serial port (e.g., 'COM3')
        baud: Baud rate (default 115200)
        timeout: Maximum seconds to monitor (default 10)
        capture_file: Optional file to save output
        
    Returns:
        List of captured lines
    """
    try:
        ser = serial.Serial(port, baud, timeout=1)
        print(f"✓ Connected to {port} @ {baud} baud")
        
        output = []
        start_time = time.time()
        line_count = 0
        
        while time.time() - start_time < timeout:
            try:
                if ser.in_waiting:
                    line = ser.readline().decode('utf-8', errors='ignore').strip()
                    if line:
                        timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]
                        print(f"[{timestamp}] {line}")
                        output.append(line)
                        line_count += 1
                else:
                    time.sleep(0.01)  # Small delay to avoid busy-waiting
            except Exception as e:
                print(f"⚠ Read error: {e}", file=sys.stderr)
                continue
        
        ser.close()
        print(f"\n✓ Captured {line_count} lines in {timeout}s")
        
        if capture_file:
            with open(capture_file, 'w', encoding='utf-8') as f:
                f.write('\n'.join(output))
            print(f"✓ Saved to {capture_file}")
        
        return output
        
    except serial.SerialException as e:
        print(f"❌ Serial error: {e}", file=sys.stderr)
        print(f"   Available ports: {[p.device for p in serial.tools.list_ports.comports()]}")
        return []
    except Exception as e:
        print(f"❌ Error: {e}", file=sys.stderr)
        return []


def main():
    """Main entry point."""
    if len(sys.argv) < 2:
        print("Usage: python monitor_serial.py <port> [baud] [timeout] [output_file]")
        print("Example: python monitor_serial.py COM3 115200 10 output.txt")
        return 1
    
    port = sys.argv[1]
    baud = int(sys.argv[2]) if len(sys.argv) > 2 else 115200
    timeout = int(sys.argv[3]) if len(sys.argv) > 3 else 10
    capture = sys.argv[4] if len(sys.argv) > 4 else None
    
    print("="*70)
    print("V9381 UART Serial Monitor")
    print("="*70)
    print(f"Port: {port}")
    print(f"Baud: {baud}")
    print(f"Timeout: {timeout}s")
    if capture:
        print(f"Output: {capture}")
    print("="*70)
    print("\nListening for serial output (press Ctrl+C to stop early)...\n")
    
    try:
        output = monitor_serial(port, baud, timeout, capture)
        
        if not output:
            print("⚠ No output received - device may not be ready")
            return 1
        
        print("\n" + "="*70)
        print("Summary:")
        
        if any("Checksum Mode" in line for line in output):
            print("✅ Device initialized with ChecksumMode")
        else:
            print("⚠ No ChecksumMode message detected")
        
        if any("ERROR" in line for line in output):
            print("⚠ ERROR messages detected in output")
        
        if any("CRC" in line for line in output):
            print("✅ CRC messages detected - validation enabled")
        
        print("="*70)
        return 0
        
    except KeyboardInterrupt:
        print("\n\nInterrupted by user")
        return 0
    except Exception as e:
        print(f"❌ Error: {e}")
        return 1


if __name__ == "__main__":
    sys.exit(main())
