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
#include "consola/consola.h"
#include "config/config_kernel.h"

pthread_mutex_t mutexPCB; //Para asegurar la mutua exclusion en la cola de PCB
sem_t SEM_MULTIPROGRAMACION;
sem_t SEM_PCB; //Para que la cola de PCB se suspenda si no tiene entradas
sem_t SEM_READY; //Para que la lista de READYs se supenda si no tiene entradas
sem_t SEM_STOP_PLANNING;

uint32_t PID_PCB = 1;
t_queue * QUEUE_PCB;
t_queue * QUEUE_NEW;
t_list * LIST_READY;

char* PATH_CONFIG = "../src/config/config.txt";
Type_Config config;

uint32_t SERVIDOR_KERNEL;
uint32_t SERVIDOR_MEMORIA;
uint32_t SERVIDOR_FILESYSTEM;

void connect_server_memoria();
void connect_server_filesystem();

void procesarPCB(void* args);
void server(void* args);
void consola_kernel(void* args);
void planificador(void* args);

void* queue_sync_pop(t_queue* self);
void queue_sync_push(t_queue* self, void* element);

#endif /* KERNEL_H_ */
