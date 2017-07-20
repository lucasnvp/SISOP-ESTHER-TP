#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <commons/collections/queue.h>
#include <commons/log.h>

#include "servidor/servidor.h"
#include "config/config_consola.h"
#include "utiles/ComandosPorConsola.h"

#define COMANDO_STOP 5
#define COMANDO_EXIT 6

pthread_mutex_t sem_consola;

char* PATH_CONFIG = "/home/utnso/Blacklist/tp-2017-1c-Blacklist/Proceso_Kernel/src/config/config.txt";
Type_Config config;

char* PATH_LOG = "/home/utnso/Blacklist/Logs/logConsola.txt";
t_log* log_Consola;

uint32_t SERVIDOR_KERNEL;

void connect_server_kernel();
void consola_comandos();
void init_log(char* pathLog);

#endif /* CONSOLA_H_ */
