#ifndef PROCESO_CPU_H_
#define PROCESO_CPU_H_

#include "servidor/servidor.h"
#include <stdint.h>

//TIPOS DE MENSAJES
#define IMPRIMIR_TEXTO 1;
#define IMPRIMIR_VALOR 2;

typedef struct pcb { // Estructura que representa el PCB
	uint32_t PID; //Número identificador del proceso único en el sistema.
	uint32_t PC; //Número de la próxima instrucción del Programa que se	debe ejecutar.
	uint32_t PageCode; //Cantidad de páginas utilizadas por el código del Programa AnSISOP, empezando por la página cero.
	uint32_t CodePointer; //Estructura auxiliar que contiene el offset del inicio y del	fin de cada sentencia del Programa.
	uint32_t TagsPointer; //Estructura auxiliar utilizada para conocer las líneas de código correspondientes al inicio de los procedimientos y a las etiquetas.
	uint32_t StackPointer; //Estructura auxiliar encargada de ordenar los valores almacenados en el Stack.
	uint32_t ExitCode; //Número entero que identifica el motivo de finalización	del proceso.

} t_PCB;

typedef struct srtIntercambio {

	//DATOS DEL SOCKET
	uint32_t disconnected;
	uint32_t socket_server;
	uint32_t size_str;

	//TIPO DE MENSAJE
	uint32_t id_MSJ;

	//HANDSHAKE_PROGRAMA_PLP y HANDSHAKE_CPU_PCP
	uint32_t identificador_cpu; //(identificador propio de CPU)

	//HANDSHAKE_PROGRAMA_PLP y ENVIO_COD_INS_ETI
	uint32_t codigo_long;
	char* codigo; //Ansisop

	//IMPRIMIR - IMPRIMIR_TEXTO
	uint32_t texto_long;
	char* texto;

	//OBTENER_VALOR y GRABAR_VALOR
	uint32_t id_var_comp_long;
	char* id_var_comp;
	int valor;

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

} t_STR_INTERCAMBIO;



t_STR_INTERCAMBIO incializarIntercambio();

#endif /* PROCESO_CPU_H_ */
