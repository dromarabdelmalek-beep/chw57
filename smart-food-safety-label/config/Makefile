# Makefile for Smart Food-Safety Label
# Target: WCH CH572D

# Project name
PROJECT = food-safety-label

# Build variants
# Set to 1 to enable BLE, 0 to disable (passive label)
ENABLE_BLE ?= 1

# Toolchain
CC = riscv-none-embed-gcc
AS = riscv-none-embed-as
LD = riscv-none-embed-ld
OBJCOPY = riscv-none-embed-objcopy
OBJDUMP = riscv-none-embed-objdump
SIZE = riscv-none-embed-size

# Directories
SRC_DIR = ../src
INC_DIR = ../inc
BUILD_DIR = ../build
SDK_DIR = ../../EVT/EXAM/SRC

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/label_app.c \
       $(SRC_DIR)/label_state.c \
       $(SRC_DIR)/label_config.c \
       $(SRC_DIR)/ui_epaper.c \
       $(SRC_DIR)/buttons.c \
       $(SRC_DIR)/ws2812.c \
       $(SRC_DIR)/sht4x.c

# Add BLE sources if enabled
ifeq ($(ENABLE_BLE), 1)
SRCS += $(SRC_DIR)/ble_label.c
CFLAGS += -DENABLE_BLE=1
endif

# Include paths
INCLUDES = -I$(INC_DIR) \
           -I$(SDK_DIR)/StdPeriphDriver/inc \
           -I$(SDK_DIR)/RVMSIS

# Compiler flags
CFLAGS += -march=rv32imac -mabi=ilp32 \
          -msmall-data-limit=8 \
          -mno-save-restore \
          -Os -fmessage-length=0 \
          -fsigned-char -ffunction-sections \
          -fdata-sections -fno-common \
          -Wall -Wextra \
          $(INCLUDES)

# Linker flags
LDFLAGS = -T $(SDK_DIR)/Ld/Link.ld \
          -nostartfiles \
          -Wl,--gc-sections \
          -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map

# Default target
all: $(BUILD_DIR)/$(PROJECT).hex $(BUILD_DIR)/$(PROJECT).bin
	@echo "Build complete!"
	@$(SIZE) $(BUILD_DIR)/$(PROJECT).elf

# Build hex file
$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).elf
	@echo "Creating hex file..."
	@$(OBJCOPY) -O ihex $< $@

# Build binary file
$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	@echo "Creating binary file..."
	@$(OBJCOPY) -O binary $< $@

# Build ELF file
$(BUILD_DIR)/$(PROJECT).elf: $(SRCS)
	@mkdir -p $(BUILD_DIR)
	@echo "Compiling and linking..."
	@$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

# Flash to device (using WCH-Link)
flash: $(BUILD_DIR)/$(PROJECT).hex
	@echo "Flashing to device..."
	# Add your flash command here (e.g., WCHISPTool command)

# Generate disassembly
disasm: $(BUILD_DIR)/$(PROJECT).elf
	@$(OBJDUMP) -D $< > $(BUILD_DIR)/$(PROJECT).dis
	@echo "Disassembly saved to $(BUILD_DIR)/$(PROJECT).dis"

.PHONY: all clean flash disasm
