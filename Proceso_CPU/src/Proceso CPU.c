#include "Proceso CPU.h"

int main(void) {
	puts("Proceso CPU");

	idCpu = 1; //Asigno id a la Cpu. 50 es que no fue asignado su valor

	//Leemos configuracion
	config = load_config(PATH_CONFIG);
	print_config(config);

	//Conexion al kernel
	connect_server_kernel();

	//Conexion a memoria
	connect_server_memoria();

	//------------------------

	// Realizar handshake con Kernel
	str_generica_msjs mensajeAEnviar = inicializar_str_msjs();
	mensajeAEnviar.id_MSJ = HANDSHAKE_CPU_KERNEL;
	mensajeAEnviar.socket_descriptor = kernel;
	mensajeAEnviar.socket_descriptor_server = kernel;
	//enviar serializado el mensaje a Kernel
	//enviarMjeSinConsola(kernel,mensajeAEnviar.id_MSJ,&mensajeAEnviar);

	//Se chequea el mensaje del kernel
	//recibir un dato y deserializarlo
	str_generica_msjs *mensajeRec;	// = recibirMjeSinConsola(kernel);

	if (mensajeRec != NULL) {

		if ((mensajeRec->id_MSJ == HANDSHAKE_CPU_KERNEL)
				&& (mensajeRec->OK == 1)) {

			idCpu = mensajeRec->identificador_cpu; //asigno el id del cpu
			printf("Se realizo Handshake con Kernel exitosamente");

		} else {
			printf("Error al realizar Handshake con Kernel");
			return EXIT_FAILURE;
		}

	}

	free(mensajeRec);

	// Recibir mensaje de Memoria

	str_generica_msjs mjeAEnviarMemoria = inicializar_str_msjs();
	mjeAEnviarMemoria.id_MSJ = HANDSHAKE_CPU_MEMORIA;
	mjeAEnviarMemoria.socket_descriptor = memoria;
	mjeAEnviarMemoria.socket_descriptor_server = memoria;
	mjeAEnviarMemoria.identificador_cpu = idCpu;
	//enviar serializado el mensaje a Kernel
	//enviarMjeSinConsola(memoria,mjeAEnviarMemoria.id_MSJ,&mjeMemoria);

	//Se chequea el mensaje de memoria
	//recibir un dato y deserializarlo

	str_generica_msjs *mensajeRecMemoria; //= recibirMjeSinConsola(memoria);

	if (mensajeRecMemoria != NULL) {

		if ((mensajeRecMemoria->id_MSJ == HANDSHAKE_CPU_MEMORIA)
				&& (mensajeRecMemoria->OK == 1)) {

			printf("Recepcion de handshake de Memoria de tipo %i por socket %i",
					mensajeRecMemoria->id_MSJ, memoria);

	} else {
		printf("Error en handshake UMV: %s", mensajeRecMemoria->mensaje);
		return EXIT_FAILURE;
	}

}
free(mensajeRecMemoria);

while (1) {

	//Quedo a la espera de recibir un PCB

	//deserializar_int(kernel);

	//Recibo el PCB y solicito a Memoria el stack con el codigo

	//Quedo a la espera de que la memoria me devuelva el codigo

	//Con el codigo doy inicio a la funcion ejecutar() para analizar linea por linea

	//Los resultados los devuelvo al Kernel

	//serializar_string(kernel);

}

return 0;

}


void ejecutar() {

t_intructions indice; //tengo que pedirle el indice a memoria

char* sentencia;//tengo que asignarle la sentencia en base a la instruccion.
analizadorLinea(strdup(sentencia), &functions, &kernel_functions);//ejecucion de primitivas

}


void connect_server_kernel() {
	//Conexion al kernel
	kernel = connect_server(config.IP_KERNEL, config.PUERTO_KERNEL);
	if (kernel > 0) {
		printf("Kernel conectado, Estoy escuchando\n");
		serializar_int(kernel,2);
	}
}

void connect_server_memoria() {
//Conexion a memoria
memoria = connect_server(config.IP_MEMORIA, config.PUERTO_MEMORIA);
if (memoria > 0) {
	printf("Memoria Conectada\n");
	serializar_int(memoria, 5);
}
}

