#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>


#include <commons/collections/list.h>
#include <commons/log.h>

#include "servidor/servidor.h"
#include "config/config_FS.h"
#include "serializador/serializador.h"

char* PATH_CONFIG = "../src/config/config.txt";
Type_Config config;

char* PATH_LOG = "/home/utnso/Blacklist/Logs/logFileSystem.txt";
t_log* log_Console;
t_log* log_FileSystem;

uint32_t SERVIDOR_FILESYSTEM;

void server(void* args);
void connection_handler(uint32_t socket, uint32_t command);
void init_fs_folder();

char* PATH_METADATA;
char* PATH_ARCHIVO;

char* METADATA= "Metadata/Metadata.bin";

char* ARCHIVO= "Archivos/";

char* BITMAP= "Metadata/Bitmap.bin";
char* PATH_BITMAP;

char* DATOS= "Bloques";
char* PATH_DATOS;


int CANT_BLOQUES;
int TAMANIO_BLOQUES;





void inicializar_bitmap();


t_config* metadata;




#endif /* FILESYSTEM_H_ */
