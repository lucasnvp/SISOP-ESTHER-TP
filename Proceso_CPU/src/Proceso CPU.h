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
#include <commons/log.h>

char* PATH_CONFIG =
		"/home/utnso/Blacklist/tp-2017-1c-Blacklist/Proceso_CPU/src/config/config.txt";
Type_Config config;

//VARIABLES PARA LOGS
char* PATH_LOG = "/home/utnso/Blacklist/Logs/logCPU.txt";
t_log* log_Console;  //MUESTRA POR PANTALLA
t_log* log_CPU; //REGISTRA

//SOCKETS DE CONEXION
uint32_t kernel;
uint32_t memoria;

//PCB QUE VA A ESTAR CARGADO AL MOMENTO DE EJECUTAR
PCB_t *pcbActivo;

//TAMANIO DE PAGINA
uint32_t tamanio_pagina;

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
void connection_handler(uint32_t socket, uint32_t command);
void init_log(char* pathLog);

#endif /* PROCESO_CPU_H_ */
