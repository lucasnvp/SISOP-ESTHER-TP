#include "config_Memoria.h"

Type_Config load_config(char* path){
	Type_Config config;
	t_config *auxConfig;
	auxConfig = config_create(path);

	config.PUERTO = config_get_int_value(auxConfig, "PUERTO");
	config.MARCOS = config_get_int_value(auxConfig, "MARCOS");
	config.MARCO_SIZE = config_get_int_value(auxConfig, "MARCO_SIZE");
	config.ENTRADAS_CACHE = config_get_int_value(auxConfig, "ENTRADAS_CACHE");
	config.CACHE_X_PROC = config_get_int_value(auxConfig, "CACHE_X_PROC");

	char* reemplazoCache = malloc(4);
	reemplazoCache = config_get_string_value(auxConfig, "REEMPLAZO_CACHE");
	config.REEMPLAZO_CACHE = malloc(strlen(reemplazoCache)+1);
	strcpy(config.REEMPLAZO_CACHE,reemplazoCache);

	config.RETARDO_MEMORIA = config_get_int_value(auxConfig, "RETARDO_MEMORIA");
	config.CANTCONEXIONES = config_get_int_value(auxConfig, "CANTCONEXIONES");

	config_destroy(auxConfig);

	return config;
}

void print_config(Type_Config auxConfig){

	puts("----------------------");
	printf("Configuracion:\n");
	puts("");
	printf("PUERTO = %d\n",auxConfig.PUERTO);
	printf("MARCOS = %d\n",auxConfig.MARCOS);
	printf("MARCO_SIZE = %d\n",auxConfig.MARCO_SIZE);
	printf("ENTRADAS_CACHE = %d\n",auxConfig.ENTRADAS_CACHE);
	printf("CACHE_X_PROC = %d\n",auxConfig.CACHE_X_PROC);
	printf("REEMPLAZO_CACHE = %s\n",auxConfig.REEMPLAZO_CACHE);
	printf("RETARDO_MEMORIA = %d\n",auxConfig.RETARDO_MEMORIA);
	printf("CANTCONEXIONES = %d\n",auxConfig.CANTCONEXIONES);
	puts("----------------------");
}

