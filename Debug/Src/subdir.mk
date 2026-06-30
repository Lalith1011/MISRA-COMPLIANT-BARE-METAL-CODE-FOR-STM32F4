################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/gpio_safe.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/uart_safe.c 

OBJS += \
./Src/gpio_safe.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/uart_safe.o 

C_DEPS += \
./Src/gpio_safe.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/uart_safe.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F401RETx -DSTM32F4 -DSTM32F401xE -c -I../Inc -I/Users/lalithk/Work/Bare_Metal_On_Arm_STM32/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Include -I/Users/lalithk/Work/Bare_Metal_On_Arm_STM32/STM32Cube_FW_F4_V1.28.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/gpio_safe.cyclo ./Src/gpio_safe.d ./Src/gpio_safe.o ./Src/gpio_safe.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/uart_safe.cyclo ./Src/uart_safe.d ./Src/uart_safe.o ./Src/uart_safe.su

.PHONY: clean-Src

