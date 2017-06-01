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

#include "servidor/servidor.h"
#include "config/config_FS.h"

char* PATH_CONFIG = "../src/config/config.txt";
Type_Config config;

uint32_t SERVIDOR_FILESYSTEM;

void server(void* args);
void connection_handler(uint32_t socket, uint32_t command);

#endif /* FILESYSTEM_H_ */
