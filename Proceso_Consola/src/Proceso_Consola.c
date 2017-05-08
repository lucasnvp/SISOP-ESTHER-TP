#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "servidor/servidor.h"
#include "config/config_consola.h"
#include "utiles/ComandosPorConsola.h"
#include "serializador/serializador.h"

char* PATH_CONFIG = "../src/config/config.cfg";

int main (void){

	puts("Proceso Consola");

	//Cargo archivo de configuracion y muestro
	abrir_config(PATH_CONFIG);
	mostrarConfig();

	//Me conecto al servidor
	int kernel = connect_server(ipKernel(),puertoKernel());

	//Si conecto, informo
	if(kernel > 0){
		printf("Ready to send \n");
	}

	int salir = 0;

	//Bucle para el ingreso de datos
	while (!salir) {
		t_Consola consola = leerComandos();
		if (!strcmp(consola.comando, "run") || !strcmp(consola.comando, "stop")) {
			if (consola.argumento == NULL)
				printf("Falta el argumento de la funcion %s\n", consola.comando);
			else //Envio el mensaje
				//Serializo el path
				serializar_path(kernel, 2, strlen(consola.argumento), consola.argumento);
				//Recibo los datos
				DatosRecibidos *buffer = deserializar_path(kernel);
				//Muestro los datos
				printf("Me llegaron %d bytes con %s\n", buffer->bytesRecibidos, buffer->datos);
		}
		else if (!strcmp(consola.comando, "exit"))
			salir = 1;
		else if (!strcmp(consola.comando, "clean"))
			system("clear");
		else
			printf("Comando incorrecto. Pruebe run | stop | exit | clean\n");
	}

	cerrar_config_actual();

    return EXIT_SUCCESS;

}
