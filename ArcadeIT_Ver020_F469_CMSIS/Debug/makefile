################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/FATFS/source/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/TransformFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/SupportFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/StatisticsFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/MatrixFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/FilteringFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/FastMathFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/ControllerFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/ComplexMathFunctions/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/CommonTables/subdir.mk
-include Drivers/CMSIS/DSP_Lib/Source/BasicMathFunctions/subdir.mk
-include Drivers/ArcadeIT_Lib/System/Units/subdir.mk
-include Drivers/ArcadeIT_Lib/System/Disk/subdir.mk
-include Drivers/ArcadeIT_Lib/System/Devices/subdir.mk
-include Drivers/ArcadeIT_Lib/System/subdir.mk
-include Drivers/ArcadeIT_Lib/Demofiles/ANSIGfx/subdir.mk
-include Drivers/ArcadeIT_Lib/Applications/Terminal/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
ArcadeIT_Ver020_F469_CMSIS.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
ArcadeIT_Ver020_F469_CMSIS.list \

OBJCOPY_BIN += \
ArcadeIT_Ver020_F469_CMSIS.bin \


# All Target
all: ArcadeIT_Ver020_F469_CMSIS.elf secondary-outputs

# Tool invocations
ArcadeIT_Ver020_F469_CMSIS.elf: $(OBJS) $(USER_OBJS) C:\Users\Francesco\Documents\GitHub\ArcadeIT\ArcadeIT_Ver020_F469_CMSIS\STM32F469IITX_FLASH.ld
	arm-none-eabi-gcc -o "ArcadeIT_Ver020_F469_CMSIS.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Francesco\Documents\GitHub\ArcadeIT\ArcadeIT_Ver020_F469_CMSIS\STM32F469IITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="ArcadeIT_Ver020_F469_CMSIS.map" -Wl,--gc-sections -Wl,--verbose -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

ArcadeIT_Ver020_F469_CMSIS.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "ArcadeIT_Ver020_F469_CMSIS.list"
	@echo 'Finished building: $@'
	@echo ' '

ArcadeIT_Ver020_F469_CMSIS.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "ArcadeIT_Ver020_F469_CMSIS.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
