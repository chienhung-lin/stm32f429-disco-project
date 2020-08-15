DFLAGS += -DSTM32F429xx

INLS_DIR += sys/arch/CMSIS/Core/Include
INLS_DIR += sys/arch/CMSIS/Device/ST/STM32F4xx/Include

SRCS += sys/arch/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/startup_stm32f429xx.s
