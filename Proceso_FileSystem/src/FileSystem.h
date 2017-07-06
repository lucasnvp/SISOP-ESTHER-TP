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
#include "setup/setup.h"

static const char* TAMANIO_BLOQUE_NULL = "TAMANIO_BLOQUES=0\n";
static const char* DUMMY_CHAR_FOR_TESTS = "Hola";

char* PATH_CONFIG = "/home/utnso/Blacklist/tp-2017-1c-Blacklist/Proceso_FileSystem/src/config/config.txt";
Type_Config config;

char* PATH_LOG = "/home/utnso/Blacklist/Logs/logFileSystem.txt";
t_log* log_Console;
t_log* log_FileSystem;

int i;
uint32_t SERVIDOR_FILESYSTEM;

void server(void* args);
void connection_handler(uint32_t socket, uint32_t command);

uint32_t ValidarArchivo(char* path);
void CrearArchivo(char* path);
void Borrar(char* path);
char* ObtenerDatos(char* path, uint32_t offset, uint32_t size);
void GuardarDatos(char* path, uint32_t offset, uint32_t size, char* buffer);

int32_t bloque_libre(t_bitarray* auxBitArray);
char** get_bloques_array(char* path);
char* get_bloque_from_int(uint32_t nroDeBloque);


#endif /* FILESYSTEM_H_ */
