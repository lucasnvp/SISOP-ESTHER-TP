################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Proceso\ Memoria.c 

OBJS += \
./src/Proceso\ Memoria.o 

C_DEPS += \
./src/Proceso\ Memoria.d 


# Each subdirectory must supply rules for building sources it contributes
src/Proceso\ Memoria.o: ../src/Proceso\ Memoria.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Proceso Memoria.d" -MT"src/Proceso\ Memoria.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


