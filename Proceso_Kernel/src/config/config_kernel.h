#ifndef CONFIG_CONSOLA_H_
#define CONFIG_CONSOLA_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <commons/string.h>
#include <commons/config.h>

typedef struct ConfigType{
	uint32_t PUERTO_PROG;
	uint32_t PUERTO_CPU;
	char IP_MEMORIA[16];
	uint32_t PUERTO_MEMORIA;
	char IP_FS[16];
	uint32_t PUERTO_FS;
	uint32_t QUANTUM;
	uint32_t QUANTUM_SLEEP;
	char ALGORITMO[5];
	uint32_t GRADO_MULTIPROG;
	//SEM_IDS=[SEM1, SEM2, SEM3]
	//SEM_INIT=[0, 0, 5]
	//SHARED_VARS=[!Global, !UnaVar, !tiempo3]
	uint32_t STACK_SIZE;
	uint32_t CANTCONEXIONES;
	//PUERTO_LISTEN=8080
} Type_Config;

Type_Config load_config(char* path);
void print_config(Type_Config auxConfig);

#endif /* CONFIG_CONSOLA_H_ */