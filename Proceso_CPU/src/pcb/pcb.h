#ifndef PCB_H_
#define PCB_H_

#include <inttypes.h>

#include <commons/collections/queue.h>
#include <parser/metadata_program.h>

typedef struct PCBType{
	uint32_t PID;
	uint32_t ProgramCounter;
	uint32_t PageCode;
	t_metadata_program * CodeTagsPointer;
	t_queue* StackPointer;
	int32_t ExitCode;
} PCB_t;

typedef struct VARIABLE_T{
	char id;
	uint32_t pagina;
	uint32_t offset;
	uint32_t size;
} VARIABLE_T;

typedef struct pedido_memoria{
	uint32_t id;
	uint32_t pagina;
	uint32_t offset;
	uint32_t size;
} t_pedido_memoria;

typedef struct STACKPOINTER_T{
	t_queue* Argumentos;
	t_queue* Variables;
	uint32_t DireccionDeRetorno;
	VARIABLE_T * VariableDeRetorno;
} STACKPOINTER_T;

int i;
int sizePila;

PCB_t* PCB_new_pointer(uint32_t PID, uint32_t PageCode, t_metadata_program * CodePointer);
STACKPOINTER_T* stack_new(t_queue* Argumentos, t_queue* Variables, uint32_t DireccionDeRetorno, VARIABLE_T * VariableDeRetorno);
VARIABLE_T* variable_new(char id, uint32_t pagina, uint32_t offset, uint32_t size);

PCB_t* buscar_PCB(uint32_t pid,t_queue* QUEUE_PCB);

void push_stack(PCB_t* pcb, STACKPOINTER_T* unStack);
STACKPOINTER_T * pull_stack(PCB_t* pcb);

void print_PCB(PCB_t* auxPCB);
void print_LineStack(STACKPOINTER_T* auxStackPointer);
void print_variable(VARIABLE_T* auxVariable);

void PCB_free(PCB_t* this);
void stack_free(STACKPOINTER_T* this);
void variable_free(VARIABLE_T* this);

#endif /* PCB_H_ */
