################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Proceso\ FileSystem.c \
../src/lectorArchivos.c \
../src/servidor.c 

OBJS += \
./src/Proceso\ FileSystem.o \
./src/lectorArchivos.o \
./src/servidor.o 

C_DEPS += \
./src/Proceso\ FileSystem.d \
./src/lectorArchivos.d \
./src/servidor.d 


# Each subdirectory must supply rules for building sources it contributes
src/Proceso\ FileSystem.o: ../src/Proceso\ FileSystem.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Blacklist/tp-2017-1c-Blacklist/Librerias/commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Proceso FileSystem.d" -MT"src/Proceso\ FileSystem.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/home/utnso/Blacklist/tp-2017-1c-Blacklist/Librerias/commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


