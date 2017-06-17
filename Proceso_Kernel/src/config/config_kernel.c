#include "config_kernel.h"

Type_Config load_config(char* path){
	Type_Config config;
	t_config *auxConfig;
	auxConfig = config_create(path);

	config.PUERTO_KERNEL = config_get_int_value(auxConfig, "PUERTO_KERNEL");

	char* ipMemoria = malloc(64);
	ipMemoria = config_get_string_value(auxConfig, "IP_MEMORIA");
	config.IP_MEMORIA = malloc(strlen(ipMemoria)+1);
	strcpy(config.IP_MEMORIA,ipMemoria);

	config.PUERTO_MEMORIA = config_get_int_value(auxConfig, "PUERTO_MEMORIA");

	char* ipFS = malloc(64);
	ipFS = config_get_string_value(auxConfig, "IP_FS");
	config.IP_FS = malloc(strlen(ipFS)+1);
	strcpy(config.IP_FS, ipFS);

	config.PUERTO_FS = config_get_int_value(auxConfig, "PUERTO_FS");
	config.QUANTUM = config_get_int_value(auxConfig, "QUANTUM");
	config.QUANTUM_SLEEP = config_get_int_value(auxConfig, "QUANTUM_SLEEP");

	char* ALGORITMO = malloc(64);
	ALGORITMO = config_get_string_value(auxConfig, "ALGORITMO");
	config.ALGORITMO = malloc(strlen(ALGORITMO)+1);
	strcpy(config.ALGORITMO, ALGORITMO);

	config.GRADO_MULTIPROG = config_get_int_value(auxConfig, "GRADO_MULTIPROG");

	config.SEM_IDS = config_get_array_value(auxConfig, "SEM_IDS");
	config.SEM_INIT = config_get_array_value(auxConfig, "SEM_INIT");
	config.SHARED_VARS = config_get_array_value(auxConfig, "SHARED_VARS");

	config.STACK_SIZE = config_get_int_value(auxConfig, "STACK_SIZE");
	config.CANTCONEXIONES = config_get_int_value(auxConfig, "CANTCONEXIONES");

	config_destroy(auxConfig);

	return config;
}

void print_config(Type_Config auxConfig){
	puts("----------------------");
	printf("Configuracion:\n");
	puts("");
	printf("PUERTO_KERNEL = %d \n", auxConfig.PUERTO_KERNEL);
	printf("IP_MEMORIA = %s \n", auxConfig.IP_MEMORIA);
	printf("PUERTO_MEMORIA = %d \n", auxConfig.PUERTO_MEMORIA);
	printf("IP_FS = %s \n", auxConfig.IP_FS);
	printf("PUERTO_FS = %d \n", auxConfig.PUERTO_FS);
	printf("QUANTUM = %d \n", auxConfig.QUANTUM);
	printf("QUANTUM_SLEEP = %d \n", auxConfig.QUANTUM_SLEEP);
	printf("ALGORITMO = %s \n", auxConfig.ALGORITMO);
	printf("GRADO_MULTIPROG = %d \n", auxConfig.GRADO_MULTIPROG);

	printf("SEM_IDS, SEM_INIT: \n");
	for(i=0 ; auxConfig.SEM_IDS[i] != NULL ; i++){
		printf("SEM_IDS[%d]: %s , SEM_INIT[%d]: %s \n", i, auxConfig.SEM_IDS[i], i, auxConfig.SEM_INIT[i]);
	}

	printf("SHARED_VARS:\n");
	for(i=0 ; auxConfig.SHARED_VARS[i] != NULL ; i++){
	    printf("[%d]: %s\n", i, auxConfig.SHARED_VARS[i]);
	}

	printf("STACK_SIZE = %d \n", auxConfig.STACK_SIZE);
	printf("CANTCONEXIONES = %d \n", auxConfig.CANTCONEXIONES);
	puts("----------------------");
}
