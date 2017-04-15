#include <stdio.h>
#include <stdlib.h>
#include "lectorArchivos.h"

char* path = "/home/utnso/Escritorio/TP/tp-2017-1c-Blacklist/Proceso Consola/src/config.txt";


int main (void){

	puts("Proceso Consola");

	/*fileDataType data[10];
	obtenerConfiguracion(data,path);

	printf("%s",data->name);
	printf("%s",data->value);*/

	mostrarConfiguracion(path);

    return EXIT_SUCCESS;

}

