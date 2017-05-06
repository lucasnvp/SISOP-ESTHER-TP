#include "config_Kernel.h"

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

int puerto_prog(){
	return config_get_int_value(config, "PUERTO_PROG");
}

int puerto_listen(){
	return config_get_int_value(config, "PUERTO_LISTEN");
}

int cant_conexiones(){
	return config_get_int_value(config, "CANTCONEXIONES");
}

int puerto_cpu(){
	return config_get_int_value(config, "PUERTO_CPU");
}

char* ip_memoria(){
	return config_get_string_value(config,"IP_MEMORIA");
}

int puerto_memoria(){
	return config_get_int_value(config, "PUERTO_MEMORIA");
}

char* ip_FS(){
	return config_get_string_value(config,"IP_FS");
}

int puerto_FS(){
	return config_get_int_value(config, "PUERTO_FS");
}

int QUANTUM(){
	return config_get_int_value(config, "QUANTUM");
}

int QUANTUM_SLEEP(){
	return config_get_int_value(config, "QUANTUM_SLEEP");
}

char* algoritmo(){
	return config_get_string_value(config,"ALGORITMO");
}

int grado_multiprog(){
	return config_get_int_value(config, "GRADO_MULTIPROG");
}

char* sem_ids(){
	return config_get_string_value(config,"SEM_IDS");
}

char* sem_init(){
	return config_get_string_value(config,"SEM_INIT");
}

char* shared_vars(){
	return config_get_string_value(config,"SHARED_VARS");
}

int shared_size(){
	return config_get_int_value(config, "STACK_SIZE");
}

void mostrarConfig(){

	puts("----------------------");
	printf("Configuracion:\n");
	puts("");
	printf("PUERTO_PROG = %d\n",puerto_prog());
	printf("PUERTO_CPU = %d\n",puerto_cpu());
	printf("IP_MEMORIA = %s\n",ip_memoria());
	printf("PUERTO_MEMORIA = %d\n",puerto_memoria());
	printf("PUERTO_LISTEN = %d\n",puerto_listen());
	printf("CANTCONEXIONES = %d\n",cant_conexiones());
	printf("IP_FS = %s\n",ip_FS());
	printf("PUERTO_FS = %d\n",puerto_FS());
	printf("QUANTUM = %d\n",QUANTUM());
	printf("QUANTUM_SLEEP = %d\n",QUANTUM_SLEEP());
	printf("ALGORITMO = %s\n",algoritmo());
	printf("GRADO_MULTIPROG = %d\n",grado_multiprog());
	printf("SEM_IDS = %s\n",sem_ids());
	printf("SEM_INIT = %s\n",sem_init());
	printf("SHARED_VARS = %s\n",shared_vars());
	printf("STACK_SIZE = %d\n",shared_size());
	puts("----------------------");
}

