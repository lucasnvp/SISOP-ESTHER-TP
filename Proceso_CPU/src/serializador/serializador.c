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
	PATH->dataString = '\0';
	recive_data(servidor, PATH->dataString, PATH->sizeString);
	//---------------------

}

void serializar_pcb(int client, PCB_t* PCB){
	uint32_t datos_size = 	sizeof(PCB_t) +
							sizeof(t_metadata_program) +
							sizeof(t_intructions) * PCB->CodePointer->instrucciones_size +
							PCB->CodePointer->etiquetas_size;
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
		size_to_send = sizeof(PCB->CodePointer->instruccion_inicio);
		memcpy(ENVIAR + offset, &(PCB->CodePointer->instruccion_inicio), size_to_send);
		offset += size_to_send;
		//Instrucciones size
		size_to_send = sizeof(PCB->CodePointer->instrucciones_size);
		memcpy(ENVIAR + offset, &(PCB->CodePointer->instrucciones_size), size_to_send);
		offset += size_to_send;
		//Puntero de instrucciones
			uint32_t i;
			for(i = 0; PCB->CodePointer->instrucciones_size > i; i++){
				size_to_send = sizeof(t_puntero_instruccion);
				memcpy(ENVIAR + offset, &(PCB->CodePointer->instrucciones_serializado[i].start), size_to_send);
				offset += size_to_send;

				size_to_send = sizeof(t_size);
				memcpy(ENVIAR + offset, &(PCB->CodePointer->instrucciones_serializado[i].offset), size_to_send);
				offset += size_to_send;
			}
		//Etiquetas size
		size_to_send = sizeof(PCB->CodePointer->etiquetas_size);
		memcpy(ENVIAR + offset, &(PCB->CodePointer->etiquetas_size), size_to_send);
		offset += size_to_send;
		//Etiquetas
		size_to_send = PCB->CodePointer->etiquetas_size;
		memcpy(ENVIAR + offset, PCB->CodePointer->etiquetas, size_to_send);
		offset += size_to_send;
		//Cantidad de funciones
		size_to_send = sizeof(PCB->CodePointer->cantidad_de_funciones);
		memcpy(ENVIAR + offset, &(PCB->CodePointer->cantidad_de_funciones), size_to_send);
		offset += size_to_send;
		//Cantidad de etiquetas
		size_to_send = sizeof(PCB->CodePointer->cantidad_de_etiquetas);
		memcpy(ENVIAR + offset, &(PCB->CodePointer->cantidad_de_etiquetas), size_to_send);
		offset += size_to_send;

	//TagsPointer
	size_to_send = sizeof(PCB->TagsPointer);
	memcpy(ENVIAR + offset, &(PCB->TagsPointer),size_to_send);
	offset += size_to_send;

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
	PCB->CodePointer = malloc(sizeof(t_metadata_program));
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
		size_to_recive = sizeof(PCB->CodePointer->instruccion_inicio);
		memcpy(&PCB->CodePointer->instruccion_inicio, buffer + offset, size_to_recive);
		offset += size_to_recive;

		//Instrucciones size
		size_to_recive = sizeof(PCB->CodePointer->instrucciones_size);
		memcpy(&PCB->CodePointer->instrucciones_size, buffer + offset, size_to_recive);
		offset += size_to_recive;

		//Puntero de instrucciones
			uint32_t i;
			PCB->CodePointer->instrucciones_serializado = malloc(sizeof(t_intructions) * PCB->CodePointer->instrucciones_size);
			for(i = 0; PCB->CodePointer->instrucciones_size > i; i++){
				size_to_recive = sizeof(t_puntero_instruccion);
				memcpy(&PCB->CodePointer->instrucciones_serializado[i].start, buffer + offset, size_to_recive);
				offset += size_to_recive;

				size_to_recive = sizeof(t_size);
				memcpy(&PCB->CodePointer->instrucciones_serializado[i].offset, buffer + offset, size_to_recive);
				offset += size_to_recive;
			}

		//Etiquetas size
		size_to_recive = sizeof(PCB->CodePointer->etiquetas_size);
		memcpy(&PCB->CodePointer->etiquetas_size, buffer + offset, size_to_recive);
		offset += size_to_recive;

		//Etiquetas
		size_to_recive = PCB->CodePointer->etiquetas_size;
		PCB->CodePointer->etiquetas = (char*) malloc(sizeof(char) * PCB->CodePointer->etiquetas_size);
		PCB->CodePointer->etiquetas = '\0';
		memcpy(&PCB->CodePointer->etiquetas, buffer + offset, size_to_recive);
		offset += size_to_recive;

		//Cantidad de funciones
		size_to_recive = sizeof(PCB->CodePointer->cantidad_de_funciones);
		memcpy(&PCB->CodePointer->cantidad_de_funciones, buffer + offset, size_to_recive);
		offset += size_to_recive;

		//Cantidad de etiquetas
		size_to_recive = sizeof(PCB->CodePointer->cantidad_de_etiquetas);
		memcpy(&PCB->CodePointer->cantidad_de_etiquetas, buffer + offset, size_to_recive);
		offset += size_to_recive;

	//TagsPointer
	size_to_recive = sizeof(PCB->TagsPointer);
	memcpy(&PCB->TagsPointer, buffer + offset, sizeof(PCB->TagsPointer));
	offset += size_to_recive;

	size_to_recive = sizeof(PCB->StackPointer);
	memcpy(&PCB->StackPointer, buffer + offset, sizeof(PCB->StackPointer));
	offset += size_to_recive;

	size_to_recive = sizeof(PCB->ExitCode);
	memcpy(&PCB->ExitCode, buffer + offset, sizeof(PCB->ExitCode));
	offset += size_to_recive;

	free(buffer);
}
