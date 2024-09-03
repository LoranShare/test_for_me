## @author		Gevorgyan Ovik
## @brief		Makefile

# Option section
MEM_SIZE ?=
CHUNK_SIZE ?=
DEFS ?=
DEFS += TEST

# Common section
PROGRAM_NAME = cunit_test

# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Werror -O3
CFLAGS += -Wunused-variable
LDFLAGS ?= -lcunit

# Source and object files
BUILD_DIR_NAME = build
SRC_DIR = test
SRC_DIR += test/support
SRC_DIR += source/src
INC_DIR = source/inc
OBJ_DIR = $(BUILD_DIR_NAME)/obj

SOURCES = $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
INC_FILES = $(addprefix -I, $(INC_DIR))
LDFLAGS += $(INC_FILES)

# Executable name
TARGET = $(BUILD_DIR_NAME)/$(PROGRAM_NAME)

# Default target
all: build_info make_dir $(TARGET)

# Parsing options
ifneq ($(MEM_SIZE),)
DEFS += MEM_SIZE=$(MEM_SIZE)
endif

ifneq ($(CHUNK_SIZE),)
DEFS += CHUNK_SIZE=$(CHUNK_SIZE)
endif

DEFS := $(addprefix -D, $(DEFS))

CFLAGS += $(DEFS)

# Rule to build the executable
$(TARGET): $(OBJECTS)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_FILES) -c -o $@ $<

# Clean the project
clean:
	@rm -rf $(BUILD_DIR_NAME)

make_dir:
	@mkdir -p $(BUILD_DIR_NAME)
	@mkdir -p $(OBJ_DIR)

build_info:
	@echo source dir:      $(SRC_DIR)
	@echo source files:    $(SOURCES)
	@echo include dir:     $(INC_DIR)
	@echo CFLAGS:          $(CFLAGS)
	@echo LDFLAGS:         $(LDFLAGS)

# .PHONY indicates that "all" and "clean" are not files
.PHONY: all clean

