#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "lectorArchivos.h"
#include "servidor.h"

char* path = "/home/utnso/Blacklist/tp-2017-1c-Blacklist/Proceso Consola/src/config.txt";


int main (void){

	puts("Proceso Consola");

	mostrarConfiguracion(path);

	//Me conecto al servidor
	int servidor = connect_server();

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

