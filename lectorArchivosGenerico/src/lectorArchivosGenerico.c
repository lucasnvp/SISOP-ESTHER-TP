#include<stdio.h>
#include <getnum.h>
#include <stdlib.h>
#include <string.h>
int fileRead (char *,char[100][100]);

typedef struct fileData fileDataType;

void trimArrayValues(char configValues[100][100],fileDataType *);

 struct fileData{
        char *name;
        int *value;
        };


int main(void){

    char pepe[100][100];


    fileDataType data[10];
    int a = fileRead("testFile.txt",pepe);

    trimArrayValues(pepe,data);
    int largo=0;
    while (largo<a){
    if (strstr((data+largo)->name, "PORT") != NULL)
       {
           printf("%s",(data+largo)->value);
       }
       largo++;
    }
}

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
// char *eq=strrchr(configValues[i],'=');
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


    //free(name);

    ++i;



}
return;
}


