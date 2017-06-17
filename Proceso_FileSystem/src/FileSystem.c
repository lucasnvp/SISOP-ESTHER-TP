#include "FileSystem.h"


int main(void){
    puts("Proceso FileSystem");

    //Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);


	//Path Metadata
	PATH_METADATA = string_new();
	string_append(&PATH_METADATA,config.PUNTO_MONTAJE);
	string_append(&PATH_METADATA,METADATA);
	printf ("La ruta es:%s\n", PATH_METADATA);

	//Path Archivos
	PATH_ARCHIVO = string_new();
	string_append(&PATH_ARCHIVO, config.PUNTO_MONTAJE);
	string_append(&PATH_ARCHIVO, ARCHIVO);
	printf ("La ruta de Archivos es:%s\n", PATH_ARCHIVO);

	//Obtener Datos Metadata
	metadata = config_create(PATH_METADATA);
	CANT_BLOQUES = config_get_int_value(metadata, "CANTIDAD_BLOQUES");
	TAMANIO_BLOQUES = config_get_int_value(metadata, "TAMANIO_BLOQUES");
	printf("Bloques: %d\n Tamanio: %d\n", CANT_BLOQUES, TAMANIO_BLOQUES);

	//Bitmap
	PATH_BITMAP= string_new();
	string_append(&PATH_BITMAP, config.PUNTO_MONTAJE);
	string_append(&PATH_BITMAP, BITMAP);
	printf("La ruta del bitmap es:%s\n", PATH_BITMAP);

	inicializar_bitmap();

	//Datos

	PATH_DATOS= string_new();
	string_append(&PATH_DATOS, config.PUNTO_MONTAJE);
	string_append(&PATH_DATOS, DATOS);
	printf("La ruta de los datos es: %s\n", PATH_DATOS);




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
		validar_archivo(path);
		free(path);
		break;
	}
	case 3: {
		//crear archivo
		break;
	}
	case 4:{
		//borrar archivo
		break;
	}
	case 5: {
		//obtener datos
		uint32_t offset = 0;
		t_SerialString* path = malloc(sizeof(t_SerialString));
		obtener_datos(path, offset);
		free(path);
		break;
	}
	case 6: {
		//guardar datos
		break;
	}
	default:
		printf("Error al recibir el comando");
	}

	return;
}

void obtener_datos(char* path, uint32_t offset){
	FILE *fd;
	fd = fopen(path, "r");
	uint32_t size = 0;
	//leer size bytes desde posicion offset
	//lseek(fd, offset, (offset + size)); -- no va en archivos mayores a 4Gb.

}


void validar_archivo (char* path){
		if( access( path, F_OK ) != -1 ) {
			printf( "El archivo existe\n" );
		} else {
			printf( "El archivo no existe\n" );
		}
	}


//Funciones de uso particular

void inicializar_bitmap(){
	char* comando = string_new();
	string_append(&comando, "dd if=/dev/zero of=");
	string_append(&comando, PATH_BITMAP);
	//tostring(CANT_BLOQUES_STRING, CANT_BLOQUES);
	string_append(&comando, " bs=1 count=10"); //necesito pasar el numemro de bloques como string
	system(comando);

	//FILE * f_bit;
	//f_bit = fopen(PATH_BITMAP, "wb");
	//int i;
	//for (i == 0; i < CANT_BLOQUES; i++){
	//	fwrite("0",1 , 1, f_bit);
	//}
	//fclose(f_bit);
}


void tostring(char str[], int num)
{
    int i, rem, len = 0, n;

    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        str[len - (i + 1)] = rem + '0';
    }
    str[len] = '\0';
}



void init_log(char* pathLog){
	mkdir("/home/utnso/Blacklist/Logs",0755);
	log_Console = log_create(pathLog, "FileSystem", true, LOG_LEVEL_INFO);
	log_FileSystem = log_create(pathLog, "FileSystem", false, LOG_LEVEL_INFO);
}
