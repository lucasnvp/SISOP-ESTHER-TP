#include "pcb.h"

PCB_t PCB_new(	uint32_t PID, uint32_t ProgramCounter, uint32_t PageCode, uint32_t CodePointer,
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

PCB_t* PCB_new_pointer(	uint32_t PID, uint32_t ProgramCounter, uint32_t PageCode, uint32_t CodePointer,
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
	printf("PID: %d\n",auxPCB->PID);
	printf("ProgramCounter: %d\n",auxPCB->ProgramCounter);
	printf("PageCOde: %d\n",auxPCB->PageCode);
	printf("CodePointer: %d\n",auxPCB->CodePointer);
	printf("TagsPointer: %d\n",auxPCB->TagsPointer);
	printf("StackPointer: %d\n",auxPCB->StackPointer);
	printf("ExitCode: %d\n",auxPCB->ExitCode);
}
