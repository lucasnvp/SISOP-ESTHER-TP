#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "servidor/servidor.h"
#include "config/config_consola.h"
#include "utiles/ComandosPorConsola.h"

char* PATH_CONFIG = "../src/config/config.txt";
Type_Config config;

int main (void){

	puts("Proceso Consola");

	//Inicializo mutex para impresion por pantalla
	pthread_mutex_init(&sem_consola, NULL);

	//Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);

	//Me conecto al servidor
	int kernel = connect_server(config.IP_KERNEL,config.PUERTO_KERNEL);

	//Si conecto, informo
	if(kernel > 0){
		printf("Ready to send \n> ");
	}

	int salir = 0;

	//Bucle para el ingreso de datos
	while (!salir) {
		t_Consola consola = leerComandos();
		consola.kernel = kernel;
		if (!strcmp(consola.comando, "run") /*|| !strcmp(consola.comando, "stop")*/) {
			if (consola.argumento == NULL)
				printf("Falta el argumento de la funcion %s\n\n> ", consola.comando);
			else {
				//Envio el mensaje
				pthread_t* hiloConsola = (pthread_t *) malloc(sizeof(pthread_t));
				pthread_create(hiloConsola, NULL, (void*) crearHiloConsola, (void*) &consola);
				free(hiloConsola);
			}
		}
		else if (!strcmp(consola.comando, "close"))
			printf("> ");
		else if (!strcmp(consola.comando, "exit"))
			salir = 1;
		else if (!strcmp(consola.comando, "clean")) {
			system("clear");
			printf("> ");
			fflush(stdout);
		}
		else
			printf("Comando incorrecto. Pruebe run | stop | exit | clean\n\n> ");
	}

    return EXIT_SUCCESS;

}
