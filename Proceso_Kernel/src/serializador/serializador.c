#include "serializador.h"

t_stream* stream_create(int size) {
	t_stream* stream = malloc(sizeof(t_stream));
	stream->size = size;
	stream->data = malloc(size);
	return stream;
}

void stream_destroy(t_stream* stream) {
	free(stream->data);
	free(stream);
}

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

void serializar_pcb(int client, PCB_t* PCB){
	uint32_t datos_size = 	sizeof(PCB_t) +
							sizeof(t_metadata_program) +
							sizeof(t_intructions) * PCB->CodeTagsPointer->instrucciones_size +
							strlen(PCB->CodeTagsPointer->etiquetas);
	void* ENVIAR = malloc(datos_size);
	uint32_t offset = 0;
	uint32_t size_to_send;

	size_to_send = sizeof(PCB->PID);
	memcpy(ENVIAR + offset, &(PCB->PID),size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(PCB->ProgramCounter);
	memcpy(ENVIAR + offset, &(PCB->ProgramCounter),size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(PCB->PageCode);
	memcpy(ENVIAR + offset, &(PCB->PageCode),size_to_send);
	offset += size_to_send;

	//Metadata Program
		//Instruccion de inicio
		size_to_send = sizeof(PCB->CodeTagsPointer->instruccion_inicio);
		memcpy(ENVIAR + offset, &(PCB->CodeTagsPointer->instruccion_inicio), size_to_send);
		offset += size_to_send;
		//Instrucciones size
		size_to_send = sizeof(PCB->CodeTagsPointer->instrucciones_size);
		memcpy(ENVIAR + offset, &(PCB->CodeTagsPointer->instrucciones_size), size_to_send);
		offset += size_to_send;
		//Puntero de instrucciones
			uint32_t i;
			for(i = 0; PCB->CodeTagsPointer->instrucciones_size > i; i++){
				size_to_send = sizeof(t_puntero_instruccion);
				memcpy(ENVIAR + offset, &(PCB->CodeTagsPointer->instrucciones_serializado[i].start), size_to_send);
				offset += size_to_send;

				size_to_send = sizeof(t_size);
				memcpy(ENVIAR + offset, &(PCB->CodeTagsPointer->instrucciones_serializado[i].offset), size_to_send);
				offset += size_to_send;
			}
		//Etiquetas size
		size_to_send = sizeof(PCB->CodeTagsPointer->etiquetas_size);
		memcpy(ENVIAR + offset, &(PCB->CodeTagsPointer->etiquetas_size), size_to_send);
		offset += size_to_send;
		//Etiquetas
		size_to_send = strlen(PCB->CodeTagsPointer->etiquetas) + 1;
		memcpy(ENVIAR + offset, PCB->CodeTagsPointer->etiquetas, size_to_send);
		offset += size_to_send;
		//Cantidad de funciones
		size_to_send = sizeof(PCB->CodeTagsPointer->cantidad_de_funciones);
		memcpy(ENVIAR + offset, &(PCB->CodeTagsPointer->cantidad_de_funciones), size_to_send);
		offset += size_to_send;
		//Cantidad de etiquetas
		size_to_send = sizeof(PCB->CodeTagsPointer->cantidad_de_etiquetas);
		memcpy(ENVIAR + offset, &(PCB->CodeTagsPointer->cantidad_de_etiquetas), size_to_send);
		offset += size_to_send;

	//StrackPointer
	size_to_send = sizeof(PCB->StackPointer);
	memcpy(ENVIAR + offset, &(PCB->StackPointer),size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(PCB->ExitCode);
	memcpy(ENVIAR + offset, &(PCB->ExitCode),size_to_send);
	offset += size_to_send;

	serializar_int(client,offset);
	send_data(client, ENVIAR, offset);
	free(ENVIAR);
}

void deserializar_pcb(int servidor, PCB_t* PCB){
	PCB->CodeTagsPointer = malloc(sizeof(t_metadata_program));
	uint32_t buffer_size = deserializar_int(servidor);
	void* buffer = malloc(buffer_size);
	uint32_t offset = 0;
	uint32_t size_to_recive;

	recive_data(servidor, buffer, buffer_size);

	size_to_recive = sizeof(PCB->PID);
	memcpy(&PCB->PID, buffer + offset, sizeof(PCB->PID));
	offset += size_to_recive;

	size_to_recive = sizeof(PCB->ProgramCounter);
	memcpy(&PCB->ProgramCounter, buffer + offset, sizeof(PCB->ProgramCounter));
	offset += size_to_recive;

	size_to_recive = sizeof(PCB->PageCode);
	memcpy(&PCB->PageCode, buffer + offset, sizeof(PCB->PageCode));
	offset += size_to_recive;

	//Metadata program
		//Instruccion de inicio
		size_to_recive = sizeof(PCB->CodeTagsPointer->instruccion_inicio);
		memcpy(&PCB->CodeTagsPointer->instruccion_inicio, buffer + offset, size_to_recive);
		offset += size_to_recive;

		//Instrucciones size
		size_to_recive = sizeof(PCB->CodeTagsPointer->instrucciones_size);
		memcpy(&PCB->CodeTagsPointer->instrucciones_size, buffer + offset, size_to_recive);
		offset += size_to_recive;

		//Puntero de instrucciones
			uint32_t i;
			PCB->CodeTagsPointer->instrucciones_serializado = malloc(sizeof(t_intructions) * PCB->CodeTagsPointer->instrucciones_size);
			for(i = 0; PCB->CodeTagsPointer->instrucciones_size > i; i++){
				size_to_recive = sizeof(t_puntero_instruccion);
				memcpy(&PCB->CodeTagsPointer->instrucciones_serializado[i].start, buffer + offset, size_to_recive);
				offset += size_to_recive;

				size_to_recive = sizeof(t_size);
				memcpy(&PCB->CodeTagsPointer->instrucciones_serializado[i].offset, buffer + offset, size_to_recive);
				offset += size_to_recive;
			}

		//Etiquetas size
		size_to_recive = sizeof(PCB->CodeTagsPointer->etiquetas_size);
		memcpy(&PCB->CodeTagsPointer->etiquetas_size, buffer + offset, size_to_recive);
		offset += size_to_recive;

		//Etiquetas
		PCB->CodeTagsPointer->etiquetas = strdup(buffer + offset);
		offset += strlen(PCB->CodeTagsPointer->etiquetas) + 1;

		//Cantidad de funciones
		size_to_recive = sizeof(PCB->CodeTagsPointer->cantidad_de_funciones);
		memcpy(&PCB->CodeTagsPointer->cantidad_de_funciones, buffer + offset, size_to_recive);
		offset += size_to_recive;

		//Cantidad de etiquetas
		size_to_recive = sizeof(PCB->CodeTagsPointer->cantidad_de_etiquetas);
		memcpy(&PCB->CodeTagsPointer->cantidad_de_etiquetas, buffer + offset, size_to_recive);
		offset += size_to_recive;

	//StackPointer
	size_to_recive = sizeof(PCB->StackPointer);
//	memcpy(&PCB->StackPointer, buffer + offset, sizeof(PCB->StackPointer));
	offset += size_to_recive;

	size_to_recive = sizeof(PCB->ExitCode);
	memcpy(&PCB->ExitCode, buffer + offset, sizeof(PCB->ExitCode));
	offset += size_to_recive;

	free(buffer);
}

void serializar_variable_t(int client, VARIABLE_T* VARIABLE){
	uint32_t datos_size = sizeof(VARIABLE_T);
	void* ENVIAR = malloc(datos_size);
	uint32_t offset = 0;
	uint32_t size_to_send;

	size_to_send = sizeof(char);
	memcpy(ENVIAR + offset, &VARIABLE->id,size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(VARIABLE->pagina);
	memcpy(ENVIAR + offset, &(VARIABLE->pagina),size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(VARIABLE->offset);
	memcpy(ENVIAR + offset, &(VARIABLE->offset),size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(VARIABLE->size);
	memcpy(ENVIAR + offset, &(VARIABLE->size),size_to_send);
	offset += size_to_send;

	send_data(client, ENVIAR, offset);
	free(ENVIAR);
}

t_stream* variable_t_serialize(VARIABLE_T* VARIABLE){
	uint32_t datos_size = sizeof(VARIABLE_T);
	t_stream* ENVIAR = stream_create(datos_size);
	uint32_t offset = 0;
	uint32_t size_to_send;

	size_to_send = sizeof(char);
	memcpy(ENVIAR->data + offset, &VARIABLE->id,size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(VARIABLE->pagina);
	memcpy(ENVIAR->data + offset, &(VARIABLE->pagina),size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(VARIABLE->offset);
	memcpy(ENVIAR->data + offset, &(VARIABLE->offset),size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(VARIABLE->size);
	memcpy(ENVIAR->data + offset, &(VARIABLE->size),size_to_send);
	offset += size_to_send;

	return ENVIAR;
}

VARIABLE_T* deserializar_variable_t(int servidor){
	VARIABLE_T* variable = malloc(sizeof(VARIABLE_T));
	uint32_t buffer_size = sizeof(VARIABLE_T);
	void* buffer = malloc(buffer_size);
	uint32_t offset = 0;
	uint32_t size_to_recive;

	recive_data(servidor, buffer, buffer_size);

	size_to_recive = sizeof(char);
	memcpy(&variable->id, buffer + offset, sizeof(variable->id));
	offset += size_to_recive;

	size_to_recive = sizeof(variable->pagina);
	memcpy(&variable->pagina, buffer + offset, sizeof(variable->pagina));
	offset += size_to_recive;

	size_to_recive = sizeof(variable->offset);
	memcpy(&variable->offset, buffer + offset, sizeof(variable->offset));
	offset += size_to_recive;

	size_to_recive = sizeof(variable->size);
	memcpy(&variable->size, buffer + offset, sizeof(variable->size));
	offset += size_to_recive;

	free(buffer);
	return variable;
}

VARIABLE_T* variable_t_deserialize(char* stream, int* size){
	VARIABLE_T* variable = malloc(sizeof(VARIABLE_T));
	uint32_t offset = 0;
	uint32_t size_to_recive;

	size_to_recive = sizeof(char);
	memcpy(&variable->id, stream + offset, sizeof(variable->id));
	offset += size_to_recive;

	size_to_recive = sizeof(variable->pagina);
	memcpy(&variable->pagina, stream + offset, sizeof(variable->pagina));
	offset += size_to_recive;

	size_to_recive = sizeof(variable->offset);
	memcpy(&variable->offset, stream + offset, sizeof(variable->offset));
	offset += size_to_recive;

	size_to_recive = sizeof(variable->size);
	memcpy(&variable->size, stream + offset, sizeof(variable->size));
	offset += size_to_recive;

	*size = offset;

	return variable;
}
