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

//PCB QUE VA A ESTAR CARGADO AL MOMENTO DE EJECUTAR
PCB_t *pcbActivo;

char *programa;

//TAMANIO DE PAGINA
uint32_t tamanio_pagina;

//SOCKETS DE CONEXION
uint32_t kernel;
uint32_t memoria;

//TIPOS DE MENSAJES
#define IMPRIMIR_TEXTO 1;
#define IMPRIMIR_VALOR 2;

// USAR ESTOS MENSAJES
#define HANDSHAKE_CPU_KERNEL 2
#define FIN_CORRECTO 3
#define HANDSHAKE_CPU_MEMORIA 5
#define SOLICITUD_INSTRUCCION_MEMORIA 6
#define ASIGNAR_VALOR_A_MEMORIA 7
#define PEDIDO_VAR_COMPARTIDA 10 // Ver que numero es el que le sirve al kernel

void ejecutar();
void connect_server_kernel();
void connect_server_memoria();
char* const solicitarInstruccionAMemoria();
void enviarDatoAMemoria(uint32_t pagina, uint32_t offset, uint32_t size);
void connection_handler(uint32_t socket, uint32_t command);
void init_log(char* pathLog);
t_puntero ansi_definirVariable(t_nombre_variable identificador_variable);
t_puntero ansi_obtenerPosicionVariable(t_nombre_variable identificador_variable);
void ansi_asignar(t_puntero direccion_variable, t_valor_variable valor);
t_valor_variable ansi_desreferenciar (t_puntero direccion_variable);
t_valor_variable ansi_obtener_valor_compartida(t_nombre_compartida identificador);

#endif /* PROCESO_CPU_H_ */
