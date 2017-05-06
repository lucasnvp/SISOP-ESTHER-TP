#ifndef CONFIG_Memoria_H_
#define CONFIG_Memoria_H_

#include <commons/config.h>

//LECTURA ARCHIVO CONFIG
void abrir_config(char* path);
void cerrar_config_actual();
int puerto();
int marcos();
int marco_size();
int entradas_cache();
int cache_x_proc();
char* reemplazo_cache();
int retardo_memoria();
int cantConexiones();
void mostrarConfig();

#endif /* CONFIG_Memoria_H_ */
