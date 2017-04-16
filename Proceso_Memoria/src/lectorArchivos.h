#ifndef LECTOR_LECTORARCHIVO_H_
#define LECTOR_LECTORARCHIVO_H_

struct fileData{
        char *name;
        char *value;
        };

typedef struct fileData fileDataType;

int fileRead (char * path,char configValues[100][100]);
void trimArrayValues(char configValues[100][100],fileDataType data[10]);
void mostrarConfiguracion(char * path);
int obtenerConfiguracion(fileDataType data[10], char * path);


#endif /* LECTOR_LECTORARCHIVO_H_ */
