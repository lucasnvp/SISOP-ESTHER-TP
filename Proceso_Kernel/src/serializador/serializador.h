#ifndef SERIALIZADOR_H_
#define SERIALIZADOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "../collections/queue.h"
#include "../collections/list.h"
#include "../servidor/servidor.h"
#include "../pcb/pcb.h"

//CREAR PATH
typedef struct{
	uint32_t sizeString;
	char* dataString;
}__attribute__((packed)) t_SerialString;

typedef struct {
	char* data;
	int size;
} t_stream;

int i, j;

t_stream* stream_create(int size);
void stream_destroy(t_stream* stream);

void serializar_int(uint32_t socket, uint32_t number);
uint32_t deserializar_int(uint32_t socket);

void serializar_string(int client, t_SerialString* PATH);
void deserializar_string(int servidor, t_SerialString* PATH);

void serializar_pcb(int client, PCB_t* PCB);
void deserializar_pcb(int servidor, PCB_t* PCB);

void serializar_stackpointer(int client, STACKPOINTER_T* lineStack);
t_stream* stackpointer_serialize(STACKPOINTER_T* lineStack);
STACKPOINTER_T* deserializar_stackpointer(int servidor);
STACKPOINTER_T* stackpointer_deserialize(char* stream, int* size);

void serializar_variable_t(int client, VARIABLE_T* VARIABLE);
t_stream* variable_t_serialize(VARIABLE_T* VARIABLE);
VARIABLE_T* deserializar_variable_t(int servidor);
VARIABLE_T* variable_t_deserialize(char* stream, int* size);

#endif /* SERIALIZADOR_H_ */
