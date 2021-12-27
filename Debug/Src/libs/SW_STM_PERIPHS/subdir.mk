################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/libs/SW_STM_PERIPHS/sw_dma.c \
../Src/libs/SW_STM_PERIPHS/sw_iwdg.c \
../Src/libs/SW_STM_PERIPHS/sw_rcc.c \
../Src/libs/SW_STM_PERIPHS/sw_spi.v2.c \
../Src/libs/SW_STM_PERIPHS/sw_tim.c 

OBJS += \
./Src/libs/SW_STM_PERIPHS/sw_dma.o \
./Src/libs/SW_STM_PERIPHS/sw_iwdg.o \
./Src/libs/SW_STM_PERIPHS/sw_rcc.o \
./Src/libs/SW_STM_PERIPHS/sw_spi.v2.o \
./Src/libs/SW_STM_PERIPHS/sw_tim.o 

C_DEPS += \
./Src/libs/SW_STM_PERIPHS/sw_dma.d \
./Src/libs/SW_STM_PERIPHS/sw_iwdg.d \
./Src/libs/SW_STM_PERIPHS/sw_rcc.d \
./Src/libs/SW_STM_PERIPHS/sw_spi.v2.d \
./Src/libs/SW_STM_PERIPHS/sw_tim.d 


# Each subdirectory must supply rules for building sources it contributes
Src/libs/SW_STM_PERIPHS/%.o: ../Src/libs/SW_STM_PERIPHS/%.c Src/libs/SW_STM_PERIPHS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DSTM32F072xB -DSTM32F072C8Tx_MINICLOCK -DARM_MATH_CM0 -c -I"D:/00_Programowanie/01_Workspaces/04_CubeIDE_Workspace_1.6.1/STM32F072C8Tx_CID_REG_MiniClock_v1.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include" -I"D:/00_Programowanie/01_Workspaces/04_CubeIDE_Workspace_1.6.1/STM32F072C8Tx_CID_REG_MiniClock_v1.2/Drivers/CMSIS/Include" -I"D:/00_Programowanie/01_Workspaces/04_CubeIDE_Workspace_1.6.1/STM32F072C8Tx_CID_REG_MiniClock_v1.2/Inc" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-libs-2f-SW_STM_PERIPHS

clean-Src-2f-libs-2f-SW_STM_PERIPHS:
	-$(RM) ./Src/libs/SW_STM_PERIPHS/sw_dma.d ./Src/libs/SW_STM_PERIPHS/sw_dma.o ./Src/libs/SW_STM_PERIPHS/sw_iwdg.d ./Src/libs/SW_STM_PERIPHS/sw_iwdg.o ./Src/libs/SW_STM_PERIPHS/sw_rcc.d ./Src/libs/SW_STM_PERIPHS/sw_rcc.o ./Src/libs/SW_STM_PERIPHS/sw_spi.v2.d ./Src/libs/SW_STM_PERIPHS/sw_spi.v2.o ./Src/libs/SW_STM_PERIPHS/sw_tim.d ./Src/libs/SW_STM_PERIPHS/sw_tim.o

.PHONY: clean-Src-2f-libs-2f-SW_STM_PERIPHS

