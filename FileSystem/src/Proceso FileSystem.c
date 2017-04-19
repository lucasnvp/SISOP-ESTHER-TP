#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "lectorArchivos.h"
#include "servidor/servidor.h"

#define CANTCONECIONES 10 	// Si quiero el maximo de conexiones posibles en el sockect reemplazar por 'SOMAXCONN'

char* path = "/home/utnso/Blacklist/tp-2017-1c-Blacklist/Proceso FileSystem/src/config.txt";

int main(void){
    puts("Proceso FileSystem");

    mostrarConfiguracion(path);

    // variables para el servidor
    int fdmax;        // número máximo de descriptores de fichero
    fd_set master;   // conjunto maestro de descriptores de fichero

    //Creacion del servidor
	int servidor = build_server(5003);

	//El socket esta listo para escuchar
	if(servidor > 0){
		printf("Servidor FileSystem Escuchando\n");
	}

	// Seteo la cantidad de conexiones
	set_listen(servidor, CANTCONECIONES);

	// seguir la pista del descriptor de fichero mayor
	fdmax = servidor; // por ahora es éste

	// bucle principal
	while(1) {
		if(fdmax == servidor){
			// acepto una nueva conexion
			fdmax = accept_conexion(servidor, &master, fdmax);
		}else{
			DatosRecibidos * buffer = recive_data(fdmax);
			// gestionar datos de un cliente
			if(buffer <= 0){
				fdmax = servidor; // eliminar del conjunto maestro
			}
		}
	}

    return EXIT_SUCCESS;
}
