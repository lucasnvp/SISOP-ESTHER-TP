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

void serializar_pcb(int client, PCB_t* PCB){
	void* ENVIAR = malloc(sizeof(PCB_t));
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

	size_to_send = sizeof(PCB->CodePointer);
	memcpy(ENVIAR + offset, &(PCB->CodePointer),size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(PCB->TagsPointer);
	memcpy(ENVIAR + offset, &(PCB->TagsPointer),size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(PCB->StackPointer);
	memcpy(ENVIAR + offset, &(PCB->StackPointer),size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(PCB->ExitCode);
	memcpy(ENVIAR + offset, &(PCB->ExitCode),size_to_send);
	offset += size_to_send;

	send_data(client, ENVIAR, offset);
	free(ENVIAR);
}

void deserializar_pcb(int servidor, PCB_t* PCB){
	uint32_t buffer_size;
	void* buffer = malloc(buffer_size = sizeof(PCB_t));
	uint32_t offset = 0;
	uint32_t size_to_recive;

	recive_data(servidor, buffer, sizeof(PCB_t));

	size_to_recive = sizeof(PCB->PID);
	memcpy(&PCB->PID, buffer + offset, sizeof(PCB->PID));
	offset += size_to_recive;

	size_to_recive = sizeof(PCB->ProgramCounter);
	memcpy(&PCB->ProgramCounter, buffer + offset, sizeof(PCB->ProgramCounter));
	offset += size_to_recive;

	size_to_recive = sizeof(PCB->PageCode);
	memcpy(&PCB->PageCode, buffer + offset, sizeof(PCB->PageCode));
	offset += size_to_recive;

	size_to_recive = sizeof(PCB->CodePointer);
	memcpy(&PCB->CodePointer, buffer + offset, sizeof(PCB->CodePointer));
	offset += size_to_recive;

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
