#include "config_consola.h"

Type_Config load_config(char* path){
	Type_Config config;
	t_config *auxConfig;
	auxConfig = config_create(path);

	config.PUERTO_KERNEL = config_get_int_value(auxConfig, "PUERTO_KERNEL");
	char* ipKernel = malloc(64);
	ipKernel = config_get_string_value(auxConfig, "IP_KERNEL");
	config.IP_KERNEL = malloc(strlen(ipKernel)+1);
	strcpy(config.IP_KERNEL,ipKernel);

	config_destroy(auxConfig);

	return config;
}

void print_config(Type_Config auxConfig){

	puts("----------------------");
	printf("Configuracion:\n");
	puts("");
	printf("IP_KERNEL = %s\n", auxConfig.IP_KERNEL);
	printf("PUERTO_KERNEL = %d\n", auxConfig.PUERTO_KERNEL);
	puts("----------------------");
}

