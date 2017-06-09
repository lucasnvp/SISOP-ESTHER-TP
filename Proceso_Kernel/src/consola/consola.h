#ifndef CONSOLA_H_
#define CONSOLA_H_

#include <stdio.h>
#include <inttypes.h>

#include <commons/collections/queue.h>

#include "../pcb/pcb.h"
#include "../programa/programa.h"

typedef struct {
	uint32_t kernel;
	char* comando;
	char* argumento;
} t_Consola;

t_Consola leerComandos();
void limpiarBufferDeEntrada();
void list_process(t_list * LIST_READY);
void list_console(t_list * LIST_CONSOLE);
void list_new(t_queue * QUEUE_NEW);
void kill_process(t_list * lista,uint32_t id_process);
void status_process(t_list * lista,uint32_t id_process);

#endif /* CONSOLA_H_ */
