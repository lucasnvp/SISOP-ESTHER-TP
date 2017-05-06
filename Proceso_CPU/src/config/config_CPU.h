
#ifndef CONFIG_CPU_H_
#define CONFIG_CPU_H_

#include <commons/config.h>

//LECTURA ARCHIVO CONFIG
void abrir_config(char* path);
void cerrar_config_actual();
int puerto();
char* montaje();
void mostrarConfig();

#endif /* CONFIG_CPU_H_ */
