# Smart Food Label - CH57x BLE Cold Chain Monitor

**Converted from ESP32-C3 to CH57x with BLE Broadcast**

## Overview

This project implements an advanced cold chain monitoring system for food products using the CH57x MCU. It combines a 2.9" e-paper display, SHT4x temperature/humidity sensor, and **BLE broadcast** to track food freshness and temperature compliance.

### Key Features

✅ **BLE Broadcast** - Temperature, humidity, and status data advertised via BLE
✅ **Cold Chain Monitoring** - Tracks if temperature stays within safe ranges
✅ **Auto-Calculate Expiry Dates** - Calculates expiration from creation date + shelf life
✅ **Low Power Operation** - Periodic sensor readings (configurable interval)
✅ **Persistent Storage** - Saves state to flash memory
✅ **E-Paper Display** - Low power display with partial refresh support
✅ **Waste Alert System** - LED indicator when product is spoiled
✅ **User Configuration** - Three-button interface for editing product details

---

## Hardware Requirements

### Components

| Component | Model | Quantity |
|-----------|-------|----------|
| Microcontroller | CH570/CH571/CH572/CH573 | 1 |
| E-Paper Display | 2.9" 296x128 (SSD1680/IL3897 compatible) | 1 |
| Temperature/Humidity Sensor | SHT40/SHT41/SHT45 | 1 |
| Buttons | Tactile switches | 3 |
| LEDs | Standard 5mm | 2 |
| Resistors | 220Ω (for LEDs), 10kΩ (for buttons) | 5 total |

### Pin Configuration

```
CH57x PIN MAPPING
=================

BUTTONS:
Pin GPIO0  ─[Button 1]─ 3.3V (+ 10kΩ pull-down) - FIELD SELECT
Pin GPIO5  ─[Button 3]─ 3.3V (+ 10kΩ pull-down) - VALUE DOWN
Pin GPIO10 ─[Button 2]─ 3.3V (+ 10kΩ pull-down) - VALUE UP

SHT SENSOR (I2C):
Pin GPIO6 (SDA) ─── SHT Sensor SDA
Pin GPIO7 (SCL) ─── SHT Sensor SCL
3.3V ─────────────── SHT Sensor VCC
GND ───────────────── SHT Sensor GND

LEDs:
Pin GPIO8  ─── Status LED ─── 220Ω ─── GND
Pin GPIO9  ─── Waste LED ─── 220Ω ─── GND (Blinks when spoiled)

E-PAPER DISPLAY (SPI):
CS   ─── GPIO4
DC   ─── GPIO1
RST  ─── GPIO2
BUSY ─── GPIO3
SCK  ─── GPIO5 (SPI Clock)
MOSI ─── GPIO7 (SPI MOSI)
GND  ─── GND
3.3V ─── 3.3V
```

---

## BLE Broadcast Format

The device broadcasts temperature, humidity, and food status via BLE advertisements.

### Advertisement Data Structure

```c
// Advertisement Packet (31 bytes max)
[Flags (3 bytes)]
[Manufacturer Specific Data (17 bytes)]
  ├─ Company ID: 0x07D7 (WCH Microelectronics)
  ├─ Food Type Index (1 byte): 0-7
  ├─ Temperature (2 bytes): int16, in 0.1°F units
  ├─ Humidity (1 byte): uint8, in %
  ├─ Days Left (1 byte): 1-90 days
  ├─ Flags (1 byte): bit 0 = cold chain broken
  ├─ Expiry Day (1 byte): 1-31
  ├─ Expiry Month (1 byte): 1-12
  └─ Expiry Year (1 byte): 25-30 (2025-2030)

// Scan Response Packet (31 bytes max)
[Local Name: "SmartFood"]
[TX Power Level: 0 dBm]
```

### Decoding Example (Python)

```python
import struct

def decode_food_label(manufacturer_data):
    """
    Decode manufacturer-specific data from BLE advertisement
    manufacturer_data: bytes starting after company ID (0x07D7)
    """
    food_types = ["CHICKEN", "BEEF", "FISH", "PORK",
                  "SEAFOOD", "VEGETABLES", "DAIRY", "FRUITS"]

    food_idx = manufacturer_data[0]
    temp_raw = struct.unpack('<h', manufacturer_data[1:3])[0]
    humidity = manufacturer_data[3]
    days_left = manufacturer_data[4]
    flags = manufacturer_data[5]
    expiry_day = manufacturer_data[6]
    expiry_month = manufacturer_data[7]
    expiry_year = manufacturer_data[8]

    temp_f = temp_raw / 10.0
    cold_chain_broken = bool(flags & 0x01)

    return {
        "food_type": food_types[food_idx],
        "temperature_f": temp_f,
        "temperature_c": (temp_f - 32) * 5/9,
        "humidity": humidity,
        "days_left": days_left,
        "expiry_date": f"20{expiry_year:02d}-{expiry_month:02d}-{expiry_day:02d}",
        "cold_chain_broken": cold_chain_broken,
        "status": "SPOILED" if cold_chain_broken else "FRESH"
    }
```

### BLE Scanner App Integration

Compatible with:
- **nRF Connect** (Android/iOS) - View raw advertisement data
- **LightBlue** (iOS) - View manufacturer data
- **Custom Apps** - Use Web Bluetooth API or native BLE APIs

---

## Food Database

The system includes 8 pre-configured food types with safe temperature ranges:

| Food Type | Shelf Life | Min Temp (°F) | Max Temp (°F) | Min Temp (°C) | Max Temp (°C) |
|-----------|------------|---------------|---------------|---------------|---------------|
| CHICKEN | 7 days | 32.0 | 40.0 | 0 | 4.4 |
| BEEF | 5 days | 32.0 | 40.0 | 0 | 4.4 |
| FISH | 3 days | 30.0 | 34.0 | -1.1 | 1.1 |
| PORK | 6 days | 32.0 | 40.0 | 0 | 4.4 |
| SEAFOOD | 2 days | 30.0 | 34.0 | -1.1 | 1.1 |
| VEGETABLES | 10 days | 32.0 | 50.0 | 0 | 10.0 |
| DAIRY | 4 days | 32.0 | 40.0 | 0 | 4.4 |
| FRUITS | 8 days | 32.0 | 45.0 | 0 | 7.2 |

---

## Software Architecture

```
┌─────────────────────────────────────────┐
│         TMOS Event Loop                 │
│  (Tiny Micros Operating System)         │
└─────────────────────────────────────────┘
           │
           ├─ FOODLABEL_SENSOR_READ_EVT (10 min)
           ├─ FOODLABEL_BLE_UPDATE_EVT (1 min)
           ├─ FOODLABEL_BUTTON_EVT (on interrupt)
           └─ FOODLABEL_WASTE_LED_EVT (5 min, if spoiled)

┌──────────────┬──────────────┬─────────────┐
│ BLE Stack    │ Sensor Layer │ Display     │
│ (Peripheral) │ (I2C/SPI)    │ (E-Paper)   │
└──────────────┴──────────────┴─────────────┘

Storage: Flash (20KB offset, 4KB block)
```

### Key Files

```
FoodLabel_BLE/
├── APP/
│   ├── foodlabel_main.c      # Main entry point, system init
│   ├── foodlabel.c           # Application logic, BLE broadcast
│   └── include/
│       └── foodlabel.h       # Application header
├── Drivers/
│   ├── epaper_driver.c       # E-paper display (STUB - needs implementation)
│   ├── sht4x_driver.c        # SHT4x I2C sensor driver
│   ├── button_driver.c       # Button handling with interrupts
│   ├── flash_storage.c       # Persistent storage
│   └── include/              # Driver headers
├── Profile/
│   └── include/
│       └── gattprofile.h     # GATT profile (for future expansion)
└── README.md                 # This file
```

---

## Building the Project

### Architecture

**CH57x MCU**: RISC-V RV32IMAC core (NOT ARM)
- RV32I: Base integer instruction set
- M: Integer multiplication and division
- A: Atomic instructions
- C: Compressed instructions

### Prerequisites

1. **RISC-V Toolchain** (riscv-none-embed-gcc)
   - **macOS**: `brew install xpack-riscv-none-embed-gcc`
   - **Linux**: Install from package manager or MounRiver Studio
   - **Windows**: MounRiver Studio (includes toolchain)
   - Download MounRiver Studio: http://www.mounriver.com/

2. **CH57x EVT SDK** (already present in this repository)

> 📖 **For detailed setup instructions**, see [QUICKSTART.md](QUICKSTART.md)

### Build Steps

#### Option 1: Using MounRiver Studio (Recommended)

1. Open MounRiver Studio
2. Import project: `File` → `Import` → `Existing Projects into Workspace`
3. Select directory: `EVT/EXAM/BLE/FoodLabel_BLE`
4. Build: `Project` → `Build All`
5. Flash: Connect debugger → `Run` → `Debug`

#### Option 2: Command Line Build

```bash
# Navigate to project directory
cd EVT/EXAM/BLE/FoodLabel_BLE

# Build (Makefile auto-detects RISC-V toolchain)
make clean
make

# Flash using WCHISPTool
make flash
# OR manually:
wchisptool -f build/FoodLabel_BLE.hex
```

---

## Configuration

### Adjustable Parameters

Edit `foodlabel.c` to customize:

```c
// Sensor reading interval (default: 10 minutes)
#define SENSOR_READ_PERIOD          600000  // ms

// Temperature change threshold for display update (default: 2°F)
#define TEMP_THRESHOLD              2.0f    // °F

// Waste LED blink interval (default: 5 minutes)
#define WASTE_LED_BLINK_PERIOD      300000  // ms

// BLE advertisement interval (default: 50ms)
#define DEFAULT_ADVERTISING_INTERVAL  80    // 625us units
```

### Food Database Customization

Edit the `foodDatabase` array in `foodlabel.c`:

```c
const FoodItem_t foodDatabase[] = {
    // {"TYPE", days, minTempF, maxTempF}
    {"CUSTOM",  14,  35.0, 45.0},  // Example: 2 weeks, 35-45°F
    // ... add more entries ...
};
```

---

## Usage Guide

### Initial Setup

1. **First Boot**:
   - Splash screen displays for 3 seconds
   - Default product: CHICKEN
   - Default expiry: 7 days from today

2. **BLE Advertising Starts Automatically**:
   - Device name: "SmartFood"
   - Advertisement interval: 50ms
   - Updates every 1 minute with latest sensor data

### Button Controls

#### Normal Mode

- **No interaction**: Device sleeps, wakes every 10 minutes to check temperature
- **Hold Button 1 (5 sec)**: Enter Edit Mode

#### Edit Mode

- **Button 1 (short press)**: Cycle through fields
  - Field 0: Product Type
  - Field 1: Created Day (DD)
  - Field 2: Created Month (MM)
  - Field 3: Created Year (YY)
  - Field 4: Days Left (shelf life)

- **Button 2 (short press)**: Increase value
- **Button 3 (short press)**: Decrease value
- **Hold Button 1 (5 sec)**: Save and exit Edit Mode

### Cold Chain Monitoring

The system automatically:

1. **Checks temperature** every 10 minutes
2. **Compares against safe range** for current product
3. **Sets "cold chain broken" flag** if out of range
4. **Updates BLE advertisement** with new status
5. **Blinks waste LED** every 5 minutes if spoiled
6. **Updates e-paper display** with "SPOILED" warning

---

## E-Paper Display Integration

⚠️ **IMPORTANT**: The e-paper driver is currently a **STUB**. You need to integrate your specific e-paper controller library.

### Recommended Libraries

1. **u8g2** - Lightweight, good for embedded systems
   - GitHub: https://github.com/olikraus/u8g2
   - Supports many e-paper controllers

2. **Custom Implementation**
   - Refer to your e-paper controller datasheet (SSD1680, IL3897, etc.)
   - Implement SPI commands in `epaper_driver.c`

3. **Port GxEPD2** (Advanced)
   - Original Arduino library
   - More work to port, but full-featured

### Integration Steps

1. Open `Drivers/epaper_driver.c`
2. Implement the SPI communication functions:
   - `EPaper_SendCommand()`
   - `EPaper_SendData()`
   - `EPaper_WaitUntilIdle()`
3. Add initialization sequence in `EPaper_Init()`
4. Implement drawing functions using your library's API
5. Refer to layout zones in the original ESP32 code

### Display Layout

```
┌─────────────────────────────────────────────────────────────┐
│ CHICKEN                                ┌──────────┐         │
│                                        │   TEMP   │         │
│                                        │  ──────  │         │
│                                        │   72°F   │         │
│                                        └──────────┘         │
│                                                              │
│ Created: 05-12-25                                           │
│ Expires: 12-12-25                                           │
│ Days left: 7                                                │
│ Status: Fresh                                               │
│                                                      [SL]    │
└─────────────────────────────────────────────────────────────┘
  296 x 128 pixels (landscape mode)
```

---

## Power Consumption

Typical power profile:

| State | Current | Notes |
|-------|---------|-------|
| Deep Sleep | < 10 μA | Between sensor readings |
| Active (sensor read) | ~15 mA | ~100ms every 10 min |
| BLE Advertising | ~8 mA | Continuous, 50ms interval |
| E-Paper Update | ~25 mA | ~2 sec for full refresh |

**Battery Life Estimate** (with 1000mAh battery):
- Continuous BLE advertising: ~125 hours (5 days)
- With optimizations (lower ad interval): ~10-14 days

---

## Troubleshooting

### BLE Not Advertising

- Check BLE stack initialization in `foodlabel_main.c`
- Verify `CH57x_BLEInit()` succeeds
- Use BLE scanner app to verify device is visible
- Check company ID: should be `0x07D7` in scan data

### Sensor Not Reading

- Verify I2C connections (SDA=GPIO6, SCL=GPIO7)
- Check I2C pull-up resistors (4.7kΩ recommended)
- Run I2C scan to detect sensor address (should be 0x44)
- Check power supply to sensor (3.3V)

### Display Not Working

- Verify SPI connections (CS, DC, RST, BUSY, CLK, MOSI)
- Check if e-paper driver is implemented (currently stub)
- Test SPI communication with simple commands
- Verify power supply to display (3.3V, check current draw)

### Flash Storage Errors

- Erase might fail if flash is write-protected
- Try full chip erase using WCHISPTool
- Flash address must be 4KB-aligned for erase operations
- Verify flash size is sufficient (256KB on CH572/573)

---

## Future Enhancements

Potential improvements:

- [ ] **GATT Services** - Allow mobile app to read/write configuration
- [ ] **OTA Updates** - Firmware updates over BLE
- [ ] **NFC Tag** - Quick scan for product info
- [ ] **Multiple Sensors** - Support multiple SHT sensors
- [ ] **Cloud Integration** - Upload data via BLE gateway
- [ ] **QR Code Display** - Generate QR code with product URL
- [ ] **Battery Monitoring** - ADC for battery level
- [ ] **Low Power Optimization** - Dynamic advertising interval

---

## License

This project is based on WCH CH57x EVT examples and follows the same license:

```
Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
Attention: This software (modified or not) and binary are used for
microcontroller manufactured by Nanjing Qinheng Microelectronics.
```

Original ESP32-C3 concept converted to CH57x by the community.

---

## References

### Documentation

- **CH57x Datasheet**: `/Datasheet/CH572DS1_en.PDF`
- **BLE Development Guide**: `/EVT/EXAM/BLE/沁恒低功耗蓝牙软件开发参考手册.PDF`
- **I2C Guide**: `/EVT/EXAM/I2C/I2C接口使用指南.PDF`

### Example Code

- **BLE Peripheral**: `/EVT/EXAM/BLE/Peripheral/`
- **SPI Example**: `/EVT/EXAM/SPI/src/Main.c`
- **I2C Example**: `/EVT/EXAM/I2C/src/app_i2c.c`
- **Flash Example**: `/EVT/EXAM/FLASH/src/Main.c`

### External Resources

- **WCH Official**: http://www.wch.cn/
- **MounRiver Studio**: http://www.mounriver.com/
- **SHT4x Datasheet**: https://www.sensirion.com/sht4x/

---

## Support

For issues and questions:

1. Check the **Troubleshooting** section above
2. Refer to CH57x EVT examples in `/EVT/EXAM/`
3. Consult CH57x datasheets and development guides
4. Open an issue on the repository

---

## Changelog

**v1.0 (2025-12-05)**
- Initial conversion from ESP32-C3 to CH57x
- BLE broadcast support added
- SHT4x I2C driver implemented
- Flash storage implemented
- Button driver with GPIO interrupts implemented
- E-paper driver stub (needs integration)
- Comprehensive documentation

---

**Made with ❤️ for the CH57x community**
