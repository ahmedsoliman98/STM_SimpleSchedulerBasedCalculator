################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/HAL/H_Switch.c \
../src/HAL/H_Switch_Cfg.c \
../src/HAL/Keypad.c \
../src/HAL/Lcd.c \
../src/HAL/Lcd_cfg.c 

OBJS += \
./src/HAL/H_Switch.o \
./src/HAL/H_Switch_Cfg.o \
./src/HAL/Keypad.o \
./src/HAL/Lcd.o \
./src/HAL/Lcd_cfg.o 

C_DEPS += \
./src/HAL/H_Switch.d \
./src/HAL/H_Switch_Cfg.d \
./src/HAL/Keypad.d \
./src/HAL/Lcd.d \
./src/HAL/Lcd_cfg.d 


# Each subdirectory must supply rules for building sources it contributes
src/HAL/%.o: ../src/HAL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra  -g -DNDEBUG -DSTM32F401xC -DUSE_HAL_DRIVER -DHSE_VALUE=25000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


