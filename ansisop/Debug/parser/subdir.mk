################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../parser/metadata_program.c \
../parser/parser.c 

OBJS += \
./parser/metadata_program.o \
./parser/parser.o 

C_DEPS += \
./parser/metadata_program.d \
./parser/parser.d 


# Each subdirectory must supply rules for building sources it contributes
parser/%.o: ../parser/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
<<<<<<< HEAD
	gcc -I"/home/utnso/Blacklist/tp-2017-1c-Blacklist/commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
=======
	gcc -I"/home/utnso/Escritorio/tp-2017-1c-Blacklist/commons" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
>>>>>>> 9a978abd1d9edd60fcc5fa4a19cbea4fff1b5713
	@echo 'Finished building: $<'
	@echo ' '


