#include "pcb.h"

PCB_t* PCB_new_pointer(	uint32_t PID, uint32_t PageCode, t_metadata_program * CodePointer, uint32_t Quantum){
	PCB_t * unPCB = malloc(sizeof(PCB_t));
	unPCB->PID = PID;
	unPCB->ProgramCounter = 0;
	unPCB->PageCode = PageCode;
	unPCB->CodeTagsPointer = CodePointer;
	unPCB->StackPointer = list_create();
	unPCB->ExitCode = 0;
	unPCB->Quantum = Quantum;
	return unPCB;
}

void PCB_free(PCB_t* this){
	free(this);
}

void print_PCB(PCB_t* auxPCB){
	int i;
	printf("PID: %d\n",auxPCB->PID);
	printf("ProgramCounter: %d\n",auxPCB->ProgramCounter);
	printf("PageCode: %d\n",auxPCB->PageCode);

	printf("CodeTagsPointer\n");
		printf("-Instruccion Inicio: %d\n", auxPCB->CodeTagsPointer->instruccion_inicio);
		printf("-Instrucciones size: %d\n", auxPCB->CodeTagsPointer->instrucciones_size);
		printf("-Instrucciones serializado: \n");
		for(i = 0; auxPCB->CodeTagsPointer->instrucciones_size > i; i++){
			printf("--Instrucccion = Puntero: %d , Size: %d \n",
					auxPCB->CodeTagsPointer->instrucciones_serializado[i].start,
					auxPCB->CodeTagsPointer->instrucciones_serializado[i].offset);
		}
		printf("-Etiquetas size: %d\n", auxPCB->CodeTagsPointer->etiquetas_size);
		printf("-Etiquetas: %s\n", auxPCB->CodeTagsPointer->etiquetas);
		printf("-Cantidad de funciones: %d\n", auxPCB->CodeTagsPointer->cantidad_de_funciones);
		printf("-Cantidad de etiquetas: %d\n", auxPCB->CodeTagsPointer->cantidad_de_etiquetas);

	printf("StackPointer: %d\n", list_size(auxPCB->StackPointer));
	sizeStack = list_size(auxPCB->StackPointer);
	for(i=0;i < sizeStack;i++){
		printf("Line StackPointer: %d\n", i);
		STACKPOINTER_T * lineSP = list_get(auxPCB->StackPointer,i);
		print_LineStack(lineSP);
	}
	printf("ExitCode: %d\n",auxPCB->ExitCode);
	printf("Quantum: %d\n",auxPCB->Quantum);
}

PCB_t* buscar_PCB(uint32_t pid,t_queue* QUEUE_PCB){
	uint32_t elemn;
	uint32_t sizeQueue = queue_size(QUEUE_PCB);
	PCB_t* pcb;

	for(elemn = 0;elemn < sizeQueue; elemn++){
		pcb = (PCB_t*) queue_pop(QUEUE_PCB);
		if(pcb->PID == pid){
			return pcb;
		}
		queue_push(QUEUE_PCB, pcb);
	}
	return 0;
}

VARIABLE_T* variable_new(char id, uint32_t pagina, uint32_t offset, uint32_t size){
	VARIABLE_T * unaVariable = malloc(sizeof(VARIABLE_T));
	unaVariable->id = id;
	unaVariable->pagina = pagina;
	unaVariable->offset = offset;
	unaVariable->size = size;
	return unaVariable;
}

void variable_free(VARIABLE_T* this){
	free(this);
}

STACKPOINTER_T* stack_new(t_list* Argumentos, t_list* Variables, uint32_t DireccionDeRetorno, VARIABLE_T * VariableDeRetorno){
	STACKPOINTER_T * unStack = malloc(sizeof(STACKPOINTER_T));
	unStack->Argumentos = Argumentos;
	unStack->Variables = Variables;
	unStack->DireccionDeRetorno = DireccionDeRetorno;
	unStack->VariableDeRetorno = VariableDeRetorno;
	return unStack;
}

void stack_free(STACKPOINTER_T* this){
	free(this);
}

void print_variable(VARIABLE_T* auxVariable){
	printf("--ID: %c\n",auxVariable->id);
	printf("--Pagina: %d\n",auxVariable->pagina);
	printf("--Offset: %d\n",auxVariable->offset);
	printf("--Size: %d\n",auxVariable->size);
}

void print_LineStack(STACKPOINTER_T* auxStackPointer){
	if(auxStackPointer->Argumentos != NULL){
		printf("-Argumentos: %d\n", list_size(auxStackPointer->Argumentos));
		int sizeArgs = list_size(auxStackPointer->Argumentos);
		for(i=0;i < sizeArgs; i++){
			printf("-Argumento: %d\n", i);
			VARIABLE_T * varArg = list_get(auxStackPointer->Argumentos,i);
			print_variable(varArg);
		}
	} else{
		printf("-Argumentos: NULL\n");
	}

	if(auxStackPointer->Variables != NULL){
		printf("-Variables: %d\n", list_size(auxStackPointer->Variables));
		int sizeVars = list_size(auxStackPointer->Variables);
		for(i=0;i < sizeVars;i++){
			printf("-Variable: %d\n", i);
			VARIABLE_T * varVariable = list_get(auxStackPointer->Variables,i);
			print_variable(varVariable);
		}
	} else{
		printf("-Variables: NULL\n");
	}

	printf("-Direccion de retorno: %d\n", auxStackPointer->DireccionDeRetorno);

	if(auxStackPointer->VariableDeRetorno != NULL){
		printf("-Variable de retorno\n");
		print_variable(auxStackPointer->VariableDeRetorno);
	} else{
		printf("-Variable de retorno: NULL\n");
	}
}

void set_PageCode(PCB_t* PCB, uint32_t PageCode){
	PCB->PageCode = PageCode;
}

void set_Quantum(PCB_t* PCB, uint32_t Quantum){
	PCB->Quantum = Quantum;
}

void next_ProgramCounter(PCB_t* this){
	this->ProgramCounter++;
}
