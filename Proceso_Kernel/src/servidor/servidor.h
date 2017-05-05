#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

typedef struct DatosRecibidosType{
	char *datos;
	int bytesRecibidos;
}DatosRecibidos;

uint32_t connect_server(char* IP , uint32_t Port);
uint32_t build_server(uint32_t Port, uint32_t quantityConexions);
void set_listen(uint32_t servidor, uint32_t sizeConexiones);
uint32_t accept_conexion(uint32_t servidor);
void send_data(int servidor, void *mensaje, int sizeMensaje);
void massive_send(int fdmax, fd_set *master, DatosRecibidos * buffer, int i, int servidor);
int recive_data(int cliente, void *buf, int bytesToRecive);

DatosRecibidos *DatosRecibidos_new(char *datos,int bytesRecibidos);
void DatosRecibidos_free(DatosRecibidos *this);

#endif
