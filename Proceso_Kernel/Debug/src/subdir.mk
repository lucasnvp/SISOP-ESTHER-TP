################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Proceso\ Kernel.c 

OBJS += \
./src/Proceso\ Kernel.o 

C_DEPS += \
./src/Proceso\ Kernel.d 


# Each subdirectory must supply rules for building sources it contributes
src/Proceso\ Kernel.o: ../src/Proceso\ Kernel.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Proceso Kernel.d" -MT"src/Proceso\ Kernel.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


