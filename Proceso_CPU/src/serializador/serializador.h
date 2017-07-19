#ifndef SERIALIZADOR_H_
#define SERIALIZADOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "../servidor/servidor.h"
#include "../pcb/pcb.h"

//CREAR PATH
typedef struct {
	uint32_t sizeString;
	char* dataString;
}__attribute__((packed)) t_SerialString;

typedef struct pedido_memoria {
	uint32_t id;
	uint32_t pagina;
	uint32_t offset;
	uint32_t size;
} t_pedido_memoria;

typedef struct {
	char* data;
	int size;
} t_stream;

void serializar_int(uint32_t socket, uint32_t number);
uint32_t deserializar_int(uint32_t socket);

void serializar_string(int client, t_SerialString* PATH);
void deserializar_string(int servidor, t_SerialString* PATH);

void serializar_pcb(int client, PCB_t* PCB);
void deserializar_pcb(int servidor, PCB_t* PCB);

void serializar_pedido_memoria(uint32_t cliente, t_pedido_memoria* pedido);
t_pedido_memoria* deserializar_pedido_memoria(uint32_t servidor);

void serializar_variable_t(int client, VARIABLE_T* VARIABLE);
t_stream* variable_t_serialize(VARIABLE_T* VARIABLE);
VARIABLE_T* deserializar_variable_t(int servidor);
VARIABLE_T* variable_t_deserialize(char* stream, int* size);

#endif /* SERIALIZADOR_H_ */
