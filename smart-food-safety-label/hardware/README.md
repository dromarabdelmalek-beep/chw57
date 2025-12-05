# Hardware Documentation

## Smart Food-Safety Label Hardware Design

### Block Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                    WCH CH572D MCU                           │
│                                                             │
│  ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐  │
│  │   BLE    │  │  TMOS    │  │   SPI    │  │   I2C    │  │
│  │  Stack   │  │  RTOS    │  │ Driver   │  │ Driver   │  │
│  └──────────┘  └──────────┘  └──────────┘  └──────────┘  │
└─────────────────────────────────────────────────────────────┘
      │              │              │              │
      │              │              │              │
      ▼              ▼              ▼              ▼
┌─────────┐    ┌─────────┐   ┌──────────┐   ┌──────────┐
│   BLE   │    │ Buttons │   │ E-Paper  │   │  SHT4x   │
│Antenna  │    │ x3      │   │GDEY029T94│   │  Temp    │
└─────────┘    └─────────┘   └──────────┘   └──────────┘
                                   │
                              ┌─────────┐
                              │WS2812   │
                              │RGB LED  │
                              └─────────┘
```

## Pin Assignments (CH572D)

### GPIO Assignments

| Pin | Function | Component | Notes |
|-----|----------|-----------|-------|
| PA6 | GPIO Input | EDIT Button | Pull-up, debounced |
| PA7 | GPIO Input | UP Button | Pull-up, debounced |
| PA8 | GPIO Input | DOWN Button | Pull-up, debounced |
| PA9 | PWM/GPIO | WS2812 Data | Timing-critical |
| PA10 | SPI CS | E-Paper CS | Active low |
| PA11 | GPIO | E-Paper DC | Data/Command select |
| PA12 | GPIO | E-Paper RST | Reset, active low |
| PA13 | GPIO Input | E-Paper BUSY | Status input |
| PA14 | I2C SDA | SHT4x SDA | Temperature sensor |
| PA15 | I2C SCL | SHT4x SCL | Temperature sensor |
| PB4 | SPI SCK | E-Paper CLK | SPI clock |
| PB5 | SPI MOSI | E-Paper DIN | SPI data out |

### Power Pins

| Pin | Function | Connection |
|-----|----------|------------|
| VDD | 3.3V Power | LDO output |
| VSS | Ground | Common ground |
| VBAT | Battery | Li-ion/LiPo cell |

## Component Details

### 1. E-Paper Display - GDEY029T94

**Specifications:**
- Resolution: 296 x 128 pixels
- Colors: Black & White
- Interface: 4-wire SPI
- Refresh time: ~2 seconds (full), ~300ms (partial)
- Power: <1mW active, 0mW static

**Connection:**
```
CH572D          GDEY029T94
------          ----------
PA10 (CS)   --> CS
PA11 (DC)   --> DC
PA12 (RST)  --> RES
PA13        <-- BUSY
PB4 (SCK)   --> CLK
PB5 (MOSI)  --> DIN
GND         --> GND
3.3V        --> VCC
```

### 2. Temperature Sensor - SHT4x

**Specifications:**
- Accuracy: ±0.2°C
- Interface: I2C (0x44)
- Measurement time: <10ms
- Power: <0.4µA standby, <800µA active

**Connection:**
```
CH572D      SHT4x
------      -----
PA14        SDA
PA15        SCL
3.3V        VDD
GND         GND
```

### 3. RGB LED - WS2812

**Specifications:**
- Type: Addressable RGB LED
- Interface: Single-wire protocol
- Current: ~20mA per color @ full brightness
- Voltage: 3.3-5V

**Connection:**
```
CH572D      WS2812
------      ------
PA9         DIN
3.3V        VDD
GND         GND
```

**Notes:**
- Add 100nF decoupling cap near LED
- Consider current limiting resistor (100-470Ω)
- Timing critical - use hardware timer

### 4. Buttons

**Specifications:**
- Type: Tactile push buttons
- Debounce: Software, 50ms
- Pull-up: Internal 40kΩ

**Connection:**
```
CH572D      Button
------      ------
PA6         EDIT (normally open to GND)
PA7         UP (normally open to GND)
PA8         DOWN (normally open to GND)
```

## Power System

### Two SKU Variants

#### A) BLE-Enabled Label
- Battery: 240mAh Li-ion/LiPo (e.g., CR2032 or similar)
- Runtime: 20-30 days
- Features: Full BLE provisioning and sync

#### B) Passive Label (No BLE)
- Power: Energy harvesting (OPV) or small battery
- Runtime: Indefinite with solar, 30+ days on battery
- Features: Display and local logging only

### Power Budget Analysis

| Component | Active Current | Duty Cycle | Average Current |
|-----------|---------------|------------|-----------------|
| CH572D (active) | 3-5mA | 1% | 50µA |
| CH572D (sleep) | 2µA | 99% | 2µA |
| SHT4x (measure) | 800µA | 0.1% | 1µA |
| E-paper (refresh) | 20mA | 0.01% | 2µA |
| WS2812 (50% bright) | 10mA | 5% | 500µA |
| BLE advertising | 8mA | 10% | 800µA |
| **Total (BLE SKU)** | | | **~1.4mA** |
| **Total (Passive)** | | | **~0.6mA** |

**Battery Life Calculation:**
- BLE SKU: 240mAh / 1.4mA ≈ **170 hours ≈ 7 days** (conservative)
- With optimizations: **20-30 days**
- Passive SKU: **30+ days** or indefinite with harvesting

## PCB Design Considerations

### Layout Guidelines

1. **Power Decoupling**
   - Place 100nF caps close to MCU VDD pins
   - 10µF bulk cap near power input
   - Separate analog and digital grounds if possible

2. **E-Paper Interface**
   - Keep SPI traces short and parallel
   - Add series resistors (22-47Ω) on SPI lines
   - Ground plane under display connector

3. **Temperature Sensor**
   - Place away from heat sources (MCU, LED)
   - Expose to ambient air (opening in enclosure)
   - Short I2C traces with 4.7kΩ pull-ups

4. **WS2812 LED**
   - Add 100nF decoupling cap
   - Keep data line short (<10cm)
   - Consider level shifter if needed

5. **Antenna**
   - Keep PCB antenna area clear (no ground plane)
   - Follow WCH reference design
   - 50Ω impedance matching

### Board Stack-up (2-layer)

```
Layer 1 (Top):    Components, signals, power traces
Layer 2 (Bottom): Ground plane (mostly), return paths
```

## Mechanical Design

### Enclosure Requirements

- **Material**: Food-safe plastic (ABS, PC, or PETG)
- **Dimensions**: ~75mm x 50mm x 10mm (estimated)
- **Features**:
  - Window for e-paper display
  - Holes for buttons
  - LED light pipe
  - Ventilation for temperature sensor
  - Battery access door (optional)
  - Mounting: Adhesive backing or clip

### Environmental Requirements

- **Operating Temperature**: 0°C to 60°C (32°F to 140°F)
- **Storage Temperature**: -20°C to 70°C
- **Humidity**: 10% to 90% RH (non-condensing)
- **IP Rating**: IP54 (splash resistant)

## Bill of Materials (BOM)

| Designator | Component | Manufacturer | Part Number | Qty |
|------------|-----------|--------------|-------------|-----|
| U1 | MCU | WCH | CH572D | 1 |
| U2 | E-Paper | GoodDisplay | GDEY029T94 | 1 |
| U3 | Temp Sensor | Sensirion | SHT40-AD1B | 1 |
| D1 | RGB LED | Worldsemi | WS2812-2020 | 1 |
| SW1-3 | Tactile Switch | Various | 6x6mm | 3 |
| C1-5 | Capacitor 100nF | Various | 0805 | 5 |
| C6 | Capacitor 10µF | Various | 0805 | 1 |
| R1-3 | Resistor 10kΩ | Various | 0805 | 3 |
| BT1 | Battery Holder | Various | CR2032 or LiPo | 1 |

## Next Steps

1. Create detailed schematic in KiCad/Eagle
2. Design PCB layout
3. Generate Gerber files
4. Prototype and test
5. Design enclosure in CAD
6. 3D print prototype enclosure
7. Prepare for manufacturing

## References

- [CH572D Datasheet](../Datasheet/CH572DS1.PDF)
- [GDEY029T94 Specification](https://www.good-display.com/)
- [SHT4x Datasheet](https://www.sensirion.com/sht4x)
- [WS2812 Datasheet](https://cdn-shop.adafruit.com/datasheets/WS2812.pdf)
