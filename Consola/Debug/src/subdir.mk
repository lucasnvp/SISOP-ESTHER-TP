################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Proceso\ Consola.c \
../src/lectorArchivos.c 

OBJS += \
./src/Proceso\ Consola.o \
./src/lectorArchivos.o 

C_DEPS += \
./src/Proceso\ Consola.d \
./src/lectorArchivos.d 


# Each subdirectory must supply rules for building sources it contributes
src/Proceso\ Consola.o: ../src/Proceso\ Consola.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Proceso Consola.d" -MT"src/Proceso\ Consola.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


