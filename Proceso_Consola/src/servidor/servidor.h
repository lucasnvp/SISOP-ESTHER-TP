#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include <stdbool.h>
#include <string.h>

int connect_server(char* IP , int Port);
int recive_data(int cliente, void *buf,int bytesToRecive);
void send_data(int servidor, void *mensaje, int sizeMensaje);

#endif
