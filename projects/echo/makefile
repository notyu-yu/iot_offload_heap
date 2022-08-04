TARGET = mcu_echo
SRCS = mcu_echo.c uart_dma.c uart.c

LINKER_SCRIPT = ../../flash/STM32L4S5VITx_FLASH.ld

# Generate debug info
DEBUG = 1

# Choose processor
CDEFS  = -DSTM32L4S5xx
# Enable FPU
#CDEFS += -D__VFP_FP__

include ../armf4.mk
