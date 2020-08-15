# Makefile for stm32f429i-discovery board

.SECONDEXPANSION:

.PHONY: all default ctags cscope tags clean-tags clean

#########################################
#

PROJECT := stm32f429_project

BUILD_DIR := build
OBJS_DIR := build
DEPS_DIR := build
MKFLS_DIR := mk
LDSCRT_DIR := .

LD_SCRIPT := $(LDSCRT_DIR)/STM32F429ZITx_FLASH.ld

#########################################
#

CROSS = arm-none-eabi-
AS := $(CROSS)as
CC := $(CROSS)gcc
LD := $(CROSS)ld
OBJCP := $(CROSS)objcopy
OBJDP := $(CROSS)objdump
SZ := $(CROSS)size

#########################################
# generate src file through filelist.mk

DFLAGS :=

INLS_DIR :=

SRCS :=

include $(MKFLS_DIR)/*.mk

#########################################
# Machine dependent flags

MCFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
MCFLAGS += -mfloat-abi=soft -mfpu=fpv4-sp-d16

#########################################
# Assembler flag

ASFLAGS = $(CFLAGS)

#########################################
# compiler flags

CFLAGS += -g3 -O0
CFLAGS += -Wdouble-promotion -Wall
CFLAGS += $(MCFLAGS)
CFLAGS += -ffreestanding -fno-common
CFLAGS += $(patsubst %, -I %, $(INLS_DIR))
CFLAGS += $(CC_DEPFLAGS)
CFLAGS += $(DFLAGS)

#########################################
# linker flags

LDFLAGS += --specs=nosys.specs # get rid of __eixt noexisted warning
LDFLAGS += -Wl,--print-memory-usage
LDFLAGS += -Wl,-T $(LD_SCRIPT)
LDFLAGS += -Wl,-Map=$(MAP)
LDFLAGS += $(MCFLAGS)

#########################################
# dependency flags

AS_DEPFLAGS = --MD $(DEPS_DIR)/$*.d
CC_DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPS_DIR)/$*.d

#########################################
# dependencies and objects

OBJS = $(patsubst %, $(OBJS_DIR)/%.o,$(basename $(SRCS)))
DEPS = $(patsubst %, $(DEPS_DIR)/%.d,$(basename $(SRCS)))

OBJS_DIRS := $(dir $(OBJS))
DEPS_DIRS := $(dir $(DEPS))

#########################################
# elf, bin, map, assembly dump file

ELF = $(BUILD_DIR)/$(PROJECT).elf
BIN = $(BUILD_DIR)/$(PROJECT).bin
MAP = $(BUILD_DIR)/$(PROJECT).map
LST = $(BUILD_DIR)/$(PROJECT).lst

#########################################
# ctags, cscope
CTAGS = tags
CSCOPES = cscope.in.out cscope.out cscope.po.out

#########################################

all: default

default: info $(ELF) $(BIN) $(LST)

%.o: %.c
$(OBJS_DIR)/%.o: %.c $(DEPS_DIR)/%.d
	@echo "CC  $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
$(OBJS_DIR)/%.o: %.s
	@echo "AS  $<"
	@$(CC) $(ASFLAGS) -c $< -o $@

$(OBJS): | $(BUILD_DIR) $$(dir $$@)

$(DEPS): | $(BUILD_DIR) $$(dir $$@)

$(sort $(OBJS_DIRS) $(BUILD_DIR) $(DEPS_DIRS)):
	@echo "MKDR  $@"
	@mkdir -p $@

$(DEPS):
include $(wildcard $(DEPS))

$(ELF): $(OBJS)
	@echo "LD  $^"
	@$(CC) $(LDFLAGS) -o $@ $^
	@$(SZ) $@

$(BIN): $(ELF)
	@echo "OBJCPY  $@"
	@$(OBJCP) -Obinary $< $@

$(LST): $(ELF)
	@echo "LIST  $@"
	@$(OBJDP) -S $< > $@

info:
	@echo "======================"
	@echo "assembler: $(AS)"
	@echo "compiler: $(CC)"
	@echo "build directory: $(BUILD_DIR)"
	@echo "object directory: $(OBJS_DIR)"
	@echo "deps directory: $(DEPS_DIR)"
	@echo "======================"
	@echo "ASFLAGS:"
	@echo "$(ASFLAGS)"
	@echo "======================"
	@echo "CFLAGS:"
	@echo "$(CFLAGS)"
	@echo "======================"
	@echo "LDFLAGS:"
	@echo "$(LDFLAGS)"
	@echo "======================"
	@echo "source files:"
	@echo "  $(SRCS)"
	@echo ""
	@echo "  $(OBJS)"
	@echo ""

tags: ctags cscope

ctags:
	@echo "CTAGS  $(CTAGS)"
	@ctags -R --exclude=@.ctagsignore .

cscope:
	@echo "CSCOPE  $(CSCOPES)"
	@cscope -R -q -b

clean-tags:
	@echo "RM  $(CTAGS) $(CSCOPES)"
	@$(RM) -r $(CTAGS) $(CSCOPES)

clean:
	@echo "RM  $(OBJS_DIR)/*"
	@$(RM) -r $(OBJS_DIR)/*
