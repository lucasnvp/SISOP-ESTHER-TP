#ifndef PCB_H_
#define PCB_H_

#include <inttypes.h>

typedef struct PCBType{
	int32_t PID;
	int32_t ProgramCounter;
	int32_t PageCode;
	int32_t CodePointer;
	int32_t TagsPointer;
	int32_t StackPointer;
	int32_t ExitCode;
} PCB;

PCB PCB_new(	int32_t PID, int32_t ProgramCounter, int32_t PageCode, int32_t CodePointer,
				int32_t TagsPointer, int32_t StackPointer, int32_t ExitCode);

PCB * PCB_new_pointer(	int32_t PID, int32_t ProgramCounter, int32_t PageCode, int32_t CodePointer,
						int32_t TagsPointer, int32_t StackPointer, int32_t ExitCode);

void PCB_free(PCB *this);

void print_PCB(PCB * auxPCB);

#endif /* PCB_H_ */
