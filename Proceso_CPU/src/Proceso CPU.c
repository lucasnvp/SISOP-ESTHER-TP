#include "Proceso CPU.h"

AnSISOP_funciones functions = {  //TODAS LAS PRIMITIVAS TIENEN QUE ESTAR ACA
		.AnSISOP_definirVariable = definirVariable,
				.AnSISOP_obtenerPosicionVariable = obtenerPosicionVariable,
				.AnSISOP_dereferenciar = desreferenciar, .AnSISOP_asignar =
						asignar, .AnSISOP_finalizar = finalizarProceso };

AnSISOP_kernel kernel_functions = {

.AnSISOP_wait = kernel_wait };  //Syscalls

static const char* PROGRAMA = "begin\n"
		"variables a, b\n"
		"a = 3\n"
		"b = 5\n"
		"a = b + 12\n"
		"end\n";

int main(void) {

	//Leemos configuracion
	config = load_config(PATH_CONFIG);
	print_config(config);

	//Inicializar Log
	init_log(PATH_LOG);
	log_info(log_Console, "Nueva CPU");

	//Conexion al kernel
	connect_server_kernel();

	//Conexion a memoria
	//connect_server_memoria();

	//Reservar memoria para el PCB
	pcbActivo = malloc(sizeof(PCB_t));

	while (true) {

		//Quedo a la espera de recibir un PCB del Kernel
		deserializar_pcb(kernel, pcbActivo);

		//Proceso de ejecucion de Primitivas Ansisop
		ejecutar();

		//Envio el mensaje al kernel de que finalizo la rafaga correctamente
		serializar_int(kernel, FIN_CORRECTO);

		//Envio a kernel PCB actualizado
		serializar_pcb(kernel, pcbActivo);

	}

	return EXIT_SUCCESS;

}

void ejecutar() {

	printf("Ejecutando en CPU");

	printf("Tamanio de pagina %d", tamanio_pagina);

	char *programa = strdup(PROGRAMA); //copia el programa entero en esa variable, lo hace el Kernel

	t_metadata_program *metadata = metadata_desde_literal(programa); //lo hace el Kernel

	int programCounter = 0;	//deberia ser el del PCB

	while (!codigoFinalizado()) {

		//instancio para utilizar
		t_puntero_instruccion start =
				metadata->instrucciones_serializado[programCounter].start;
		//instancio para utilizar
		t_size offset =
				metadata->instrucciones_serializado[programCounter].offset;	//que me devuelva la siguiente linea la memoria

		//solicito a memoria la instruccion.
		char* const instruccion = solicitarInstruccionAMemoria(programa, start,
				offset); //falta la pagina en donde esta la instruccion.

		analizadorLinea(instruccion, &functions, &kernel_functions); //ejecuta las primitivas

		free(instruccion);

		programCounter++;

	}

	metadata_destruir(metadata); //esto lo hace el kernel tambien.

}

char* const solicitarInstruccionAMemoria(char* prgrama,
		t_puntero_instruccion offset, t_size size) {

	//Serializo la estructura para enviar a Memoria

	//Quedo a la espera de deserializar la instruccion de Memoria.
	char *aRetornar = calloc(1, 100);
	memcpy(aRetornar, prgrama + offset, size);
	return aRetornar;

}

void connect_server_kernel() {
//Conexion al kernel
	kernel = connect_server(config.IP_KERNEL, config.PUERTO_KERNEL);
	if (kernel > 0) {
		printf("Kernel conectado, Estoy escuchando\n");
		serializar_int(kernel, HANDSHAKE_CPU_KERNEL);

	}
}

void connect_server_memoria() {
//Conexion a memoria
	memoria = connect_server(config.IP_MEMORIA, config.PUERTO_MEMORIA);
	if (memoria > 0) {
		printf("Memoria Conectada\n");
		serializar_int(memoria, HANDSHAKE_CPU_MEMORIA);
		tamanio_pagina = deserializar_int(memoria);

	}
}

void connection_handler(uint32_t socket, uint32_t command) {

	switch (command) {
	case 1: {
		break;
	}
	default: {
		printf("Error de comando\n");
		break;
	}
	}

	return;
}

void init_log(char* pathLog) {
	mkdir("/home/utnso/Blacklist/Logs", 0755);
	log_Console = log_create(pathLog, "CPU", true, LOG_LEVEL_INFO);
	log_CPU = log_create(pathLog, "CPU", false, LOG_LEVEL_INFO);
}

