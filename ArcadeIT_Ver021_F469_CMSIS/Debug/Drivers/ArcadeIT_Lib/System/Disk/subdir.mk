################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/ArcadeIT_Lib/System/Disk/ArcadeIT_Storage.c 

OBJS += \
./Drivers/ArcadeIT_Lib/System/Disk/ArcadeIT_Storage.o 

C_DEPS += \
./Drivers/ArcadeIT_Lib/System/Disk/ArcadeIT_Storage.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ArcadeIT_Lib/System/Disk/ArcadeIT_Storage.o: ../Drivers/ArcadeIT_Lib/System/Disk/ArcadeIT_Storage.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu99 -g3 '-DHSE_STARTUP_TIMEOUT=360000000' '-DHSE_VALUE=8000000' -DUSE_USB_OTG_FS -DUSE_ULPI_PHY -DARM_MATH_CM4 -DSTM32F469xx -D__FPU_PRESENT -c -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Core/Inc" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib/System" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib/System/Devices" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib/System/Units" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/FATFS/source" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/CMSIS/Include" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/CMSIS/DSP_Lib/Include" -Og -ffunction-sections -fdata-sections -mslow-flash-data -fno-strict-aliasing -Wall -v -fstack-usage -MMD -MP -MF"Drivers/ArcadeIT_Lib/System/Disk/ArcadeIT_Storage.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

