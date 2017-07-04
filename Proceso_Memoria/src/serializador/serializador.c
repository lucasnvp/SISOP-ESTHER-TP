#include "serializador.h"

void serializar_int(uint32_t socket, uint32_t number){
	send_data(socket, &number, sizeof(uint32_t));
}

uint32_t deserializar_int(uint32_t socket){
	uint32_t aux;
	uint32_t bytesRecibidos = recive_data(socket,&aux,sizeof(uint32_t));
	if(bytesRecibidos <= 0){
		aux = bytesRecibidos;
	}
	return aux;
}

void serializar_string(int client, t_SerialString* PATH){
	void* ENVIAR = malloc(PATH->sizeString);
	uint32_t offset = 0;
	uint32_t size_to_send;

	size_to_send = sizeof(PATH->sizeString);
	memcpy(ENVIAR + offset, &(PATH->sizeString),size_to_send);
	offset += size_to_send;

	size_to_send = PATH->sizeString;
	memcpy(ENVIAR + offset, PATH->dataString, size_to_send);
	offset += size_to_send;

	send_data(client, ENVIAR, offset);
	free(ENVIAR);
}

void deserializar_string(int servidor, t_SerialString* PATH){
	uint32_t buffer_size;
	void* buffer = malloc(buffer_size = sizeof(uint32_t));

	//---------------------
	recive_data(servidor, buffer, sizeof(PATH->sizeString));
	memcpy(&PATH->sizeString, buffer, buffer_size);
	PATH->dataString = (char*) malloc(sizeof(char) * PATH->sizeString);
	recive_data(servidor, PATH->dataString, PATH->sizeString);
	PATH->dataString[PATH->sizeString] = '\0';
	//---------------------

}
