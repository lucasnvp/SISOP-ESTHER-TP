#include "Proceso CPU.h"

AnSISOP_funciones functions = {  //TODAS LAS PRIMITIVAS TIENEN QUE ESTAR ACA
		.AnSISOP_finalizar = finalizarCodigo,

		};

AnSISOP_kernel kernel_functions = {

};  //Syscalls

static const char* PROGRAMA = "begin\n"
		"variables a, b\n"
		"a = 3\n"
		"b = 5\n"
		"a = b + 12\n"
		"end\n";

int main(void) {
	puts("Proceso CPU");

	//Leemos configuracion
	config = load_config(PATH_CONFIG);
	print_config(config);

	//Conexion al kernel
	connect_server_kernel();

	//Conexion a memoria
	connect_server_memoria();

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

	char *programa = strdup(PROGRAMA); //copia el programa entero en esa variable, lo hace el Kernel

	t_metadata_program *metadata = metadata_desde_literal(programa); //lo hace el Kernel

	int programCounter = 0;	//deberia ser el del PCB

	while (!programaFinalizado()) {

		//instancio para utilizar
		t_puntero_instruccion start =
				metadata->instrucciones_serializado[programCounter].start;
		//instancio para utilizar
		t_size offset =
				metadata->instrucciones_serializado[programCounter].offset;	//que me devuelva la siguiente linea la memoria

		//solicito a memoria la instruccion.
		char* const instruccion = solicitarInstruccionAMemoria(1, start,
				offset); //falta la pagina en donde esta la instruccion.

		analizadorLinea(instruccion, functions, kernel_functions); //ejecuta las primitivas

		free(instruccion);

		programCounter++;

	}

	metadata_destruir(metadata); //esto lo hace el kernel tambien.

	sleep(5);

}

char* const solicitarInstruccionAMemoria(uint32_t pid,
		t_puntero_instruccion offset, t_size size) {

	//Serializo la estructura para enviar a Memoria

	//Quedo a la espera de deserializar la instruccion de Memoria.
	char* const ret;

	return ret; //para que no quede le warning ja

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
	}
}

void finalizarCodigo(void) {
	termino = true;
	printf("Finalizar\n");
}
bool programaFinalizado(void) {
	return termino;
}

