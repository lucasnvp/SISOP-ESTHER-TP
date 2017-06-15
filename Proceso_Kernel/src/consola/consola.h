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
void list_console(t_list * LIST_CONSOLE);
void list_queue(t_queue * QUEUE, uint32_t mensaje);
void list_new(t_queue * QUEUE_NEW);
void list_ready(t_queue * QUEUE_READY);
void kill_process(t_queue * queue,uint32_t pid);
void status_process(t_queue * queue,uint32_t pid);

#endif /* CONSOLA_H_ */
