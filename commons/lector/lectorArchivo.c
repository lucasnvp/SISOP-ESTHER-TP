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
char* obtenerConfiguracion(char* parametro,char* path){



    char configValues[100][100];

    fileDataType data[10];

    int cantidaFilas = fileRead(path,configValues);

    trimArrayValues(configValues,data);

    int i=0;
    while (i<cantidaFilas){
    if (strstr((data+i)->name, parametro) != NULL)
       {
           return ((data+i)->value);
       }
       i++;
    }

return 0;
}

void mostrarConfiguracion(char * path){
int filas=0;
char configValues[100][100];
filas = fileRead (path,configValues);
int i;
for (i=0;i<=filas;i++){
    printf("%s",configValues[i]);
}
}
