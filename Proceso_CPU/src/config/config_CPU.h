
#ifndef CONFIG_CPU_H_
#define CONFIG_CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <commons/string.h>
#include <commons/config.h>

typedef struct ConfigType{
	uint32_t PUERTO_KERNEL;
	char IP_KERNEL[16];
	char IP_MEMORIA[16];
	uint32_t PUERTO_MEMORIA;
} Type_Config;

Type_Config load_config(char* path);
void print_config(Type_Config auxConfig);

#endif /* CONFIG_CPU_H_ */
