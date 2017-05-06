#include "config_consola.h"
#include <stdlib.h>
#include <commons/string.h>
#include <commons/config.h>

t_config *config;

void abrir_config(char* path) {
	config = config_create(path);
}

void cerrar_config_actual() {
	config_destroy(config);
}

int puertoKernel() {
	return config_get_int_value(config, "PUERTO_KERNEL");
}

char* ipKernel() {
	return config_get_string_value(config, "IP_KERNEL");
}

void mostrarConfig() {

	puts("----------------------");
	printf("Configuracion:\n");
	puts("");
	printf("IP_KERNEL = %s\n", ipKernel());
	printf("PUERTO_KERNEL = %d\n", puertoKernel());
	puts("----------------------");
}

