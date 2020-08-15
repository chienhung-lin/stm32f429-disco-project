INLS_DIR += sys/drivers/STM32F4xx_HAL_Driver/Inc
INLS_DIR += sys/drivers/STM32F4xx_HAL_Driver/Inc/Legacy

HAL_LL_PATH = sys/drivers/STM32F4xx_HAL_Driver/Src

LL_FILES = $(wildcard $(HAL_LL_PATH)/Legacy/stm32f4xx_hal*.c)
 
HAL_TEMPLS = $(wildcard $(HAL_LL_PATH)/stm32f4xx_hal_*_template.c)
HAL_FILES = $(filter-out $(HAL_TEMPLS), $(wildcard $(HAL_LL_PATH)/stm32f4xx_hal*.c))

# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_cortex.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_dma.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_dma_ex.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_flash.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_flash_ex.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_gpio.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_i2c.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_i2c_ex.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_pwr.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_pwr_ex.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_rcc.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_rcc_ex.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_spi.c
# HAL_FILES += $(HAL_LL_PATH)/stm32f4xx_hal_uart.c

SRCS += $(LL_FILES)
SRCS += $(HAL_FILES)
