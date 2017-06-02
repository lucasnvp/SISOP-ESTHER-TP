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


static const char* PROGRAMA =
						"begin\n"
						"variables a, b\n"
						"a = 3\n"
						"b = 5\n"
						"a = b + 12\n"
						"end\n"
						"\n";

//TODAS LAS PRIMITIVAS TIENEN QUE ESTAR ACA
/*AnSISOP_funciones functions = {
	.AnSISOP_definirVariable	= dummy_definirVariable,
	.AnSISOP_obtenerPosicionVariable= dummy_obtenerPosicionVariable,
	.AnSISOP_finalizar = dummy_finalizar,
	.AnSISOP_dereferenciar	= dummy_dereferenciar,
	.AnSISOP_asignar	= dummy_asignar,

	 .AnSISOP_obtenerValorCompartida
	 .AnSISOP_asignarValorCompartida
	 .AnSISOP_irAlLabel
	 .AnSISOP_llamarSinRetorno
	 .AnSISOP_llamarConRetorno
	 .AnSISOP_retornar

};*/

AnSISOP_kernel kernel_functions = {/*
		.AnSISOP_wait
		.AnSISOP_signal
		.AnSISOP_reservar
		.AnSISOP_liberar
		.AnSISOP_abrir
		.AnSISOP_borrar
		.AnSISOP_cerrar
		.AnSISOP_moverCursor
		.AnSISOP_escribir
		.AnSISOP_leer
		*/

};//NO SE PARA QUE ES ESTO


// ------------------------ VARIABLES GLOBALES ---------------------------------

uint32_t idCpu; // Identificador de CPU.
uint32_t existePCB = 0;		//Cuando esto incremente a 1 , significa que ya recibio un PCB correcto
							//si queda en 0 significa que no hay todavia. Cuando la CPU se libere del PCB actual porque
							//ya realizo todas sus operaciones correspondientes , entonces se vuelve a setear en 0

t_list *listaDePCBs;  //Esta lista va a manejar todos los PCBs activos que representarian a las distintas CPUs que se encuentran procesando.

//Variables para sockets

int kernel;
int memoria;


int main(void) {
	puts("Proceso CPU");

	idCpu = 1;

	//Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);

	//Conexion al servidor
	int kernel = connect_server(config.IP_KERNEL, config.PUERTO_KERNEL);

	//Si conecto, informo
	if (kernel > 0) {
		printf("Estoy escuchando\n");
	}

	listaDePCBs = list_create(); //Instancio la lista de PCBs

	while (existePCB == 0) {

		ejecutarPrograma();

	}

	return EXIT_SUCCESS;
}

bool terminoElPrograma(void){
	return termino;
}


void ejecutarPrograma(){

	recibirPCB(kernel);

	char *programa = strdup(PROGRAMA);//copia el programa entero en esa variable

	t_metadata_program *metadata = metadata_desde_literal(programa);//hacerlo por que si

	int programCounter = 0;//deberia ser el del PCB

	while(!terminoElPrograma()){

//		char* const linea = conseguirDatosDeLaMemoria(
//				programa,
//				//	metadata->instrucciones_serializado[programCounter].start,
//				metadata->instrucciones_serializado[programCounter].offset);//que me devuelva la siguiente linea la memoria
//				printf("\t Evaluando -> %s", linea);
//			//	analizadorLinea(linea, &functions, &kernel_functions);//que haga lo que tenga q hacer
//		free(linea);
//		programCounter++;
	}

	metadata_destruir(metadata);//por que si

//conseguirDatosMemoria(pcb,0,8);
//connectionHandler(socketKernel);

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

void recibirPCB(int socket){

	//t_PCB *pcbRecibido = deserializar_data(socket); //Deserializo la info que trae el socket
	t_PCB *pcbRecibido;

	if(pcbRecibido == NULL){  //Si el socket no tiene info valida tiro error.

		printf("%s","Error al recibir PCB desde Kernel");

	}

	if(pcbRecibido != NULL){//Si el socket viene con info valida lo almaceno

		existePCB++; //Incremento el contador de existencia de PCBs

			t_PCB *pcbInterno = malloc(sizeof(t_PCB)); //Pido memoria para inicializar el pcb interno

			//Cargo el PCB interno con los datos que provienen del Deserializador

			pcbInterno->ProgramCounter = 1; //pcbRecibido->ProgramCounter;
			pcbInterno->StackPointer = 1; // pcbRecibido->stackPointer;
			pcbInterno->PageCode = 1; // pcbRecibido->PageCode;
			pcbInterno->TagsPointer = 1; //pcbRecibido->TagsPointer;
			pcbInterno->ExitCode = 0; // pcbRecibido->ExitCode
			pcbInterno->CodePointer = 1; //pcbRecibido->CodePointer;
			list_add(listaDePCBs,pcbInterno); //Agrego el pcb interno a la lista de PCBs activos.
			free(pcbInterno); //libero memoria solicitada


	}

}








