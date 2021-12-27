################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/libs/SW_GRAPHICS/SW_HT1632C/sw_ht3216c.c 

OBJS += \
./Src/libs/SW_GRAPHICS/SW_HT1632C/sw_ht3216c.o 

C_DEPS += \
./Src/libs/SW_GRAPHICS/SW_HT1632C/sw_ht3216c.d 


# Each subdirectory must supply rules for building sources it contributes
Src/libs/SW_GRAPHICS/SW_HT1632C/%.o: ../Src/libs/SW_GRAPHICS/SW_HT1632C/%.c Src/libs/SW_GRAPHICS/SW_HT1632C/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DSTM32F072xB -DSTM32F072C8Tx_MINICLOCK -DARM_MATH_CM0 -c -I"D:/00_Programowanie/01_Workspaces/04_CubeIDE_Workspace_1.6.1/STM32F072C8Tx_CID_REG_MiniClock_v1.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include" -I"D:/00_Programowanie/01_Workspaces/04_CubeIDE_Workspace_1.6.1/STM32F072C8Tx_CID_REG_MiniClock_v1.2/Drivers/CMSIS/Include" -I"D:/00_Programowanie/01_Workspaces/04_CubeIDE_Workspace_1.6.1/STM32F072C8Tx_CID_REG_MiniClock_v1.2/Inc" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-libs-2f-SW_GRAPHICS-2f-SW_HT1632C

clean-Src-2f-libs-2f-SW_GRAPHICS-2f-SW_HT1632C:
	-$(RM) ./Src/libs/SW_GRAPHICS/SW_HT1632C/sw_ht3216c.d ./Src/libs/SW_GRAPHICS/SW_HT1632C/sw_ht3216c.o

.PHONY: clean-Src-2f-libs-2f-SW_GRAPHICS-2f-SW_HT1632C

