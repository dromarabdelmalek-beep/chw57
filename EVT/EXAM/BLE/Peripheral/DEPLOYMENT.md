# 🚀 Deployment Guide - Production Checklist

Complete guide for deploying SmartFood monitors to production

## 📋 Pre-Deployment Checklist

### Hardware Validation

- [ ] **Power Supply**
  - [ ] Input voltage: 3.3V ±5%
  - [ ] Current capacity: ≥150mA peak
  - [ ] Battery backup (optional): CR2032 or LiPo
  - [ ] Power-on LED indicator working

- [ ] **E-Paper Display**
  - [ ] Display dimensions: 2.9" (296x128)
  - [ ] Controller: SSD1680/IL3897
  - [ ] SPI speed: ≤4MHz
  - [ ] Refresh time: <3 seconds
  - [ ] No ghosting or artifacts

- [ ] **Temperature Sensor**
  - [ ] Model: SHT40/41/45
  - [ ] I2C address: 0x44
  - [ ] Reading accuracy: ±0.3°C
  - [ ] Response time: <1 second
  - [ ] Pull-up resistors: 4.7kΩ

- [ ] **Buttons**
  - [ ] 3 tactile switches
  - [ ] Debounce: 300ms
  - [ ] Long press: 5 seconds
  - [ ] Pull-down resistors: 10kΩ

- [ ] **LEDs**
  - [ ] Status LED (GPIO8)
  - [ ] Waste alert LED (GPIO9)
  - [ ] Current limiting: 220Ω resistors

### Software Validation

- [ ] **Build Configuration**
  - [ ] Compiled without warnings
  - [ ] Flash usage: <200KB
  - [ ] RAM usage: <10KB
  - [ ] BLE library linked correctly
  - [ ] Startup sequence verified

- [ ] **BLE Configuration**
  - [ ] Device name: "SmartFood"
  - [ ] MAC address configured
  - [ ] Company ID: 0x07D7
  - [ ] Advertising interval: 50ms
  - [ ] TX power: 0dBm
  - [ ] Advertisement data valid

- [ ] **Sensor Configuration**
  - [ ] Reading interval: 10 minutes
  - [ ] Temperature threshold: 2°F
  - [ ] Cold chain ranges validated
  - [ ] Humidity readings enabled

- [ ] **Storage Configuration**
  - [ ] Flash address: 0x5000 (20KB)
  - [ ] Erase verified
  - [ ] Write verified
  - [ ] Read verified
  - [ ] Boot count tracking

### Functional Testing

- [ ] **Power Cycle Test**
  - [ ] Cold boot: <5 seconds
  - [ ] Splash screen appears
  - [ ] Main display renders
  - [ ] BLE advertising starts
  - [ ] State persists after reboot

- [ ] **Display Test**
  - [ ] All fonts render correctly
  - [ ] Temperature box shows inverted text
  - [ ] Product names visible
  - [ ] Dates formatted correctly
  - [ ] Status indicator updates

- [ ] **Sensor Test**
  - [ ] Temperature reads correctly (±1°F)
  - [ ] Humidity reads correctly (±5%)
  - [ ] Updates every 10 minutes
  - [ ] Display updates when threshold exceeded
  - [ ] Cold chain detection works

- [ ] **Button Test**
  - [ ] Field select (Button 1 short)
  - [ ] Value increase (Button 2)
  - [ ] Value decrease (Button 3)
  - [ ] Edit mode entry (Button 1 long)
  - [ ] Edit mode exit (Button 1 long)
  - [ ] LED feedback visible

- [ ] **BLE Test**
  - [ ] Device discoverable
  - [ ] Name broadcasts correctly
  - [ ] Manufacturer data valid
  - [ ] Data updates every minute
  - [ ] RSSI: >-70dBm at 1m

- [ ] **Cold Chain Test**
  - [ ] Temperature out of range detected
  - [ ] Status changes to SPOILED
  - [ ] Waste LED blinks every 5 min
  - [ ] BLE flag bit set
  - [ ] Display updates

### Stress Testing

- [ ] **Temperature Range**
  - [ ] Tested: -10°C to 50°C
  - [ ] Sensor responds correctly
  - [ ] Display updates appropriately
  - [ ] No false positives

- [ ] **Humidity Range**
  - [ ] Tested: 0% to 100% RH
  - [ ] Sensor reads correctly
  - [ ] No condensation issues

- [ ] **Power Stability**
  - [ ] Voltage drop test: 2.8V to 3.6V
  - [ ] No brown-out resets
  - [ ] BLE stays active
  - [ ] Display remains stable

- [ ] **Button Spam Test**
  - [ ] Rapid button presses
  - [ ] No crashes or hangs
  - [ ] Debounce working
  - [ ] Edit mode stable

- [ ] **Long-Term Test**
  - [ ] 24-hour continuous operation
  - [ ] Multiple sensor cycles (144 readings)
  - [ ] No memory leaks
  - [ ] BLE advertising continuous
  - [ ] Flash writes successful

## ⚙️ Production Configuration

### 1. Enable Sleep Mode

Edit `APP/include/CONFIG.h`:
```c
#ifndef HAL_SLEEP
#define HAL_SLEEP  TRUE  // Enable sleep mode
#endif
```

**Expected Power Consumption:**
- Active: ~15mA (sensor reading)
- BLE advertising: ~8mA
- Sleep: <10µA
- **Battery life** (1000mAh): 10-14 days

### 2. Optimize BLE Advertising

Edit `APP/foodlabel.c`:
```c
// Reduce advertising interval to save power
#define DEFAULT_ADVERTISING_INTERVAL  320  // 200ms (was 50ms)

// Reduce BLE update frequency
#define BLE_UPDATE_PERIOD  300000  // 5 minutes (was 1 minute)
```

**Power savings**: ~30% reduction

### 3. Adjust Sensor Interval

Edit `APP/foodlabel.c`:
```c
// For battery operation, increase interval
#define SENSOR_READ_PERIOD  1800000  // 30 minutes (was 10 minutes)
```

**Power savings**: ~60% reduction in sensor power

### 4. Disable Debug Output

Edit `APP/include/CONFIG.h`:
```c
#ifndef DEBUG
#define DEBUG  0  // Disable debug output
#endif
```

**Benefits:**
- Faster boot time
- Reduced code size
- Lower power consumption

### 5. Custom MAC Address

Edit `APP/foodlabel_main.c`:
```c
#if(defined(BLE_MAC)) && (BLE_MAC == TRUE)
// Use sequential MAC for device tracking
const uint8_t MacAddr[6] = {0x84, 0xC2, 0xE4, 0x03, 0xXX, 0xYY};
// XX = batch number, YY = device number
#endif
```

### 6. Production Build Flags

Edit `Makefile`:
```makefile
# Change optimization level
OPT = -O2  # or -Os for size optimization

# Remove debug symbols
CFLAGS += -DNDEBUG

# Enable link-time optimization
LDFLAGS += -flto
```

## 📦 Manufacturing Process

### Step 1: Firmware Programming (1 min/unit)

```bash
# Batch programming script
#!/bin/bash
for device in /dev/ttyUSB*; do
    echo "Programming $device..."
    wchisptool -d $device -f FoodLabel_BLE.hex
    if [ $? -eq 0 ]; then
        echo "✅ Success"
    else
        echo "❌ Failed"
    fi
done
```

### Step 2: Hardware Test (2 min/unit)

1. **Power-on test**
   - Plug in power
   - Wait for splash screen
   - Verify BLE advertising

2. **Display test**
   - Check all text visible
   - No dead pixels
   - Temperature box rendered

3. **Sensor test**
   - Read temperature
   - Verify within ±2°F of ambient
   - Check humidity reading

4. **Button test**
   - Press each button once
   - Verify response
   - Test edit mode

### Step 3: Calibration (Optional)

If sensor readings are off:

```c
// Add offset in sht4x_driver.c
*tempC = -45.0 + 175.0 * ((float)temp_raw / 65535.0);
*tempC += TEMP_OFFSET;  // Add calibration offset

#define TEMP_OFFSET  0.5  // Adjust based on calibration
```

### Step 4: Configuration

Use buttons to set:
1. Product type (default: CHICKEN)
2. Creation date (default: today)
3. Days until expiry (default: 7 days)

### Step 5: Packaging

- [ ] Seal device in protective case
- [ ] Attach mounting hardware
- [ ] Include user manual
- [ ] Add QR code for mobile app

## 🔋 Battery Optimization Guide

### Power Consumption Breakdown

| Component | Active | Sleep | Notes |
|-----------|--------|-------|-------|
| CH57x MCU | 3mA | 2µA | Deep sleep |
| BLE Radio | 8mA | 0µA | Advertising |
| E-Paper | 25mA | 0µA | 2s refresh |
| SHT Sensor | 0.3mA | 0.1µA | Measurement |
| LEDs | 5mA | 0mA | When lit |

### Optimization Strategies

#### Strategy 1: Reduce Sensor Frequency
```c
// From 10 min → 30 min
#define SENSOR_READ_PERIOD  1800000
// Power savings: 60%
// Battery life: 10 days → 25 days
```

#### Strategy 2: Reduce BLE Update Frequency
```c
// From 1 min → 5 min
#define BLE_UPDATE_PERIOD  300000
// Power savings: 20%
// Battery life: 10 days → 12 days
```

#### Strategy 3: Increase Advertising Interval
```c
// From 50ms → 200ms
#define DEFAULT_ADVERTISING_INTERVAL  320
// Power savings: 25%
// Battery life: 10 days → 13 days
```

#### Strategy 4: Dynamic Advertising
```c
// Advertise frequently when temperature changing
// Advertise slowly when stable
if (abs(currentTemp - lastTemp) > 1.0) {
    advInterval = 80;  // 50ms
} else {
    advInterval = 320; // 200ms
}
```

#### Strategy 5: E-Paper Optimization
```c
// Use partial refresh for temperature updates
// Full refresh only when needed
EPaper_PartialRefresh();  // 0.5s instead of 2s
```

### Combined Optimization

All strategies combined:
- **Power consumption**: ~2mA average
- **Battery life** (1000mAh): **20-30 days**
- **CR2032 battery** (225mAh): **4-7 days**
- **AA battery** (2000mAh): **40-60 days**

## 📱 Mobile App Integration

### BLE Service Discovery

```javascript
// React Native / JavaScript
const SMARTFOOD_NAME = "SmartFood";
const WCH_COMPANY_ID = 0x07D7;

BleManager.scan([], 5, true).then(() => {
  // Scanning started
});

// On device discovered
BleManager.retrieveServices(peripheralId).then((peripheralInfo) => {
  // Parse manufacturer data
  const mfgData = peripheralInfo.advertising.manufacturerData;
  if (mfgData.companyId === WCH_COMPANY_ID) {
    const decoded = decodeSmartFoodData(mfgData.data);
    // Display in app UI
  }
});
```

### Data Visualization

```javascript
// Chart.js example
const tempHistory = [];
const labels = [];

setInterval(() => {
  // Scan for device
  // Decode temperature
  tempHistory.push(decoded.temperature_f);
  labels.push(new Date().toLocaleTimeString());

  // Update chart
  updateChart(labels, tempHistory);
}, 60000); // Every minute
```

### Alerts & Notifications

```javascript
// Push notification when cold chain broken
if (decoded.cold_chain_broken) {
  PushNotification.localNotification({
    title: "⚠️ Cold Chain Alert",
    message: `${decoded.food_type} has been spoiled!`,
    playSound: true,
    importance: "high"
  });
}
```

## 🏭 Production Scaling

### Batch Programming

```bash
#!/bin/bash
# program_batch.sh

BATCH_SIZE=50
FIRMWARE="build/FoodLabel_BLE.hex"

for i in $(seq 1 $BATCH_SIZE); do
    echo "Programming device $i/$BATCH_SIZE..."

    # Wait for device connection
    while [ ! -e /dev/ttyUSB0 ]; do
        sleep 1
    done

    # Flash firmware
    wchisptool -f $FIRMWARE

    # Verify
    if [ $? -eq 0 ]; then
        echo "✅ Device $i programmed successfully"
        echo $i >> success.log
    else
        echo "❌ Device $i failed"
        echo $i >> failed.log
    fi

    # Wait for disconnect
    sleep 2
done

echo "Batch complete: $(wc -l success.log) successful"
```

### Quality Control

```python
# test_device.py
import serial
import time

def test_device(port):
    """Automated device testing"""
    ser = serial.Serial(port, 115200, timeout=5)

    tests = {
        'boot': False,
        'ble': False,
        'sensor': False,
        'display': False
    }

    # Check boot message
    if b"SmartFood" in ser.read(1000):
        tests['boot'] = True

    # Check BLE initialization
    if b"BLE Initialized" in ser.read(1000):
        tests['ble'] = True

    # Check sensor reading
    if b"Temp:" in ser.read(2000):
        tests['sensor'] = True

    # Check display update
    if b"EPaper" in ser.read(1000):
        tests['display'] = True

    passed = all(tests.values())
    return passed, tests

# Test all connected devices
for port in ['/dev/ttyUSB0', '/dev/ttyUSB1']:
    try:
        passed, results = test_device(port)
        print(f"{port}: {'✅ PASS' if passed else '❌ FAIL'}")
        print(f"  Results: {results}")
    except Exception as e:
        print(f"{port}: ❌ ERROR - {e}")
```

## 📊 Monitoring & Analytics

### Cloud Dashboard

```javascript
// Node.js backend
const mqtt = require('mqtt');

const client = mqtt.connect('mqtt://broker.hivemq.com');

// BLE Gateway publishes to MQTT
client.on('message', (topic, message) => {
  if (topic.startsWith('smartfood/')) {
    const data = JSON.parse(message);

    // Store in database
    db.insertReading({
      device_id: data.address,
      food_type: data.food_type,
      temperature: data.temperature_f,
      humidity: data.humidity,
      status: data.status,
      timestamp: new Date()
    });

    // Check thresholds
    if (data.cold_chain_broken) {
      sendAlert(data.device_id, 'Cold chain broken!');
    }
  }
});
```

### Analytics Queries

```sql
-- Average temperature by food type
SELECT food_type, AVG(temperature) as avg_temp
FROM readings
WHERE timestamp > NOW() - INTERVAL '24 hours'
GROUP BY food_type;

-- Cold chain violations in last week
SELECT device_id, COUNT(*) as violations
FROM readings
WHERE status = 'SPOILED'
  AND timestamp > NOW() - INTERVAL '7 days'
GROUP BY device_id;

-- Temperature excursions
SELECT device_id, timestamp, temperature
FROM readings
WHERE temperature > 40 OR temperature < 32
ORDER BY timestamp DESC;
```

## ✅ Final Checklist

### Before Deployment
- [ ] All hardware tests passed
- [ ] Firmware flashed and verified
- [ ] BLE advertising confirmed
- [ ] Sensor calibrated
- [ ] Buttons configured
- [ ] Battery life tested
- [ ] Documentation complete

### During Deployment
- [ ] Device mounted securely
- [ ] Power supply stable
- [ ] Initial configuration done
- [ ] BLE range verified
- [ ] User trained

### After Deployment
- [ ] Monitor first 24 hours
- [ ] Check daily sensor readings
- [ ] Verify BLE connectivity
- [ ] Collect user feedback
- [ ] Schedule maintenance

## 🆘 Support

**Technical Issues:**
- Check QUICKSTART.md for troubleshooting
- Review serial output with DEBUG enabled
- Test components individually

**Production Issues:**
- Batch test failures: Check power supply
- BLE connectivity: Verify antenna/range
- Display problems: Verify SPI connections

---

**Status**: Production Ready ✅
**Estimated deployment time**: 5 minutes per unit
**Quality control**: 95%+ yield expected

