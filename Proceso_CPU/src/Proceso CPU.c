#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Proceso CPU.h"
#include "config/config_CPU.h"

char* PATH_CONFIG = "../src/config/config.txt";
Type_Config config;

uint32_t kernel;
uint32_t memoria;
uint32_t idCpu;


int main(void) {

	puts("Proceso CPU");

	idCpu = 1; //Asigno id a la Cpu. 50 es que no fue asignado su valor

	//Leemos configuracion
	config = load_config(PATH_CONFIG);
	print_config(config);

	//Conexion al kernel
	kernel = connect_server(config.IP_KERNEL, config.PUERTO_KERNEL);
	if (kernel > 0) {
		printf("Kernel conectado, Estoy escuchando\n");
		serializar_int(kernel,2);
	}

	//Conexion a memoria
	memoria = connect_server(config.IP_MEMORIA, config.PUERTO_MEMORIA);
	if (memoria > 0) {
		printf("Memoria Conectada\n");
	}

	serializar_int(memoria,5);


	//Mutex que controlan estado de conexion con Kernel y Memoria.
	pthread_mutex_t mutex_kernel = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t mutex_memoria = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&mutex_kernel);
	pthread_mutex_lock(&mutex_memoria);

	// Realizar handshake con Kernel
	t_KER_PRO_CPU_UMV mensajeAEnviar = obtener_nueva_shared_str();
	mensajeAEnviar.gen_msg.id_MSJ = HANDSHAKE_CPU_KERNEL;
	mensajeAEnviar.gen_msg.socket_descriptor = kernel;
	mensajeAEnviar.gen_msg.socket_descriptor_server = kernel;
	//enviar serializado el mensaje a Kernel
	//enviarMjeSinConsola(kernel,mensajeAEnviar.gen_msg.id_MSJ,&mensajeAEnviar);

	//Se chequea el mensaje del kernel
	//recibir un dato y deserializarlo
	t_KER_PRO_CPU_UMV *mensajeRec;	// = recibirMjeSinConsola(kernel);

	if (mensajeRec != NULL) {

		if ((mensajeRec->gen_msg.id_MSJ == HANDSHAKE_CPU_KERNEL)
				&& (mensajeRec->OK == 1)) {

			idCpu = mensajeRec->identificador_cpu; //asigno el id del cpu
			pthread_mutex_unlock(&mutex_kernel); //si recibi los datos bien desbloqueo el mutex.
			printf("Se realizo Handshake con Kernel exitosamente");

		} else {
			printf("Error al realizar Handshake con Kernel");
			return EXIT_FAILURE;
		}

	}

	free(mensajeRec);

	// Realizar handshake con Memoria
	t_KER_PRO_CPU_UMV mjeAEnviarMemoria = obtener_nueva_shared_str();
	mjeAEnviarMemoria.gen_msg.id_MSJ = HANDSHAKE_CPU_MEMORIA;
	mjeAEnviarMemoria.gen_msg.socket_descriptor = memoria;
	mjeAEnviarMemoria.gen_msg.socket_descriptor_server = memoria;
	mjeAEnviarMemoria.identificador_cpu = idCpu;
	//enviar serializado el mensaje a Kernel
	//enviarMjeSinConsola(memoria,mjeAEnviarMemoria.gen_msg.id_MSJ,&mjeMemoria);


	//Se chequea el mensaje de memoria
	//recibir un dato y deserializarlo

	t_KER_PRO_CPU_UMV *mensajeRecMemoria; //= recibirMjeSinConsola(memoria);

	if (mensajeRecMemoria != NULL) {

		if ((mensajeRecMemoria->gen_msg.id_MSJ == HANDSHAKE_CPU_MEMORIA)
				&& (mensajeRecMemoria->OK == 1)) {

			printf("Recepcion de handshake de Memoria de tipo %i por socket %i",
					mensajeRecMemoria->gen_msg.id_MSJ, memoria);
			pthread_mutex_unlock(&mutex_memoria); //si recibi los datos bien desbloqueo el mutex.
		} else {
			printf("Error en handshake UMV: %s", mensajeRecMemoria->mensaje);
			return EXIT_FAILURE;
		}

	}
	free(mensajeRecMemoria);

	return 0;

}

