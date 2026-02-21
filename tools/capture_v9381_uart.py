from __future__ import annotations

import datetime
from pathlib import Path
import subprocess

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

        print(f"Capture saved to {out_dir}")


if __name__ == "__main__":
    main()
