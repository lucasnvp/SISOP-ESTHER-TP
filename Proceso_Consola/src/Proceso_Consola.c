#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "lector/lectorArchivos.h"
#include "servidor/servidor.h"
#include "serializador/serializador.h"

char* path = "src/config.txt";

void limpiarBufferDeEntrada();

int main (void){

	puts("Proceso Consola");

	mostrarConfiguracion(path);

	//Me conecto al servidor
	int kernel = connect_server("127.0.0.1",5010);

	//Si conecto, informo
	if(kernel > 0){
		printf("Ready to send \n");
	}

	int salir = 0;

	//Bucle para el ingreso de datos
	while (!salir) {
		//Mensaje
		char* mensaje = (char *) malloc(sizeof(char) * 1000);
		printf("> ");
		scanf("%[^\n]s", mensaje);
		char* comando = strtok(mensaje, " ");
		char* argumento = strtok(NULL, " ");
		if (!strcmp(comando, "run") || !strcmp(comando, "stop")) {
			if (argumento == NULL)
				printf("Falta el argumento de la funcion %s\n", comando);
			else //Envio el mensaje
				//Serializo el path
				serializar_path(kernel, 2, strlen(argumento), argumento);

				//Recibo los datos
				DatosRecibidos *buffer = deserializar_path(kernel);
				//Muestro los datos
				printf("Me llegaron %d bytes con %s\n", buffer->bytesRecibidos, buffer->datos);

		}
		else if (!strcmp(comando, "exit"))
			salir = 1;
		else if (!strcmp(comando, "clean"))
			system("clear");
		else
			printf("Comando incorrecto. Pruebe run | stop | exit | clean\n");
		free(mensaje);
		limpiarBufferDeEntrada();
	}

    return EXIT_SUCCESS;

}

void limpiarBufferDeEntrada() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF) { }
}
