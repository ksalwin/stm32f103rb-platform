# **************************************************************************** #
#                                  Makefile                                    #
# **************************************************************************** #

# Project name
PRJ_NAME := stm32f103rb_platform

# ---------------------------------------------------------------------------- #
#                       References to Third-Party resources                    #
# ---------------------------------------------------------------------------- #

# CMSIS with:
# - core_cm3.h	Cortex-M3 Core Peripheral Access Layer Header File
#   			Required by stm32f103xb.h
CMSIS_INC_DIR = third_party/CMSIS_6/CMSIS/Core/Include

# CMSIS for STM32F1 with:
# - stm32f103xb.h		register definitions and peripheral constants
# - system_stm32f1xx.h	system header file
CMSIS_DEVICE_INC_DIR = third_party/cmsis_device_f1/Include/

# ---------------------------------------------------------------------------- #
#                                Toolchain Setup                               #
# ---------------------------------------------------------------------------- #

CC := arm-none-eabi-gcc	# Compiler
AS := arm-none-eabi-as	# Assembler
LD := arm-none-eabi-gcc	# Linker (using GCC for linking)

# ---------------------------------------------------------------------------- #
#                                  Directories                                 #
# ---------------------------------------------------------------------------- #

# Project include, source, object and binary directories
INC_DIR := \
	inc \
	$(CMSIS_INC_DIR) \
	$(CMSIS_DEVICE_INC_DIR)

SRC_DIR := src
ASM_DIR := src

OBJ_DIR := build/obj
BIN_DIR := build/

# ---------------------------------------------------------------------------- #
#                                   Targets                                    #
# ---------------------------------------------------------------------------- #

TARGET = $(BIN_DIR)/$(PRJ_NAME)

# ---------------------------------------------------------------------------- #
#                           Compiler and Linker Flags                          #
# ---------------------------------------------------------------------------- #

MCU	:= -mcpu=cortex-m3 -mthumb

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

ASFLAGS := \
	$(MCU) \
	-g

# Linker file
LD_FILE := $(PRJ_NAME).ld

LDFLAGS := \
	$(MCU) \
	-T $(LD_FILE)

# ---------------------------------------------------------------------------- #
#                                 Source Files                                #
# ---------------------------------------------------------------------------- #


# Find all C and Assembly source files in the SRC_DIR
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
C_SOURCES += third_party/cmsis_device_f1/Source/Templates/system_stm32f1xx.c

S_SOURCES = $(wildcard $(ASM_DIR)/*.s)
SOURCES = $(C_SOURCES) $(S_SOURCES)

# Convert source file paths to object file paths
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES)) \
          $(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.o,$(S_SOURCES))

# ---------------------------------------------------------------------------- #
#                                Build Rules                                  #
# ---------------------------------------------------------------------------- #

# Rule to compile C source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to assemble Assembly source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	$(AS) $(ASFLAGS) $< -o $@

# Rule to link all object files into the final ELF executable
all: build
build: $(OBJECTS) | $(BIN_DIR)
	$(LD) $(CFLAGS) $(LDFLAGS) $^ -o $(TARGET).elf

# ---------------------------------------------------------------------------- #
#                              Directory Creation                             #
# ---------------------------------------------------------------------------- #

# Ensure the binary directory exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)
all: build

# Ensure the object directory exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -r $(BIN_DIR)
