#!/usr/bin/env python3
"""
Analyze V9381 UART Captures for Checksum Mode Verification

This script analyzes captured UART data (from capture_v9381_uart.py)
and verifies CRC calculations in both Clean and Dirty modes.

Usage:
    python analyze_checksum_captures.py [capture_directory]

If no directory specified, uses the most recent capture in tools/captures/
"""

import csv
import sys
import re
from pathlib import Path
from typing import Optional, Tuple, List


def calculate_crc8_v9381(payload_bytes: List[int]) -> int:
    """Calculate CRC-8 for V9381 UART protocol."""
    checksum = sum(payload_bytes) & 0xFF
    crc = (0x33 + (~checksum & 0xFF)) & 0xFF
    return crc


def decode_address(cmd1: int, cmd2: int) -> Tuple[int, str]:
    """Decode register address and operation from CMD1 and CMD2."""
    address = cmd2 & 0x7F  # 7-bit address
    
    # Extract operation from cmd1 bits [1:0]
    operation_bits = cmd1 & 0x03
    operations = {0: "BROADCAST", 1: "READ", 2: "WRITE", 3: "BLOCK"}
    operation = operations.get(operation_bits, "UNKNOWN")
    
    return address, operation


def parse_uart_csv(csv_file: Path) -> List[int]:
    """Parse UART CSV file and extract byte values."""
    frames = []
    
    try:
        with open(csv_file, 'r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            for row in reader:
                if row.get('error', '').strip():
                    continue
                
                if 'data' in row and row['data']:
                    try:
                        # Try to parse as hex or as character
                        data_str = row['data'].strip()
                        if data_str.startswith('0x'):
                            value = int(data_str, 16)
                        else:
                            # Try as ASCII character
                            value = ord(data_str[0]) if data_str else None
                        
                        if value is not None:
                            frames.append(value & 0xFF)
                    except (ValueError, IndexError):
                        continue
    except Exception as e:
        print(f"Error parsing CSV: {e}")
        return []
    
    return frames


def analyze_request_frame(frame_bytes: List[int], frame_num: int) -> dict:
    """Analyze a UART request frame [0x7D, CMD1, CMD2, CRC]."""
    result = {
        "type": "REQUEST",
        "number": frame_num,
        "valid": False,
        "bytes": frame_bytes,
        "crc_expected": None,
        "crc_received": None,
        "clean_mode_result": None,
        "dirty_mode_result": None,
    }
    
    if len(frame_bytes) < 4 or frame_bytes[0] != 0x7D:
        result["error"] = "Invalid frame header"
        return result
    
    marker, cmd1, cmd2, crc_rx = frame_bytes[:4]
    
    # Calculate expected CRC
    crc_expected = calculate_crc8_v9381([cmd1, cmd2])
    result["crc_expected"] = crc_expected
    result["crc_received"] = crc_rx
    result["valid"] = (crc_expected == crc_rx)
    
    address, operation = decode_address(cmd1, cmd2)
    result["address"] = f"0x{address:02X}"
    result["operation"] = operation
    
    # Determine behavior in each mode
    if result["valid"]:
        result["clean_mode_result"] = "CONTINUE"
        result["dirty_mode_result"] = "CONTINUE"
    else:
        result["clean_mode_result"] = "ERROR"
        result["dirty_mode_result"] = "WARNING (continue)"
    
    return result


def analyze_response_frame(frame_bytes: List[int], frame_num: int, cmd1: Optional[int] = None, 
                          cmd2: Optional[int] = None) -> dict:
    """Analyze a UART response frame [0x7D, Data0-3, CRC]."""
    result = {
        "type": "RESPONSE",
        "number": frame_num,
        "valid": False,
        "bytes": frame_bytes,
        "crc_expected": None,
        "crc_received": None,
        "clean_mode_result": None,
        "dirty_mode_result": None,
    }
    
    if len(frame_bytes) < 6 or frame_bytes[0] != 0x7D:
        result["error"] = "Invalid frame header or length"
        return result
    
    marker = frame_bytes[0]
    data = frame_bytes[1:5]
    crc_rx = frame_bytes[5]
    
    # Calculate expected CRC - need CMD1 and CMD2 from request
    if cmd1 is not None and cmd2 is not None:
        crc_expected = calculate_crc8_v9381([cmd1, cmd2] + list(data))
        result["crc_expected"] = crc_expected
        result["crc_received"] = crc_rx
        result["valid"] = (crc_expected == crc_rx)
        
        # Determine behavior in each mode
        if result["valid"]:
            result["clean_mode_result"] = "CONTINUE"
            result["dirty_mode_result"] = "CONTINUE"
        else:
            result["clean_mode_result"] = "ERROR"
            result["dirty_mode_result"] = "WARNING (continue)"
    else:
        result["error"] = "Cannot verify - no CMD1/CMD2 from request"
        result["crc_received"] = crc_rx
    
    # Convert data to 32-bit value
    data_value = int.from_bytes(bytes(data), byteorder='little')
    result["data"] = f"0x{data_value:08X}"
    
    return result


def analyze_uart_capture(csv_file: Path) -> List[dict]:
    """Analyze all frames in a UART capture."""
    frames = parse_uart_csv(csv_file)
    
    analyzed = []
    i = 0
    request_num = 1
    response_num = 1
    last_cmd1 = None
    last_cmd2 = None
    
    while i < len(frames):
        if frames[i] == 0x7D:  # Frame marker
            # Check if this looks like a request (4 bytes) or response (6 bytes)
            if i + 3 < len(frames):
                # Tentatively assume request
                potential_request = frames[i:i+4]
                
                # Check if next marker is within reasonable distance
                # Requests are typically 4 bytes, responses are 6 bytes
                next_marker_idx = None
                for j in range(i + 4, min(i + 10, len(frames))):
                    if frames[j] == 0x7D:
                        next_marker_idx = j
                        break
                
                if next_marker_idx is not None:
                    gap = next_marker_idx - i
                    
                    if gap == 4:  # Likely a request
                        result = analyze_request_frame(potential_request, request_num)
                        analyzed.append(result)
                        last_cmd1 = potential_request[1]
                        last_cmd2 = potential_request[2]
                        request_num += 1
                        i += 4
                    elif gap == 6:  # Likely a response
                        potential_response = frames[i:i+6]
                        result = analyze_response_frame(potential_response, response_num, last_cmd1, last_cmd2)
                        analyzed.append(result)
                        response_num += 1
                        i += 6
                    else:
                        i += 1
                else:
                    # Handle last frame(s)
                    if i + 5 < len(frames):
                        potential_response = frames[i:i+6]
                        result = analyze_response_frame(potential_response, response_num, last_cmd1, last_cmd2)
                        analyzed.append(result)
                        response_num += 1
                        i += 6
                    elif i + 3 < len(frames):
                        result = analyze_request_frame(potential_request, request_num)
                        analyzed.append(result)
                        last_cmd1 = potential_request[1]
                        last_cmd2 = potential_request[2]
                        request_num += 1
                        i += 4
                    else:
                        i += 1
            else:
                i += 1
        else:
            i += 1
    
    return analyzed


def print_analysis_report(csv_file: Path, analyzed: List[dict]) -> None:
    """Print a formatted analysis report."""
    print("\n" + "="*100)
    print(f"V9381 UART CAPTURE CHECKSUM ANALYSIS")
    print(f"File: {csv_file.name}")
    print("="*100)
    
    if not analyzed:
        print("\n❌ No valid UART frames found in capture")
        return
    
    print(f"\n✓ Analyzed {len(analyzed)} frames\n")
    
    # Summary statistics
    total_frames = len(analyzed)
    valid_crcs = sum(1 for f in analyzed if f.get("valid", False))
    invalid_crcs = sum(1 for f in analyzed if "error" not in f and not f.get("valid", False))
    errors = sum(1 for f in analyzed if "error" in f)
    
    print("SUMMARY:")
    print("-" * 100)
    print(f"  Total Frames: {total_frames}")
    print(f"  Valid CRCs: {valid_crcs} ({100*valid_crcs//total_frames if total_frames else 0}%)")
    print(f"  Invalid CRCs: {invalid_crcs} ({100*invalid_crcs//total_frames if total_frames else 0}%)")
    print(f"  Errors: {errors}")
    print()
    
    # Detailed frame analysis
    print("DETAILED FRAME ANALYSIS:")
    print("-" * 100)
    
    for idx, frame in enumerate(analyzed, 1):
        frame_type = frame.get("type", "UNKNOWN")
        
        if "error" in frame:
            print(f"\nFrame #{idx} ({frame_type}):")
            print(f"  ❌ ERROR: {frame['error']}")
            continue
        
        print(f"\nFrame #{idx} ({frame_type} #{frame.get('number', '?')}):")
        
        if frame_type == "REQUEST":
            print(f"  Address: {frame.get('address', '?')} | Operation: {frame.get('operation', '?')}")
            print(f"  Bytes: {' '.join(f'0x{b:02X}' for b in frame.get('bytes', [])[:4])}")
        elif frame_type == "RESPONSE":
            print(f"  Data: {frame.get('data', '?')}")
            print(f"  Bytes: {' '.join(f'0x{b:02X}' for b in frame.get('bytes', [])[:6])}")
        
        print(f"  CRC Analysis:")
        print(f"    Expected: 0x{frame.get('crc_expected', 0):02X}")
        print(f"    Received: 0x{frame.get('crc_received', 0):02X}")
        
        valid = frame.get("valid", False)
        status = "✅ VALID" if valid else "❌ MISMATCH"
        print(f"    Status: {status}")
        
        print(f"  Mode Behavior:")
        print(f"    Clean Mode: {frame.get('clean_mode_result', '?')}")
        print(f"    Dirty Mode: {frame.get('dirty_mode_result', '?')}")
    
    # Final assessment
    print("\n" + "="*100)
    print("ASSESSMENT:")
    print("-" * 100)
    
    if invalid_crcs == 0 and errors == 0:
        print("✅ All frames have valid CRCs!")
        print("   → Your V9381 device and CRC calculation are correctly implemented")
        print("   → Use Clean mode for production (enforce CRC)")
    elif invalid_crcs > 0:
        print(f"⚠️  Found {invalid_crcs} frames with CRC mismatches")
        print("   → Use Dirty mode to debug and identify the issue")
        print("   → Check:")
        print("     • UART wiring and signal integrity")
        print("     • Device firmware version")
        print("     • CRC calculation implementation")
    
    if errors > 0:
        print(f"\n❌ {errors} frames could not be analyzed properly")
        print("   → Check capture file format and UART settings")
    
    print("="*100 + "\n")


def find_latest_capture() -> Optional[Path]:
    """Find the most recent capture directory."""
    captures_dir = Path(__file__).parent / "captures"
    
    if not captures_dir.exists():
        return None
    
    uart_dirs = sorted(
        captures_dir.glob("v9381_uart_*"),
        key=lambda p: p.name,
        reverse=True
    )
    
    if uart_dirs:
        return uart_dirs[0]
    
    return None


def main():
    """Main entry point."""
    print("\n" + "="*100)
    print("V9381 UART CHECKSUM MODE - CAPTURE ANALYZER")
    print("="*100 + "\n")
    
    # Determine capture directory
    if len(sys.argv) > 1:
        capture_dir = Path(sys.argv[1])
    else:
        capture_dir = find_latest_capture()
        if capture_dir:
            print(f"Using latest capture: {capture_dir}\n")
        else:
            print("No capture directory specified and no captures found.")
            print("Usage: python analyze_checksum_captures.py [capture_directory]")
            return 1
    
    if not capture_dir.exists():
        print(f"❌ Capture directory not found: {capture_dir}")
        return 1
    
    # Look for UART CSV files
    rx_file = capture_dir / "uart_rx_noparity.csv"
    tx_file = capture_dir / "uart_tx_noparity.csv"
    
    if not rx_file.exists():
        print(f"❌ RX capture file not found: {rx_file}")
        return 1
    
    # Analyze RX (device responses)
    print("\n" + "="*100)
    print("ANALYZING RX DATA (Device Responses)")
    print("="*100)
    
    analyzed_rx = analyze_uart_capture(rx_file)
    print_analysis_report(rx_file, analyzed_rx)
    
    # Analyze TX (requests) if available
    if tx_file.exists():
        print("\n" + "="*100)
        print("ANALYZING TX DATA (Requests)")
        print("="*100)
        
        analyzed_tx = analyze_uart_capture(tx_file)
        print_analysis_report(tx_file, analyzed_tx)
    
    return 0


if __name__ == "__main__":
    sys.exit(main())
