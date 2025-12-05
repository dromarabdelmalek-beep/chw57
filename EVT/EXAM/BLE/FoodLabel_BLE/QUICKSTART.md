# 🚀 Quick Start Guide - SmartFood Cold Chain Monitor

Get your CH57x Food Label up and running in 10 minutes!

## ⚡ Prerequisites (5 min)

### 1. Install Toolchain

#### Option A: MounRiver Studio (Recommended for Windows)
```bash
# Download from: http://www.mounriver.com/
# Install and add to PATH
```

#### Option B: ARM GCC Toolchain (Linux/Mac)
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install gcc-arm-none-eabi

# Mac (via Homebrew)
brew install arm-none-eabi-gcc

# Verify installation
arm-none-eabi-gcc --version
```

### 2. Clone Repository
```bash
git clone https://github.com/dromarabdelmalek-beep/chw57.git
cd chw57/EVT/EXAM/BLE/FoodLabel_BLE
```

## 🔌 Hardware Setup (3 min)

### Required Components
- [ ] CH570/571/572/573 development board
- [ ] 2.9" E-Paper display (SSD1680 controller)
- [ ] SHT40/41/45 temperature sensor
- [ ] 3x tactile buttons
- [ ] 2x LEDs (with 220Ω resistors)
- [ ] Breadboard and jumper wires
- [ ] 3.3V power supply

### Pin Connections

#### E-Paper Display (SPI)
```
Display    →  CH57x Pin
────────────────────────
VCC        →  3.3V
GND        →  GND
DIN(MOSI)  →  GPIO7
CLK        →  GPIO5
CS         →  GPIO4
DC         →  GPIO1
RST        →  GPIO2
BUSY       →  GPIO3
```

#### SHT4x Sensor (I2C)
```
Sensor     →  CH57x Pin
────────────────────────
VCC        →  3.3V
GND        →  GND
SDA        →  GPIO6
SCL        →  GPIO7
```

#### Buttons & LEDs
```
Component      →  CH57x Pin
──────────────────────────────
Button 1 (Field)    →  GPIO0 + 10kΩ pull-down to GND
Button 2 (Up)       →  GPIO10 + 10kΩ pull-down to GND
Button 3 (Down)     →  GPIO5 + 10kΩ pull-down to GND

Status LED          →  GPIO8 → 220Ω → GND
Waste LED           →  GPIO9 → 220Ω → GND
```

### Visual Wiring Check
```
        CH57x Board
    ┌──────────────────┐
    │                  │
    │  GPIO0  ──── [BTN1] ──┐
    │  GPIO5  ──── [BTN3] ──┤── to 3.3V (with pull-downs to GND)
    │  GPIO10 ──── [BTN2] ──┘
    │                  │
    │  GPIO1  ──── DC     │
    │  GPIO2  ──── RST    │── E-Paper
    │  GPIO3  ──── BUSY   │
    │  GPIO4  ──── CS     │
    │  GPIO5  ──── CLK    │
    │  GPIO7  ──── MOSI  ─┘
    │                  │
    │  GPIO6  ──── SDA ─┬── SHT4x
    │  GPIO7  ──── SCL ─┘
    │                  │
    │  GPIO8  ──── LED ────── Status
    │  GPIO9  ──── LED ────── Waste
    │                  │
    └──────────────────┘
```

## 🔨 Build Firmware (1 min)

```bash
# In FoodLabel_BLE directory
make clean
make

# Output files in build/:
# - FoodLabel_BLE.elf
# - FoodLabel_BLE.hex
# - FoodLabel_BLE.bin
```

### Troubleshooting Build Errors

**Error: `arm-none-eabi-gcc: command not found`**
```bash
# Add toolchain to PATH
export PATH=$PATH:/path/to/arm-toolchain/bin
```

**Error: `libCH572BLE_PERI.a: No such file`**
```bash
# Verify BLE library exists
ls ../../BLE/LIB/libCH572BLE_PERI.a
```

**Error: `Link.ld: No such file`**
```bash
# Verify linker script exists
ls ../../../SRC/Ld/Link.ld
```

## 💾 Flash to Device (1 min)

### Method 1: Using Makefile
```bash
make flash
```

### Method 2: WCHISPTool GUI
1. Open WCHISPTool
2. Connect CH57x via USB/debugger
3. Select `build/FoodLabel_BLE.hex`
4. Click "Download"

### Method 3: Command Line
```bash
wchisptool -f build/FoodLabel_BLE.hex
```

## ✅ First Boot Test

### Expected Behavior

**Step 1: Power On**
- Status LED should light up briefly
- E-paper display initializes

**Step 2: Splash Screen (3 seconds)**
```
┌─────────────────────────┐
│                         │
│     SmartFood          │
│  Cold Chain Monitor    │
│                         │
└─────────────────────────┘
```

**Step 3: Main Display**
```
┌──────────────────────────────────────┐
│ CHICKEN           ┌──────────┐       │
│                   │   TEMP   │       │
│                   │  ──────  │       │
│                   │   72°F   │       │
│                   └──────────┘       │
│                                      │
│ Created: 05-12-25                    │
│ Expires: 12-12-25                    │
│ Days left: 7                         │
│ Status: Fresh                [SL]    │
└──────────────────────────────────────┘
```

**Step 4: BLE Advertising**
- Open BLE scanner app (nRF Connect, LightBlue)
- Look for device "SmartFood"
- Verify manufacturer data (Company ID: 0x07D7)

## 🧪 Quick Tests

### Test 1: Button Response
```bash
# Press Button 1 (short press)
# Expected: No response (not in edit mode)

# Hold Button 1 (5 seconds)
# Expected: 5 LED blinks, enter edit mode

# In edit mode, press Button 1 (short)
# Expected: Product name highlighted

# Press Button 2
# Expected: Product changes (CHICKEN → BEEF → FISH → ...)

# Hold Button 1 (5 seconds) again
# Expected: 3 LED blinks, exit edit mode, display updates
```

### Test 2: Temperature Sensor
```bash
# Check serial output (if DEBUG enabled):
# Should see: "Temp: XX.X°F (YY.Y°C) | Humidity: ZZ.Z%"

# Warm the sensor with your finger
# Wait ~30 seconds
# Display should update if temp changed by >2°F
```

### Test 3: BLE Broadcast
```bash
# Use Python scanner (requires bleak library)
cd tools
pip3 install bleak
python3 ble_scanner.py

# Expected output:
# 📱 Device Found: SmartFood
# 📍 Address: XX:XX:XX:XX:XX:XX
# 📶 RSSI: -XX dBm
# ✅ SmartFood Device Detected!
# 📊 Decoded Information:
#   🍖 Food Type: CHICKEN
#   🌡️  Temperature: 72.0°F (22.2°C)
#   💧 Humidity: 45%
#   📅 Days Left: 7
#   📆 Expiry Date: 2025-12-12
#   ✅ Status: FRESH
```

### Test 4: Cold Chain Monitoring
```bash
# Heat sensor above 40°F (if CHICKEN selected)
# OR cool below 32°F

# Wait 10 minutes (next sensor reading)

# Expected:
# - Display shows "Status: SPOILED!"
# - Waste LED blinks every 5 minutes
# - BLE advertisement flag bit changes
```

## 📱 BLE Testing

### Using nRF Connect (Android/iOS)

1. **Open nRF Connect**
2. **Tap "SCAN"**
3. **Find "SmartFood"**
4. **Expand "RAW" data**
5. **Look for "Manufacturer Data"**
   - Company ID: `0x07D7`
   - Data: `00 01 f4 01 46 05 01 0c 0c 19` (example)

6. **Decode the data** (use `decode_advert.py`):
   ```bash
   python3 tools/decode_advert.py 0001f4014605010c0c19
   ```

### Using Python Scanner

**Single Scan:**
```bash
python3 tools/ble_scanner.py
```

**Continuous Monitoring:**
```bash
python3 tools/ble_scanner.py --continuous
```

## 🐛 Common Issues

### Issue: Display stays blank
**Cause**: SPI communication problem
**Fix**:
```bash
# Check connections
# Verify CS, DC, RST pins
# Try hardware reset: cycle power
```

### Issue: No BLE device visible
**Cause**: BLE not initialized
**Fix**:
```bash
# Check serial output for "BLE Initialized"
# Verify BLE library is linked
# Check MAC address configuration
```

### Issue: Sensor reads 0°F
**Cause**: I2C communication failure
**Fix**:
```bash
# Verify SDA/SCL connections
# Check I2C address (0x44)
# Add 4.7kΩ pull-up resistors
```

### Issue: Build fails
**Cause**: Missing dependencies
**Fix**:
```bash
# Verify all source files exist
ls APP/*.c Drivers/*.c
# Check library paths in Makefile
# Ensure SDK is at correct location (../../..)
```

## 📊 Configuration

### Change Food Type Default
Edit `APP/foodlabel.c`:
```c
FoodLabelState_t appState = {
    .currentFoodIndex = 0,  // 0=CHICKEN, 1=BEEF, etc.
    // ...
};
```

### Adjust Sensor Interval
Edit `APP/foodlabel.c`:
```c
#define SENSOR_READ_PERIOD  600000  // 10 minutes (in ms)
```

### Change BLE Advertising Interval
Edit `APP/foodlabel.c`:
```c
#define DEFAULT_ADVERTISING_INTERVAL  80  // 50ms (in 625us units)
```

### Enable Debug Output
Edit `APP/include/CONFIG.h`:
```c
#define DEBUG  1  // Enable UART debug output
```

## 🎯 Next Steps

1. **Optimize Power**: Enable sleep mode in CONFIG.h
2. **Customize Display**: Modify layout in `epaper_driver_full.c`
3. **Add Foods**: Extend `foodDatabase[]` array
4. **Mobile App**: Build React Native app to scan BLE
5. **Cloud Integration**: Add MQTT/HTTP gateway

## 📚 Documentation

- **Full README**: `README.md`
- **E-Paper Guide**: `Drivers/README_EPAPER.md`
- **BLE Scanner**: `tools/ble_scanner.py --help`
- **Decoder**: `tools/decode_advert.py --help`

## 🆘 Getting Help

**Check Serial Output:**
```bash
# Connect UART (GPIO2=TX, GPIO3=RX at 115200 baud)
screen /dev/ttyUSB0 115200
# OR
minicom -D /dev/ttyUSB0 -b 115200
```

**Enable Verbose Logging:**
```c
// In foodlabel.c, add more PRINT() statements
PRINT("State: %d\n", currentState);
```

**Test Individual Components:**
```bash
# Test SPI (blink CS pin)
# Test I2C (scan for devices)
# Test GPIO (toggle LEDs)
```

## ✅ Checklist

- [ ] Toolchain installed
- [ ] Repository cloned
- [ ] Hardware wired correctly
- [ ] Firmware built successfully
- [ ] Device flashed
- [ ] Splash screen appears
- [ ] Main display shows
- [ ] BLE advertising visible
- [ ] Buttons respond
- [ ] Temperature updates
- [ ] Edit mode works

---

**🎉 Congratulations! Your SmartFood monitor is ready!**

**Total time**: ~10 minutes
**Difficulty**: Intermediate
**Status**: Production Ready ✅

For advanced configuration and optimization, see the main README.md
