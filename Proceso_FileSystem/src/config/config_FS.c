#include "config_FS.h"

Type_Config load_config(char* path){
	Type_Config config;
	t_config *auxConfig;
	auxConfig = config_create(path);

	config.PUERTO_FILESYSTEM = config_get_int_value(auxConfig, "PUERTO");

	char* puntoDeMontaje = malloc(64);
	puntoDeMontaje = config_get_string_value(auxConfig, "PUNTO_MONTAJE");
	config.PUNTO_MONTAJE = malloc(strlen(puntoDeMontaje)+1);
	strcpy(config.PUNTO_MONTAJE,puntoDeMontaje);

	config.CANTCONEXIONES = config_get_int_value(auxConfig, "CANTCONEXIONES");

	config_destroy(auxConfig);

	return config;
}



void print_config(Type_Config auxConfig){

	puts("----------------------");
	printf("Configuracion:\n");
	puts("");
    printf("PUERTO = %d\n",auxConfig.PUERTO_FILESYSTEM);
    printf("PUNTO_MONTAJE = %s\n",auxConfig.PUNTO_MONTAJE);
    printf("CANTCONEXIONES = %d\n",auxConfig.CANTCONEXIONES);
    puts("----------------------");
}
