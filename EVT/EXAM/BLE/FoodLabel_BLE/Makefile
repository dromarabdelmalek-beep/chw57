#################################################################################
# Makefile for CH57x Food Label BLE Application
# Target: CH570/CH571/CH572/CH573 (RISC-V RV32IMAC)
# Toolchain: riscv-none-embed-gcc (from MounRiver Studio or xPack)
#################################################################################

# Project name
PROJECT = FoodLabel_BLE

# Build directory
BUILD_DIR = build

# SDK paths
SDK_ROOT = ../../..
BLE_LIB_DIR = $(SDK_ROOT)/EXAM/BLE/LIB
HAL_DIR = $(SDK_ROOT)/EXAM/BLE/HAL
STDPERIPH_DIR = $(SDK_ROOT)/EXAM/SRC/StdPeriphDriver

# Toolchain - RISC-V (detect available toolchain)
# Try different prefixes in order of preference
TOOLCHAIN_PREFIXES = riscv-none-embed- riscv64-unknown-elf- riscv-none-elf- riscv32-unknown-elf-

# Detect which toolchain is available
PREFIX := $(shell \
    for prefix in $(TOOLCHAIN_PREFIXES); do \
        if command -v $${prefix}gcc >/dev/null 2>&1; then \
            echo $$prefix; \
            break; \
        fi; \
    done)

# Fallback if detection fails
ifeq ($(PREFIX),)
    PREFIX = riscv-none-embed-
    $(warning WARNING: RISC-V toolchain not detected, using default: $(PREFIX))
    $(warning Install with: brew install xpack-riscv-none-embed-gcc)
endif

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
AR = $(PREFIX)ar
OD = $(PREFIX)objdump
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#################################################################################
# RISC-V CPU settings
#################################################################################
# CH57x uses RISC-V RV32IMAC core
# RV32I: Base integer instruction set
# M: Integer multiplication and division
# A: Atomic instructions
# C: Compressed instructions
ARCH = -march=rv32imac
ABI = -mabi=ilp32

# MCU settings
MCU = $(ARCH) $(ABI) -msmall-data-limit=8 -mno-save-restore

#################################################################################
# Source files
#################################################################################

# C sources
C_SOURCES =  \
APP/foodlabel_main.c \
APP/foodlabel.c \
Drivers/sht4x_driver.c \
Drivers/button_driver.c \
Drivers/flash_storage.c \
Drivers/epaper_driver_full.c \
Drivers/gfx.c \
$(HAL_DIR)/MCU.c \
$(HAL_DIR)/RTC.c \
$(HAL_DIR)/SLEEP.c \
$(STDPERIPH_DIR)/CH57x_gpio.c \
$(STDPERIPH_DIR)/CH57x_i2c.c \
$(STDPERIPH_DIR)/CH57x_spi.c \
$(STDPERIPH_DIR)/CH57x_flash.c \
$(STDPERIPH_DIR)/CH57x_pwr.c \
$(STDPERIPH_DIR)/CH57x_sys.c \
$(STDPERIPH_DIR)/CH57x_clk.c \
$(STDPERIPH_DIR)/CH57x_uart.c

# ASM sources
ASM_SOURCES =  \
$(SDK_ROOT)/EXAM/SRC/Startup/startup_CH572.s

#################################################################################
# Include paths
#################################################################################
C_INCLUDES =  \
-IAPP/include \
-IDrivers/include \
-IProfile/include \
-I$(HAL_DIR)/include \
-I$(STDPERIPH_DIR)/inc \
-I$(BLE_LIB_DIR) \
-I$(SDK_ROOT)/EXAM/SRC/Ld

#################################################################################
# Compiler flags
#################################################################################
# C defines
C_DEFS =  \
-DCH572 \
-DCHIP_ID=ID_CH572

# Optimization flags
OPT = -Og

# C flags
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#################################################################################
# Linker flags
#################################################################################
# Link script
LDSCRIPT = $(SDK_ROOT)/EXAM/SRC/Ld/Link.ld

# Libraries
LIBS = -L$(BLE_LIB_DIR) -lCH572BLE_PERI -lc -lm -lnosys
LIBDIR =

LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map,--cref -Wl,--gc-sections

#################################################################################
# Build targets
#################################################################################

# Default target
all: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).hex $(BUILD_DIR)/$(PROJECT).bin

# List of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

# List of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo "CC $<"
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo "AS $<"
	@$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(PROJECT).elf: $(OBJECTS) Makefile
	@echo "LD $@"
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo "HEX $@"
	@$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo "BIN $@"
	@$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#################################################################################
# Clean
#################################################################################
clean:
	-rm -fR $(BUILD_DIR)

#################################################################################
# Flash
#################################################################################
flash: all
	@echo "Flashing $(BUILD_DIR)/$(PROJECT).hex"
	wchisptool -f $(BUILD_DIR)/$(PROJECT).hex

#################################################################################
# Help
#################################################################################
help:
	@echo "Usage:"
	@echo "  make          - Build the project"
	@echo "  make clean    - Clean build files"
	@echo "  make flash    - Build and flash to device"
	@echo ""
	@echo "Prerequisites:"
	@echo "  - RISC-V toolchain in PATH (riscv-none-embed-gcc)"
	@echo "  - wchisptool for flashing (optional)"

#################################################################################
# Dependencies
#################################################################################
-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean flash help
