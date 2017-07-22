#ifndef PROCESO_CPU_H_
#define PROCESO_CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <parser/parser.h>
#include <parser/metadata_program.h>
#include "servidor/servidor.h"
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

//Variable que indica si el codigo finalizo
bool termino_codigo = false;

//Cantidad de Instrucciones que le quedan al proceso para finalizar.
uint32_t cant_instrucciones;

//Variable que indica que el proceso se encuentra bloqueado
bool procesoBloqueado;

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
#define PEDIDO_VAR_COMPARTIDA 9
#define ASIGNAR_VAR_COMPARTIDA 10
#define KERNEL_WAIT 7
#define KERNEL_SIGNAL 8
#define FS_ABRIR 11
#define FS_BORRAR 12
#define FS_CERRAR 13
#define FS_MOVER_CURSOR 14
#define FS_LEER 15
#define FS_ESCRIBIR 16

//Funcion que ejecuta el codigo
void ejecutar();

//Funciones internas
bool codigoFinalizado();

//Conexiones
void connect_server_kernel();
void connect_server_memoria();

//Pedidos a memoria
char* const solicitarInstruccionAMemoria();
void enviarDatoAMemoria(uint32_t pagina, uint32_t offset, uint32_t size);

//Logs
void init_log(char* pathLog);

//Primitivas
t_puntero ansi_definirVariable(t_nombre_variable identificador_variable);
t_puntero ansi_obtenerPosicionVariable(t_nombre_variable identificador_variable);
void ansi_asignar(t_puntero direccion_variable, t_valor_variable valor);
t_valor_variable ansi_desreferenciar(t_puntero direccion_variable);
t_valor_variable ansi_obtener_valor_compartida(
		t_nombre_compartida identificador);
t_valor_variable ansi_asignar_valor_compartida(
		t_nombre_compartida identificador, t_valor_variable valor);
void ansi_irAlLabel(t_nombre_etiqueta t_nombre_etiqueta);
void ansi_llamarSinRetorno(t_nombre_etiqueta etiqueta);
void ansi_llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero punto_retorno);
void ansi_finalizar(void);
void ansi_retornar(t_valor_variable retorno);
void kernel_wait();
t_descriptor_archivo abrir_archivo(t_direccion_archivo direccion,
		t_banderas flags);
void borrar_archivo(t_descriptor_archivo descriptor_archivo);
char* concatenarPermisos(t_banderas flags);
void moverCursor_archivo(t_descriptor_archivo descriptor_archivo,
		t_valor_variable posicion);
void leer_archivo(t_descriptor_archivo descriptor_archivo,
		t_puntero informacion, t_valor_variable tamanio);
void escribir(t_descriptor_archivo descriptor_archivo, void* informacion,
		t_valor_variable tamanio);

#endif /* PROCESO_CPU_H_ */
