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
	void* ENVIAR = malloc( sizeof(t_SerialString) + PATH->sizeString);
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

	free(buffer);
}

void serializar_pedido_memoria(uint32_t socket, t_pedido_memoria* pedido) {
	uint32_t datos_size = sizeof(t_pedido_memoria);
	void* ENVIAR = malloc(datos_size);
	uint32_t offset = 0;
	uint32_t size_to_send;

	size_to_send = sizeof(pedido->id);
	memcpy(ENVIAR + offset, &(pedido->id), size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(pedido->offset);
	memcpy(ENVIAR + offset, &(pedido->offset), size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(pedido->pagina);
	memcpy(ENVIAR + offset, &(pedido->pagina), size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(pedido->size);
	memcpy(ENVIAR + offset, &(pedido->size), size_to_send);
	offset += size_to_send;

	send_data(socket, ENVIAR, offset);
	free(ENVIAR);

}

t_pedido_memoria* deserializar_pedido_memoria(uint32_t servidor){
	t_pedido_memoria* pedido = malloc(sizeof(t_pedido_memoria));
	uint32_t buffer_size = sizeof(t_pedido_memoria);
	void* buffer = malloc(buffer_size);
	uint32_t offset = 0;
	uint32_t size_to_recive;

	recive_data(servidor, buffer, buffer_size);

	size_to_recive = sizeof(char);
	memcpy(&pedido->id, buffer + offset, sizeof(pedido->id));
	offset += size_to_recive;

	size_to_recive = sizeof(pedido->pagina);
	memcpy(&pedido->pagina, buffer + offset, sizeof(pedido->pagina));
	offset += size_to_recive;

	size_to_recive = sizeof(pedido->offset);
	memcpy(&pedido->offset, buffer + offset, sizeof(pedido->offset));
	offset += size_to_recive;

	size_to_recive = sizeof(pedido->size);
	memcpy(&pedido->size, buffer + offset, sizeof(pedido->size));
	offset += size_to_recive;

	free(buffer);
	return pedido;
}
