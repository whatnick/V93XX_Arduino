from __future__ import annotations

import csv
import datetime
from pathlib import Path
import subprocess
import sys

from saleae import automation

# Capture settings
CAPTURE_SECONDS = 5.0
DIGITAL_SAMPLE_RATE = 4_000_000

# Logic channels (shared with SPI wiring)
UART_RX_CHANNEL = 1  # V9381 TX/MISO -> ESP32 RX
UART_TX_CHANNEL = 7  # ESP32 TX/MOSI -> V9381 RX
A0_CHANNEL = 2        # A0/SCK
A1_CHANNEL = 0        # A1/CS

# UART settings
UART_BAUD = 19200
UART_BITS = 8
UART_STOP_BITS = 1
UART_PARITY = "Odd Parity Bit"
UART_PARITY_NOP = "No Parity Bit (Standard)"
UART_LSB_FIRST = True
UART_INVERT = False
UART_MODE = "Normal"

# Optional channel scan to locate UART TX line.
SCAN_CHANNELS = [0, 1, 2, 7]
SCAN_INVERTED = True

# ESP32 reset (uses Arduino-packaged esptool)
ESPTOOL_PATH = r"C:\Users\tisha\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\5.1.0\esptool.exe"
ESPTOOL_PORT = "COM7"


def calculate_checksum(cmd1: int, cmd2: int, data_bytes: list[int] | None = None) -> int:
    """
    Calculate checksum per V9381 datasheet.
    
    For request: CKSUM = 0x33 + ~(CMD1 + CMD2)
    For response: CKSUM = 0x33 + ~(CMD1 + CMD2 + Data[0] + Data[1] + Data[2] + Data[3])
    """
    if data_bytes is None:
        # Request checksum
        sum_val = cmd1 + cmd2
    else:
        # Response checksum
        sum_val = cmd1 + cmd2 + sum(data_bytes)
    
    checksum = (0x33 + (~sum_val & 0xFF)) & 0xFF
    return checksum


def decode_address(cmd1: int, cmd2: int) -> tuple[int, bool]:
    """Decode register address from CMD1 and CMD2."""
    address_lower = (cmd2 >> 1)  # Remove R/W bit
    address = (cmd1 << 8) | address_lower
    is_read = (cmd2 & 0x01) == 1
    return address, is_read


def analyze_uart_frames(csv_file: Path) -> dict:
    """
    Analyze UART frames from exported CSV and verify checksums.
    
    Returns dict with analysis results.
    """
    if not csv_file.exists():
        return {"error": f"File not found: {csv_file}"}
    
    try:
        with open(csv_file, 'r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            rows = list(reader)
        
        if not rows:
            return {"error": "CSV file is empty"}
        
        # Parse frames from CSV
        # Saleae Async Serial analyzer exports data as characters in 'data' column
        frames = []
        for row in rows:
            # Skip rows with errors (framing errors, etc.)
            if row.get('error', '').strip():
                continue
                
            # Extract data value
            if 'data' in row and row['data']:
                data_str = row['data'].strip()
                if data_str:
                    # Convert character to byte value
                    try:
                        value = ord(data_str[0])  # Take first character
                        frames.append(value)
                    except (ValueError, IndexError):
                        continue
        
        return {
            "success": True,
            "frame_count": len(frames),
            "frames": frames
        }
    
    except Exception as e:
        return {"error": f"Analysis failed: {e}"}


def print_frame_analysis(tx_csv: Path, rx_csv: Path) -> None:
    """Print detailed analysis of captured UART frames."""
    print("\n" + "=" * 80)
    print("V9381 UART FRAME ANALYSIS")
    print("=" * 80)
    
    # Analyze TX (requests)
    print("\n[TX] Request Frames (ESP32 → V9381)")
    print("-" * 80)
    tx_result = analyze_uart_frames(tx_csv)
    
    if "error" in tx_result:
        print(f"❌ {tx_result['error']}")
    else:
        frames = tx_result.get("frames", [])
        print(f"✓ Captured {len(frames)} bytes")
        
        # Group into request frames (4 bytes: marker, cmd1, cmd2, checksum)
        i = 0
        req_num = 1
        while i + 3 < len(frames):
            if frames[i] == 0x7D:  # Marker byte
                marker = frames[i]
                cmd1 = frames[i + 1]
                cmd2 = frames[i + 2]
                cksum = frames[i + 3]
                
                # Decode address
                address, is_read = decode_address(cmd1, cmd2)
                
                # Verify checksum
                expected_cksum = calculate_checksum(cmd1, cmd2)
                valid = "✅" if expected_cksum == cksum else "❌"
                
                print(f"\nRequest #{req_num}:")
                print(f"  Frame: {marker:02X} {cmd1:02X} {cmd2:02X} {cksum:02X}")
                print(f"  Address: 0x{address:04X} ({address})")
                print(f"  Operation: {'READ' if is_read else 'WRITE'}")
                print(f"  Checksum: 0x{cksum:02X} (expected: 0x{expected_cksum:02X}) {valid}")
                
                req_num += 1
                i += 4
            else:
                i += 1
    
    # Analyze RX (responses)
    print("\n[RX] Response Frames (V9381 → ESP32)")
    print("-" * 80)
    rx_result = analyze_uart_frames(rx_csv)
    
    if "error" in rx_result:
        print(f"❌ {rx_result['error']}")
    else:
        frames = rx_result.get("frames", [])
        print(f"✓ Captured {len(frames)} bytes")
        
        # Group into response frames (6 bytes: marker, data0-3, checksum)
        # Need to match with TX requests to get CMD1/CMD2 for checksum verification
        i = 0
        resp_num = 1
        while i + 5 < len(frames):
            if frames[i] == 0x7D:  # Marker byte
                marker = frames[i]
                data = [frames[i + 1], frames[i + 2], frames[i + 3], frames[i + 4]]
                cksum = frames[i + 5]
                
                print(f"\nResponse #{resp_num}:")
                print(f"  Frame: {marker:02X} {data[0]:02X} {data[1]:02X} {data[2]:02X} {data[3]:02X} {cksum:02X}")
                print(f"  Data: 0x{data[0]:02X}{data[1]:02X}{data[2]:02X}{data[3]:02X}")
                print(f"  Checksum: 0x{cksum:02X}")
                print(f"  ⚠ Note: Need corresponding request CMD1/CMD2 to verify checksum")
                
                resp_num += 1
                i += 6
            else:
                i += 1
    
    print("\n" + "=" * 80)
    print("VERIFICATION SUMMARY")
    print("=" * 80)
    print("✅ Capture completed successfully")
    print("✅ UART frames detected and parsed")
    print("📋 Review the data above to verify communication")
    print("\nNOTE: Response checksums require CMD1+CMD2 from the corresponding")
    print("      request frame. Match request/response pairs manually for full")
    print("      verification, or use manual analysis with known test data.")
    print("=" * 80 + "\n")


def main() -> None:
    timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    out_dir = Path(__file__).resolve().parent / "captures" / f"v9381_uart_{timestamp}"
    out_dir.mkdir(parents=True, exist_ok=True)

    with automation.Manager.connect(port=10430) as manager:
        devices = manager.get_devices()
        if not devices:
            raise RuntimeError("No Saleae devices found. Connect a device and try again.")

        device = devices[0]

        device_config = automation.LogicDeviceConfiguration(
            enabled_digital_channels=[UART_RX_CHANNEL, UART_TX_CHANNEL, A0_CHANNEL, A1_CHANNEL],
            digital_sample_rate=DIGITAL_SAMPLE_RATE,
        )

        capture_config = automation.CaptureConfiguration(
            capture_mode=automation.TimedCaptureMode(CAPTURE_SECONDS)
        )

        capture = manager.start_capture(
            device_configuration=device_config,
            device_id=device.device_id,
            capture_configuration=capture_config,
        )

        # Reset after capture has started to avoid missing early UART traffic.
        subprocess.run([ESPTOOL_PATH, "--port", ESPTOOL_PORT, "run"], check=False)

        capture.wait()

        # Save the raw capture for inspection in Logic 2.
        capture.save_capture(str(out_dir / "capture.sal"))

        # Export raw digital data for offline analysis.
        capture.export_raw_data_csv(
            str(out_dir),
            digital_channels=[UART_RX_CHANNEL, UART_TX_CHANNEL, A0_CHANNEL, A1_CHANNEL],
        )

        # Try to add an async serial analyzer and export decoded data.
        try:
            analyzer = capture.add_analyzer(
                "Async Serial",
                settings={
                    "Input Channel": UART_RX_CHANNEL,
                    "Bit Rate (Bits/s)": UART_BAUD,
                    "Bits per Frame": UART_BITS,
                    "Stop Bits": UART_STOP_BITS,
                    "Parity Bit": UART_PARITY,
                    "Significant Bit": "Least Significant Bit Sent First (Standard)"
                    if UART_LSB_FIRST
                    else "Most Significant Bit Sent First",
                    "Signal inversion": "Inverted" if UART_INVERT else "Non Inverted (Standard)",
                    "Mode": UART_MODE,
                },
            )
            capture.export_data_table(str(out_dir / "uart_rx.csv"), analyzers=[analyzer])

            analyzer_rx_nop = capture.add_analyzer(
                "Async Serial",
                settings={
                    "Input Channel": UART_RX_CHANNEL,
                    "Bit Rate (Bits/s)": UART_BAUD,
                    "Bits per Frame": UART_BITS,
                    "Stop Bits": UART_STOP_BITS,
                    "Parity Bit": UART_PARITY_NOP,
                    "Significant Bit": "Least Significant Bit Sent First (Standard)"
                    if UART_LSB_FIRST
                    else "Most Significant Bit Sent First",
                    "Signal inversion": "Inverted" if UART_INVERT else "Non Inverted (Standard)",
                    "Mode": UART_MODE,
                },
            )
            capture.export_data_table(
                str(out_dir / "uart_rx_noparity.csv"), analyzers=[analyzer_rx_nop]
            )

            analyzer_tx = capture.add_analyzer(
                "Async Serial",
                settings={
                    "Input Channel": UART_TX_CHANNEL,
                    "Bit Rate (Bits/s)": UART_BAUD,
                    "Bits per Frame": UART_BITS,
                    "Stop Bits": UART_STOP_BITS,
                    "Parity Bit": UART_PARITY,
                    "Significant Bit": "Least Significant Bit Sent First (Standard)"
                    if UART_LSB_FIRST
                    else "Most Significant Bit Sent First",
                    "Signal inversion": "Inverted" if UART_INVERT else "Non Inverted (Standard)",
                    "Mode": UART_MODE,
                },
            )
            capture.export_data_table(str(out_dir / "uart_tx.csv"), analyzers=[analyzer_tx])

            analyzer_tx_nop = capture.add_analyzer(
                "Async Serial",
                settings={
                    "Input Channel": UART_TX_CHANNEL,
                    "Bit Rate (Bits/s)": UART_BAUD,
                    "Bits per Frame": UART_BITS,
                    "Stop Bits": UART_STOP_BITS,
                    "Parity Bit": UART_PARITY_NOP,
                    "Significant Bit": "Least Significant Bit Sent First (Standard)"
                    if UART_LSB_FIRST
                    else "Most Significant Bit Sent First",
                    "Signal inversion": "Inverted" if UART_INVERT else "Non Inverted (Standard)",
                    "Mode": UART_MODE,
                },
            )
            capture.export_data_table(
                str(out_dir / "uart_tx_noparity.csv"), analyzers=[analyzer_tx_nop]
            )

            for channel in SCAN_CHANNELS:
                analyzer_scan = capture.add_analyzer(
                    "Async Serial",
                    settings={
                        "Input Channel": channel,
                        "Bit Rate (Bits/s)": UART_BAUD,
                        "Bits per Frame": UART_BITS,
                        "Stop Bits": UART_STOP_BITS,
                        "Parity Bit": UART_PARITY_NOP,
                        "Significant Bit": "Least Significant Bit Sent First (Standard)"
                        if UART_LSB_FIRST
                        else "Most Significant Bit Sent First",
                        "Signal inversion": "Inverted" if UART_INVERT else "Non Inverted (Standard)",
                        "Mode": UART_MODE,
                    },
                )
                capture.export_data_table(
                    str(out_dir / f"uart_ch{channel}_noparity.csv"),
                    analyzers=[analyzer_scan],
                )

                if SCAN_INVERTED:
                    analyzer_scan_inv = capture.add_analyzer(
                        "Async Serial",
                        settings={
                            "Input Channel": channel,
                            "Bit Rate (Bits/s)": UART_BAUD,
                            "Bits per Frame": UART_BITS,
                            "Stop Bits": UART_STOP_BITS,
                            "Parity Bit": UART_PARITY_NOP,
                            "Significant Bit": "Least Significant Bit Sent First (Standard)"
                            if UART_LSB_FIRST
                            else "Most Significant Bit Sent First",
                            "Signal inversion": "Inverted",
                            "Mode": UART_MODE,
                        },
                    )
                    capture.export_data_table(
                        str(out_dir / f"uart_ch{channel}_inv_noparity.csv"),
                        analyzers=[analyzer_scan_inv],
                    )
        except Exception as exc:  # noqa: BLE001 - best-effort analyzer export
            print(f"UART analyzer export failed: {exc}")

        print(f"\n✓ Capture saved to {out_dir}")
        
        # Analyze captured frames
        tx_csv = out_dir / "uart_tx_noparity.csv"
        rx_csv = out_dir / "uart_rx_noparity.csv"
        
        if tx_csv.exists() and rx_csv.exists():
            print_frame_analysis(tx_csv, rx_csv)
        else:
            print("\n⚠ Analyzer CSV files not found. Skipping frame analysis.")
            print(f"  Expected: {tx_csv}")
            print(f"           {rx_csv}")


if __name__ == "__main__":
    main()
