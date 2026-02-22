import csv
import sys
from pathlib import Path


def parse_hex(value: str):
    if value is None:
        return None
    value = value.strip()
    if not value:
        return None
    if value.lower().startswith("0x"):
        return int(value, 16)
    return int(value, 16)


def checksum(cmd, data_bytes):
    total = (cmd + sum(data_bytes)) & 0xFF
    return (0x33 + (~total & 0xFF)) & 0xFF


def analyze(path: Path):
    frames = []
    current = []
    with path.open("r", newline="") as handle:
        reader = csv.DictReader(handle)
        for row in reader:
            row_type = (row.get("type") or "").strip().lower()
            if row_type == "enable":
                current = []
                continue
            if row_type == "disable":
                if current:
                    frames.append(current)
                current = []
                continue
            if row_type == "result":
                current.append(row)
        if current:
            frames.append(current)

    summary_rows = []
    valid = 0
    for idx, frame_rows in enumerate(frames, start=1):
        mosi = [parse_hex(r.get("mosi", "")) for r in frame_rows]
        miso = [parse_hex(r.get("miso", "")) for r in frame_rows]

        row = {
            "frame": idx,
            "count": len(frame_rows),
            "cmd_mosi": "",
            "data_miso": "",
            "checksum_expected": "",
            "checksum_miso": "",
            "checksum_ok": "",
            "has_data": "",
        }

        if len(mosi) >= 6 and len(miso) >= 6:
            cmd = mosi[0]
            data = miso[1:5]
            rx_ck = miso[5]
            row["cmd_mosi"] = f"0x{cmd:02X}" if cmd is not None else ""
            row["data_miso"] = " ".join(f"0x{b:02X}" for b in data if b is not None)
            exp = checksum(cmd or 0, [b or 0 for b in data])
            row["checksum_expected"] = f"0x{exp:02X}"
            row["checksum_miso"] = f"0x{rx_ck:02X}" if rx_ck is not None else ""
            ok = (rx_ck == exp)
            row["checksum_ok"] = "yes" if ok else "no"
            row["has_data"] = "yes" if any(b not in (0x00, 0xFF) for b in data if b is not None) else "no"
            if ok:
                valid += 1
        else:
            row["checksum_ok"] = "no"
            row["has_data"] = "no"

        summary_rows.append(row)

    out_path = path.with_name(path.stem + "_annotated.csv")
    with out_path.open("w", newline="") as handle:
        fieldnames = [
            "frame",
            "count",
            "cmd_mosi",
            "data_miso",
            "checksum_expected",
            "checksum_miso",
            "checksum_ok",
            "has_data",
        ]
        writer = csv.DictWriter(handle, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(summary_rows)

    total = len(summary_rows)
    print(f"Frames: {total}, checksum valid: {valid}")
    print(f"Annotated CSV: {out_path}")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python tools/analyze_spi_csv.py <path-to-csv>")
        sys.exit(1)
    analyze(Path(sys.argv[1]))
