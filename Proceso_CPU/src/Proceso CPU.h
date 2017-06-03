#ifndef PROCESO_CPU_H_
#define PROCESO_CPU_H_

#include "servidor/servidor.h"
#include <stdint.h>
#include "primitivas/ansi_primit.h"
#include <parser/parser.h>

AnSISOP_funciones functions = {
		.AnSISOP_definirVariable		= definirVariable,
//		.AnSISOP_imprimirTexto			= imprimirTexto,
		.AnSISOP_obtenerPosicionVariable= obtenerPosicionVariable,
		.AnSISOP_dereferenciar			= dereferenciar,
		.AnSISOP_asignar				= asignar,
		//.AnSISOP_imprimir				= imprimir,
		.AnSISOP_irAlLabel				= irAlLabel,
		.AnSISOP_asignarValorCompartida = asignarValorCompartida,
//		.AnSISOP_entradaSalida 			= entradaSalida,
//		.AnSISOP_obtenerValorCompartida	= obtenerValorCompartida,
		.AnSISOP_llamarSinRetorno		= llamarSinRetorno,
		.AnSISOP_llamarConRetorno		= llamarConRetorno,
		.AnSISOP_finalizar				= finalizar,
		.AnSISOP_retornar				= retornar,
};

AnSISOP_kernel kernel_functions = {
		.AnSISOP_signal 				= semaforo_signal,
		.AnSISOP_wait					= semaforo_wait,
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

typedef struct strGenMsg { // estructura generica, debe estar en todas las estructuras
	uint32_t id_MSJ;
	uint32_t disconnected;
	uint32_t socket_descriptor;
	uint32_t socket_descriptor_server;
	uint32_t size_str;
} __attribute__ ((__packed__)) t_GEN_MSG;


typedef struct strKernelProgramaCPUUMV {
	t_GEN_MSG gen_msg;

	//HANDSHAKE_PROGRAMA_PLP y HANDSHAKE_CPU_PCP
	uint32_t identificador_cpu; //(identificador propio de CPU)
	//HANDSHAKE_PROGRAMA_PLP y ENVIO_COD_INS_ETI
	uint32_t codigo_long;
	char* codigo; //Ansisop
	//IMPRIMIR - IMPRIMIR_TEXTO
	uint32_t texto_long;
	char* texto;
	//CREAR_SEGMENTOS
	uint32_t tamanio_segmento;
	uint32_t base_segmento;

	//EJECUTAR_PROGRAMA
	uint32_t quantum;
	uint32_t retardo;
	//OBTENER_VALOR y GRABAR_VALOR
	uint32_t id_var_comp_long;
	char* id_var_comp;
	int valor;
	//WAIT y SIGNAL
	uint32_t id_sem_long;
	char* id_sem;
	//ENTRADA_SALIDA
	uint32_t id_dispositivo_long;
	char* id_dispositivo;
	uint32_t utilizacion;
	//SOLICITAR_BYTES y ENVIAR_BYTES
	uint32_t  base_bytes;
	uint32_t  offset_bytes;
	uint32_t  tamanio_bytes;
	//SOLICITAR_BYTES, ENVIAR_BYTES y SOLICTUD_INSTRUCCION
	uint32_t buffer_long;
	char* buffer;
	//EJECUTAR_PROGRAMA
	t_PCB PCB;
	//HANDSHAKE_PROGRAMA_PLP, HANDSHAKE_CPU_PCP, HANDSHAKE_CPU_UMV, CAMBIO_PROCESO_ACTIVO, FINALIZAR_EJECUCION, DESTRUIR_SEGMENTOS, NOTIFICACION_QUANTUM
	uint32_t PID;
	//ENTRADA_INDICE
	uint32_t  indice;
	// Proposito Gral
	uint32_t OK;
	uint32_t mensaje_long;
	char* mensaje;

}__attribute__ ((__packed__)) t_KER_PRO_CPU_UMV;



t_KER_PRO_CPU_UMV obtener_nueva_shared_str() {
	t_KER_PRO_CPU_UMV nueva;

	nueva.buffer = "\0";
	nueva.codigo = "\0";
	nueva.id_dispositivo = "\0";
	nueva.id_sem = "\0";
	nueva.id_var_comp = "\0";
	nueva.mensaje = "\0";
	nueva.texto = "\0";
	nueva.PID = 0;
	nueva.OK = 0;
	nueva.PCB.PID = 0;
	nueva.PCB.ProgramCounter = 0;
	nueva.PCB.CodePointer = 0;
	nueva.PCB.StackPointer = 0;
	nueva.PCB.TagsPointer = 0;
	nueva.PCB.PageCode = 0;
	nueva.PCB.ExitCode = 0;
	nueva.base_bytes = 0;
	nueva.base_segmento = 0;
	nueva.gen_msg.disconnected = 0;
	nueva.gen_msg.id_MSJ = 0;
	nueva.gen_msg.socket_descriptor = 0;
	nueva.gen_msg.socket_descriptor_server = 0;
	nueva.identificador_cpu = 0;
	nueva.indice = 0;
	nueva.offset_bytes = 0;
	nueva.quantum = 0;
	nueva.retardo = 0;
	nueva.tamanio_bytes = 0;
	nueva.tamanio_segmento = 0;
	nueva.utilizacion = 0;
	nueva.valor = 0;
	return nueva;
}



#endif /* PROCESO_CPU_H_ */
