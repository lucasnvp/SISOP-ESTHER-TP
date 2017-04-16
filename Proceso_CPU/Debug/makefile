################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Proceso\ CPU

dependents:
	-cd /home/utnso/Escritorio/TP/tp-2017-1c-Blacklist/commons/Debug && $(MAKE) clean all

# Tool invocations
Proceso\ CPU: $(OBJS) $(USER_OBJS) /home/utnso/Escritorio/TP/tp-2017-1c-Blacklist/commons/Debug/libcommons.so
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L"/home/utnso/Escritorio/TP/tp-2017-1c-Blacklist/commons/Debug" -o "Proceso CPU" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) "Proceso CPU"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:
/home/utnso/Escritorio/TP/tp-2017-1c-Blacklist/commons/Debug/libcommons.so:

-include ../makefile.targets
