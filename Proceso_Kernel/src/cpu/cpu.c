#include "cpu.h"

CPU_t* CPU_new(uint32_t CPU_ID){
	CPU_t * unCPU = malloc(sizeof(CPU_t));
	unCPU->CPU_ID = CPU_ID;
	unCPU->Status = disponible;
	unCPU->PCB = 0;
	return unCPU;
}

void print_CPU(CPU_t* auxCPU){
	printf("CPU ID: %d\n",auxCPU->CPU_ID);
	printf("Status: %d\n",auxCPU->Status);
}

void CPU_free(CPU_t* this){
	free(this);
}

void AsignarPCB(CPU_t* cpu, PCB_t* pcb){
	cpu->Status = ocupado;
	cpu->PCB = pcb;
}

void LiberarCPU(CPU_t* cpu){
	cpu->Status = disponible;
	cpu->PCB = 0;
}

CPU_t* buscar_CPU(uint32_t idCPU,t_queue* QUEUE_CPUS){
	uint32_t elemn;
	uint32_t sizeQueue = queue_size(QUEUE_CPUS);
	CPU_t* cpu;

	for(elemn = 0;elemn < sizeQueue; elemn++){
		cpu = (CPU_t*) queue_pop(QUEUE_CPUS);
		if(cpu->CPU_ID == idCPU){
			return cpu;
		}
		queue_push(QUEUE_CPUS, cpu);
	}
	return 0;
}
