################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/ArcadeIT_Lib/System/Units/ArcadeIT_Scheduler.c 

OBJS += \
./Drivers/ArcadeIT_Lib/System/Units/ArcadeIT_Scheduler.o 

C_DEPS += \
./Drivers/ArcadeIT_Lib/System/Units/ArcadeIT_Scheduler.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ArcadeIT_Lib/System/Units/ArcadeIT_Scheduler.o: ../Drivers/ArcadeIT_Lib/System/Units/ArcadeIT_Scheduler.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DHSE_STARTUP_TIMEOUT=360000000' '-DHSE_VALUE=8000000' -DUSE_USB_OTG_FS -DUSE_ULPI_PHY -DARM_MATH_CM4 -DSTM32F469xx -c -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver020_F469_CMSIS/Core/Inc" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver020_F469_CMSIS/Drivers/CMSIS/Include" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver020_F469_CMSIS/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver020_F469_CMSIS/Drivers" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver020_F469_CMSIS/Drivers/ArcadeIT_Lib" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver020_F469_CMSIS/Drivers/ArcadeIT_Lib/System" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver020_F469_CMSIS/Drivers/ArcadeIT_Lib/System/Devices" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver020_F469_CMSIS/Drivers/ArcadeIT_Lib/System/Units" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/ArcadeIT_Lib/System/Units/ArcadeIT_Scheduler.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
