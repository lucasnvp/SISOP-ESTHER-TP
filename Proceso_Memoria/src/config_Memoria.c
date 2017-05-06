#include "config_Memoria.h"
#include <stdlib.h>
#include <commons/string.h>
#include <commons/config.h>

t_config *config;

void abrir_config(char* path){
	config = config_create(path);
}

void cerrar_config_actual(){
	config_destroy(config);
}

int puerto(){
	return config_get_int_value(config, "PUERTO");
}

int marcos(){
	return config_get_int_value(config, "MARCOS");
}

int marco_size(){
	return config_get_int_value(config, "MARCO_SIZE");
}

int entradas_cache(){
	return config_get_int_value(config, "ENTRADAS_CACHE");
}

int cache_x_proc(){
	return config_get_int_value(config, "CACHE_X_PROC");
}

char* reemplazo_cache(){
	return config_get_string_value(config,"REEMPLAZO_CACHE");
}


int retardo_memoria(){
	return config_get_int_value(config, "RETARDO_MEMORIA");
}

int cantConexiones(){
	return config_get_int_value(config, "CANTCONEXIONES");
}

void mostrarConfig(){

	puts("----------------------");
	printf("Configuracion:\n");
	puts("");
	printf("PUERTO = %d\n",puerto());
	printf("MARCOS = %d\n",marcos());
	printf("MARCO_SIZE = %d\n",marco_size());
	printf("ENTRADAS_CACHE = %d\n",entradas_cache());
	printf("CACHE_X_PROC = %d\n",cache_x_proc());
	printf("REEMPLAZO_CACHE = %s\n",reemplazo_cache());
	printf("RETARDO_MEMORIA = %d\n",retardo_memoria());
	printf("CANTCONEXIONES = %d\n",cantConexiones());
	puts("----------------------");
}

