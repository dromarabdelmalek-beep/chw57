# E-Paper Driver Files

This directory contains two e-paper driver implementations:

## 1. `epaper_driver.c` (STUB)
- **Status**: Stub implementation with integration guide
- **Purpose**: Template for custom e-paper controller integration
- **Use when**: You want to integrate your own e-paper library

## 2. `epaper_driver_full.c` (COMPLETE)
- **Status**: ✅ Full implementation for SSD1680 controller
- **Purpose**: Ready-to-use driver for 2.9" 296x128 e-paper displays
- **Controller**: SSD1680 / IL3897 compatible
- **Use when**: You have a standard 2.9" e-paper with SSD1680

## Which One To Use?

### For Quick Start (Recommended)
**Use `epaper_driver_full.c`** - This includes:
- Complete SPI communication
- SSD1680 controller initialization
- Full graphics rendering
- Text/number drawing
- Partial and full refresh
- Edit mode highlights

### For Custom Integration
**Use `epaper_driver.c`** - This provides:
- Function stubs
- Integration guide
- Comments explaining each step
- Reference to controller datasheets

## Build Configuration

To use the **full driver**, ensure your build system includes:
```makefile
Drivers/epaper_driver_full.c
Drivers/gfx.c
```

To use the **stub driver**, include:
```makefile
Drivers/epaper_driver.c
# Add your custom library files here
```

## Graphics Library

The full implementation uses `gfx.c` which provides:
- Pixel drawing
- Line/rectangle drawing
- Filled shapes
- Text rendering (8x8, 12x16, 16x24 fonts)
- Number formatting

## Controller Compatibility

The `epaper_driver_full.c` is designed for:
- **SSD1680** (Sitronix)
- **IL3897** (Good Display)
- **UC8151D** (similar commands)

For other controllers (e.g., SSD1675, SSD1608), you may need to:
1. Modify initialization sequence in `EPaper_Init()`
2. Adjust resolution settings
3. Update refresh commands

## Display Specifications

Current implementation supports:
- **Resolution**: 296 x 128 pixels
- **Color**: Black/White (1-bit)
- **Interface**: SPI (4-wire)
- **Voltage**: 3.3V
- **Refresh time**: ~2 seconds (full)

## Notes

- The framebuffer uses 1 bit per pixel (4736 bytes total)
- Partial refresh is available but full refresh is used for reliability
- Font data is included in `gfx.c` (basic 8x8 font)
- For larger fonts, extend the font arrays

## Testing

To test the e-paper driver:
1. Connect display to SPI pins (see main README.md)
2. Build project with `epaper_driver_full.c`
3. Flash to CH57x
4. Splash screen should appear on first boot
5. Food label should display after 3 seconds

## Troubleshooting

**Display stays blank:**
- Check SPI connections (CS, DC, RST, BUSY)
- Verify power supply (3.3V, sufficient current)
- Check BUSY pin is reading correctly
- Try hardware reset

**Garbled display:**
- Verify framebuffer endianness
- Check SPI clock speed (max 4MHz for most displays)
- Ensure DC pin toggles correctly

**Slow refresh:**
- Normal for e-paper (2-3 seconds)
- Use partial refresh for faster updates (implementation available)

## Future Enhancements

Potential improvements:
- [ ] Partial refresh for temperature updates
- [ ] LUT (Look-Up Table) optimization for faster refresh
- [ ] Grayscale support (4-level)
- [ ] Image/bitmap rendering
- [ ] Additional font sizes
- [ ] Rotation support

---

For questions, refer to:
- SSD1680 datasheet
- `epaper_driver_full.c` implementation
- Main project README.md
