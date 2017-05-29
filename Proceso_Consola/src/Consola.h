#ifndef CONSOLA_H_
#define CONSOLA_H_

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

#include <commons/collections/queue.h>

#include "servidor/servidor.h"
#include "config/config_consola.h"
#include "utiles/ComandosPorConsola.h"

pthread_mutex_t sem_consola;

char* PATH_CONFIG = "../src/config/config.txt";
Type_Config config;

uint32_t SERVIDOR_KERNEL;

void connect_server_kernel();
void consola_comandos();

#endif /* CONSOLA_H_ */
