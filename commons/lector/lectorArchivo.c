#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lectorArchivo.h"

int fileRead (char * path,char configValues[100][100]){

FILE * file;

file = fopen(path,"r"); /*Le asigno al puntero file la ruta del archivo que abro con la funcion fopens*/
int i=0;
    if (file == NULL){

        return(1);
    }else{
    while (feof(file)==0)
        {

                fgets(configValues[i],100,file);

                ++i;
        }

         strcpy(configValues[i], "END");

}
    return i;
}

void trimArrayValues(char configValues[100][100],fileDataType data[10]){
int i=0;
char *row;
char *name;

while (strcmp(configValues[i],"END")){

    row=strrchr(configValues[i],'='); //Puntero a '=' de cada linea

    int nameLength= (strlen(configValues[i])-1)-(strlen(row)-1);

    (data+i)->value = malloc(strlen(row));

    memmove((data+i)->value,row+1,strlen(row)); //Guardo el valor de cada linea desde el =

    name = malloc(nameLength);

    memmove(name,configValues[i],nameLength);

    memmove((name+nameLength),"\0",1);

    (data+i)->name = malloc(nameLength);

    memmove((data+i)->name,name,nameLength+1);

    ++i;

}


}

void leerConfiguracion(char* path){

	 	FILE *archivo;

	 	char caracteres[100];

	 	archivo = fopen(path,"r");

	 	if (archivo == NULL)
	 		printf("Error");
	 		exit(1);

	 	printf("\nEl contenido del archivo de prueba es \n\n");
	 	while (feof(archivo) == 0)
	 	{
	 		fgets(caracteres,100,archivo);
	 		printf("%s",caracteres);
	 	}
	        fclose(archivo);

}
