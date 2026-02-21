#!/usr/bin/env python3
"""
Advanced UART diagnostics:
- Captures ESP32 serial debug output
- Logs register read transactions with timing
- Compares expected vs. actual response frames
- Identifies parity/checksum/framing issues
"""

import serial
import sys
import json
from pathlib import Path
from datetime import datetime
from typing import Optional, Tuple

PORT = "COM7"
BAUD = 115200
TIMEOUT = 0.1  # Short timeout for non-blocking reads

# V9381 protocol constants (per datasheet)
FRAME_MARKER = 0x7D
CHECKSUM_POLY = 0xFF  # For odd parity checksum

def calculate_checksum(data: bytes) -> int:
    """Calculate V9381 odd-parity checksum."""
    xor_result = 0
    for byte in data:
        xor_result ^= byte
    return xor_result


def decode_register_frame(raw: bytes) -> dict:
    """Attempt to decode a 5-byte register frame."""
    if len(raw) < 5:
        return {"valid": False, "reason": "short", "len": len(raw)}
    
    frame = raw[:5]
    marker, addr, cmd, hi, lo = frame
    checksum_calc = calculate_checksum(frame[:4])
    
    return {
        "valid": marker == 0x7D,
        "marker": f"0x{marker:02X}",
        "addr": f"0x{addr:02X}",
        "cmd": f"0x{cmd:02X}",
        "value": f"0x{(hi << 8) | lo:04X}",
        "checksum_expected": f"0x{lo:02X}",
        "checksum_calculated": f"0x{checksum_calc:02X}",
        "match": checksum_calc == lo,
    }


def monitor_with_diagnostics(duration_sec: int = 60):
    """Monitor serial port with detailed diagnostics."""
    
    log_file = Path("uart_monitor.jsonl")
    print(f"Opening {PORT} at {BAUD} baud (log: {log_file})...")
    
    with serial.Serial(PORT, BAUD, timeout=TIMEOUT) as ser, open(log_file, "w") as log:
        print(f"Listening for {duration_sec} seconds...\n")
        print("Waiting for ESP32 debug output...")
        print("=" * 70)
        
        start = datetime.now()
        timeout_at = start.timestamp() + duration_sec
        
        buffer = bytearray()
        line_buffer = ""
        frame_count = 0
        error_count = 0
        
        while datetime.now().timestamp() < timeout_at:
            try:
                byte_data = ser.read(256)
                if not byte_data:
                    continue
                
                now = datetime.now()
                elapsed = (now - start).total_seconds()
                
                # Handle both text (debug output) and binary (UART frames)
                for byte in byte_data:
                    buffer.append(byte)
                    
                    if 32 <= byte < 127 or byte in (9, 10, 13):
                        # Printable or whitespace—collect as text
                        line_buffer += chr(byte)
                        
                        if byte == 10:  # Newline
                            if line_buffer.strip():
                                record = {
                                    "time": elapsed,
                                    "type": "text",
                                    "content": line_buffer.strip()
                                }
                                log.write(json.dumps(record) + "\n")
                                print(f"[{elapsed:7.3f}s] {line_buffer.strip()}")
                            line_buffer = ""
                    else:
                        # Non-printable—treat as frame data
                        if line_buffer.strip():
                            record = {
                                "time": elapsed,
                                "type": "text",
                                "content": line_buffer.strip()
                            }
                            log.write(json.dumps(record) + "\n")
                            print(f"[{elapsed:7.3f}s] {line_buffer.strip()}")
                            line_buffer = ""
                        
                        # Attempt to decode frame starting at buffer marker
                        if byte == FRAME_MARKER or len(buffer) >= 5:
                            if len(buffer) >= 5 and buffer[0] == FRAME_MARKER:
                                frame = bytes(buffer[:5])
                                decoded = decode_register_frame(frame)
                                frame_count += 1
                                
                                record = {
                                    "time": elapsed,
                                    "type": "frame",
                                    "number": frame_count,
                                    "raw_hex": " ".join(f"{b:02X}" for b in frame),
                                    "decoded": decoded
                                }
                                log.write(json.dumps(record) + "\n")
                                
                                status = "✓" if decoded.get("match") else "✗"
                                print(f"[{elapsed:7.3f}s] {status} Frame #{frame_count}: {decoded}")
                                
                                if not decoded.get("match"):
                                    error_count += 1
                                
                                del buffer[:5]
                            elif len(buffer) > 5:
                                # Resync: skip first byte
                                bad = buffer.pop(0)
                                record = {
                                    "time": elapsed,
                                    "type": "noise",
                                    "byte": f"0x{bad:02X}"
                                }
                                log.write(json.dumps(record) + "\n")
                
            except serial.SerialException as e:
                print(f"Serial error: {e}")
                break
            except KeyboardInterrupt:
                print("\n[Interrupted by user]")
                break
        
        print("=" * 70)
        elapsed_total = (datetime.now() - start).total_seconds()
        print(f"\n=== SUMMARY ===")
        print(f"Duration: {elapsed_total:.1f}s")
        print(f"Frames detected: {frame_count}")
        print(f"Checksum errors: {error_count}")
        print(f"Log file: {log_file}")
        
        if buffer:
            print(f"Remaining buffer: {' '.join(f'{b:02X}' for b in buffer)} ({len(buffer)} bytes)")


if __name__ == "__main__":
    duration = int(sys.argv[1]) if len(sys.argv) > 1 else 60
    try:
        monitor_with_diagnostics(duration)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)
