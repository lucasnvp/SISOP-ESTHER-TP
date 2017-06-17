#ifndef PROCESO_CPU_H_
#define PROCESO_CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <parser/parser.h>
#include <parser/metadata_program.h>
#include "servidor/servidor.h"
#include "primitivas/ansisop_primitivas.h"
#include "config/config_CPU.h"
#include "serializador/serializador.h"
#include "pcb/pcb.h"

char* PATH_CONFIG = "../src/config/config.txt";
Type_Config config;

uint32_t kernel;
uint32_t memoria;

PCB_t *pcbActivo;

//TIPOS DE MENSAJES
#define IMPRIMIR_TEXTO 1;
#define IMPRIMIR_VALOR 2;

// USAR ESTOS MENSAJES
#define HANDSHAKE_CPU_KERNEL 2
#define FIN_CORRECTO 3
#define HANDSHAKE_CPU_MEMORIA 5

void ejecutar();
void connect_server_kernel();
void connect_server_memoria();
char* const solicitarInstruccionAMemoria();

#endif /* PROCESO_CPU_H_ */
