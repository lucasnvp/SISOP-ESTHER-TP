#ifndef LECTOR_LECTORARCHIVO_H_
#define LECTOR_LECTORARCHIVO_H_

typedef struct fileData fileDataType;
int fileRead (char * ,char [100][100]);
void trimArrayValues(char [100][100],fileDataType *);
void leerConfiguracion(char* path);

struct fileData{
        char *name;
        int *value;
        };

#endif /* LECTOR_LECTORARCHIVO_H_ */
