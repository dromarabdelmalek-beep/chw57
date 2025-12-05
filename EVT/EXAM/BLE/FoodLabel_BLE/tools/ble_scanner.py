#!/usr/bin/env python3
"""
BLE Scanner for SmartFood Cold Chain Monitor
Scans for SmartFood devices and decodes their advertisement data
Requires: bleak library (pip install bleak)
"""

import asyncio
import struct
from bleak import BleakScanner
from datetime import datetime

# Food types lookup
FOOD_TYPES = [
    "CHICKEN", "BEEF", "FISH", "PORK",
    "SEAFOOD", "VEGETABLES", "DAIRY", "FRUITS"
]

# WCH Company ID
WCH_COMPANY_ID = 0x07D7

def decode_manufacturer_data(data):
    """
    Decode manufacturer-specific data from SmartFood device

    Format (after company ID):
    Byte 0:   Food type (0-7)
    Byte 1-2: Temperature (int16, in 0.1°F units)
    Byte 3:   Humidity (0-100%)
    Byte 4:   Days left (1-90)
    Byte 5:   Flags (bit 0 = cold chain broken)
    Byte 6:   Expiry day (1-31)
    Byte 7:   Expiry month (1-12)
    Byte 8:   Expiry year (25-30)
    """
    if len(data) < 9:
        return None

    try:
        food_idx = data[0]
        temp_raw = struct.unpack('<h', data[1:3])[0]
        humidity = data[3]
        days_left = data[4]
        flags = data[5]
        expiry_day = data[6]
        expiry_month = data[7]
        expiry_year = data[8]

        temp_f = temp_raw / 10.0
        temp_c = (temp_f - 32) * 5 / 9
        cold_chain_broken = bool(flags & 0x01)

        food_type = FOOD_TYPES[food_idx] if food_idx < len(FOOD_TYPES) else "UNKNOWN"
        status = "SPOILED" if cold_chain_broken else "FRESH"

        return {
            "food_type": food_type,
            "temperature_f": round(temp_f, 1),
            "temperature_c": round(temp_c, 1),
            "humidity": humidity,
            "days_left": days_left,
            "expiry_date": f"20{expiry_year:02d}-{expiry_month:02d}-{expiry_day:02d}",
            "cold_chain_broken": cold_chain_broken,
            "status": status,
            "raw_data": data.hex()
        }
    except Exception as e:
        print(f"Error decoding data: {e}")
        return None

def format_device_info(device, advertisement_data):
    """Format device information for display"""
    output = []
    output.append("\n" + "="*60)
    output.append(f"📱 Device Found: {device.name or 'Unknown'}")
    output.append(f"📍 Address: {device.address}")
    output.append(f"📶 RSSI: {advertisement_data.rssi} dBm")
    output.append("="*60)

    # Check manufacturer data
    if advertisement_data.manufacturer_data:
        for company_id, data in advertisement_data.manufacturer_data.items():
            output.append(f"\n🏢 Company ID: 0x{company_id:04X}")

            if company_id == WCH_COMPANY_ID:
                output.append("✅ SmartFood Device Detected!")

                decoded = decode_manufacturer_data(data)
                if decoded:
                    output.append("\n📊 Decoded Information:")
                    output.append(f"  🍖 Food Type: {decoded['food_type']}")
                    output.append(f"  🌡️  Temperature: {decoded['temperature_f']}°F ({decoded['temperature_c']}°C)")
                    output.append(f"  💧 Humidity: {decoded['humidity']}%")
                    output.append(f"  📅 Days Left: {decoded['days_left']}")
                    output.append(f"  📆 Expiry Date: {decoded['expiry_date']}")

                    status_emoji = "❌" if decoded['cold_chain_broken'] else "✅"
                    output.append(f"  {status_emoji} Status: {decoded['status']}")

                    if decoded['cold_chain_broken']:
                        output.append("  ⚠️  WARNING: Cold chain has been broken!")
                        output.append("  ⚠️  Product should be discarded!")

                    output.append(f"\n  📝 Raw Data: {decoded['raw_data']}")
                else:
                    output.append(f"  ⚠️  Failed to decode data: {data.hex()}")
            else:
                output.append(f"  📝 Data: {data.hex()}")

    # Service UUIDs
    if advertisement_data.service_uuids:
        output.append("\n🔧 Services:")
        for uuid in advertisement_data.service_uuids:
            output.append(f"  - {uuid}")

    return "\n".join(output)

async def scan_once():
    """Perform a single scan"""
    print("🔍 Scanning for SmartFood devices...")
    print("⏱️  Scan duration: 5 seconds")
    print("-" * 60)

    devices = await BleakScanner.discover(timeout=5.0)

    smartfood_found = False
    for device in devices:
        # Get advertisement data
        scanner = BleakScanner()
        async with scanner:
            await asyncio.sleep(0.1)
            if hasattr(scanner, '_advertisement_data_callback'):
                continue

        # Filter for SmartFood devices
        if device.name and "SmartFood" in device.name:
            smartfood_found = True
            # Need to scan again to get full advertisement data
            break

    if not smartfood_found:
        print("\n❌ No SmartFood devices found")
        print("💡 Make sure the device is powered on and advertising")

    return smartfood_found

async def continuous_scan():
    """Continuously scan and monitor devices"""
    print("🔄 Starting continuous monitoring...")
    print("⏱️  Updating every 2 seconds")
    print("🛑 Press Ctrl+C to stop")
    print("-" * 60)

    seen_devices = {}

    def detection_callback(device, advertisement_data):
        # Only process SmartFood devices or devices with WCH manufacturer data
        is_smartfood = (device.name and "SmartFood" in device.name)
        has_wch_data = WCH_COMPANY_ID in advertisement_data.manufacturer_data

        if is_smartfood or has_wch_data:
            info = format_device_info(device, advertisement_data)
            print(info)
            seen_devices[device.address] = {
                'last_seen': datetime.now(),
                'rssi': advertisement_data.rssi
            }

    async with BleakScanner(detection_callback=detection_callback) as scanner:
        try:
            while True:
                await asyncio.sleep(2)

                # Show summary of seen devices
                if seen_devices:
                    print(f"\n📊 Monitoring {len(seen_devices)} device(s)...")
        except KeyboardInterrupt:
            print("\n\n🛑 Stopped by user")

async def main():
    """Main entry point"""
    import sys

    print("\n" + "="*60)
    print("🍖 SmartFood BLE Scanner")
    print("="*60)

    if len(sys.argv) > 1 and sys.argv[1] == "--continuous":
        await continuous_scan()
    else:
        await scan_once()

        print("\n" + "="*60)
        print("💡 Tip: Use --continuous for live monitoring")
        print("   Example: python3 ble_scanner.py --continuous")
        print("="*60)

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\n\n👋 Goodbye!")
    except ImportError:
        print("\n❌ Error: bleak library not found")
        print("📦 Install with: pip install bleak")
    except Exception as e:
        print(f"\n❌ Error: {e}")
