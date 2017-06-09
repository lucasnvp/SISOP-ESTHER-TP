#ifndef PROCESO_CPU_H_
#define PROCESO_CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "servidor/servidor.h"
#include "primitivas/ansi_primit.h"
#include <parser/parser.h>
#include <parser/metadata_program.h>
#include "config/config_CPU.h"

AnSISOP_funciones functions = {
		.AnSISOP_definirVariable = definirVariable,
//		.AnSISOP_imprimirTexto			= imprimirTexto,
		.AnSISOP_obtenerPosicionVariable = obtenerPosicionVariable,
		.AnSISOP_dereferenciar = dereferenciar,
		.AnSISOP_asignar = asignar,
		//.AnSISOP_imprimir	= imprimir,

};

AnSISOP_kernel kernel_functions = {
		.AnSISOP_signal = semaforo_signal,
		.AnSISOP_wait = semaforo_wait,
};

//TIPOS DE MENSAJES
#define IMPRIMIR_TEXTO 1;
#define IMPRIMIR_VALOR 2;

bool termino = false;

uint32_t cpuOcupado;
uint32_t idCPU;

// USAR ESTOS MENSAJES
#define HANDSHAKE_PROGRAMA_PLP 1
#define HANDSHAKE_CPU_KERNEL 2
#define HANDSHAKE_CPU_MEMORIA 3

typedef struct pcb { // Estructura que representa el PCB
	uint32_t PID; //Número identificador del proceso único en el sistema.
	uint32_t ProgramCounter; //Número de la próxima instrucción del Programa que se	debe ejecutar.
	uint32_t PageCode; //Cantidad de páginas utilizadas por el código del Programa AnSISOP, empezando por la página cero.
	uint32_t CodePointer; //Estructura auxiliar que contiene el offset del inicio y del	fin de cada sentencia del Programa.
	uint32_t TagsPointer; //Estructura auxiliar utilizada para conocer las líneas de código correspondientes al inicio de los procedimientos y a las etiquetas.
	uint32_t StackPointer; //Estructura auxiliar encargada de ordenar los valores almacenados en el Stack.
	uint32_t ExitCode; //Número entero que identifica el motivo de finalización	del proceso.

} t_PCB;

typedef struct strKernelProgramaCPUUMV {

	//Datos para conexion
	uint32_t id_MSJ;
	uint32_t disconnected;
	uint32_t socket_descriptor;
	uint32_t socket_descriptor_server;
	uint32_t size_str;
	uint32_t identificador_cpu; //(identificador propio de CPU)
	uint32_t codigo_long; //Para uso ansisop
	char* codigo; //Para uso ansisop
	t_PCB PCB; //PCB
	uint32_t PID; //Identificador del proceso
	uint32_t OK;	// Para paso de Mensajes
	uint32_t mensaje_long;	// Para paso de Mensajes
	char* mensaje;	// Para paso de Mensajes

}__attribute__ ((__packed__)) str_generica_msjs;

str_generica_msjs inicializar_str_msjs() {
	str_generica_msjs nueva;

	nueva.codigo = "\0";
	nueva.mensaje = "\0";
	nueva.PID = 0;
	nueva.OK = 0;
	nueva.PCB.PID = 0;
	nueva.PCB.ProgramCounter = 0;
	nueva.PCB.CodePointer = 0;
	nueva.PCB.StackPointer = 0;
	nueva.PCB.TagsPointer = 0;
	nueva.PCB.PageCode = 0;
	nueva.PCB.ExitCode = 0;
	nueva.disconnected = 0;
	nueva.id_MSJ = 0;
	nueva.socket_descriptor = 0;
	nueva.socket_descriptor_server = 0;
	nueva.identificador_cpu = 0;

	return nueva;
}

#endif /* PROCESO_CPU_H_ */
