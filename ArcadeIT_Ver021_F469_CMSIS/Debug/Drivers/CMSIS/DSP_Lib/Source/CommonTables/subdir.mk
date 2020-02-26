################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.c \
../Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.c 

OBJS += \
./Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.o \
./Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.o 

C_DEPS += \
./Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.d \
./Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.o: ../Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu99 -g3 '-DHSE_STARTUP_TIMEOUT=360000000' '-DHSE_VALUE=8000000' -DUSE_USB_OTG_FS -DUSE_ULPI_PHY -DARM_MATH_CM4 -DSTM32F469xx -D__FPU_PRESENT -c -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Core/Inc" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib/System" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib/System/Devices" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib/System/Units" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/FATFS/source" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/CMSIS/Include" -Og -ffunction-sections -fdata-sections -Wall -v -fstack-usage -MMD -MP -MF"Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_common_tables.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.o: ../Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu99 -g3 '-DHSE_STARTUP_TIMEOUT=360000000' '-DHSE_VALUE=8000000' -DUSE_USB_OTG_FS -DUSE_ULPI_PHY -DARM_MATH_CM4 -DSTM32F469xx -D__FPU_PRESENT -c -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Core/Inc" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib/System" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib/System/Devices" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/ArcadeIT_Lib/System/Units" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/FATFS/source" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/Francesco/STM32CubeIDE/workspace_1.3.0/ArcadeIT_Ver021_F469_CMSIS/Drivers/CMSIS/Include" -Og -ffunction-sections -fdata-sections -Wall -v -fstack-usage -MMD -MP -MF"Drivers/CMSIS/DSP_Lib/Source/CommonTables/arm_const_structs.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

