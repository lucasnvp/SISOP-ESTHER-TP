#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "lector/lectorArchivos.h"
#include "servidor/servidor.h"
#include "serializador/serializador.h"
#include "pcb/pcb.h"

char* path = "src/config.txt";

#define PORT 8080   		// puerto en el que escuchamos
#define CANTCONECIONES 10 	// Si quiero el maximo de conexiones posibles en el sockect reemplazar por 'SOMAXCONN'

//variable temporal, cambiar por la que se lee en el archivo de config.
int32_t nivelMultiprogramacion = 1;
int32_t statusMultiprogramacion = 0;

int main(void){
    puts("Proceso Kernel");

    mostrarConfiguracion(path);

    //Almaceno todos los PCBs en ejecucion.
    PCB listaPCB;

    //Conexion al servidor FileSystem
	int memoria = connect_server("127.0.0.1",5002);

	//Si conecto, informo
	if(memoria > 0){
		printf("Connect Memoria\n");
	}

    //Conexion al servidor FileSystem
	int fileSystem = connect_server("127.0.0.1",5003);

	//Si conecto, informo
	if(fileSystem > 0){
		printf("Connect File System\n");
	}

    // Variables para el servidor
    fd_set master;   	// conjunto maestro de descriptores de fichero
	fd_set read_fds; 	// conjunto temporal de descriptores de fichero para select()
	uint32_t fdmax;			// número máximo de descriptores de fichero
	int i;				// variable para el for
	FD_ZERO(&master);	// borra los conjuntos maestro
	FD_ZERO(&read_fds);	// borra los conjuntos temporal

	//Creacion del servidor consola
	int servidorConsola = build_server(5010, CANTCONECIONES);

	//El socket esta listo para escuchar
	if(servidorConsola > 0){
		printf("Servidor escuchando\n");
	}

	// añadir listener al conjunto maestro
	FD_SET(servidorConsola, &master);

	// seguir la pista del descriptor de fichero mayor
	fdmax = servidorConsola; // por ahora es éste

	// bucle principal
	while(1) {
		read_fds = master; // cópialo
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			exit(1);
		}
		// explorar conexiones existentes en busca de datos que leer
		for(i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) { // ¡¡tenemos datos!!
				if (i == servidorConsola) {
					// acepto una nueva conexion
					uint32_t newfd = accept_conexion(servidorConsola);
					FD_SET(newfd, &master); // añadir al conjunto maestro
					if (newfd > fdmax) {    // actualizar el máximo
						fdmax = newfd;
					}
				} else {
					//Recibo los datos
					DatosRecibidos *buffer = deserializar_path(i);

					// gestionar datos de un cliente
					if(buffer <= 0){
						FD_CLR(i, &master); // eliminar del conjunto maestro
					}else {
						//Muestro los datos
						printf("Me llegaron %d bytes con %s\n", buffer->bytesRecibidos, buffer->datos);

						//Valido el nivel de multiprocesamiento
						uint32_t pidPCB = validarMultiprogramacion(listaPCB);
						//Envio el PID a la consola
						serializar_path(i, pidPCB, 4, "PID");

						//Manda la info a la memoria
						send_data(memoria, buffer->datos, buffer->bytesRecibidos);
						//Manda la info al FS
						send_data(fileSystem, buffer->datos, buffer->bytesRecibidos);
						//Manda la info a todas las cpu
						massive_send(fdmax, &master, buffer, i, servidorConsola);
					}
				}
			}
		}
	}

    return EXIT_SUCCESS;
}

int32_t validarMultiprogramacion(PCB listaPCB){
	if(nivelMultiprogramacion > statusMultiprogramacion){
		printf("Nuevo proceso\n");
		uint32_t pid = fork();
		printf("El pid del proceso es: %d \n", pid);
		listaPCB = PCB_new(pid, 0, 0, 0, 0, 0, 0);

		//Muestro el PID Del proceso
		print_PCB(listaPCB);

		statusMultiprogramacion++;

		return pid;
	}else{
		printf("Nivel de multiprocesamiento al maximo\n");
		return -1;
	}
}
