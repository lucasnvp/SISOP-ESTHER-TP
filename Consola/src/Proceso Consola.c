#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "lectorArchivos.h"
#include "servidor/servidor.h"

char* path = "/config.txt";


int main (void){

	puts("Proceso Consola");

	mostrarConfiguracion(path);

	//Me conecto al servidor
	int servidor = connect_server("127.0.0.1",8080);

	//Si conecto, informo
	if(servidor > 0){
		printf("Ready to send \n");
	}

	//Bucle para el ingreso de datos
	while (1) {
		//Mensaje
		char* mensaje = malloc(1000);
		scanf("%s", mensaje);
		//Envio el mensaje
		send(servidor, mensaje, strlen(mensaje), 0);
		free(mensaje);
	}

    return EXIT_SUCCESS;

}

