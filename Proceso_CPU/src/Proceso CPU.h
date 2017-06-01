#ifndef PROCESO_CPU_H_
#define PROCESO_CPU_H_

#include "servidor/servidor.h"
#include <stdint.h>

//TIPOS DE MENSAJES
#define IMPRIMIR_TEXTO 1;
#define IMPRIMIR_VALOR 2;

bool termino = false;

typedef struct pcb { // Estructura que representa el PCB
	uint32_t PID; //Número identificador del proceso único en el sistema.
	uint32_t ProgramCounter; //Número de la próxima instrucción del Programa que se	debe ejecutar.
	uint32_t PageCode; //Cantidad de páginas utilizadas por el código del Programa AnSISOP, empezando por la página cero.
	uint32_t CodePointer; //Estructura auxiliar que contiene el offset del inicio y del	fin de cada sentencia del Programa.
	uint32_t TagsPointer; //Estructura auxiliar utilizada para conocer las líneas de código correspondientes al inicio de los procedimientos y a las etiquetas.
	uint32_t StackPointer; //Estructura auxiliar encargada de ordenar los valores almacenados en el Stack.
	uint32_t ExitCode; //Número entero que identifica el motivo de finalización	del proceso.

} t_PCB;

bool terminoElPrograma(void);
void ejecutarPrograma();
void recibirPCB(int socket);

#endif /* PROCESO_CPU_H_ */
