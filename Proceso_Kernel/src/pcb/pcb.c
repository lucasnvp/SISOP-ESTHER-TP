#include "pcb.h"

PCB PCB_new(	int32_t PID, int32_t ProgramCounter, int32_t PageCode, int32_t CodePointer,
				int32_t TagsPointer, int32_t StackPointer, int32_t ExitCode){
	PCB unPCB;
	unPCB.PID = PID;
	unPCB.ProgramCounter = ProgramCounter;
	unPCB.PageCode = PageCode;
	unPCB.CodePointer = CodePointer;
	unPCB.TagsPointer = TagsPointer;
	unPCB.StackPointer = StackPointer;
	unPCB.ExitCode = ExitCode;
	return unPCB;
}

PCB * PCB_new_pointer(	int32_t PID, int32_t ProgramCounter, int32_t PageCode, int32_t CodePointer,
						int32_t TagsPointer, int32_t StackPointer, int32_t ExitCode){
	PCB * unPCB = malloc(sizeof(PCB));
	unPCB->PID = PID;
	unPCB->ProgramCounter = ProgramCounter;
	unPCB->PageCode = PageCode;
	unPCB->CodePointer = CodePointer;
	unPCB->TagsPointer = TagsPointer;
	unPCB->StackPointer = StackPointer;
	unPCB->ExitCode = ExitCode;
	return unPCB;
}

void PCB_free(PCB *this){
	free(this);
}

void print_PCB(PCB * auxPCB){
	printf("PID: %d\n",auxPCB->PID);
	printf("ProgramCounter: %d\n",auxPCB->ProgramCounter);
	printf("PageCOde: %d\n",auxPCB->PageCode);
	printf("CodePointer: %d\n",auxPCB->CodePointer);
	printf("TagsPointer: %d\n",auxPCB->TagsPointer);
	printf("StackPointer: %d\n",auxPCB->StackPointer);
	printf("ExitCode: %d\n",auxPCB->ExitCode);
}
