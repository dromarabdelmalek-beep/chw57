#!/usr/bin/env python3
"""
SmartFood Advertisement Data Decoder
Decodes raw manufacturer data from BLE advertisements
Usage: python3 decode_advert.py <hex_data>
Example: python3 decode_advert.py 0001f4014605010c0c19
"""

import sys
import struct

FOOD_TYPES = [
    "CHICKEN", "BEEF", "FISH", "PORK",
    "SEAFOOD", "VEGETABLES", "DAIRY", "FRUITS"
]

def decode_data(hex_string):
    """Decode hex string to SmartFood data"""
    # Remove spaces and convert to bytes
    hex_string = hex_string.replace(" ", "").replace("0x", "")

    try:
        data = bytes.fromhex(hex_string)
    except ValueError as e:
        print(f"❌ Error: Invalid hex string - {e}")
        return None

    if len(data) < 9:
        print(f"❌ Error: Data too short (need 9 bytes, got {len(data)})")
        return None

    # Decode fields
    food_idx = data[0]
    temp_raw = struct.unpack('<h', data[1:3])[0]
    humidity = data[3]
    days_left = data[4]
    flags = data[5]
    expiry_day = data[6]
    expiry_month = data[7]
    expiry_year = data[8]

    # Calculate values
    temp_f = temp_raw / 10.0
    temp_c = (temp_f - 32) * 5 / 9
    cold_chain_broken = bool(flags & 0x01)

    food_type = FOOD_TYPES[food_idx] if food_idx < len(FOOD_TYPES) else f"UNKNOWN({food_idx})"
    status = "SPOILED" if cold_chain_broken else "FRESH"

    return {
        "raw": hex_string,
        "food_type": food_type,
        "food_index": food_idx,
        "temperature_f": round(temp_f, 1),
        "temperature_c": round(temp_c, 1),
        "temp_raw": temp_raw,
        "humidity": humidity,
        "days_left": days_left,
        "flags": flags,
        "cold_chain_broken": cold_chain_broken,
        "expiry_day": expiry_day,
        "expiry_month": expiry_month,
        "expiry_year": expiry_year,
        "expiry_date": f"20{expiry_year:02d}-{expiry_month:02d}-{expiry_day:02d}",
        "status": status
    }

def print_decoded(data):
    """Pretty print decoded data"""
    print("\n" + "="*60)
    print("📦 SmartFood Advertisement Data Decoder")
    print("="*60)

    print(f"\n📝 Raw Data: {data['raw']}")
    print(f"   Length: {len(data['raw'])//2} bytes")

    print("\n📊 Decoded Information:")
    print("-" * 60)

    # Food type with emoji
    food_emoji = {"CHICKEN": "🐔", "BEEF": "🥩", "FISH": "🐟", "PORK": "🐷",
                  "SEAFOOD": "🦞", "VEGETABLES": "🥗", "DAIRY": "🥛", "FRUITS": "🍎"}
    emoji = food_emoji.get(data['food_type'], "🍽️")

    print(f"  {emoji} Food Type: {data['food_type']} (index: {data['food_index']})")
    print(f"  🌡️  Temperature: {data['temperature_f']}°F ({data['temperature_c']}°C)")
    print(f"     Raw value: {data['temp_raw']} (0.1°F units)")
    print(f"  💧 Humidity: {data['humidity']}%")
    print(f"  📅 Days Until Expiry: {data['days_left']} days")
    print(f"  📆 Expiry Date: {data['expiry_date']}")
    print(f"     (Day: {data['expiry_day']}, Month: {data['expiry_month']}, Year: 20{data['expiry_year']:02d})")

    status_emoji = "❌" if data['cold_chain_broken'] else "✅"
    print(f"  {status_emoji} Status: {data['status']}")
    print(f"     Flags: 0x{data['flags']:02X} (Cold chain broken: {data['cold_chain_broken']})")

    if data['cold_chain_broken']:
        print("\n⚠️  WARNING: Cold chain has been broken!")
        print("⚠️  Product should be discarded!")

    print("\n" + "="*60)

def generate_test_data(food_idx=0, temp_f=38.5, humidity=65, days_left=7):
    """Generate test advertisement data"""
    temp_raw = int(temp_f * 10)

    # Calculate expiry date (simplified - adds days to today)
    from datetime import datetime, timedelta
    expiry = datetime.now() + timedelta(days=days_left)

    cold_chain_broken = 0  # Fresh
    flags = cold_chain_broken

    data = struct.pack('<BhBBBBBB',
                       food_idx,
                       temp_raw,
                       humidity,
                       days_left,
                       flags,
                       expiry.day,
                       expiry.month,
                       expiry.year % 100)

    return data.hex()

def main():
    if len(sys.argv) < 2:
        print("📖 Usage: python3 decode_advert.py <hex_data>")
        print("\nExamples:")
        print("  python3 decode_advert.py 0001f4014605010c0c19")
        print("  python3 decode_advert.py '00 01 f4 01 46 05 01 0c 0c 19'")
        print("\nGenerate test data:")
        print("  python3 decode_advert.py --test")
        print("\nDecode test data:")
        test_hex = generate_test_data()
        print(f"  python3 decode_advert.py {test_hex}")
        return

    if sys.argv[1] == "--test":
        print("\n🔧 Generating test data...")
        print("\nTest Cases:")

        test_cases = [
            (0, 38.5, 65, 7, "CHICKEN, 38.5°F, Fresh"),
            (1, 35.0, 70, 5, "BEEF, 35.0°F, Fresh"),
            (2, 31.0, 80, 3, "FISH, 31.0°F, Fresh"),
            (5, 45.0, 60, 10, "VEGETABLES, 45.0°F, Fresh"),
        ]

        for food_idx, temp, hum, days, desc in test_cases:
            hex_data = generate_test_data(food_idx, temp, hum, days)
            print(f"\n  {desc}")
            print(f"  Hex: {hex_data}")

        print("\n💡 Copy any hex string above and decode it:")
        print("   python3 decode_advert.py <hex_string>")
        return

    hex_data = sys.argv[1]
    decoded = decode_data(hex_data)

    if decoded:
        print_decoded(decoded)
    else:
        print("\n❌ Failed to decode data")
        print("💡 Make sure the hex string is valid and contains at least 9 bytes")

if __name__ == "__main__":
    main()
