#include <stdio.h>
#include <stdlib.h>
#include "servidor/servidor.h"
#include "lector/lectorArchivos.h"

char* path = "src/config.txt";

int main(void){
    puts("Proceso CPU");

    mostrarConfiguracion(path);

    //Conexion al servidor
    int kernel = connect_server("127.0.0.1",5010);

	//Si conecto, informo
	if(kernel > 0){
		printf("Estoy escuchando\n");
	}

	while(1){
		//Recibo los datos
		recive_data(kernel);
	}

    return EXIT_SUCCESS;
}
