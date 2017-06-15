#ifndef CPU_H_
#define CPU_H_

#include <inttypes.h>

#include <commons/collections/queue.h>

#include "../pcb/pcb.h"

#define disponible 0
#define ocupado 1

typedef struct CPUType{
	uint32_t CPU_ID;
	uint32_t Status;
	PCB_t* PCB;
} CPU_t;

CPU_t* CPU_new(uint32_t CPU_ID);
CPU_t* buscar_CPU(uint32_t idCPU, t_queue* QUEUE_CPUS);

void print_CPU(CPU_t* auxCPU);
void CPU_free(CPU_t* this);
void AsignarPCB(CPU_t* cpu, PCB_t* PCB);
void LiberarCPU(CPU_t* cpu);

#endif /* CPU_H_ */
