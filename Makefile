# **************************************************************************** #
#                                  Makefile                                    #
# **************************************************************************** #

PRJ_NAME := stm32f103rb_platform

DEVICE := STM32F103xB

# ---------------------------------------------------------------------------- #
#                       References to Third-Party resources                    #
# ---------------------------------------------------------------------------- #

# Third-Party root directory
THIRD_PARTY_DIR := third_party


# CMSIS for STM32F1 directory with:
# - stm32f103xb.h		register definitions and peripheral constants
# - system_stm32f1xx.h	system header file
THIRD_PARTY_INC_DIR += $(THIRD_PARTY_DIR)/cmsis_device_f1/Include

# CMSIS for STM32F1 directory with source files
# - system_stm32f1xx.c	system source file
THIRD_PARTY_SRC_DIR += $(THIRD_PARTY_DIR)/cmsis_device_f1/Source/Templates
THIRD_PARTY_SRC		+= system_stm32f1xx.c
# - startup_stm32f103xb.s	startup script
THIRD_PARTY_SRC_DIR += $(THIRD_PARTY_DIR)/cmsis_device_f1/Source/Templates/gcc
THIRD_PARTY_SRC		+= startup_stm32f103xb.s 


# CMSIS directory with:
# - core_cm3.h	Cortex-M3 Core Peripheral Access Layer Header File
#   			Required by stm32f103xb.h
THIRD_PARTY_INC_DIR += $(THIRD_PARTY_DIR)/CMSIS_6/CMSIS/Core/Include

# ---------------------------------------------------------------------------- #
#                                  Directories                                 #
# ---------------------------------------------------------------------------- #

# Project root source directores (excluding third-party source directories)
PRJ_ROOT_SRC_DIR += \
	main \
	mcal \
	os

INC_DIR += \
	$(shell find $(PRJ_ROOT_SRC_DIR) -type d) \
	$(THIRD_PARTY_INC_DIR)
#$(info Include dirs: $(INC_DIR))

PRJ_SRC_DIR := $(shell find $(PRJ_ROOT_SRC_DIR) -type d)
#$(info Project all source dirs: $(PRJ_SRC_DIR))

vpath %.c $(PRJ_SRC_DIR) $(THIRD_PARTY_SRC_DIR)
vpath %.s $(PRJ_SRC_DIR) $(THIRD_PARTY_SRC_DIR)


BIN_DIR := gen_build
OBJ_DIR := $(BIN_DIR)/obj

# ---------------------------------------------------------------------------- #
#                                 Source Files                                 #
# ---------------------------------------------------------------------------- #

# List of all C source files
SRC := \
	$(shell find $(PRJ_ROOT_SRC_DIR) -type f -name "*.c") \
	$(THIRD_PARTY_SRC)
#$(info Source files: $(SRC))

# Convert source file paths to object file paths
OBJECTS = \
	$(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(basename $(notdir $(SRC)))))

#$(info Obj $(OBJECTS))

# ---------------------------------------------------------------------------- #
#                                   Targets                                    #
# ---------------------------------------------------------------------------- #

TARGET := $(PRJ_NAME)

MAP_FILE := $(BIN_DIR)/$(TARGET).map
ELF_FILE := $(BIN_DIR)/$(TARGET).elf

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
	-D$(DEVICE) \
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

LD_FILE := $(PRJ_NAME).ld

# -Map					: Generate map file
# --print-memory-usage	: Print memory usage to console and map file
LDFLAGS := \
	$(MCU) \
	-T $(LD_FILE) \
	-Wl,-Map=$(MAP_FILE) \
	-Wl,--print-memory-usage

# ---------------------------------------------------------------------------- #
#                                Build Rules                                   #
# ---------------------------------------------------------------------------- #

.PHONY: all build

all: build

# Rule to build and link all files into the final ELF executable
build: $(OBJECTS) | $(BIN_DIR)
	@echo
	@echo -n "Building:   $(ELF_FILE)"
	@echo
	@$(LD) $(CFLAGS) $(LDFLAGS) $^ -o $(ELF_FILE)
	@echo -e "\t[ok]"

# Rule to compile C source files to object files
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@echo -n "Compiling:  $<"
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo -e "\t[ok]"

# Rule to assemble Assembly source files to object files
$(OBJ_DIR)/%.o: %.s | $(OBJ_DIR)
	@echo -n "Assembling: $<" 
	@$(AS) $(ASFLAGS) $< -o $@
	@echo -e "\t[ok]"

# ---------------------------------------------------------------------------- #
#                              Directory Creation                              #
# ---------------------------------------------------------------------------- #

.PHONY: $(BIN_DIR) $(OBJ_DIR)

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
	@echo "Cleaning:"
	@echo -n "- Removing $(BIN_DIR) dir."
	@rm -rf $(BIN_DIR)
	@echo -e "\t[ok]"

# ---------------------------------------------------------------------------- #
#                           Target MCU Rules                                   #
# ---------------------------------------------------------------------------- #

.PHONY: flash program

# Program MCU with built software
flash program:
	openocd \
	-f interface/stlink.cfg \
	-f target/stm32f1x.cfg \
	-c "program $(ELF_FILE) verify reset exit"
