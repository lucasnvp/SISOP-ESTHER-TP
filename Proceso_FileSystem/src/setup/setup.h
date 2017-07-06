#ifndef SETUP_H_
#define SETUP_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

#include <commons/bitarray.h>
#include <commons/config.h>
#include <commons/string.h>

static const char* TAMANIO_BLOQUE = "TAMANIO_BLOQUES=64\n";
static const char* CANTIDAD_BLOQUE = "CANTIDAD_BLOQUES=5192\n";
static const char* MAGIC_NUMBER = "MAGIC_NUMBER=SADICA\n";

t_config * metadataConfig;

FILE * metadata;
FILE * bitmap;

t_bitarray * bitarray;
char* mmapBitmap;

char* puntoDeMontaje;
char* montajeMetadata;
char* montajeArchivos;
char* montajeBloques;

int i;

int CANT_BLOQUES;
int TAMANIO_BLOQUES;

void punto_montaje_setup(char* pdm);
void setup();
void carpetas_setup();
void metadata_setup(char* metadatabin);
void new_metadata_setup(char* metadatabin);
void bitmap_setup(char* bitmapbin);
void new_bitmap_setup(char* bitmapbin);
void bloques_setup();

#endif /* SETUP_H_ */
