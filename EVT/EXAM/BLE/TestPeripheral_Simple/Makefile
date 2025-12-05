#################################################################################
# Makefile for Simple BLE Peripheral Test
# Target: CH572 RISC-V RV32IMAC
#################################################################################

PROJECT = TestPeripheral
BUILD_DIR = build

# SDK paths
SDK_ROOT = ../../..
BLE_LIB_DIR = $(SDK_ROOT)/EXAM/BLE/LIB
HAL_DIR = $(SDK_ROOT)/EXAM/BLE/HAL
STDPERIPH_DIR = $(SDK_ROOT)/EXAM/SRC/StdPeriphDriver
RVMSIS_DIR = $(SDK_ROOT)/EXAM/SRC/RVMSIS

# Toolchain detection
TOOLCHAIN_PREFIXES = riscv-none-embed- riscv64-unknown-elf- riscv-none-elf- riscv32-unknown-elf-

PREFIX := $(shell \
    for prefix in $(TOOLCHAIN_PREFIXES); do \
        if command -v $${prefix}gcc >/dev/null 2>&1; then \
            echo $$prefix; \
            break; \
        fi; \
    done)

ifeq ($(PREFIX),)
    PREFIX = riscv-none-embed-
    $(warning WARNING: RISC-V toolchain not detected, using default: $(PREFIX))
    $(warning Install with: brew install xpack-riscv-none-embed-gcc)
endif

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

# CPU settings
ARCH = -march=rv32imac_zicsr_zifencei
ABI = -mabi=ilp32
MCU = $(ARCH) $(ABI) -msmall-data-limit=8 -mno-save-restore

# C sources (minimal - just main and HAL)
C_SOURCES = \
main.c \
$(HAL_DIR)/MCU.c \
$(HAL_DIR)/RTC.c \
$(HAL_DIR)/SLEEP.c \
$(STDPERIPH_DIR)/CH57x_gpio.c \
$(STDPERIPH_DIR)/CH57x_sys.c \
$(STDPERIPH_DIR)/CH57x_pwr.c \
$(STDPERIPH_DIR)/CH57x_clk.c \
$(STDPERIPH_DIR)/CH57x_uart.c

# ASM sources
ASM_SOURCES = \
$(SDK_ROOT)/EXAM/SRC/Startup/startup_CH572.S

# Include paths
C_INCLUDES = \
-I. \
-I$(HAL_DIR)/include \
-I$(STDPERIPH_DIR)/inc \
-I$(BLE_LIB_DIR) \
-I$(RVMSIS_DIR) \
-I$(SDK_ROOT)/EXAM/SRC/Ld

# C defines
C_DEFS = \
-DCH572 \
-DCHIP_ID=ID_CH572

# Compiler flags
OPT = -Os
CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# Linker script
LDSCRIPT = Link.ld

# Libraries
LIBS = -L$(BLE_LIB_DIR) -L$(STDPERIPH_DIR) -lCH572BLE_PERI -lISP572 -lc -lm -lnosys

# Linker flags
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map,--cref -Wl,--gc-sections

# Build
all: $(BUILD_DIR)/$(PROJECT).elf $(BUILD_DIR)/$(PROJECT).hex $(BUILD_DIR)/$(PROJECT).bin
	@echo ""
	@echo "✅ Build Complete!"
	@echo ""

# List of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo "CC $<"
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
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
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
