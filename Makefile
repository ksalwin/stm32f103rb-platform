# **************************************************************************** #
#                                  Makefile                                    #
# **************************************************************************** #

# Project name
PRJ_NAME := stm32f103rb_platform

# ---------------------------------------------------------------------------- #
#                       References to Third-Party resources                    #
# ---------------------------------------------------------------------------- #



# CMSIS for STM32F1 with:
# - stm32f103xb.h		register definitions and peripheral constants
# - system_stm32f1xx.h	system header file
CMSIS_DEVICE_INC_DIR := third_party/cmsis_device_f1/Include

CMSIS_DEVICE_SRC_DIR := third_party/cmsis_device_f1/Source/Templates


# System file from CMSIS
SYSTEM_FILE := third_party/cmsis_device_f1/Source/Templates/system_stm32f1xx.c

# Startup script from CMSIS
STARTUP_SCRIPT := \
	third_party/cmsis_device_f1/Source/Templates/gcc/startup_stm32f103xb.s

# CMSIS with:
# - core_cm3.h	Cortex-M3 Core Peripheral Access Layer Header File
#   			Required by stm32f103xb.h
CMSIS_INC_DIR = third_party/CMSIS_6/CMSIS/Core/Include

# ---------------------------------------------------------------------------- #
#                                  Directories                                 #
# ---------------------------------------------------------------------------- #

INC_DIR := \
	inc \
	$(CMSIS_INC_DIR) \
	$(CMSIS_DEVICE_INC_DIR)

SRC_DIR := src
ASM_DIR := src

BIN_DIR := build
OBJ_DIR := build/obj

vpath %.c $(SRC_DIR) $(CMSIS_DEVICE_SRC_DIR)

# ---------------------------------------------------------------------------- #
#                                 Source Files                                 #
# ---------------------------------------------------------------------------- #

# List of all C source files
C_SRC := $(wildcard $(SRC_DIR)/*.c)
C_SRC += $(SYSTEM_FILE)

# List of all assembly source files
S_SRC := $(wildcard $(ASM_DIR)/*.s)
S_SRC += $(STARTUP_SCRIPT)

# Convert source file paths to object file paths
# TODO: startup file and system not put into object dir
OBJECTS = \
	$(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(C_SRC))) \
	$(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(S_SRC))

$(info List of object files: $(OBJECTS))

# ---------------------------------------------------------------------------- #
#                                   Targets                                    #
# ---------------------------------------------------------------------------- #

TARGET = $(BIN_DIR)/$(PRJ_NAME)

# ---------------------------------------------------------------------------- #
# Toolchain: Compiler                                                          #
# ---------------------------------------------------------------------------- #

# MCU definition
# -mcpu=cortex-m3	: Specify the target CPU architecture
# -mthumb			: Generate Thumb instruction set
MCU := -mcpu=cortex-m3 -mthumb

# Compiler
CC := arm-none-eabi-gcc

# Compiler flags:
# -mcpu=cortex-m3	: Specify the target CPU architecture
# -mthumb			: Generate Thumb instruction set
# -Wall				: Enable all compiler's warning messages
# -g				: Include debugging information
# $(INC)			: Include directories defined above
# -DSTM32F103xB		: Define the STM32F103xB macro
# -fno-exceptions	: Do not generate code for exceptions (try, catch, throw)
# -fno-unwind-tables: Disables generation of unwind tables used by
#  exception-handling and debugging features
CFLAGS := \
	$(MCU) \
	-O0 \
	-Wall \
	-g \
	$(addprefix -I, $(INC_DIR)) \
	-DSTM32F103xB \
	-fno-exceptions \
	-fno-unwind-tables

# ---------------------------------------------------------------------------- #
# Toolchain: Assembler                                                         #
# ---------------------------------------------------------------------------- #

# Assembler
AS := arm-none-eabi-as	# Assembler

# Assembler flags:
# -g				: Include debugging information
ASFLAGS := \
	$(MCU) \
	-g

# ---------------------------------------------------------------------------- #
# Toolchain: Linker                                                            #
# ---------------------------------------------------------------------------- #

# Linker (using GCC for linking)
LD := arm-none-eabi-gcc

# Linker file
LD_FILE := $(PRJ_NAME).ld

LDFLAGS := \
	$(MCU) \
	-T $(LD_FILE)

# ---------------------------------------------------------------------------- #
#                                Build Rules                                   #
# ---------------------------------------------------------------------------- #

.PHONY: all build

all: build

# Rule to build and link all files into the final ELF executable
build: $(OBJECTS) | $(BIN_DIR)
	$(LD) $(CFLAGS) $(LDFLAGS) $^ -o $(TARGET).elf

# Rule to compile C source files to object files
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@echo "Compiling: $@"
	@echo " - Dependency: $<"
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Done"

# Rule to assemble Assembly source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	$(AS) $(ASFLAGS) $< -o $@

# ---------------------------------------------------------------------------- #
#                              Directory Creation                              #
# ---------------------------------------------------------------------------- #

# Ensure the binary directory exists
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Ensure the object directory exists
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# ---------------------------------------------------------------------------- #
#                           Clean build artifacts                              #
# ---------------------------------------------------------------------------- #

.PHONY: clean

clean:
	@echo "Rule: $@"
	@echo "- Removing $(BIN_DIR) dir."
	@rm -rf $(BIN_DIR)
	@echo "Done"

# ---------------------------------------------------------------------------- #
#                           Target MCU Rules                                   #
# ---------------------------------------------------------------------------- #

.PHONY: flash program

# Program MCU with built software
flash program:
	openocd \
	-f interface/stlink.cfg \
	-f target/stm32f1x.cfg \
	-c "program build/stm32f103rb_platform.elf verify reset exit"
