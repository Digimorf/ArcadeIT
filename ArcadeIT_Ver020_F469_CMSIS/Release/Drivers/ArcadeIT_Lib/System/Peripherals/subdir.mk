################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/ArcadeIT_Lib/System/Peripherals/ArcadeIT_TestPads.c 

OBJS += \
./Drivers/ArcadeIT_Lib/System/Peripherals/ArcadeIT_TestPads.o 

C_DEPS += \
./Drivers/ArcadeIT_Lib/System/Peripherals/ArcadeIT_TestPads.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ArcadeIT_Lib/System/Peripherals/ArcadeIT_TestPads.o: ../Drivers/ArcadeIT_Lib/System/Peripherals/ArcadeIT_TestPads.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DSTM32F469xx -DDEBUG -c -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/ArcadeIT_Lib/System/Peripherals/ArcadeIT_TestPads.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

