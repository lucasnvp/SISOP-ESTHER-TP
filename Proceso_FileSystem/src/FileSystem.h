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

#include "servidor/servidor.h"
#include "config/config_FS.h"

char* PATH_CONFIG = "/home/utnso/Blacklist/tp-2017-1c-Blacklist/Proceso_FileSystem/src/config/config.txt";
Type_Config config;

#endif /* FILESYSTEM_H_ */
