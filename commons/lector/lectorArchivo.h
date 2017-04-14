#ifndef LECTOR_LECTORARCHIVO_H_
#define LECTOR_LECTORARCHIVO_H_

struct fileData{
        char *name;
        int *value;
        };

typedef struct fileData fileDataType;

int fileRead (char * path,char configValues[100][100]);
void trimArrayValues(char configValues[100][100],fileDataType data[10]);
void mostrarConfiguracion(char * path);
char* obtenerConfiguracion(char* parametro,char* path);


#endif /* LECTOR_LECTORARCHIVO_H_ */
