#include <stdio.h>
#include <stdlib.h>
#include <commons/lectorArchivos.h>

char* path = "/home/utnso/Blacklist/tp-2017-1c-Blacklist/Proceso CPU/src/config.txt";


int main(void){
    puts("Proceso CPU");

	fileDataType data[10];
	obtenerConfiguracion(data,path);

	printf("%s",data->name);
	printf("%s",data->value);

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
