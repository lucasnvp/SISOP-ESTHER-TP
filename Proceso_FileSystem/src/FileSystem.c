#include "FileSystem.h"

int main(void){
    puts("Proceso FileSystem");

    //Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);

	//Config. inicial de FS
	punto_montaje_setup(config.PUNTO_MONTAJE);

	// Variables hilos
	pthread_t thread_server;

	//Creo el hilo del servidor
	pthread_create(&thread_server,NULL,(void*) server,"Servidor");

	pthread_join(thread_server, (void**) NULL);

    return EXIT_SUCCESS;
}

void server(void* args){
	// Variables para el servidor
	fd_set master;   	// conjunto maestro de descriptores de fichero
	fd_set read_fds; 	// conjunto temporal de descriptores de fichero para select()
	uint32_t fdmax;			// número máximo de descriptores de fichero
	int i;				// variable para el for
	FD_ZERO(&master);	// borra los conjuntos maestro
	FD_ZERO(&read_fds);	// borra los conjuntos temporal
	char command;

	//Creacion del servidor
	uint32_t SERVIDOR_FILESYSTEM = build_server(config.PUERTO_FILESYSTEM, config.CANTCONEXIONES);

	//El socket esta listo para escuchar
	if(SERVIDOR_FILESYSTEM > 0){
		printf("Servidor FileSystem Escuchando\n");
	}

	// añadir listener al conjunto maestro
	FD_SET(SERVIDOR_FILESYSTEM, &master);

	// seguir la pista del descriptor de fichero mayor
	fdmax = SERVIDOR_FILESYSTEM; // por ahora es éste

	// bucle principal
	while(true) {
		read_fds = master; // cópialo
		if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
			perror("select");
			exit(1);
		}
		// explorar conexiones existentes en busca de datos que leer
		for(i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) { // ¡¡tenemos datos!!
				if (i == SERVIDOR_FILESYSTEM) {
					// acepto una nueva conexion
					uint32_t newfd = accept_conexion(SERVIDOR_FILESYSTEM);
					FD_SET(newfd, &master); // añadir al conjunto maestro
					if (newfd > fdmax) {    // actualizar el máximo
						fdmax = newfd;
					}
				} else {
					//Recibo el comando
					uint32_t command = deserializar_int(i);

					// gestionar datos de un cliente
					if(command <= 0){
						close(i); // Close conexion
						FD_CLR(i, &master); // eliminar del conjunto maestro
					}else {
						connection_handler(i, command);
					}
				}
			}
		}
	}
}

void connection_handler(uint32_t socket, uint32_t command){
	switch(command){
	case 1:{
		//Se conecto un kernel
		printf("Se conecto el kernel");
		break;
	}
	case 2:{
		//validar archivo
		t_SerialString* path = malloc(sizeof(t_SerialString));
		deserializar_string(socket, path);
		ValidarArchivo(path->dataString);
		free(path);
		break;
	}
	case 3: {
		t_SerialString* path = malloc(sizeof(t_SerialString));
		deserializar_string(socket, path);
		CrearArchivo(path->dataString);
		free(path);
		break;
	}
	case 4:{
		t_SerialString* path = malloc(sizeof(t_SerialString));
		deserializar_string(socket, path);
		Borrar(path->dataString);
		free(path);
		break;
	}
	case 5: {
		t_SerialString* path = malloc(sizeof(t_SerialString));
		deserializar_string(socket, path);
		uint32_t offset = deserializar_int(socket);
		uint32_t size = deserializar_int(socket);
		ObtenerDatos(path->dataString, offset, size);
		free(path);
		break;
	}
	case 6: {
		//guardar datos
		t_SerialString* path = malloc(sizeof(t_SerialString));
		deserializar_string(socket, path);
		t_SerialString* buffer = malloc(sizeof(t_SerialString));
		deserializar_string(socket, buffer);
		uint32_t offset = deserializar_int(socket);
		uint32_t size = deserializar_int(socket);
		GuardarDatos(path->dataString , offset, size, buffer->dataString);
		free(buffer);
		free(path);
		break;
	}
	default:
		printf("Error al recibir el comando");
	}

	return;
}

uint32_t ValidarArchivo(char* path){
	uint32_t archivo_existente;
	if(access(path, F_OK) != -1){
		archivo_existente = true;
	} else{
		archivo_existente = false;
	}
	return archivo_existente;
}

void CrearArchivo(char* path){
	if(ValidarArchivo(path) == false){
		FILE * newFD;
		newFD = fopen(path, "w+");
		fwrite(TAMANIO_BLOQUE_NULL,1,strlen(TAMANIO_BLOQUE_NULL),newFD);
		//Agregar el bloque
	 	int32_t bloque_to_add = bloque_libre(bitarray);
		if(bloque_to_add != -1){
			char* bloquesDelArchivo = string_new();
			string_append(&bloquesDelArchivo, "BLOQUES=[");
			string_append(&bloquesDelArchivo, string_itoa(bloque_to_add));
			string_append(&bloquesDelArchivo, "]\n");
			fwrite(bloquesDelArchivo,1,strlen(bloquesDelArchivo),newFD);

			bitarray_set_bit(bitarray, bloque_to_add);
		} else{
			//No hay bloques
		}
		fclose(newFD);
	}
}

int32_t bloque_libre(t_bitarray* auxBitArray){
	//bool testBit;
	for(i = 0; i <= bitarray_get_max_bit(auxBitArray); i++){
		if(bitarray_test_bit(auxBitArray, i) == false){
			return i;
		}
	}
	//No hay bloques
	return -1;
}

void Borrar(char* path){
	char** bloquestodelete = get_bloques_array(path);
	for(i=0 ; bloquestodelete[i] != NULL ; i++){
		bitarray_clean_bit(bitarray, atoi(bloquestodelete[i]));
	}
	free(bloquestodelete);

	if(remove(path) == 0){
		printf("Se elimino el archivo exitosamente\n");
	} else{
		printf("Error al eliminar el archivo\n");
	}
}

char* ObtenerDatos(char* path, uint32_t offset, uint32_t size){
	char** bloquestoread = get_bloques_array(path);
	char* pathDato = strdup(get_bloque_from_int(atoi(bloquestoread[offset])));
	FILE * fileToRead = fopen(pathDato,"r");
	char* datosObtenidos = malloc( sizeof(char) * size);
	fgets(datosObtenidos,size + 1,fileToRead);
	fclose(fileToRead);
	return datosObtenidos;
}

void GuardarDatos(char* path, uint32_t offset, uint32_t size, char* buffer){
	char** bloquestosave = get_bloques_array(path);
	//en el offset tiene que ir el resultado de una formula para calcular en que bloque esta.
	char* pathDato = strdup(get_bloque_from_int(atoi(bloquestosave[offset])));
//	printf("%s\n", pathDato);
	FILE * fileToWrite = fopen(pathDato, "w+b");
	fwrite(buffer, sizeof(char), size, fileToWrite);
	fclose(fileToWrite);
	free(bloquestosave);
}

char** get_bloques_array(char* path){
	t_config* filetogetbloques = config_create(path);
	char** bloquesarray = config_get_array_value(filetogetbloques, "BLOQUES");
	config_destroy(filetogetbloques);
	return bloquesarray;
}

char* get_bloque_from_int(uint32_t nroDeBloque){
	char* nroBloque = string_new();
	string_append(&nroBloque, montajeBloques);
	string_append(&nroBloque, string_itoa(nroDeBloque));
	string_append(&nroBloque, ".bin");
	return nroBloque;
}

void init_log_FS(char* pathLog){
	mkdir("/home/utnso/Blacklist/Logs",0755);
	log_Console = log_create(pathLog, "FileSystem", true, LOG_LEVEL_INFO);
	log_FileSystem = log_create(pathLog, "FileSystem", false, LOG_LEVEL_INFO);
}
