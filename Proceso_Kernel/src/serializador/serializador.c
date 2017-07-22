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

	uint32_t len_etiquetas = 0;
	if(PCB->CodeTagsPointer->etiquetas != NULL){
		len_etiquetas = strlen(PCB->CodeTagsPointer->etiquetas);
	}

	uint32_t datos_size = 	sizeof(PCB_t) +
							sizeof(t_metadata_program) +
							sizeof(t_intructions) * PCB->CodeTagsPointer->instrucciones_size +
							len_etiquetas;
	t_stream* ENVIAR = stream_create(datos_size);
	uint32_t offset = 0;
	uint32_t size_to_send;

	size_to_send = sizeof(PCB->PID);
	memcpy(ENVIAR->data + offset, &(PCB->PID),size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(PCB->ProgramCounter);
	memcpy(ENVIAR->data + offset, &(PCB->ProgramCounter),size_to_send);
	offset += size_to_send;

	size_to_send = sizeof(PCB->PageCode);
	memcpy(ENVIAR->data + offset, &(PCB->PageCode),size_to_send);
	offset += size_to_send;

	//Metadata Program
		//Instruccion de inicio
		size_to_send = sizeof(PCB->CodeTagsPointer->instruccion_inicio);
		memcpy(ENVIAR->data + offset, &(PCB->CodeTagsPointer->instruccion_inicio), size_to_send);
		offset += size_to_send;
		//Instrucciones size
		size_to_send = sizeof(PCB->CodeTagsPointer->instrucciones_size);
		memcpy(ENVIAR->data + offset, &(PCB->CodeTagsPointer->instrucciones_size), size_to_send);
		offset += size_to_send;
		//Puntero de instrucciones
			for(i = 0; PCB->CodeTagsPointer->instrucciones_size > i; i++){
				size_to_send = sizeof(t_puntero_instruccion);
				memcpy(ENVIAR->data + offset, &(PCB->CodeTagsPointer->instrucciones_serializado[i].start), size_to_send);
				offset += size_to_send;

				size_to_send = sizeof(t_size);
				memcpy(ENVIAR->data + offset, &(PCB->CodeTagsPointer->instrucciones_serializado[i].offset), size_to_send);
				offset += size_to_send;
			}
		//Etiquetas size
		size_to_send = sizeof(PCB->CodeTagsPointer->etiquetas_size);
		memcpy(ENVIAR->data + offset, &(PCB->CodeTagsPointer->etiquetas_size), size_to_send);
		offset += size_to_send;
		//Etiquetas
		size_to_send = sizeof(PCB->CodeTagsPointer->etiquetas);
		memcpy(ENVIAR->data + offset, &(len_etiquetas), size_to_send);
		offset += size_to_send;

		if(PCB->CodeTagsPointer->etiquetas != NULL){
			size_to_send = strlen(PCB->CodeTagsPointer->etiquetas) + 1;
			memcpy(ENVIAR->data + offset, PCB->CodeTagsPointer->etiquetas, size_to_send);
			offset += size_to_send;
		}
		//Cantidad de funciones
		size_to_send = sizeof(PCB->CodeTagsPointer->cantidad_de_funciones);
		memcpy(ENVIAR->data + offset, &(PCB->CodeTagsPointer->cantidad_de_funciones), size_to_send);
		offset += size_to_send;
		//Cantidad de etiquetas
		size_to_send = sizeof(PCB->CodeTagsPointer->cantidad_de_etiquetas);
		memcpy(ENVIAR->data + offset, &(PCB->CodeTagsPointer->cantidad_de_etiquetas), size_to_send);
		offset += size_to_send;

	//StrackPointer
	void serialize_element_stack(void* element){
		STACKPOINTER_T* lineStack = element;
		t_stream* stream_lineStack = stackpointer_serialize(lineStack);
		ENVIAR->data = realloc(ENVIAR->data, ENVIAR->size + stream_lineStack->size);
		memcpy(ENVIAR->data + offset, stream_lineStack->data, stream_lineStack->size);
		ENVIAR->size += stream_lineStack->size;
		offset += stream_lineStack->size;
		stream_destroy(stream_lineStack);
	}

	uint32_t count_line_stack = list_size(PCB->StackPointer);
	size_to_send = sizeof(count_line_stack);
	memcpy(ENVIAR->data + offset, &(count_line_stack),size_to_send);
	offset += size_to_send;

	list_iterate(PCB->StackPointer, serialize_element_stack);

	//ExitCode
	size_to_send = sizeof(PCB->ExitCode);
	memcpy(ENVIAR->data + offset, &(PCB->ExitCode),size_to_send);
	offset += size_to_send;

	//Quantum
	size_to_send = sizeof(PCB->Quantum);
	memcpy(ENVIAR->data + offset, &(PCB->Quantum),size_to_send);
	offset += size_to_send;

	serializar_int(client,ENVIAR->size);
	send_data(client, ENVIAR->data, ENVIAR->size);
	free(ENVIAR);
}

void deserializar_pcb(int servidor, PCB_t* PCB){
	PCB->CodeTagsPointer = malloc(sizeof(t_metadata_program));
	uint32_t buffer_size = deserializar_int(servidor);
	void* buffer = malloc(buffer_size);
	uint32_t offset = 0;
	uint32_t size_to_recive;
	uint32_t tmp_size = 0;

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
		size_to_recive = sizeof(PCB->CodeTagsPointer->etiquetas);
		memcpy(&PCB->CodeTagsPointer->etiquetas, buffer + offset, size_to_recive);
		offset += size_to_recive;

		if(PCB->CodeTagsPointer->etiquetas != NULL){
			PCB->CodeTagsPointer->etiquetas = strdup(buffer + offset);
			offset += strlen(PCB->CodeTagsPointer->etiquetas) + 1;
		}

		//Cantidad de funciones
		size_to_recive = sizeof(PCB->CodeTagsPointer->cantidad_de_funciones);
		memcpy(&PCB->CodeTagsPointer->cantidad_de_funciones, buffer + offset, size_to_recive);
		offset += size_to_recive;

		//Cantidad de etiquetas
		size_to_recive = sizeof(PCB->CodeTagsPointer->cantidad_de_etiquetas);
		memcpy(&PCB->CodeTagsPointer->cantidad_de_etiquetas, buffer + offset, size_to_recive);
		offset += size_to_recive;

	//StackPointer
	uint32_t count_line_stack = 0;
	size_to_recive = sizeof(count_line_stack);
	memcpy(&count_line_stack, buffer + offset, size_to_recive);
	offset += size_to_recive;

	for(i = 0; i < count_line_stack; ++i){
		STACKPOINTER_T* lineStack = stackpointer_deserialize(buffer + offset, &tmp_size);
		offset += tmp_size;
		list_add(PCB->StackPointer, lineStack);
	}

	//ExitCode
	size_to_recive = sizeof(PCB->ExitCode);
	memcpy(&PCB->ExitCode, buffer + offset, sizeof(PCB->ExitCode));
	offset += size_to_recive;

	//Quantum
	size_to_recive = sizeof(PCB->Quantum);
	memcpy(&PCB->Quantum, buffer + offset, sizeof(PCB->Quantum));
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
	uint32_t datos_size = 	sizeof(VARIABLE->id) + sizeof(VARIABLE->offset) +
							sizeof(VARIABLE->pagina) + sizeof(VARIABLE->size);
	t_stream* ENVIAR = stream_create(datos_size);
	uint32_t offset = 0;
	uint32_t size_to_send;

	size_to_send = sizeof(VARIABLE->id);
	memcpy(ENVIAR->data + offset, &(VARIABLE->id),size_to_send);
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

	size_to_recive = sizeof(variable->id);
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

	size_to_recive = sizeof(variable->id);
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

t_stream* stackpointer_serialize(STACKPOINTER_T* lineStack){
	uint32_t datos_size = sizeof(STACKPOINTER_T);
	t_stream* ENVIAR = stream_create(datos_size);
	uint32_t offset = 0;
	uint32_t size_to_send;

	void serialize_element_variables(void* element){
		VARIABLE_T* var = element;
		t_stream* stream_var = variable_t_serialize(var);
		ENVIAR->data = realloc(ENVIAR->data, ENVIAR->size + stream_var->size);
		memcpy(ENVIAR->data + offset, stream_var->data, stream_var->size);
		ENVIAR->size += stream_var->size;
		offset += stream_var->size;
		stream_destroy(stream_var);
	}

	uint32_t count_line_argumentos;
	if(lineStack->Argumentos != NULL){
		count_line_argumentos = list_size(lineStack->Argumentos);
	} else{
		count_line_argumentos = 0;
	}
	size_to_send = sizeof(count_line_argumentos);
	memcpy(ENVIAR->data + offset, &(count_line_argumentos),size_to_send);
	offset += size_to_send;

	if(lineStack->Argumentos != NULL){
		list_iterate(lineStack->Argumentos, serialize_element_variables);
	}

	uint32_t count_line_variables;
	if(lineStack->Variables != NULL){
		count_line_variables = list_size(lineStack->Variables);
	} else{
		count_line_variables = 0;
	}
	size_to_send = sizeof(count_line_variables);
	memcpy(ENVIAR->data + offset, &(count_line_variables),size_to_send);
	offset += size_to_send;

	if(lineStack->Variables != NULL){
		list_iterate(lineStack->Variables, serialize_element_variables);
	}

	size_to_send = sizeof(lineStack->DireccionDeRetorno);
	memcpy(ENVIAR->data + offset, &(lineStack->DireccionDeRetorno),size_to_send);
	offset += size_to_send;

	if(lineStack->VariableDeRetorno != NULL){
		uint32_t varDeRetorno = 1;
		size_to_send = sizeof(varDeRetorno);
		memcpy(ENVIAR->data + offset, &(lineStack->VariableDeRetorno),size_to_send);
		offset += size_to_send;

		t_stream* stream_variable = variable_t_serialize(lineStack->VariableDeRetorno);
		ENVIAR->data = realloc(ENVIAR->data, ENVIAR->size + stream_variable->size);
		memcpy(ENVIAR->data + offset, stream_variable->data, stream_variable->size);
		ENVIAR->size += stream_variable->size;
		offset += stream_variable->size;
		stream_destroy(stream_variable);
	} else{
		uint32_t varDeRetorno = 0;
		size_to_send = sizeof(varDeRetorno);
		memcpy(ENVIAR->data + offset, &(lineStack->VariableDeRetorno),size_to_send);
		offset += size_to_send;
	}

	return ENVIAR;
}

STACKPOINTER_T* stackpointer_deserialize(char* stream, int* size){
	STACKPOINTER_T* lineStack = malloc(sizeof(STACKPOINTER_T));
	uint32_t tmp_size = 0;
	uint32_t offset = 0;
	uint32_t size_to_recive;

	uint32_t count_line_argumentos = 0;
	size_to_recive = sizeof(count_line_argumentos);
	memcpy(&count_line_argumentos, stream + offset, size_to_recive);
	offset += size_to_recive;
	lineStack->Argumentos = list_create();

	for(j = 0; j < count_line_argumentos; ++j){
		VARIABLE_T* varStack = variable_t_deserialize(stream + offset, &tmp_size);
		offset += tmp_size;
		list_add(lineStack->Argumentos, varStack);
	}

	uint32_t count_line_variables = 0;
	size_to_recive = sizeof(count_line_variables);
	memcpy(&count_line_variables, stream + offset, size_to_recive);
	offset += size_to_recive;
	lineStack->Variables = list_create();

	for(j = 0; j < count_line_variables; ++j){
		VARIABLE_T* varStack = variable_t_deserialize(stream + offset, &tmp_size);
		offset += tmp_size;
		list_add(lineStack->Variables, varStack);
	}

	size_to_recive = sizeof(lineStack->DireccionDeRetorno);
	memcpy(&lineStack->DireccionDeRetorno, stream + offset, size_to_recive);
	offset += size_to_recive;

	size_to_recive = sizeof(lineStack->VariableDeRetorno);
	memcpy(&lineStack->VariableDeRetorno, stream + offset, size_to_recive);
	offset += size_to_recive;

	if(lineStack->VariableDeRetorno != NULL){
		lineStack->VariableDeRetorno = variable_t_deserialize(stream + offset, &tmp_size);
		offset += tmp_size;
	}

	*size = offset;

	return lineStack;
}

void serializar_stackpointer(int client, STACKPOINTER_T* lineStack){
	uint32_t datos_size = sizeof(STACKPOINTER_T);
	t_stream* ENVIAR = stream_create(datos_size);
	uint32_t offset = 0;
	uint32_t size_to_send;

	void serialize_element_variables(void* element){
		VARIABLE_T* var = element;
		t_stream* stream_var = variable_t_serialize(var);
		ENVIAR->data = realloc(ENVIAR->data, ENVIAR->size + stream_var->size);
		memcpy(ENVIAR->data + offset, stream_var->data, stream_var->size);
		ENVIAR->size += stream_var->size;
		offset += stream_var->size;
		stream_destroy(stream_var);
	}

	uint32_t count_line_argumentos;
	if(lineStack->Argumentos != NULL){
		count_line_argumentos = list_size(lineStack->Argumentos);
	} else{
		count_line_argumentos = 0;
	}
	size_to_send = sizeof(count_line_argumentos);
	memcpy(ENVIAR->data + offset, &(count_line_argumentos),size_to_send);
	offset += size_to_send;

	if(lineStack->Argumentos != NULL){
		list_iterate(lineStack->Argumentos, serialize_element_variables);
	}

	uint32_t count_line_variables;
	if(lineStack->Variables != NULL){
		count_line_variables = list_size(lineStack->Variables);
	} else{
		count_line_variables = 0;
	}
	size_to_send = sizeof(count_line_variables);
	memcpy(ENVIAR->data + offset, &(count_line_variables),size_to_send);
	offset += size_to_send;

	if(lineStack->Variables != NULL){
		list_iterate(lineStack->Variables, serialize_element_variables);
	}

	size_to_send = sizeof(lineStack->DireccionDeRetorno);
	memcpy(ENVIAR->data + offset, &(lineStack->DireccionDeRetorno),size_to_send);
	offset += size_to_send;

	if(lineStack->VariableDeRetorno != NULL){
		uint32_t varDeRetorno = 1;
		size_to_send = sizeof(varDeRetorno);
		memcpy(ENVIAR->data + offset, &(lineStack->VariableDeRetorno),size_to_send);
		offset += size_to_send;

		t_stream* stream_variable = variable_t_serialize(lineStack->VariableDeRetorno);
		ENVIAR->data = realloc(ENVIAR->data, ENVIAR->size + stream_variable->size);
		memcpy(ENVIAR->data + offset, stream_variable->data, stream_variable->size);
		ENVIAR->size += stream_variable->size;
		offset += stream_variable->size;
		stream_destroy(stream_variable);
	} else{
		uint32_t varDeRetorno = 0;
		size_to_send = sizeof(varDeRetorno);
		memcpy(ENVIAR->data + offset, &(lineStack->VariableDeRetorno),size_to_send);
		offset += size_to_send;
	}

	serializar_int(client, ENVIAR->size);
	send_data(client, ENVIAR->data, ENVIAR->size);
	free(ENVIAR);
}

STACKPOINTER_T* deserializar_stackpointer(int servidor){
	uint32_t buffer_size = deserializar_int(servidor);
	STACKPOINTER_T* lineStack = malloc(sizeof(STACKPOINTER_T));
	void* buffer = malloc(buffer_size);
	uint32_t tmp_size = 0;
	uint32_t offset = 0;
	uint32_t size_to_recive;

	recive_data(servidor, buffer, buffer_size);

	uint32_t count_line_argumentos = 0;
	size_to_recive = sizeof(count_line_argumentos);
	memcpy(&count_line_argumentos, buffer + offset, size_to_recive);
	offset += size_to_recive;
	lineStack->Argumentos = list_create();

	for(i = 0; i < count_line_argumentos; ++i){
		VARIABLE_T* varStack = variable_t_deserialize(buffer + offset, &tmp_size);
		offset += tmp_size;
		list_add(lineStack->Argumentos, varStack);
	}

	uint32_t count_line_variables = 0;
	size_to_recive = sizeof(count_line_variables);
	memcpy(&count_line_variables, buffer + offset, size_to_recive);
	offset += size_to_recive;
	lineStack->Variables = list_create();

	for(i = 0; i < count_line_variables; ++i){
		VARIABLE_T* varStack = variable_t_deserialize(buffer + offset, &tmp_size);
		offset += tmp_size;
		list_add(lineStack->Variables, varStack);
	}

	size_to_recive = sizeof(lineStack->DireccionDeRetorno);
	memcpy(&lineStack->DireccionDeRetorno, buffer + offset, size_to_recive);
	offset += size_to_recive;

	size_to_recive = sizeof(lineStack->VariableDeRetorno);
	memcpy(&lineStack->VariableDeRetorno, buffer + offset, size_to_recive);
	offset += size_to_recive;

	if(lineStack->VariableDeRetorno != NULL){
		lineStack->VariableDeRetorno = variable_t_deserialize(buffer + offset, &tmp_size);
		offset += tmp_size;
	}

	free(buffer);
	return lineStack;
}
