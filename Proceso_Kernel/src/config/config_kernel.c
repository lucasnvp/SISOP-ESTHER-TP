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
	//SEM_IDS=[SEM1, SEM2, SEM3]
	//SEM_INIT=[0, 0, 5]
	//SHARED_VARS=[!Global, !UnaVar, !tiempo3]
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
	//SEM_IDS=[SEM1, SEM2, SEM3]
	//SEM_INIT=[0, 0, 5]
	//SHARED_VARS=[!Global, !UnaVar, !tiempo3]
	printf("STACK_SIZE = %d \n", auxConfig.STACK_SIZE);
	printf("CANTCONEXIONES = %d \n", auxConfig.CANTCONEXIONES);
	puts("----------------------");
}
