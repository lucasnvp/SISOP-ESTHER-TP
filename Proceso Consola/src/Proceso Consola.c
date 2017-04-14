#include <stdio.h>
#include <stdlib.h>
#include "../commons/lector/lectorArchivo.c"

int main(void){
    puts("Proceso Consola");

    leerConfiguracion("/home/utnso/Escritorio/TP/tp-2017-1c-Blacklist/Proceso Consola/src/config.txt");

    return EXIT_SUCCESS;
}
