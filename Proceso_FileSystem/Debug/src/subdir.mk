################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Proceso\ FileSystem.c 

OBJS += \
./src/Proceso\ FileSystem.o 

C_DEPS += \
./src/Proceso\ FileSystem.d 


# Each subdirectory must supply rules for building sources it contributes
src/Proceso\ FileSystem.o: ../src/Proceso\ FileSystem.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Proceso FileSystem.d" -MT"src/Proceso\ FileSystem.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


