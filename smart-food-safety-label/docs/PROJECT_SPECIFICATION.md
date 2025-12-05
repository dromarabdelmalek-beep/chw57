# Smart Food-Safety Label
## Project Specification

### 1. Project Overview

The project is a **Smart Food-Safety Label** intended for large restaurant chains in the USA (QSRs, fast-casual, catering, institutional kitchens).

It replaces handwritten paper labels used for:
- Tracking food preparation time
- Monitoring shelf-life countdown
- Temperature compliance
- Staff initials
- "Out of fridge" handling
- Automatic expiration and alerting
- Cloud audit/history

The system integrates:
- Ultra-low-power MCU
- E-paper display
- Digital temperature/humidity sensor
- Battery or energy-harvested power
- BLE (optional SKU) for provisioning + history sync
- Local visual warning (RGB LED)

The device behaves like a smart, self-contained IoT label with multi-day operation and minimal user interaction.

---

## 2. Hardware Components

### 2.1 Microcontroller – WCH CH572D

The **CH572D** is selected because it offers:
- Integrated Bluetooth Low Energy 5.0
- 12 KB RAM, 240 KB Flash
- Very low active and sleep current
- Good GPIO availability for buttons, LED, and SPI e-paper driving
- Works efficiently with energy-constrained systems

**Key functions provided by CH572D:**
- UI state machine
- E-paper SPI driving
- Temperature logging
- Button handling (short/long press)
- BLE advertising and provisioning
- Energy monitoring
- Shelf-life + expiration logic

### 2.2 E-Paper Display – GDEY029T94 (GoodDisplay 2.9")

This is a black/white ultra-low-power e-paper module.

**Reasons for selection:**
- Zero power consumption when static
- Perfect readability in kitchens
- Supports partial refresh (we only update changed fields)
- Large enough to show:
  - Product name
  - Days remaining
  - Preparation date
  - Expiration date
  - Temperature (°F)
  - Initials
  - Fridge/out-of-fridge status indicator

Driven via 4-wire SPI directly from CH572D.

### 2.3 Temperature Sensor – Sensirion SHT4x

**Provides:**
- ±0.2°C temperature accuracy
- Ultra-low standby current
- Fast measurement for minimal power waste
- I²C interface

**Used for:**
- Real-time product temperature
- Detection of cold-chain violations (temperature > user-defined threshold for >30 minutes)
- Computation of spoilage risk
- Cloud reporting (BLE SKU only)

The label only samples periodically (configurable 1–10 minutes).

### 2.4 Buttons (3 physical inputs)

Three tactile buttons:

**EDIT (PA6)**
- Long press (5 seconds): enter EDIT MODE
- Short press: mark "OUT OF FRIDGE" / "RETURNED TO FRIDGE"

**UP (PA7)**
- Navigate or increment values in editing mode

**DOWN (PA8)**
- Navigate or decrement values in editing mode

Buttons are configured with internal pull-ups and debouncing.

### 2.5 RGB Indicator LED – WS2812 (Single LED)

Provides visual status at a glance:
- **Green**: food is safe (normal temperature, days left > threshold)
- **Yellow**: approaching expiration (example: <12–24 hours left)
- **Red**: expired or temperature violation detected
- **Blink every 10 minutes**: indicates active violation tracking

The LED is updated whenever label status changes.

### 2.6 Power System

**Two SKUs:**

**A) BLE-Enabled Smart Label**
- Small Li-ion/LiPo or CR battery
- Power budget ~20–30 days depending on interval settings
- BLE used only for provisioning and daily sync

**B) Passive Digital Label (No BLE)**
- Energy harvesting from OPV (organic solar) OR
- Very small battery
- Only operates when energy is available
- Broadcasts no BLE data
- Shows only visual information + logs locally

Both SKUs support energy level detection via CH572 ADC.

---

## 3. Functional Behavior

The device implements a finite-state application:

### 3.1 Normal Operating Mode

The label displays:
- Product name (from product list)
- Preparation date
- Expiration date
- Days left (countdown)
- Temperature in °F (small box on top-right)
- Initials of preparer (2 characters)
- Fridge/out-of-fridge icon
- Warning overlay (if needed)

Temperature is sampled periodically (e.g., every 5 minutes).
Countdown is recomputed daily at midnight.

The e-paper is partially refreshed, meaning only changed fields update.

### 3.2 Edit Mode (staff interaction)

**ENTRY:** Press and hold EDIT button for 5 seconds.

**UI steps:**

Field sequence:
1. Product →
2. Days left →
3. Initial letter #1 →
4. Initial letter #2 →
5. Exit

**Button meaning in EDIT MODE:**
- **UP/DOWN**: scroll product list or increase/decrease values
- **SHORT EDIT press**: move to next editable field
- **LONG EDIT press**: save and exit

**When saved:**
- Expiration date = created date + default_days
- E-paper updated
- BLE provisioning info updated (if BLE exists)

### 3.3 Out-of-Fridge Mode

**Triggered by:** short press on EDIT button (NOT long-press)

**Behavior:**
- Temperature logging pauses
- Label displays an "OUT" icon
- Countdown continues normally
- LED gives slow pulse (optional)

**Returning to fridge:**
- Press short EDIT again → resume temp logging
- Temperature displayed in white text inside a black box

### 3.4 Temperature Violation Logic

If SHT4x reports temperature above `max_temp_f` continuously for >30 minutes:
- `temp_violation = true`
- LED turns RED and blinks every 10 minutes
- BLE SKU broadcasts violation
- Cloud or supervisor can inspect violation history (BLE SKU)

### 3.5 Expiration Logic

Every hour the system computes:
```
days_left = expiry_date − current_date
```

**If `days_left <= 0`:**
- `expired = true`
- LED stays RED
- E-paper shows EXPIRED badge
- BLE SKU advertises expiration alert

**If `days_left < 1 day`:**
- LED turns YELLOW
- E-paper highlights "1 day left"

**Otherwise:**
- LED is GREEN

---

## 4. BLE Functionality (BLE SKU only)

BLE functionality uses CH572 SimpleBLEPeripheral profile:

### Provisioning (encrypted config mode):
- Product selection pushed from mobile app
- Default days per product
- Min/max temperature limits
- Staff initials
- Review of previous logs

### Advertising (runtime):

We broadcast a compressed status packet:
- **Byte0**: product_index
- **Byte1**: days_left
- **Byte2**: temp_f (offset)
- **Byte3**: status_flags (expired | out_of_fridge | violation)
- **Byte4**: energy_level (0–100)

Advertising interval: configurable (e.g., 1 second).

The cloud backend can reconstruct the full history when staff sync the label during food rotation.

---

## 5. Software Architecture

### 5.1 Modules

| Module | Role |
|--------|------|
| `label_state.c` | Stores all persistent runtime state |
| `label_app.c` | Main application state machine / logic |
| `label_config.c` | Provides defaults / preconfigured values |
| `ui_epaper.c` | Graphics + rendering to GDEY029T94 |
| `buttons.c` | Debounced button scanning, short/long event generation |
| `ws2812.c` | RGB alert LED control |
| `ble_label.c` | Advertising payload + provisioning |
| `peripheral.c` | WCH BLE baseline driver (SimpleBLEPeripheral) |
| `main.c` | System init + TMOS loop |

---

## 6. Summary of Key User Interactions

### Staff Workflow (Real Restaurant Scenario)

1. Prepare salad/meat/fish/etc.
2. Press + hold EDIT → enter edit mode
3. Select product type (UP/DOWN)
4. Press EDIT → confirm
5. Adjust "Days Left" if needed
6. Press EDIT → enter initials
7. Press EDIT → save and exit

**Label now shows correct:**
- Creation date
- Expiration date
- Countdown
- Initials

Label automatically detects temperature compliance.
Supervisors can scan (BLE version) to sync history and verify logs.

---

## 7. Why This System Outperforms Manual Labels

✓ Eliminates human error in date calculation
✓ Automated expiration
✓ Automated temperature logging
✓ Cloud history for audits (BLE SKU)
✓ No ink or handwriting issues
✓ Impossible to falsify logs
✓ LED visible from far distance for expired items
✓ Low power, multi-week operation
✓ E-paper always readable, even when power is low

---

## Next Steps

This documentation serves as the foundation for:
- Engineering documentation
- FDA/HACCP compliance planning
- Product pitch to restaurant clients
- Investor presentations
- Patent filings

Future documentation can include:
- Block diagrams
- Full system requirements specification
- State machine diagrams
- BLE protocol specification
- UI flow diagrams
- PCB architecture for CH572D + E-paper + SHT4x
