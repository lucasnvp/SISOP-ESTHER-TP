#include "config_CPU.h"

Type_Config load_config(char* path){
	Type_Config config;
	t_config *auxConfig;
	auxConfig = config_create(path);

	config.PUERTO_KERNEL = config_get_int_value(auxConfig, "PUERTO_KERNEL");

	char* ipKernel = malloc(64);
	ipKernel = config_get_string_value(auxConfig, "IP_KERNEL");
	config.IP_KERNEL = malloc(strlen(ipKernel)+1);
	strcpy(config.IP_KERNEL,ipKernel);

	config.PUERTO_MEMORIA = config_get_int_value(auxConfig, "PUERTO_MEMORIA");

	char* ipMemoria = malloc(64);
	ipMemoria = config_get_string_value(auxConfig, "IP_MEMORIA");
	config.IP_MEMORIA = malloc(strlen(ipMemoria)+1);
	strcpy(config.IP_MEMORIA,ipMemoria);


	config_destroy(auxConfig);

	return config;
}

void print_config(Type_Config auxConfig){

	puts("----------------------");
	printf("Configuracion:\n");
	puts("");
	printf("IP_KERNEL = %s\n", auxConfig.IP_KERNEL);
	printf("PUERTO_KERNEL = %d\n", auxConfig.PUERTO_KERNEL);
	printf("IP_MEMORIA = %s\n", auxConfig.IP_MEMORIA);
	printf("PUERTO_MEMORIA = %d\n", auxConfig.PUERTO_MEMORIA);
	puts("----------------------");
}

