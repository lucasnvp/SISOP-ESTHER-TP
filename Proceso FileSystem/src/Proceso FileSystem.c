#include <stdio.h>
#include <stdlib.h>
#include <commons/lectorArchivos.h>

char* path = "/home/utnso/Escritorio/TP/tp-2017-1c-Blacklist/Proceso FileSystem/src/config.txt";


int main(void){
    puts("Proceso FileSystem");

    fileDataType data[10];
    obtenerConfiguracion(data,path);

    printf("%s",data->name);
    printf("%s",data->value);

    mostrarConfiguracion(path);

    return EXIT_SUCCESS;
}
