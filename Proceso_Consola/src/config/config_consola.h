
#ifndef CONFIG_CONSOLA_H_
#define CONFIG_CONSOLA_H_

#include <commons/config.h>

//LECTURA ARCHIVO CONFIG
void abrir_config(char* path);
void cerrar_config_actual();
int puertoKernel();
char* ipKernel();
void mostrarConfig();

#endif /* CONFIG_CONSOLA_H_ */
