#ifndef CONFIG_CONSOLA_H_
#define CONFIG_CONSOLA_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <commons/string.h>
#include <commons/config.h>

typedef struct ConfigType{
	uint32_t PUERTO_KERNEL;
	char* IP_MEMORIA;
	uint32_t PUERTO_MEMORIA;
	char* IP_FS;
	uint32_t PUERTO_FS;
	uint32_t QUANTUM;
	uint32_t QUANTUM_SLEEP;
	char* ALGORITMO;
	uint32_t GRADO_MULTIPROG;
	char** SEM_IDS;
	char** SEM_INIT;
	char** SHARED_VARS;
	uint32_t STACK_SIZE;
	uint32_t CANTCONEXIONES;
} Type_Config;

int i;

Type_Config load_config(char* path);
void print_config(Type_Config auxConfig);

#endif /* CONFIG_CONSOLA_H_ */
