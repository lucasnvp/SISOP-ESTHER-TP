#ifndef CONFIG_Kernel_H_
#define CONFIG_Kernel_H_

#include <commons/config.h>

//LECTURA ARCHIVO CONFIG
void abrir_config(char* path);
void cerrar_config_actual();
int puerto_prog();
int puerto_listen();
int cant_conexiones();
int puerto_cpu();
char* ip_memoria();
int puerto_memoria();
char* ip_FS();
int puerto_FS();
int QUANTUM();
int QUANTUM_SLEEP();
char* algoritmo();
int grado_multiprog();
char* sem_ids();
char* sem_init();
char* shared_vars();
int shared_size();
void mostrarConfig();

#endif /* CONFIG_Kernel_H_ */
