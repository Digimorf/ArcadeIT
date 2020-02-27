################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32f469iitx.s 

OBJS += \
./Core/Startup/startup_stm32f469iitx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s
	arm-none-eabi-gcc -mcpu=cortex-m4 '-DHSE_STARTUP_TIMEOUT=360000000' '-DHSE_VALUE=8000000' -DUSE_USB_OTG_FS -DUSE_ULPI_PHY -DARM_MATH_CM4 -DSTM32F469xx -D__FPU_PRESENT -c -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Core/Inc" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib/System" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib/System/Devices" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib/System/Units" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/FATFS/source" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/CMSIS/Include" -I"C:/Users/Francesco/Documents/GitHub/ArcadeIT/ArcadeIT_Ver021_F469_CMSIS/Drivers/CMSIS/DSP_Lib/Include" -x assembler-with-cpp --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

