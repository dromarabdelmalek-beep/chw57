#################################################################################
# Makefile for CH57x Food Label BLE Application
# Target: CH570/CH571/CH572/CH573
# Toolchain: arm-none-eabi-gcc (from MounRiver Studio or ARM GCC)
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

# Toolchain
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#################################################################################
# CPU and FPU settings
#################################################################################
CPU = -mcpu=cortex-m0

# FPU settings (CH57x has no FPU)
FPU =

# Float ABI
FLOAT-ABI =

# MCU settings
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

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
	@echo "  - arm-none-eabi-gcc toolchain in PATH"
	@echo "  - wchisptool for flashing (optional)"

#################################################################################
# Dependencies
#################################################################################
-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean flash help
