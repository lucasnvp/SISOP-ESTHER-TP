#include "servidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int connect_server(char* IP, int Port){
	//Estructura del socket
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr(IP);
	direccionServidor.sin_port = htons(Port);

	//Nro de cliente
	int cliente;
	if ((cliente = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	//Valido la conexion
	if (connect(cliente, (void*) &direccionServidor, sizeof(direccionServidor)) != 0) {
		perror("No se pudo conectar");
		return 1;
	}

	//Retorno nro de conexion
	return cliente;
}

int recive_data(int cliente, void *buf,int bytesToRecive){
	//Cantidad de bytes a recibir
	int numbytes =recv(cliente, buf, bytesToRecive, 0);
	if(numbytes <=0){
		if ((numbytes) <0) {
			perror("Error al recibir datos");
		}else{
			printf("Socket %d hung up\n",cliente);
		}
		close(cliente);
	}
	return numbytes;
}

void send_data(int servidor, void *mensaje, int sizeMensaje){
	if(send(servidor, mensaje, sizeMensaje, 0) < 0){
		perror("Error al enviar data");
	}
}
