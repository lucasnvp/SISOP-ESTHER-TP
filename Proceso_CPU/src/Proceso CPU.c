#include "Proceso CPU.h"

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

	//Con el codigo doy inicio a la funcion ejecutar() para analizar linea por linea

	return EXIT_SUCCESS;

}

void ejecutar() {

	//Quedo a la espera de que la memoria me devuelva el codigo

	t_intructions indice; //tengo que pedirle el indice a memoria

	printf("Ejecutando en CPU");

	sleep(5);

	char* sentencia; //tengo que asignarle la sentencia en base a la instruccion.
	analizadorLinea(strdup(sentencia), &functions, &kernel_functions); //ejecucion de primitivas

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

