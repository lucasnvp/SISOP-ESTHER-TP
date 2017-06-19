#ifndef SERIALIZADOR_H_
#define SERIALIZADOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "../servidor/servidor.h"


//CREAR PATH
typedef struct{
	uint32_t sizeString;
	char* dataString;

}__attribute__((packed)) t_SerialString;

void serializar_int(uint32_t socket, uint32_t number);
uint32_t deserializar_int(uint32_t socket);

void serializar_string(int client, t_SerialString* PATH);
void deserializar_string(int servidor, t_SerialString* PATH);



#endif /* SERIALIZADOR_H_ */
