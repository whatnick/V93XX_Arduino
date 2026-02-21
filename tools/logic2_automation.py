#!/usr/bin/env python3
"""
Advanced Saleae Logic 2 integration for UART debugging.
Automates capture configuration, decoder setup, and data export.

Features:
- Programmatic Logic 2 control via socket API
- Multi-UART decoder configuration (8O1, 8N1, 8E1)
- Automated capture triggering and data export
- Cross-reference with pySerial diagnostics

Usage:
    # Standalone capture
    python logic2_automation.py --duration 5s --channels 0,1,2,7
    
    # With correlation to pySerial output
    python logic2_automation.py --correlate uart_debug.jsonl
"""

import subprocess
import socket
import json
import time
import sys
from pathlib import Path
from datetime import datetime
from typing import Optional, Dict, List, Tuple

LOGIC2_HOST = "localhost"
LOGIC2_PORT = 10430  # Default Logic 2 automation port


class Logic2Client:
    """Control Logic 2 via socket API"""
    
    def __init__(self, host: str = LOGIC2_HOST, port: int = LOGIC2_PORT):
        self.host = host
        self.port = port
        self.socket = None
        self.connected = False
    
    def connect(self) -> bool:
        """Connect to Logic 2 automation server"""
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect((self.host, self.port))
            self.connected = True
            print(f"[LOGIC2] Connected to {self.host}:{self.port}")
            return True
        except ConnectionRefusedError:
            print(f"[LOGIC2] ERROR: Could not connect to {self.host}:{self.port}")
            print("[LOGIC2] Ensure Logic 2 is running with automation disabled (Settings → Developer → Enable Automation Socket)")
            self.connected = False
            return False
        except Exception as e:
            print(f"[LOGIC2] Error: {e}")
            self.connected = False
            return False
    
    def send_command(self, command: Dict) -> Optional[Dict]:
        """Send command to Logic 2 and receive response"""
        if not self.connected:
            return None
        
        try:
            # Send command as JSON
            cmd_str = json.dumps(command)
            self.socket.sendall(cmd_str.encode() + b'\0')
            
            # Receive response
            response = b''
            while True:
                chunk = self.socket.recv(4096)
                if not chunk:
                    break
                response += chunk
                if b'\0' in response:
                    break
            
            # Parse response
            resp_str = response.rstrip(b'\0').decode()
            if resp_str:
                return json.loads(resp_str)
            return None
        except Exception as e:
            print(f"[LOGIC2] Send error: {e}")
            return None
    
    def disconnect(self):
        """Close connection"""
        if self.socket:
            self.socket.close()
            self.connected = False
    
    def get_devices(self) -> List[Dict]:
        """List connected Logic devices"""
        resp = self.send_command({"command": "get_devices"})
        return resp.get("devices", []) if resp else []
    
    def get_capture_info(self) -> Optional[Dict]:
        """Get current capture info"""
        resp = self.send_command({"command": "get_capture_info"})
        return resp
    
    def set_capture_settings(self, sample_rate: str, duration: str, channels: List[int]) -> bool:
        """Configure capture parameters"""
        cmd = {
            "command": "set_capture_settings",
            "sample_rate": sample_rate,
            "capture_seconds": self._parse_duration(duration),
            "channels": channels
        }
        resp = self.send_command(cmd)
        return resp and resp.get("success", False)
    
    def start_capture(self) -> bool:
        """Start capture"""
        resp = self.send_command({"command": "capture"})
        return resp and resp.get("success", False)
    
    def stop_capture(self) -> bool:
        """Stop capture"""
        resp = self.send_command({"command": "stop_capture"})
        return resp and resp.get("success", False)
    
    def add_uart_decoder(self, tx_channel: int, rx_channel: int, baud: int, 
                         bits: int = 8, parity: str = "ODD", stop_bits: int = 1) -> bool:
        """Add UART protocol decoder"""
        cmd = {
            "command": "add_decoder",
            "decoder_type": "uart",
            "settings": {
                "tx_channel": tx_channel,
                "rx_channel": rx_channel,
                "baud_rate": baud,
                "bits": bits,
                "parity": parity,
                "stop_bits": stop_bits
            }
        }
        resp = self.send_command(cmd)
        return resp and resp.get("success", False)
    
    def export_data(self, output_path: str, format: str = "csv") -> bool:
        """Export capture data"""
        cmd = {
            "command": "export_data",
            "filepath": output_path,
            "data_type": format
        }
        resp = self.send_command(cmd)
        return resp and resp.get("success", False)
    
    @staticmethod
    def _parse_duration(duration_str: str) -> float:
        """Parse duration string like '5s', '100ms' to seconds"""
        duration_str = duration_str.strip().lower()
        if duration_str.endswith('ms'):
            return float(duration_str[:-2]) / 1000
        elif duration_str.endswith('s'):
            return float(duration_str[:-1])
        elif duration_str.endswith('m'):
            return float(duration_str[:-1]) * 60
        return float(duration_str)


class DataCorrelator:
    """Correlate Saleae and pySerial data"""
    
    def __init__(self, saleae_csv: Path, pyserial_log: Path):
        self.saleae_csv = saleae_csv
        self.pyserial_log = pyserial_log
        self.saleae_frames = []
        self.pyserial_frames = []
    
    def load_saleae_decode(self):
        """Load UART decode from Saleae CSV"""
        if not self.saleae_csv.exists():
            print(f"[CORRELATE] Saleae file not found: {self.saleae_csv}")
            return False
        
        print(f"[CORRELATE] Loading Saleae data: {self.saleae_csv}")
        try:
            with open(self.saleae_csv) as f:
                # Skip header
                lines = f.readlines()
                for line in lines[1:]:  # Skip CSV header
                    parts = line.strip().split(',')
                    if len(parts) >= 3:
                        time_str = parts[0]
                        channel = parts[1]
                        value = parts[2] if len(parts) > 2 else ""
                        self.saleae_frames.append({
                            'time': time_str,
                            'channel': channel,
                            'value': value
                        })
            print(f"[CORRELATE] Loaded {len(self.saleae_frames)} Saleae entries")
            return True
        except Exception as e:
            print(f"[CORRELATE] Error loading Saleae: {e}")
            return False
    
    def load_pyserial_log(self):
        """Load pySerial log"""
        if not self.pyserial_log.exists():
            print(f"[CORRELATE] pySerial log not found: {self.pyserial_log}")
            return False
        
        print(f"[CORRELATE] Loading pySerial data: {self.pyserial_log}")
        try:
            with open(self.pyserial_log) as f:
                for line in f:
                    if line.strip():
                        record = json.loads(line)
                        self.pyserial_frames.append(record)
            print(f"[CORRELATE] Loaded {len(self.pyserial_frames)} pySerial entries")
            return True
        except Exception as e:
            print(f"[CORRELATE] Error loading pySerial: {e}")
            return False
    
    def correlate(self):
        """Find matching frames between sources"""
        print(f"\n{'='*70}")
        print("CORRELATION ANALYSIS")
        print(f"{'='*70}")
        
        if not self.saleae_frames or not self.pyserial_frames:
            print("[CORRELATE] Insufficient data for correlation")
            return
        
        # Find TX frames (Saleae CH7) and corresponding RX frames
        tx_frames = [f for f in self.saleae_frames if f.get('channel') == '7']
        rx_frames = [f for f in self.saleae_frames if f.get('channel') == '1']
        
        print(f"\nSaleae Analysis:")
        print(f"  TX frames (CH7): {len(tx_frames)}")
        print(f"  RX frames (CH1): {len(rx_frames)}")
        
        pyserial_valid = sum(1 for f in self.pyserial_frames 
                            if f.get('type') == 'frame' and f.get('decoded', {}).get('match'))
        pyserial_invalid = sum(1 for f in self.pyserial_frames 
                              if f.get('type') == 'frame' and not f.get('decoded', {}).get('match'))
        
        print(f"\npySerial Analysis:")
        print(f"  Valid frames: {pyserial_valid}")
        print(f"  Invalid frames (checksum mismatch): {pyserial_invalid}")
        
        # Report discrepancies
        if len(rx_frames) > pyserial_valid:
            print(f"\n⚠️  DISCREPANCY DETECTED:")
            print(f"   Saleae shows {len(rx_frames)} RX frames")
            print(f"   pySerial decoded {pyserial_valid} valid + {pyserial_invalid} invalid")
            print(f"   → {len(rx_frames) - pyserial_valid - pyserial_invalid} frames not captured by pySerial!")
            print(f"\n   Possible issues:")
            print(f"   - Timeout/dropped bytes in ESP32 RX buffer")
            print(f"   - Serial framing error or data corruption")
            print(f"   - WaitForRx() not capturing all bytes")
        
        if pyserial_invalid > 0:
            print(f"\n⚠️  {pyserial_invalid} checksum failures detected in pySerial!")
            print(f"   Check V93XX_Raccoon.cpp RegisterRead() validation logic")


def main():
    import argparse
    
    parser = argparse.ArgumentParser(description="Saleae Logic 2 UART automation")
    parser.add_argument("--host", default=LOGIC2_HOST, help="Logic 2 host")
    parser.add_argument("--port", type=int, default=LOGIC2_PORT, help="Logic 2 port")
    parser.add_argument("--duration", default="5s", help="Capture duration (e.g., 5s, 100ms)")
    parser.add_argument("--channels", default="0,1,2,7", help="Channels to capture (comma-separated)")
    parser.add_argument("--sample-rate", default="24MHz", help="Sample rate")
    parser.add_argument("--correlate", type=Path, help="Correlate with pySerial log (jsonl)")
    parser.add_argument("--export", type=Path, help="Export to CSV after capture")
    
    args = parser.parse_args()
    
    # Parse channels
    channels = [int(c.strip()) for c in args.channels.split(",")]
    
    # Connect to Logic 2
    logic2 = Logic2Client(args.host, args.port)
    if not logic2.connect():
        print("[ERROR] Cannot connect to Logic 2 automation socket")
        print("[INFO] Open Logic 2 and enable automation in Settings → Developer")
        sys.exit(1)
    
    try:
        # Verify capture active
        info = logic2.get_capture_info()
        if info:
            print(f"[LOGIC2] Capture info: {info}")
        
        # Configure capture
        print(f"\n[LOGIC2] Configuring capture...")
        print(f"  Channels: {channels}")
        print(f"  Duration: {args.duration}")
        print(f"  Sample rate: {args.sample_rate}")
        
        if logic2.set_capture_settings(args.sample_rate, args.duration, channels):
            print("[LOGIC2] ✓ Capture configured")
        
        # Add UART decoders
        print(f"\n[LOGIC2] Adding UART decoders...")
        if logic2.add_uart_decoder(tx_channel=7, rx_channel=1, baud=19200, 
                                   bits=8, parity="ODD", stop_bits=1):
            print("[LOGIC2] ✓ UART decoder added (8O1)")
        
        # Start capture
        print(f"\n[LOGIC2] Starting capture...\n")
        if logic2.start_capture():
            print("[LOGIC2] ✓ Capture started")
            print(f"[LOGIC2] Capturing for {args.duration}...")
            time.sleep(logic2._parse_duration(args.duration) + 1)
            print("[LOGIC2] Capture complete")
        
        # Export if requested
        if args.export:
            print(f"\n[LOGIC2] Exporting to {args.export}...")
            if logic2.export_data(str(args.export)):
                print(f"[LOGIC2] ✓ Data exported")
        
        # Correlate if requested
        if args.correlate:
            print(f"\n[LOGIC2] Correlating with pySerial data...")
            correlator = DataCorrelator(args.export or Path("capture.csv"), args.correlate)
            if correlator.load_saleae_decode() and correlator.load_pyserial_log():
                correlator.correlate()
    
    finally:
        logic2.disconnect()


if __name__ == "__main__":
    main()
