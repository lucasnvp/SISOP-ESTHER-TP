#ifndef CONFIG_Memoria_H_
#define CONFIG_Memoria_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <commons/string.h>
#include <commons/config.h>

typedef struct ConfigType{
	uint32_t PUERTO;
	uint32_t MARCOS;
	uint32_t MARCO_SIZE;
	uint32_t ENTRADAS_CACHE;
	uint32_t CACHE_X_PROC;
	char* REEMPLAZO_CACHE;
	uint32_t RETARDO_MEMORIA;
	uint32_t CANTCONEXIONES;
} Type_Config;

Type_Config load_config(char* path);
void print_config(Type_Config auxConfig);

#endif /* CONFIG_Memoria_H_ */
