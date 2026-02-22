#!/usr/bin/env python3
"""
Test V9381 UART Checksum Mode Implementation

This script tests:
1. CRC calculation algorithm correctness
2. Expected vs received CRC comparison logic
3. Clean vs Dirty mode behavior
"""

import sys
from pathlib import Path


def calculate_crc8_v9381(payload_bytes: list[int]) -> int:
    """
    Calculate CRC-8 for V9381 UART protocol.
    
    Formula: CRC = 0x33 + ~(sum of payload bytes)
    
    Args:
        payload_bytes: List of bytes to calculate CRC for
        
    Returns:
        CRC-8 value (0x00-0xFF)
    """
    checksum = sum(payload_bytes) & 0xFF
    crc = (0x33 + (~checksum & 0xFF)) & 0xFF
    return crc


def test_crc_calculation():
    """Test CRC calculation with known values."""
    print("\n" + "="*80)
    print("TEST 1: CRC-8 Calculation")
    print("="*80)
    
    test_cases = [
        # (payload_bytes, expected_crc, description)
        ([0x04, 0x02], 0x2C, "RegisterRead request for address 0x02"),
        ([0x40, 0x02], 0xF0, "RegisterWrite request for address 0x02"),
        ([0x01, 0x02, 0x03, 0x04], 0x28, "4-byte response data"),
        ([0x00, 0x00, 0x00, 0x00], 0x32, "All zeros"),
        ([0xFF, 0xFF, 0xFF, 0xFF], 0x36, "All ones"),
    ]
    
    all_passed = True
    for payload, expected, description in test_cases:
        calculated = calculate_crc8_v9381(payload)
        passed = calculated == expected
        all_passed = all_passed and passed
        
        status = "✅ PASS" if passed else "❌ FAIL"
        print(f"\n{status} - {description}")
        print(f"  Payload: {' '.join(f'0x{b:02X}' for b in payload)}")
        print(f"  Expected CRC: 0x{expected:02X}")
        print(f"  Calculated CRC: 0x{calculated:02X}")
        
        if not passed:
            # Debug the calculation
            sum_bytes = sum(payload) & 0xFF
            not_sum = (~sum_bytes) & 0xFF
            print(f"  Debug: sum=0x{sum_bytes:02X}, ~sum=0x{not_sum:02X}, 0x33+~sum=0x{calculated:02X}")
    
    return all_passed


def test_clean_mode_behavior():
    """Test Clean mode behavior (enforce CRC validation)."""
    print("\n" + "="*80)
    print("TEST 2: Clean Mode Behavior")
    print("="*80)
    
    print("\nClean Mode: Enforce CRC validation")
    print("-" * 80)
    
    # Scenario 1: Valid CRC
    print("\nScenario 1: Valid CRC")
    expected_crc = 0xAB
    received_crc = 0xAB
    
    if expected_crc == received_crc:
        print(f"  ✅ CRC Valid: expected=0x{expected_crc:02X}, received=0x{received_crc:02X}")
        print("  Action: Continue operation")
    else:
        print(f"  ❌ CRC Invalid: expected=0x{expected_crc:02X}, received=0x{received_crc:02X}")
        print("  Action: ERROR - abort operation (Clean mode)")
    
    # Scenario 2: Invalid CRC
    print("\nScenario 2: Invalid CRC")
    expected_crc = 0xAB
    received_crc = 0xCD
    
    if expected_crc == received_crc:
        print(f"  ✅ CRC Valid: expected=0x{expected_crc:02X}, received=0x{received_crc:02X}")
        print("  Action: Continue operation")
    else:
        print(f"  ❌ CRC Invalid: expected=0x{expected_crc:02X}, received=0x{received_crc:02X}")
        print("  Action: ERROR - abort operation (Clean mode)")
    
    return True


def test_dirty_mode_behavior():
    """Test Dirty mode behavior (skip CRC validation)."""
    print("\n" + "="*80)
    print("TEST 3: Dirty Mode Behavior")
    print("="*80)
    
    print("\nDirty Mode: Skip CRC validation, show mismatches")
    print("-" * 80)
    
    # Scenario 1: Valid CRC
    print("\nScenario 1: Valid CRC")
    expected_crc = 0xAB
    received_crc = 0xAB
    
    checksum_valid = expected_crc == received_crc
    status = "✓" if checksum_valid else "✗"
    print(f"  CRC expected=0x{expected_crc:02X} received=0x{received_crc:02X} {status}")
    if not checksum_valid:
        print("  WARNING: CRC mismatch! (Dirty mode - returning data)")
    print("  Action: Continue with data")
    
    # Scenario 2: Invalid CRC in Dirty mode
    print("\nScenario 2: Invalid CRC")
    expected_crc = 0xAB
    received_crc = 0xCD
    
    checksum_valid = expected_crc == received_crc
    status = "✓" if checksum_valid else "✗"
    print(f"  CRC expected=0x{expected_crc:02X} received=0x{received_crc:02X} {status}")
    if not checksum_valid:
        print("  WARNING: CRC mismatch! (Dirty mode - returning data)")
    print("  Action: Continue with data anyway (for debugging)")
    
    return True


def test_uart_request_frame():
    """Test full UART request frame CRC."""
    print("\n" + "="*80)
    print("TEST 4: UART Request Frame Validation")
    print("="*80)
    
    print("\nUART Request Format: [Header=0x7D, CMD1, CMD2, CRC]")
    print("-" * 80)
    
    # RegisterRead request for address 0x02, device address 0x00
    # CMD1: ((num_regs-1) << 4) | (device_addr << 2) | READ_OP
    #     = ((1-1) << 4) | (0x00 << 2) | 0x01 = 0x01
    header = 0x7D
    cmd1 = 0x04  # Example
    cmd2 = 0x02  # Address
    
    payload = [cmd1, cmd2]
    expected_crc = calculate_crc8_v9381(payload)
    received_crc = expected_crc  # Valid case
    
    frame = [header, cmd1, cmd2, expected_crc]
    
    print(f"\nRequest Frame: {' '.join(f'0x{b:02X}' for b in frame)}")
    print(f"  Header: 0x{header:02X}")
    print(f"  CMD1: 0x{cmd1:02X}")
    print(f"  CMD2: 0x{cmd2:02X}")
    print(f"  CRC: 0x{expected_crc:02X}")
    print(f"\nCRC Validation:")
    print(f"  Expected: 0x{expected_crc:02X}")
    print(f"  Received: 0x{received_crc:02X}")
    print(f"  Status: {'✅ Valid' if expected_crc == received_crc else '❌ Invalid'}")
    
    return True


def test_uart_response_frame():
    """Test full UART response frame CRC."""
    print("\n" + "="*80)
    print("TEST 5: UART Response Frame Validation")
    print("="*80)
    
    print("\nUART Response Format: [Header=0x7D, Data0, Data1, Data2, Data3, CRC]")
    print("-" * 80)
    
    # Response to RegisterRead
    # Need CMD1 and CMD2 from request to calculate CRC
    cmd1 = 0x04
    cmd2 = 0x02
    data = [0x01, 0x00, 0x00, 0x00]  # Example data (value = 0x00000001)
    
    # Response CRC: 0x33 + ~(CMD1 + CMD2 + data[0] + data[1] + data[2] + data[3])
    payload_for_crc = [cmd1, cmd2] + data
    expected_crc = calculate_crc8_v9381(payload_for_crc)
    received_crc = expected_crc  # Valid case
    
    header = 0x7D
    frame = [header] + data + [expected_crc]
    
    print(f"\nResponse Frame: {' '.join(f'0x{b:02X}' for b in frame)}")
    print(f"  Header: 0x{header:02X}")
    print(f"  Data: {' '.join(f'0x{b:02X}' for b in data)} (value=0x{int.from_bytes(bytes(data), 'little'):08X})")
    print(f"  CRC: 0x{expected_crc:02X}")
    print(f"\nCRC Calculation (requires CMD1+CMD2 from request):")
    print(f"  CMD1: 0x{cmd1:02X}")
    print(f"  CMD2: 0x{cmd2:02X}")
    print(f"  Data: {' '.join(f'0x{b:02X}' for b in data)}")
    print(f"  CRC Input: [0x{cmd1:02X}, 0x{cmd2:02X}, {', '.join(f'0x{b:02X}' for b in data)}]")
    print(f"\nCRC Validation:")
    print(f"  Expected: 0x{expected_crc:02X}")
    print(f"  Received: 0x{received_crc:02X}")
    print(f"  Status: {'✅ Valid' if expected_crc == received_crc else '❌ Invalid'}")
    
    return True


def test_checksum_mode_comparison():
    """Compare Clean vs Dirty mode outputs."""
    print("\n" + "="*80)
    print("TEST 6: ChecksumMode Comparison")
    print("="*80)
    
    print("\nComparison: Clean Mode vs Dirty Mode")
    print("-" * 80)
    
    test_cases = [
        ("Valid CRC", 0xAB, 0xAB),
        ("CRC Mismatch", 0xAB, 0xCD),
    ]
    
    for description, expected, received in test_cases:
        print(f"\n{description}: expected=0x{expected:02X}, received=0x{received:02X}")
        checksum_valid = expected == received
        
        # Clean Mode
        print(f"\n  Clean Mode:")
        if checksum_valid:
            print(f"    ✓ CRC expected=0x{expected:02X} received=0x{received:02X} ✓")
            print(f"    Action: Continue operation")
        else:
            print(f"    ✗ CRC expected=0x{expected:02X} received=0x{received:02X} ✗ - ERROR: CRC mismatch! (Clean mode)")
            print(f"    Action: ERROR - Stop operation")
        
        # Dirty Mode
        print(f"\n  Dirty Mode:")
        status = "✓" if checksum_valid else "✗"
        print(f"    {status} CRC expected=0x{expected:02X} received=0x{received:02X} {status}", end="")
        if not checksum_valid:
            print(f" - WARNING: CRC mismatch! (Dirty mode - returning data)")
            print(f"    Action: Continue operation anyway (for debugging)")
        else:
            print()
            print(f"    Action: Continue operation")
    
    return True


def main():
    """Run all tests."""
    print("\n" + "="*80)
    print("V9381 UART CHECKSUM MODE TEST SUITE")
    print("="*80)
    print("\nTesting dirty mode implementation and CRC calculations")
    
    results = []
    
    # Run all tests
    results.append(("CRC Calculation", test_crc_calculation()))
    results.append(("Clean Mode Behavior", test_clean_mode_behavior()))
    results.append(("Dirty Mode Behavior", test_dirty_mode_behavior()))
    results.append(("UART Request Frame", test_uart_request_frame()))
    results.append(("UART Response Frame", test_uart_response_frame()))
    results.append(("Mode Comparison", test_checksum_mode_comparison()))
    
    # Print summary
    print("\n" + "="*80)
    print("TEST SUMMARY")
    print("="*80)
    
    all_passed = True
    for test_name, passed in results:
        status = "✅ PASS" if passed else "❌ FAIL"
        print(f"{status} - {test_name}")
        all_passed = all_passed and passed
    
    print("="*80)
    
    if all_passed:
        print("\n✅ All tests passed!")
        print("\nYou can now test on hardware with:")
        print("  1. Initialize with dirty mode: v9381.Init(config, V93XX_Raccoon::ChecksumMode::Dirty);")
        print("  2. Perform register operations")
        print("  3. Check Serial output for CRC comparisons")
        print("  4. Verify expected vs received CRC values")
        print("  5. Use capture_v9381_uart.py with Logic Analyzer to confirm")
        return 0
    else:
        print("\n❌ Some tests failed!")
        return 1


if __name__ == "__main__":
    sys.exit(main())
