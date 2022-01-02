################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/libs/SW_GRAPHICS/SW_FONTS/Arial6pt_8_var.c \
../Src/libs/SW_GRAPHICS/SW_FONTS/Arial6pt_var.c \
../Src/libs/SW_GRAPHICS/SW_FONTS/ArialBoldDigits8.c \
../Src/libs/SW_GRAPHICS/SW_FONTS/LCD_5x8.c \
../Src/libs/SW_GRAPHICS/SW_FONTS/MagicMatrix_3x5_numbers.c \
../Src/libs/SW_GRAPHICS/SW_FONTS/MagicMatrix_3x7_numbers.c \
../Src/libs/SW_GRAPHICS/SW_FONTS/MicrosoftSansSerif8ptFont_pl.c \
../Src/libs/SW_GRAPHICS/SW_FONTS/PlayIcons8x8_fixed.c \
../Src/libs/SW_GRAPHICS/SW_FONTS/PlayIcons8x8_tight.c 

OBJS += \
./Src/libs/SW_GRAPHICS/SW_FONTS/Arial6pt_8_var.o \
./Src/libs/SW_GRAPHICS/SW_FONTS/Arial6pt_var.o \
./Src/libs/SW_GRAPHICS/SW_FONTS/ArialBoldDigits8.o \
./Src/libs/SW_GRAPHICS/SW_FONTS/LCD_5x8.o \
./Src/libs/SW_GRAPHICS/SW_FONTS/MagicMatrix_3x5_numbers.o \
./Src/libs/SW_GRAPHICS/SW_FONTS/MagicMatrix_3x7_numbers.o \
./Src/libs/SW_GRAPHICS/SW_FONTS/MicrosoftSansSerif8ptFont_pl.o \
./Src/libs/SW_GRAPHICS/SW_FONTS/PlayIcons8x8_fixed.o \
./Src/libs/SW_GRAPHICS/SW_FONTS/PlayIcons8x8_tight.o 

C_DEPS += \
./Src/libs/SW_GRAPHICS/SW_FONTS/Arial6pt_8_var.d \
./Src/libs/SW_GRAPHICS/SW_FONTS/Arial6pt_var.d \
./Src/libs/SW_GRAPHICS/SW_FONTS/ArialBoldDigits8.d \
./Src/libs/SW_GRAPHICS/SW_FONTS/LCD_5x8.d \
./Src/libs/SW_GRAPHICS/SW_FONTS/MagicMatrix_3x5_numbers.d \
./Src/libs/SW_GRAPHICS/SW_FONTS/MagicMatrix_3x7_numbers.d \
./Src/libs/SW_GRAPHICS/SW_FONTS/MicrosoftSansSerif8ptFont_pl.d \
./Src/libs/SW_GRAPHICS/SW_FONTS/PlayIcons8x8_fixed.d \
./Src/libs/SW_GRAPHICS/SW_FONTS/PlayIcons8x8_tight.d 


# Each subdirectory must supply rules for building sources it contributes
Src/libs/SW_GRAPHICS/SW_FONTS/%.o: ../Src/libs/SW_GRAPHICS/SW_FONTS/%.c Src/libs/SW_GRAPHICS/SW_FONTS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DSTM32F072xB -DSTM32F072C8Tx_MINICLOCK -DARM_MATH_CM0 -c -I"D:/00_Programowanie/01_Workspaces/04_CubeIDE_Workspace_1.6.1/STM32F072C8Tx_CID_REG_MiniClock_v1.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include" -I"D:/00_Programowanie/01_Workspaces/04_CubeIDE_Workspace_1.6.1/STM32F072C8Tx_CID_REG_MiniClock_v1.2/Drivers/CMSIS/Include" -I"D:/00_Programowanie/01_Workspaces/04_CubeIDE_Workspace_1.6.1/STM32F072C8Tx_CID_REG_MiniClock_v1.2/Inc" -I"D:/00_Programowanie/01_Workspaces/04_CubeIDE_Workspace_1.6.1/STM32F072C8Tx_CID_REG_MiniClock_v1.2/src/libs/STM_PERIPHS" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src-2f-libs-2f-SW_GRAPHICS-2f-SW_FONTS

clean-Src-2f-libs-2f-SW_GRAPHICS-2f-SW_FONTS:
	-$(RM) ./Src/libs/SW_GRAPHICS/SW_FONTS/Arial6pt_8_var.d ./Src/libs/SW_GRAPHICS/SW_FONTS/Arial6pt_8_var.o ./Src/libs/SW_GRAPHICS/SW_FONTS/Arial6pt_var.d ./Src/libs/SW_GRAPHICS/SW_FONTS/Arial6pt_var.o ./Src/libs/SW_GRAPHICS/SW_FONTS/ArialBoldDigits8.d ./Src/libs/SW_GRAPHICS/SW_FONTS/ArialBoldDigits8.o ./Src/libs/SW_GRAPHICS/SW_FONTS/LCD_5x8.d ./Src/libs/SW_GRAPHICS/SW_FONTS/LCD_5x8.o ./Src/libs/SW_GRAPHICS/SW_FONTS/MagicMatrix_3x5_numbers.d ./Src/libs/SW_GRAPHICS/SW_FONTS/MagicMatrix_3x5_numbers.o ./Src/libs/SW_GRAPHICS/SW_FONTS/MagicMatrix_3x7_numbers.d ./Src/libs/SW_GRAPHICS/SW_FONTS/MagicMatrix_3x7_numbers.o ./Src/libs/SW_GRAPHICS/SW_FONTS/MicrosoftSansSerif8ptFont_pl.d ./Src/libs/SW_GRAPHICS/SW_FONTS/MicrosoftSansSerif8ptFont_pl.o ./Src/libs/SW_GRAPHICS/SW_FONTS/PlayIcons8x8_fixed.d ./Src/libs/SW_GRAPHICS/SW_FONTS/PlayIcons8x8_fixed.o ./Src/libs/SW_GRAPHICS/SW_FONTS/PlayIcons8x8_tight.d ./Src/libs/SW_GRAPHICS/SW_FONTS/PlayIcons8x8_tight.o

.PHONY: clean-Src-2f-libs-2f-SW_GRAPHICS-2f-SW_FONTS

