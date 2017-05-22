#include "config_kernel.h"

Type_Config load_config(char* path){
	Type_Config config;
	t_config *auxConfig;
	auxConfig = config_create(path);

	config.PUERTO_KERNEL = config_get_int_value(auxConfig, "PUERTO_KERNEL");
	char *ipMemoria = config_get_string_value(auxConfig, "IP_MEMORIA");
	memcpy(config.IP_MEMORIA, ipMemoria, strlen(ipMemoria)+1);
	free (ipMemoria);
	config.PUERTO_MEMORIA = config_get_int_value(auxConfig, "PUERTO_MEMORIA");
	char *IP_FS = config_get_string_value(auxConfig, "IP_FS");
	memcpy(config.IP_FS, IP_FS, strlen(IP_FS)+1);
	free (IP_FS);
	config.PUERTO_FS = config_get_int_value(auxConfig, "PUERTO_FS");
	config.QUANTUM = config_get_int_value(auxConfig, "QUANTUM");
	config.QUANTUM_SLEEP = config_get_int_value(auxConfig, "QUANTUM_SLEEP");
	char *ALGORITMO = config_get_string_value(auxConfig, "ALGORITMO");
	memcpy(config.ALGORITMO, ALGORITMO, strlen(ALGORITMO)+1);
	free (ALGORITMO);
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
