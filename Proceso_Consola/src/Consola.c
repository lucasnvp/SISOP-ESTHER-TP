#include "Consola.h"

int main (void){
	puts("Proceso Consola");

	//Configuracion inicial
	//config = load_config(PATH_CONFIG);
	//print_config(config);

	// Variables hilos
	pthread_t thread_comandos;

	//Inicializo mutex para impresion por pantalla
	pthread_mutex_init(&sem_consola, NULL);

	connect_server_kernel();

	//Hilo de consola
	pthread_create(&thread_comandos,NULL,(void*) consola_comandos,"Consola Comandos");

	pthread_join(thread_comandos, (void**) NULL);

    return EXIT_SUCCESS;

}

void connect_server_kernel(){
	//Me conecto al servidor
	//SERVIDOR_KERNEL = connect_server(config.IP_KERNEL,config.PUERTO_KERNEL);
	SERVIDOR_KERNEL = connect_server("127.0.0.1",5010);

	//Si conecto, informo
	if(SERVIDOR_KERNEL > 0){
		printf("Ready to send \n> ");
	}
}

void consola_comandos(){
	while(true) {
		t_Consola* consola = (t_Consola *) malloc(sizeof(t_Consola));
		consola = leerComandos();
		consola->kernel = SERVIDOR_KERNEL;
		if (!strcmp(consola->comando, "run")) {
			if (consola->argumento == NULL)
				printf("Falta el argumento de la funcion %s\n\n> ", consola->comando);
			else {
				//Envio el mensaje
				pthread_t* hiloConsola = (pthread_t *) malloc(sizeof(pthread_t));
				pthread_create(hiloConsola, NULL, (void*) crearHiloConsola, (void*) consola);
			}
		}
		else if (!strcmp(consola->comando, "close"))
			printf("> ");
		else if (!strcmp(consola->comando, "exit"))
			exit(0);
		else if (!strcmp(consola->comando, "clean")) {
			system("clear");
			printf("> ");
			fflush(stdout);
		}
		else
			printf("Comando incorrecto. Pruebe run | stop | exit | clean\n\n> ");
	}
}
