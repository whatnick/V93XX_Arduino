#!/usr/bin/env python3
"""Plot and analyze V9360 UART waveform buffer samples."""

from __future__ import annotations

import argparse
from pathlib import Path
from typing import List

import matplotlib.pyplot as plt
import numpy as np


def _parse_samples(text: str) -> List[float]:
    tokens = [tok for tok in text.replace("\n", ",").split(",") if tok.strip()]
    return [float(tok.strip()) for tok in tokens]


def load_samples(path: Path | None) -> List[float]:
    if path is None:
        # Placeholder samples; replace with captured data if no file is supplied.
        return [0.0, 0.5, 1.0, 0.5, 0.0, -0.5, -1.0, -0.5]

    text = path.read_text(encoding="utf-8")
    return _parse_samples(text)


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Plot waveform samples and FFT from V9360 UART captures."
    )
    parser.add_argument(
        "--csv",
        type=Path,
        help="Path to a CSV/text file with one sample per line or comma-separated.",
    )
    args = parser.parse_args()

    wave_data = load_samples(args.csv)

    plt.figure("Waveform")
    plt.plot(wave_data)
    plt.title("V9360 Waveform Samples")
    plt.xlabel("Sample")
    plt.ylabel("Value")

    fft_data = np.fft.fft(wave_data)
    plt.figure("FFT")
    plt.plot(np.abs(fft_data))
    plt.title("FFT Magnitude")
    plt.xlabel("Bin")
    plt.ylabel("Magnitude")

    plt.show()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
