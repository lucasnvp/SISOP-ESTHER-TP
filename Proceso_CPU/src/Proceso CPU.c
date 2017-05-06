#include <stdio.h>
#include <stdlib.h>
#include "servidor/servidor.h"
#include "config_CPU.h"
#include <parser/parser.h>

char* PATH_CONFIG = "config.cfg";

int main(void){
    puts("Proceso CPU");

	//Cargo archivo de configuracion y muestro
	abrir_config(PATH_CONFIG);
	mostrarConfig();

    //Conexion al servidor
    int kernel = connect_server(ipKernel(),puertoKernel());

	//Si conecto, informo
	if(kernel > 0){
		printf("Estoy escuchando\n");
	}

	while(1){
		//Recibo los datos
		recive_data(kernel);
	}

	cerrar_config_actual();

    return EXIT_SUCCESS;
}
