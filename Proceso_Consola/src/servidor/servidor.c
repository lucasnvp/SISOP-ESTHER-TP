#include "servidor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <inttypes.h>

uint32_t connect_server(char* IP, uint32_t Port){
	//Estructura del socket
	struct sockaddr_in direccionServidor;
	direccionServidor.sin_family = AF_INET;
	direccionServidor.sin_addr.s_addr = inet_addr(IP);
	direccionServidor.sin_port = htons(Port);

	//Nro de cliente
	uint32_t cliente;
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

uint32_t build_server(uint32_t Port, uint32_t quantityConexions){
	//Estructura del socket
	struct sockaddr_in datosServidor;
	datosServidor.sin_family = AF_INET;
	datosServidor.sin_addr.s_addr = INADDR_ANY;
	datosServidor.sin_port = htons(Port);
	memset(&(datosServidor.sin_zero), '\0', 8);

	//Nro de cliente
	uint32_t servidor;

	// obtener socket a la escucha
	if ((servidor = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	// obviar el mensaje "address already in use" (la dirección ya se está usando)
    int yes=1;        // para setsockopt() SO_REUSEADDR
	if (setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
	// enlazar
	if (bind(servidor, (struct sockaddr *)&datosServidor, sizeof(datosServidor)) == -1) {
		perror("bind");
		exit(1);
	}

	// Seteo la cantidad de conexiones
	set_listen(servidor, quantityConexions);

	return servidor;
}

void set_listen(uint32_t servidor, uint32_t sizeConexiones){
    if (listen(servidor, sizeConexiones) == -1) {
        perror("listen");
        exit(1);
    }
}

uint32_t accept_conexion(uint32_t servidor){
	// dirección del cliente
	struct sockaddr_in remoteaddr;
	// gestionar nuevas conexiones
	uint32_t addrlen = sizeof(remoteaddr);
	// descriptor de socket de nueva conexión aceptada
	uint32_t newfd;

	if ((newfd = accept(servidor, (struct sockaddr *)&remoteaddr, &addrlen)) == -1) {
		perror("accept");
	} else {
		printf("Server: new connection from %s on socket %d\n", inet_ntoa(remoteaddr.sin_addr), newfd);
	}

	return newfd;
}

int recive_data(int cliente, void *buf, int bytesToRecive){
	//Recibo datos
	int bytesRecibidos = recv(cliente, buf, bytesToRecive, 0);
	//Valido los datos
	if(bytesRecibidos <= 0){
		// error o conexión cerrada por el cliente
		if (bytesRecibidos == 0) {
			// conexión cerrada
			printf("Socket %d hung up\n", cliente);
		} else {
			perror("Error al recibir datos");
		}
		close(cliente);
	}

	return bytesRecibidos;
}

void send_data(int servidor, void *mensaje, int sizeMensaje){
	if(send(servidor, mensaje, sizeMensaje, 0) < 0){
		perror("Error al enviar data");
	}
}

void massive_send(int fdmax, fd_set *master, DatosRecibidos * buffer, int i, int servidor){
	int j;
    for(j = 0; j <= fdmax; j++) {
        // ¡enviar a todo el mundo!
        if (FD_ISSET(j, master)) {
            // excepto al listener y a nosotros mismos
            if (j != servidor && j != i) {
                if (send(j, buffer->datos, buffer->bytesRecibidos, 0) == -1) {
                    perror("send");
                }
            }
        }
    }
    DatosRecibidos_free(buffer);
}

DatosRecibidos * DatosRecibidos_new(char *datos,int bytesRecibidos){
	DatosRecibidos * unosDatosRecibidos = malloc(sizeof(DatosRecibidos));
	unosDatosRecibidos->datos=datos;
	unosDatosRecibidos->bytesRecibidos=bytesRecibidos;
	return unosDatosRecibidos;
}

void DatosRecibidos_free(DatosRecibidos *this){
	free(this);
}
