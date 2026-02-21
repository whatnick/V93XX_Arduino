from __future__ import annotations

import datetime
from pathlib import Path
import subprocess

from saleae import automation

# Capture settings
CAPTURE_SECONDS = 5.0
DIGITAL_SAMPLE_RATE = 8_000_000

# Logic channels
SPI_SCK_CHANNEL = 2
SPI_MOSI_CHANNEL = 7
SPI_MISO_CHANNEL = 1
SPI_CS_CHANNEL = 0

# SPI settings
SPI_BITS_PER_TRANSFER = 8
SPI_BIT_ORDER = "Most Significant Bit First (Standard)"
SPI_ENABLE_ACTIVE = "Enable line is Active Low (Standard)"

# ESP32 reset (uses Arduino-packaged esptool)
ESPTOOL_PATH = r"C:\Users\tisha\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\5.1.0\esptool.exe"
ESPTOOL_PORT = "COM7"


def main() -> None:
    timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    out_dir = Path(__file__).resolve().parent / "captures" / f"v9381_spi_{timestamp}"
    out_dir.mkdir(parents=True, exist_ok=True)

    with automation.Manager.connect(port=10430) as manager:
        devices = manager.get_devices()
        if not devices:
            raise RuntimeError("No Saleae devices found. Connect a device and try again.")

        device = devices[0]

        device_config = automation.LogicDeviceConfiguration(
            enabled_digital_channels=[SPI_SCK_CHANNEL, SPI_MOSI_CHANNEL, SPI_MISO_CHANNEL, SPI_CS_CHANNEL],
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

        # Reset after capture has started to avoid missing early SPI traffic.
        subprocess.run([ESPTOOL_PATH, "--port", ESPTOOL_PORT, "run"], check=False)

        capture.wait()

        # Save the raw capture for inspection in Logic 2.
        capture.save_capture(str(out_dir / "capture.sal"))

        # Export raw digital data for offline analysis.
        capture.export_raw_data_csv(
            str(out_dir),
            digital_channels=[SPI_SCK_CHANNEL, SPI_MOSI_CHANNEL, SPI_MISO_CHANNEL, SPI_CS_CHANNEL],
        )

        # Try to add a SPI analyzer and export decoded data.
        try:
            analyzer = capture.add_analyzer(
                "SPI",
                settings={
                    "MISO": SPI_MISO_CHANNEL,
                    "MOSI": SPI_MOSI_CHANNEL,
                    "Clock": SPI_SCK_CHANNEL,
                    "Enable": SPI_CS_CHANNEL,
                    "Bits per Transfer": SPI_BITS_PER_TRANSFER,
                    "Significant Bit": SPI_BIT_ORDER,
                    "Enable Line": SPI_ENABLE_ACTIVE,
                },
            )
            capture.export_data_table(str(out_dir / "spi.csv"), analyzers=[analyzer])
        except Exception as exc:  # noqa: BLE001 - best-effort analyzer export
            print(f"SPI analyzer export failed: {exc}")

        print(f"Capture saved to {out_dir}")


if __name__ == "__main__":
    main()
