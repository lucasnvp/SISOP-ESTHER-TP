#include "setup.h"

void punto_montaje_setup(char* pdm){
	puntoDeMontaje =  strdup(pdm);

	montajeMetadata = string_new();
	string_append(&montajeMetadata, puntoDeMontaje);
	string_append(&montajeMetadata, "Metadata/");

	montajeArchivos = string_new();
	string_append(&montajeArchivos, puntoDeMontaje);
	string_append(&montajeArchivos, "Archivos/");

	montajeBloques = string_new();
	string_append(&montajeBloques, puntoDeMontaje);
	string_append(&montajeBloques, "Bloques/");

	setup();
}

void setup(){
	carpetas_setup();

	//Archivo Metadata.bin
	char* metadatabin = string_new();
	string_append(&metadatabin, montajeMetadata);
	string_append(&metadatabin, "Metadata.bin");

	if(access(metadatabin, F_OK) != -1){
		metadata_setup(metadatabin);
	} else{
		new_metadata_setup(metadatabin);
		metadata_setup(metadatabin);
	}

	//Archivo Bitmap.bin
	char* bitmapbin = string_new();
	string_append(&bitmapbin, montajeMetadata);
	string_append(&bitmapbin, "Bitmap.bin");

	if(access(bitmapbin, F_OK) != -1){
		bitmap_setup(bitmapbin);
	} else{
		new_bitmap_setup(bitmapbin);
		bitmap_setup(bitmapbin);
	}

	//Bloques
	bloques_setup();
}

void carpetas_setup(){
	//Crea la carpeta de montaje
	mkdir(puntoDeMontaje, 0777);
	//Crea la carpeta Metadata
	mkdir(montajeMetadata, 0777);
	//Crea la carpeta Archivos
	mkdir(montajeArchivos, 0777);
	//Crea la carpeta Bloques
	mkdir(montajeBloques, 0777);
}

void metadata_setup(char* metadatabin){
	//Obtener Datos Metadata
	metadataConfig = config_create(metadatabin);
	CANT_BLOQUES = config_get_int_value(metadataConfig, "CANTIDAD_BLOQUES");
	TAMANIO_BLOQUES = config_get_int_value(metadataConfig, "TAMANIO_BLOQUES");
	config_destroy(metadataConfig);
}

void new_metadata_setup(char* metadatabin){
	metadata = fopen(metadatabin, "w+b");
	fwrite(TAMANIO_BLOQUE,1,strlen(TAMANIO_BLOQUE),metadata);
	fwrite(CANTIDAD_BLOQUE,1,strlen(CANTIDAD_BLOQUE),metadata);
	fwrite(MAGIC_NUMBER,1,strlen(MAGIC_NUMBER),metadata);
	fclose(metadata);
}

void bitmap_setup(char* bitmapbin){
	struct stat bitmapStat;
	int32_t fd = open(bitmapbin,O_RDWR);
	fstat(fd,&bitmapStat);
	mmapBitmap = mmap(0,bitmapStat.st_size,PROT_READ|PROT_WRITE, MAP_SHARED, fd,0);
	bitarray = bitarray_create_with_mode(mmapBitmap,bitmapStat.st_size, LSB_FIRST);
}

void new_bitmap_setup(char* bitmapbin){
	bitmap = fopen(bitmapbin, "w+b");
	char data[CANT_BLOQUES];
	t_bitarray* bitarray_aux = bitarray_create_with_mode(data,sizeof(data),LSB_FIRST);
	for(i=0; i <= CANT_BLOQUES; i++){
		bitarray_clean_bit(bitarray_aux, i);
	}
	fwrite(bitarray_aux,1,sizeof(bitarray_aux),bitmap);
	fclose(bitmap);
}

void bloques_setup(){
	FILE * newBloque;
	for(i=0; i <= CANT_BLOQUES; i++){
		char* nroBloque = string_new();
		string_append(&nroBloque, montajeBloques);
		string_append(&nroBloque, string_itoa(i));
		string_append(&nroBloque, ".bin");
		newBloque = fopen(nroBloque,"w+b");
		fclose(newBloque);
		free(nroBloque);
	}
}
