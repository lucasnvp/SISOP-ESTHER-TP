#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>

#include <commons/collections/queue.h>

#include "pcb/pcb.h"
#include "programa/programa.h"
#include "servidor/servidor.h"
#include "serializador/serializador.h"

void connect_server_memoria();
void connect_server_filesystem();

void procesarPCB(void* args);
void server(void* args);

void* queue_sync_pop(t_queue* self);
void queue_sync_push(t_queue* self, void* element);

#endif /* KERNEL_H_ */
