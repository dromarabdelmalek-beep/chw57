# Smart Food-Safety Label

## Overview

An intelligent IoT-enabled food safety label for restaurant chains, replacing manual labeling systems with automated tracking, temperature monitoring, and expiration management.

### Key Features

- **WCH CH572D MCU** - Ultra-low power BLE 5.0 microcontroller
- **2.9" E-Paper Display** - Always-on, readable display with zero static power
- **SHT4x Temperature Sensor** - Precise cold-chain monitoring
- **RGB Status LED** - Visual alerts (safe/warning/expired)
- **Multi-day Battery Life** - 20-30 days on single charge
- **BLE Connectivity** - Optional provisioning and cloud sync

## Project Structure

```
smart-food-safety-label/
├── src/              # Source code implementation
├── inc/              # Header files
├── docs/             # Documentation
├── hardware/         # Hardware schematics and PCB files
└── config/           # Configuration files
```

## Hardware Components

| Component | Part Number | Function |
|-----------|-------------|----------|
| MCU | WCH CH572D | BLE 5.0, 12KB RAM, 240KB Flash |
| Display | GDEY029T94 | 2.9" B/W e-paper, SPI interface |
| Sensor | Sensirion SHT4x | Temperature/humidity, I2C |
| LED | WS2812 | RGB status indicator |
| Buttons | 3x Tactile | EDIT, UP, DOWN |

## Software Modules

- **label_state** - Runtime state management
- **label_app** - Main application logic and state machine
- **label_config** - Configuration and defaults
- **ui_epaper** - E-paper display driver and UI
- **buttons** - Debounced button input handling
- **ws2812** - RGB LED control
- **ble_label** - BLE advertising and provisioning
- **sht4x** - Temperature sensor driver

## Getting Started

### Prerequisites

- WCH CH572 SDK and toolchain
- MounRiver Studio IDE (optional)
- WCH-Link programmer

### Building

```bash
# Navigate to project directory
cd smart-food-safety-label

# Build the project
make all

# Flash to device
make flash
```

## Usage

### Staff Workflow

1. **Enter Edit Mode**: Press and hold EDIT button (5 seconds)
2. **Select Product**: Use UP/DOWN to navigate products
3. **Set Days**: Adjust shelf-life days if needed
4. **Enter Initials**: Set staff initials (2 characters)
5. **Save**: Long press EDIT to save and exit

### Out-of-Fridge Mode

- **Short press EDIT**: Mark as "out of fridge"
- Temperature logging pauses
- Press again to return to normal mode

### Status LED Indicators

- **Green**: Food is safe
- **Yellow**: Approaching expiration (<24 hours)
- **Red**: Expired or temperature violation
- **Blinking Red**: Active temperature violation

## Documentation

- [Project Specification](docs/PROJECT_SPECIFICATION.md) - Complete technical specification
- [Hardware Design](hardware/README.md) - Hardware schematics and PCB
- [Software Architecture](docs/SOFTWARE_ARCHITECTURE.md) - Detailed module documentation
- [BLE Protocol](docs/BLE_PROTOCOL.md) - BLE advertising and provisioning spec

## License

See LICENSE file for details.

## Support

For technical support and questions, please open an issue in the project repository.
