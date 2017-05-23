#include "FileSystem.h"

int main(void){
    puts("Proceso FileSystem");

    //Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);

    // variables para el servidor
    int fdmax;        // número máximo de descriptores de fichero
    fd_set master;   // conjunto maestro de descriptores de fichero

    //Creacion del servidor
	int servidor = build_server(config.PUERTO_FILESYSTEM);

	//El socket esta listo para escuchar
	if(servidor > 0){
		printf("Servidor FileSystem Escuchando\n");
	}

	// Seteo la cantidad de conexiones
	set_listen(servidor, config.CANTCONEXIONES);

	// seguir la pista del descriptor de fichero mayor
	fdmax = servidor; // por ahora es éste

	// bucle principal
	while(1) {
		if(fdmax == servidor){
			// acepto una nueva conexion
			fdmax = accept_conexion(servidor, &master, fdmax);
		}else{
			DatosRecibidos * buffer = recive_data(fdmax);
			// gestionar datos de un cliente
			if(buffer <= 0){
				fdmax = servidor; // eliminar del conjunto maestro
			}
		}
	}

    return EXIT_SUCCESS;
}