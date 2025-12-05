# Simple BLE Peripheral Test

A minimal BLE Peripheral project to verify CH572 toolchain and BLE stack work correctly.

## Purpose

This is a simplified test project to:
1. Verify RISC-V toolchain is working
2. Test BLE library linking
3. Confirm basic CH572 compilation
4. Test without complex application code

## What It Does

- Initializes CH572 system
- Initializes BLE stack
- Starts as a BLE Peripheral (advertises)
- Minimal code - just the essentials

## Build

```bash
cd EVT/EXAM/BLE/TestPeripheral_Simple
make clean
make
```

## Expected Output

```
CC main.c
CC ../../../EXAM/BLE/HAL/MCU.c
...
LD build/TestPeripheral.elf
   text    data     bss     dec     hex filename
  12345    1234    5678   19257    4b39 build/TestPeripheral.elf
HEX build/TestPeripheral.hex
BIN build/TestPeripheral.bin

✅ Build Complete!
```

## Files

- `main.c` - Minimal main() with BLE initialization
- `CONFIG.h` - Basic BLE configuration (6KB heap)
- `Makefile` - Build configuration
- `Link.ld` - Linker script (16KB RAM)

## Next Steps

If this builds successfully:
1. ✅ Toolchain is working
2. ✅ BLE library links correctly
3. ✅ Basic CH572 setup is good
4. → Can proceed with FoodLabel_BLE project

If this fails:
- Check toolchain installation
- Verify SDK paths
- Check error messages
