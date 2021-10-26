PROJ_NAME = seismobot

ASF = /home/chrono/software/asf/asf-3.51

SRC_DIR = src
INC_DIR = include
BLD_DIR = bin

CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-objdump

CFLAGS = -mcpu=cortex-m0plus -mthumb
CFLAGS += -Wall -std=c11
CFLAGS += -O1
#CFLAGS += -g
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wl,--gc-sections -Wl,-Map=$(BLD_DIR)/$(PROJ_NAME).map
CFLAGS += -T $(ASF)/sam0/utils/linker_scripts/samd09/gcc/samd09c13a_flash.ld

CFLAGS += -D __SAMD09C13A__
CFLAGS += -D SYSTICK_MODE
#CFLAGS += -D EXTINT_CALLBACK_MODE
CFLAGS += -D USART_CALLBACK_MODE

LDFLAGS += -lm

ELF = $(BLD_DIR)/$(PROJ_NAME).elf
BIN = $(BLD_DIR)/$(PROJ_NAME).bin
LST = $(BLD_DIR)/$(PROJ_NAME).lst

########
# system
########

INC_FLAGS += -I $(ASF)/common/utils
INC_FLAGS += -I $(ASF)/sam0/utils
INC_FLAGS += -I $(ASF)/sam0/utils/preprocessor
INC_FLAGS += -I $(ASF)/sam0/utils/header_files
INC_FLAGS += -I $(ASF)/sam0/utils/cmsis/samd09/source
INC_FLAGS += -I $(ASF)/sam0/utils/cmsis/samd09/include
INC_FLAGS += -I $(ASF)/thirdparty/CMSIS/Include
INC_FLAGS += -I $(ASF)/sam0/drivers/system
INC_FLAGS += -I $(ASF)/sam0/drivers/system/clock
INC_FLAGS += -I $(ASF)/sam0/drivers/system/clock/clock_samd09_d10_d11
INC_FLAGS += -I $(ASF)/sam0/drivers/system/pinmux
INC_FLAGS += -I $(ASF)/sam0/drivers/system/power/power_sam_d_r_h
INC_FLAGS += -I $(ASF)/sam0/drivers/system/reset/reset_sam_d_r_h
INC_FLAGS += -I $(ASF)/sam0/drivers/system/interrupt
INC_FLAGS += -I $(ASF)/sam0/drivers/system/interrupt/system_interrupt_samd09

SRCS += $(ASF)/sam0/utils/cmsis/samd09/source/gcc/startup_samd09.c
SRCS += $(ASF)/sam0/drivers/system/system.c
SRCS += $(ASF)/sam0/drivers/system/pinmux/pinmux.c
SRCS += $(ASF)/sam0/drivers/system/clock/clock_samd09_d10_d11/gclk.c

INC_FLAGS += -I $(ASF)/common/utils/interrupt
SRCS += $(ASF)/common/utils/interrupt/interrupt_sam_nvic.c
SRCS += $(ASF)/sam0/drivers/system/clock/clock_samd09_d10_d11/clock.c

INC_FLAGS += -I $(ASF)/sam0/drivers/system/clock/clock_samd09_d10_d11/module_config

#############
# API modules
#############

# port
INC_FLAGS += -I $(ASF)/sam0/drivers/port
SRCS += $(ASF)/sam0/drivers/port/port.c

# USART/SERCOM
INC_FLAGS += -I $(ASF)/sam0/drivers/sercom
INC_FLAGS += -I $(ASF)/sam0/drivers/sercom/usart
SRCS += $(ASF)/sam0/drivers/sercom/sercom.c
SRCS += $(ASF)/sam0/drivers/sercom/usart/usart.c
SRCS += $(ASF)/sam0/drivers/sercom/usart/usart_interrupt.c
SRCS += $(ASF)/sam0/drivers/sercom/sercom_interrupt.c

################################
# Project-specific source files
################################

INC_FLAGS += -I $(INC_DIR)

SRCS += $(SRC_DIR)/main.c
SRCS += $(SRC_DIR)/delay.c
SRCS += $(SRC_DIR)/serial.c

#########
# rules
#########

.PHONY: all flash clean

all: $(BIN)

$(BIN): $(ELF) $(LST)
	$(OBJCOPY) $< $@ -O binary

$(LST): $(ELF)
	$(OBJDUMP) -D $^ > $@

$(ELF): $(SRCS)
	mkdir -p $(BLD_DIR)
	$(CC) $(CFLAGS) $(INC_FLAGS) $^ -o $@ $(LDFLAGS)

flash:
	openocd -f seismobot.cfg -c "program $(ELF) verify reset exit"

clean:
	rm -rf $(BLD_DIR)

