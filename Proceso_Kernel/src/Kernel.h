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
#include <commons/collections/list.h>
#include <commons/log.h>

#include "pcb/pcb.h"
#include "programa/programa.h"
#include "servidor/servidor.h"
#include "serializador/serializador.h"
#include "consola/consola.h"
#include "config/config_kernel.h"
#include "cpu/cpu.h"
#include "variable-compartida/variable-compartida.h"

#include "dummy-componets/dummy-program.h"

#define NUEVO_PROCESO 1
#define NUEVA_CONEXION_CPU 2
#define FIN_EJECUCION_CPU 3
#define IMPRIMIR_POR_PANTALLA 4
#define KILL_PROCESS 5
#define KILL_ALL_PROCESS 6
#define WAIT_CPU 7
#define SIGNAL_CPU 8
#define LECTURA_VARIABLE_COMPARTIDA 9
#define ESCRITURA_VARIABLE_COMPARTIDA 10

pthread_mutex_t mutexPCB; //Para asegurar la mutua exclusion en la cola de PCB
sem_t SEM_MULTIPROGRAMACION; // semaforo de multiprogramacion
sem_t SEM_PCB; //Para que la cola de PCB se suspenda si no tiene entradas
sem_t SEM_READY; //Para que la lista de READYs se supenda si no tiene entradas
sem_t SEM_STOP_PLANNING; //Para la planificacion
sem_t SEM_COMMAND; //Espera a un nuevo comando
sem_t SEM_CPU; //Planifica si hay CPUs conectadas
sem_t SEM_CPU_DISPONIBLE; //Hay CPUs disponibles
sem_t SEM_EXECUTE; //Hay procesos para ejecutar

uint32_t PID_PCB = 1;
t_queue * QUEUE_PCB;
t_queue * QUEUE_NEW;
t_queue * QUEUE_EXIT;
t_queue * QUEUE_CPUS;
t_queue * QUEUE_READY;
t_queue * QUEUE_EXEC;
t_list * LIST_CONSOLAS;

char* PATH_CONFIG = "/home/utnso/Blacklist/tp-2017-1c-Blacklist/Proceso_Kernel/src/config/config.txt";
//char* PATH_CONFIG = "../src/config/config.txt";
Type_Config config;

char* PATH_LOG = "/home/utnso/Blacklist/Logs/logKernel.txt";
t_log* log_Console;
t_log* log_Kernel;

uint32_t SERVIDOR_KERNEL;
uint32_t SERVIDOR_MEMORIA;
uint32_t SERVIDOR_FILESYSTEM;

void connect_server_memoria();
void connect_server_filesystem();

void procesarPCB(void* args);
void server(void* args);
void consola_kernel(void* args);
void planificador(void* args);
void connection_handler(uint32_t socket, uint32_t command);
void init_log(char* pathLog);
void ejecutar();

void* queue_sync_pop(t_queue* self);
void queue_sync_push(t_queue* self, void* element);

#endif /* KERNEL_H_ */
