#ifndef CONFIG_CONSOLA_H_
#define CONFIG_CONSOLA_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <commons/string.h>
#include <commons/config.h>

typedef struct ConfigType{
	uint32_t PUERTO_KERNEL;
	char* IP_KERNEL;
} Type_Config;

Type_Config load_config(char* path);
void print_config(Type_Config auxConfig);

#endif /* CONFIG_CONSOLA_H_ */
