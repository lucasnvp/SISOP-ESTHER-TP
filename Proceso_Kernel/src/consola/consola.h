#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <stdio.h>
#include <inttypes.h>

#include <commons/collections/queue.h>

#include "../pcb/pcb.h"

typedef struct {
	uint32_t kernel;
	char* comando;
	char* argumento;
} t_Consola;

t_Consola leerComandos();
void limpiarBufferDeEntrada();
void list_process(t_list * LIST_READY);
void kill_process(t_list * lista,uint32_t id_process);

#endif /* CONSOLA_H_ */
