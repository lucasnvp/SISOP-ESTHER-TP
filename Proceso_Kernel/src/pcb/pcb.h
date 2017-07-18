#ifndef PCB_H_
#define PCB_H_

#include <inttypes.h>

#include <commons/collections/queue.h>

typedef struct PCBType {
	uint32_t PID;
	uint32_t ProgramCounter;
	uint32_t PageCode;
	uint32_t CodePointer;
	uint32_t TagsPointer;
	uint32_t StackPointer;
	uint32_t ExitCode;
} PCB_t;

PCB_t PCB_new(uint32_t PID, uint32_t ProgramCounter, uint32_t PageCode,
		uint32_t CodePointer, uint32_t TagsPointer, uint32_t StackPointer,
		uint32_t ExitCode);

PCB_t* PCB_new_pointer(uint32_t PID, uint32_t ProgramCounter, uint32_t PageCode,
		uint32_t CodePointer, uint32_t TagsPointer, uint32_t StackPointer,
		uint32_t ExitCode);

PCB_t* buscar_PCB(uint32_t pid, t_queue* QUEUE_PCB);

void PCB_free(PCB_t* this);
void print_PCB(PCB_t* auxPCB);

#endif /* PCB_H_ */
