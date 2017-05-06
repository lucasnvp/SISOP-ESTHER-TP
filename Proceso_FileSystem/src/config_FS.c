#include "config_FS.h"
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

int puerto() {
	return config_get_int_value(config, "PUERTO");
}

char* montaje() {
	return config_get_string_value(config, "PUNTO_MONTAJE");
}

int cantConexiones() {
	return config_get_int_value(config, "CANTCONEXIONES");
}

void mostrarConfig() {

	puts("----------------------");
	printf("Configuracion:\n");
	puts("");
    printf("PUERTO = %d\n",puerto());
    printf("PUNTO_MONTAJE = %s\n",montaje());
    printf("CANTCONEXIONES = %d\n",cantConexiones());
    puts("----------------------");
}
