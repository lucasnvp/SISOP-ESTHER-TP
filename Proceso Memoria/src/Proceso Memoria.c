#include <stdio.h>
#include <stdlib.h>

#include "lectorArchivos.h"
#include "servidor.h"

char* path = "/home/utnso/Blacklist/tp-2017-1c-Blacklist/Proceso Memoria/src/config.txt";


int main(void){
    puts("Proceso Memoria");

    mostrarConfiguracion(path);

    //Conexion al servidor
	int cliente = connect_server();

	//Si conecto, informo
	if(cliente > 0){
		printf("Estoy escuchando\n");
	}

	while(1){
		//Recibo los datos
		recive_data(cliente);
	}

    return EXIT_SUCCESS;
}
