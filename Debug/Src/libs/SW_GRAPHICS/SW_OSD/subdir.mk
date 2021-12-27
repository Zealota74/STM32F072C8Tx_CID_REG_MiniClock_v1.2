################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/libs/SW_GRAPHICS/SW_OSD/sw_osd.c \
../Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_fx.c \
../Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_module_BRIGHT.c \
../Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_module_RTC.c 

OBJS += \
./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd.o \
./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_fx.o \
./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_module_BRIGHT.o \
./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_module_RTC.o 

C_DEPS += \
./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd.d \
./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_fx.d \
./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_module_BRIGHT.d \
./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_module_RTC.d 


# Each subdirectory must supply rules for building sources it contributes
Src/libs/SW_GRAPHICS/SW_OSD/%.o: ../Src/libs/SW_GRAPHICS/SW_OSD/%.c Src/libs/SW_GRAPHICS/SW_OSD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DSTM32F072xB -DSTM32F072C8Tx_MINICLOCK -DARM_MATH_CM0 -c -I"D:/00_Programowanie/01_Workspaces/04_CubeIDE_Workspace_1.6.1/STM32F072C8Tx_CID_REG_MiniClock_v1.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include" -I"D:/00_Programowanie/01_Workspaces/04_CubeIDE_Workspace_1.6.1/STM32F072C8Tx_CID_REG_MiniClock_v1.2/Drivers/CMSIS/Include" -I"D:/00_Programowanie/01_Workspaces/04_CubeIDE_Workspace_1.6.1/STM32F072C8Tx_CID_REG_MiniClock_v1.2/Inc" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-libs-2f-SW_GRAPHICS-2f-SW_OSD

clean-Src-2f-libs-2f-SW_GRAPHICS-2f-SW_OSD:
	-$(RM) ./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd.d ./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd.o ./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_fx.d ./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_fx.o ./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_module_BRIGHT.d ./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_module_BRIGHT.o ./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_module_RTC.d ./Src/libs/SW_GRAPHICS/SW_OSD/sw_osd_module_RTC.o

.PHONY: clean-Src-2f-libs-2f-SW_GRAPHICS-2f-SW_OSD

