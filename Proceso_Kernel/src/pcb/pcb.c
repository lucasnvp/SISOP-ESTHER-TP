#include "pcb.h"

PCB_t PCB_new(	uint32_t PID, uint32_t ProgramCounter, uint32_t PageCode, t_metadata_program * CodePointer,
				uint32_t TagsPointer, uint32_t StackPointer, uint32_t ExitCode){
	PCB_t unPCB;
	unPCB.PID = PID;
	unPCB.ProgramCounter = ProgramCounter;
	unPCB.PageCode = PageCode;
	unPCB.CodePointer = CodePointer;
	unPCB.TagsPointer = TagsPointer;
	unPCB.StackPointer = StackPointer;
	unPCB.ExitCode = ExitCode;
	return unPCB;
}

PCB_t* PCB_new_pointer(	uint32_t PID, uint32_t ProgramCounter, uint32_t PageCode, t_metadata_program * CodePointer,
						uint32_t TagsPointer, uint32_t StackPointer, uint32_t ExitCode){
	PCB_t * unPCB = malloc(sizeof(PCB_t));
	unPCB->PID = PID;
	unPCB->ProgramCounter = ProgramCounter;
	unPCB->PageCode = PageCode;
	unPCB->CodePointer = CodePointer;
	unPCB->TagsPointer = TagsPointer;
	unPCB->StackPointer = StackPointer;
	unPCB->ExitCode = ExitCode;
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

	printf("CodePointer\n");
		printf("-Instruccion Inicio: %d\n", auxPCB->CodePointer->instruccion_inicio);
		printf("-Instrucciones size: %d\n", auxPCB->CodePointer->instrucciones_size);
		printf("-Instrucciones serializado: \n");
		for(i = 0; auxPCB->CodePointer->instrucciones_size > i; i++){
			printf("--Instrucccion = Puntero: %d , Size: %d \n",
					auxPCB->CodePointer->instrucciones_serializado[i].start,
					auxPCB->CodePointer->instrucciones_serializado[i].offset);
		}
		printf("-Etiquetas size: %d\n", auxPCB->CodePointer->etiquetas_size);
		printf("-Etiquetas: %s\n", auxPCB->CodePointer->etiquetas);
		printf("-Cantidad de funciones: %d\n", auxPCB->CodePointer->cantidad_de_funciones);
		printf("-Cantidad de etiquetas: %d\n", auxPCB->CodePointer->cantidad_de_etiquetas);

	printf("TagsPointer: %d\n",auxPCB->TagsPointer);
	printf("StackPointer: %d\n",auxPCB->StackPointer);
	printf("ExitCode: %d\n",auxPCB->ExitCode);
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
