#ifndef PROCESO_MEMORIA_SRC_SERVIDOR_H_
#define PROCESO_MEMORIA_SRC_SERVIDOR_H_

#include <stdbool.h>
#include <string.h>

int connect_server();
void recive_data(int cliente);

#endif