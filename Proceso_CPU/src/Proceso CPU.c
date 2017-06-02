#include <stdio.h>
#include <stdlib.h>
#include "serializador/serializador.h"
#include "config/config_CPU.h"
#include "pcb/pcb.h"
#include "commons/collections/list.h"
#include "Proceso CPU.h"
#include <parser/metadata_program.h>

char* PATH_CONFIG = "../src/config/config.txt";
Type_Config config;

static const char* PROGRAMA = "begin\n"
		"variables a, b\n"
		"a = 3\n"
		"b = 5\n"
		"a = b + 12\n"
		"end\n"
		"\n";



// ------------------------ VARIABLES GLOBALES ---------------------------------

uint32_t idCpu; // Identificador de CPU.
//uint32_t existePCB = 0;	Cuando esto incremente a 1 , significa que ya recibio un PCB correcto
//si queda en 0 significa que no hay todavia. Cuando la CPU se libere del PCB actual porque
//ya realizo todas sus operaciones correspondientes , entonces se vuelve a setear en 0

t_list *listaDePCBs; //Esta lista va a manejar todos los PCBs activos que representarian a las distintas CPUs que se encuentran procesando.

//Variables para sockets

int kernel;
int memoria;

int main(void) {
	puts("Proceso CPU");

	//Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);

	//Conexion al kernel
	kernel = connect_server(config.IP_KERNEL, config.PUERTO_KERNEL);

	//Si conecto, informo
	if (kernel > 0) {
		printf("Kernel conectado, Estoy escuchando\n");
	}

	//Conexion a memoria
	memoria = connect_server(config.IP_MEMORIA, config.PUERTO_MEMORIA);

	//Si conecto, informo
	if (memoria > 0) {
		printf("Memoria Conectada\n");
	}

	listaDePCBs = list_create(); //Instancio la lista de PCBs

	while (1) {

		ejecutarPrograma();

	}

	return EXIT_SUCCESS;
}

bool terminoElPrograma(void) {
	return true;
}

void ejecutarPrograma() {

	recibirPCB(kernel);



	char *programa = strdup(PROGRAMA); //copia el programa entero en esa variable

	t_metadata_program *metadata = metadata_desde_literal(programa); //hacerlo por que si

	int programCounter = 0; //deberia ser el del PCB

	while (!terminoElPrograma()) {

/*		char* const linea = conseguirDatosMemoria(programa,
				metadata->instrucciones_serializado[programCounter].start,
				metadata->instrucciones_serializado[programCounter].offset); //que me devuelva la siguiente linea la memoria

		printf("\t Evaluando -> %s", linea);

		analizadorLinea(linea, &functions, &kernel_functions); //que haga lo que tenga q hacer

		free(linea);*/

		programCounter++;
	}

	metadata_destruir(metadata); //por que si

	/*
	 if (idMje == 1) { //IMPRIMIR TEXTO
	 str_mensaje.id_MSJ = IMPRIMIR_TEXTO;
	 serializar_data(str_mensaje.socket_server,str_mensaje.id_MSJ ,str_mensaje.size_str, str_mensaje.texto);
	 } else if (idMje == 2) { //IMPRIMIR VALOR
	 str_mensaje.id_MSJ = IMPRIMIR_VALOR;
	 str_mensaje.valor = valor;
	 }
	 serializar_data(str_mensaje.socket_server,1,str_mensaje.size_str, str_mensaje.texto);
	 */
}

//Funcion para esperar un PCB del Kernel

void recibirPCB(int socket) {

	t_PCB *pcbRecibido = deserializar_data(socket); //Deserializo la info que trae el socket


	if (pcbRecibido == NULL) {  //Si el socket no tiene info valida tiro error.

		printf("%s", "Error al recibir PCB desde Kernel");

	}

	if (pcbRecibido != NULL) {  //Si el socket viene con info valida lo almaceno


		t_PCB *pcbInterno = malloc(sizeof(t_PCB)); //Pido memoria para inicializar el pcb interno

		//Cargo el PCB interno con los datos que provienen del Deserializador

		pcbInterno->ProgramCounter = 1; //pcbRecibido->ProgramCounter;
		pcbInterno->StackPointer = 1; // pcbRecibido->stackPointer;
		pcbInterno->PageCode = 1; // pcbRecibido->PageCode;
		pcbInterno->TagsPointer = 1; //pcbRecibido->TagsPointer;
		pcbInterno->ExitCode = 0; // pcbRecibido->ExitCode
		pcbInterno->CodePointer = 1; //pcbRecibido->CodePointer;
		list_add(listaDePCBs, pcbInterno); //Agrego el pcb interno a la lista de PCBs activos.
		free(pcbInterno); //libero memoria solicitada
	}

}

void solicitarDatosDeMemoria(uint32_t pid) {


}

void recibirDatosMemoria(){



}

