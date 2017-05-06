
#ifndef CONFIG_FS_H_
#define CONFIG_FS_H_

#include <commons/config.h>

//LECTURA ARCHIVO CONFIG
void abrir_config(char* path);
void cerrar_config_actual();
int puerto();
char* montaje();
void mostrarConfig();

#endif /* CONFIG_FS_H_ */
