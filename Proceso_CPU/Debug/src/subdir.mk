################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Proceso\ CPU.c 

OBJS += \
./src/Proceso\ CPU.o 

C_DEPS += \
./src/Proceso\ CPU.d 


# Each subdirectory must supply rules for building sources it contributes
src/Proceso\ CPU.o: ../src/Proceso\ CPU.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Proceso CPU.d" -MT"src/Proceso\ CPU.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


