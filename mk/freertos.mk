#########################################
# os directory path

OS_DIR = sys/os


#########################################
# freertos directory path

FREERTOS_DIR = $(OS_DIR)/FreeRTOS


#########################################
# source and include file path

FREERTOS_GEN_INLS_DIR = $(FREERTOS_DIR)/Source/include
FREERTOS_GEN_SRCS_DIR = $(FREERTOS_DIR)/Source

FREERTOS_PORT_INLS_DIR = $(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F
FREERTOS_PORT_SRCS_DIR = $(FREERTOS_DIR)/Source/portable/GCC/ARM_CM4F

#FREERTOS_PORT_MPU_WRAP_SRCS_DIR = $(FREERTOS_DIR)/Source/portable/Common

FREERTOS_MEMMANG_INLS_DIR = $(FREERTOS_DIR)/Source/portable/MemMang
FREERTOS_MEMMANG_SRCS_DIR = $(FREERTOS_DIR)/Source/portable/MemMang

FREERTOS_CONFIG_INLS_DIR = inc


#########################################
# general sources

FREERTOS_GEN_SRCS += croutine.c
FREERTOS_GEN_SRCS += event_groups.c
FREERTOS_GEN_SRCS += list.c
FREERTOS_GEN_SRCS += queue.c
FREERTOS_GEN_SRCS += stream_buffer.c
FREERTOS_GEN_SRCS += tasks.c
FREERTOS_GEN_SRCS += timers.c

FREERTOS_GEN_SRCS := $(patsubst %, $(FREERTOS_GEN_SRCS_DIR)/%, $(FREERTOS_GEN_SRCS))


#########################################
# port sources

FREERTOS_PORT_SRCS += port.c

FREERTOS_PORT_SRCS := $(patsubst %, $(FREERTOS_PORT_SRCS_DIR)/%, $(FREERTOS_PORT_SRCS))


#########################################
# port mpu wrapper sources

#FREERTOS_PORT_MPU_WRAP_SRCS += mpu_wrappers.c

#FREERTOS_PORT_MPU_WRAP_SRCS := $(patsubst %, $(FREERTOS_PORT_MPU_WRAP_SRCS_DIR)/%, $(FREERTOS_PORT_MPU_WRAP_SRCS))


#########################################
# port mpu wrapper sources

FREERTOS_MEMMANG_SRCS += heap_4.c

FREERTOS_MEMMANG_SRCS := $(patsubst %, $(FREERTOS_MEMMANG_SRCS_DIR)/%, $(FREERTOS_MEMMANG_SRCS))


#########################################
# add sources and include path to main makefile variables

INLS_DIR += $(FREERTOS_GEN_INLS_DIR)
INLS_DIR += $(FREERTOS_PORT_INLS_DIR)
INLS_DIR += $(FREERTOS_MEMMANGE_INLS_DIR)

SRCS += $(FREERTOS_GEN_SRCS)
SRCS += $(FREERTOS_PORT_SRCS)
SRCS += $(FREERTOS_PORT_MPU_WRAP_SRCS)
SRCS += $(FREERTOS_MEMMANG_SRCS)
