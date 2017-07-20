#include "Consola.h"

int main(void) {
	puts("Proceso Consola");

	//Configuracion inicial
	//config = load_config(PATH_CONFIG);
	//print_config(config);

	//Inicializar Log
	init_log(PATH_LOG);

	// Variables hilos
	pthread_t thread_comandos;

	//Inicializo mutex para impresion por pantalla
	pthread_mutex_init(&sem_consola, NULL);

	connect_server_kernel();

	//Hilo de consola
	pthread_create(&thread_comandos, NULL, (void*) consola_comandos, "Consola Comandos");

	pthread_join(thread_comandos, (void**) NULL);

	return EXIT_SUCCESS;

}

void init_log(char* pathLog) {
	mkdir("/home/utnso/Blacklist/Logs", 0755);
	log_Consola = log_create(pathLog, "Consola", false, LOG_LEVEL_INFO);
}

void connect_server_kernel() {
	//Me conecto al servidor
	//SERVIDOR_KERNEL = connect_server(config.IP_KERNEL, config.PUERTO_KERNEL);
	SERVIDOR_KERNEL = connect_server("127.0.0.1", 5010);

	//Si conecto, informo
	if (SERVIDOR_KERNEL > 0) {
		printf("Ready to send \n> ");
		log_info(log_Consola, "Ready to send.");
	}
}

void consola_comandos() {
	while (true) {
		t_Consola* consola = (t_Consola *) malloc(sizeof(t_Consola));
		consola = leerComandos();
		consola->kernel = SERVIDOR_KERNEL;
		if (!strcmp(consola->comando, "run")) {
			if (consola->argumento == NULL) {
				printf("Falta el argumento de la funcion %s\n\n> ", consola->comando);
				log_warning(log_Consola, "Falta el argumento de la funcion %s.", consola->comando);
			}
			else {
				//Envio el mensaje
				pthread_t* hiloConsola = (pthread_t *) malloc(sizeof(pthread_t));
				pthread_create(hiloConsola, NULL, (void*) crearHiloConsola, (void*) consola);
			}
		}
		else if (!strcmp(consola->comando, "stop")) {
			if (consola->argumento == NULL) {
				printf("Falta el argumento de la funcion %s\n\n> ", consola->comando);
				log_warning(log_Consola, "Falta el argumento de la funcion %s.", consola->comando);
			}
			else {
				uint32_t Arg_PID = atoi(consola->argumento);
				if (Arg_PID > 0) {
					//Ejecuto el comando stop en el servidor
					serializar_int(consola->kernel, COMANDO_STOP);
					serializar_int(consola->kernel, Arg_PID);
					uint32_t PID_Rta_Kernel = deserializar_int(consola->kernel);
					if (PID_Rta_Kernel == 0) {
						printf("No existe proceso con PID %i en el sistema\n\n> ", Arg_PID);
						log_error(log_Consola, "No existe proceso con PID %i en el sistema.", Arg_PID);
					}
				}
				else {
					printf("Error de argumento o tipo desconocido en %s\n\n> ", consola->comando);
					log_error(log_Consola, "Error de argumento o tipo desconocido en %s.", consola->comando);
				}
			}
		}
		else if (!strcmp(consola->comando, "exit")) {
			serializar_int(consola->kernel, COMANDO_EXIT);
			printf("Saliendo...\n\n");
			uint32_t Rta_Kernel = deserializar_int(consola->kernel);
			log_info(log_Consola, "Ejecutado comando EXIT, salida del sistema.");
			exit(Rta_Kernel);
		}
		else if (!strcmp(consola->comando, "clean")) {
			system("clear");
			printf("> ");
			fflush(stdout);
		} else
			printf("Comando incorrecto. Pruebe run | stop | exit | clean\n\n> ");
	}
}
