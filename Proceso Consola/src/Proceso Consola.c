#include <stdio.h>
#include <stdlib.h>
#include "../commons/lector/lectorArchivo.c"




int main (void){

char * pp=obtenerConfiguracion("IP_KERNEL","config.txt");

printf("%s",pp);

mostrarConfiguracion("testFile.txt");

}

