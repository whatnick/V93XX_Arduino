#!/usr/bin/env python3
"""
Monitor UART responses from ESP32 and V9381 on COM7.
Logs raw bytes, decoded frames, and diagnostics.
"""

import serial
import sys
from pathlib import Path
from datetime import datetime

PORT = "COM7"
BAUD = 115200
TIMEOUT = 2

def hex_dump(data: bytes, label: str = "") -> str:
    """Format bytes as hex with ASCII."""
    if not data:
        return f"{label}: (empty)"
    hex_str = " ".join(f"{b:02x}" for b in data)
    ascii_str = "".join(chr(b) if 32 <= b < 127 else "." for b in data)
    return f"{label}: {hex_str} | {ascii_str}"


def main():
    print(f"Opening {PORT} at {BAUD} baud...")
    with serial.Serial(PORT, BAUD, timeout=TIMEOUT) as ser:
        print(f"Port open. Listening for {30} seconds...")
        
        start = datetime.now()
        timeout_at = start.timestamp() + 30
        
        buffer = bytearray()
        frame_count = 0
        
        while datetime.now().timestamp() < timeout_at:
            try:
                data = ser.read(256)
                if not data:
                    continue
                
                buffer.extend(data)
                now = datetime.now()
                print(f"\n[{now.strftime('%H:%M:%S.%f')[:-3]}] Received {len(data)} bytes:")
                print(hex_dump(data, "Raw"))
                
                # Look for UART frame patterns (0x7D marker in our protocol)
                while len(buffer) >= 4:
                    if buffer[0] == 0x7D:  # Frame start
                        # Assume 5-byte frame: [0x7D, addr, cmd, hi, lo]
                        if len(buffer) >= 5:
                            frame = bytes(buffer[:5])
                            frame_count += 1
                            addr = buffer[1]
                            cmd = buffer[2]
                            hi = buffer[3]
                            lo = buffer[4]
                            value = (hi << 8) | lo
                            print(f"  Frame #{frame_count}: {hex_dump(frame, 'UART Frame')}")
                            print(f"    Addr=0x{addr:02X}, Cmd=0x{cmd:02X}, Value=0x{value:08X}")
                            del buffer[:5]
                        else:
                            break
                    else:
                        # Skip noise byte and resync
                        noise = buffer.pop(0)
                        print(f"  Skipped noise byte: 0x{noise:02X}")
                
            except serial.SerialException as e:
                print(f"Serial error: {e}")
                break
            except KeyboardInterrupt:
                break
        
        if buffer:
            print(f"\nRemaining buffer ({len(buffer)} bytes): {hex_dump(bytes(buffer), 'Buffer')}")
        print(f"\nTotal frames detected: {frame_count}")


if __name__ == "__main__":
    main()
