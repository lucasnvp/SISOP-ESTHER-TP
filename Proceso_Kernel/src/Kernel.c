#include "Kernel.h"

pthread_mutex_t mutexPCB; //Para asegurar la mutua exclusion en la cola de PCB
sem_t SEM_MULTIPROGRAMACION;
sem_t SEM_PCB; //Para que la cola de PCB se suspenda si no tiene trabajos

uint32_t PID_PCB = 1;
t_queue * QUEUE_PCB;

uint32_t SERVIDOR_KERNEL;
uint32_t SERVIDOR_MEMORIA;
uint32_t SERVIDOR_FILESYSTEM;

int main(void) {
	puts("Proceso Kernel");

	// Variables hilos
	pthread_t programa;
	pthread_t thread_server;
	pthread_t thread_consola;

	QUEUE_PCB = queue_create();

	pthread_mutex_init(&mutexPCB, NULL);	//Inicializo el mutex
	sem_init(&SEM_PCB,0,0); //Iniciazilo el semaforo de la cola de PCB
	sem_init(&SEM_MULTIPROGRAMACION,0,1); 	//Semaforo de multi programacion

	//Conexion al servidor FileSystem
	connect_server_memoria();

	//Conexion al servidor FileSystem
	connect_server_filesystem();

	//Creo el hilo del servidor
	pthread_create(&thread_server,NULL,(void*) server,"Servidor");

	//Hilo de consola
	pthread_create(&thread_consola,NULL,(void*) consola_kernel,"Consola");

	//Hilo por cada programa
	pthread_create(&programa,NULL,(void*) procesarPCB,NULL);

	pthread_join(thread_server, (void**) NULL);
	pthread_join(thread_consola, (void**) NULL);
	pthread_join(programa, (void**) NULL);

	return EXIT_SUCCESS;
}

void connect_server_memoria(){
    //Conexion al servidor FileSystem
	//SERVIDOR_MEMORIA = connect_server(ip_memoria(),puerto_memoria());
	SERVIDOR_MEMORIA = connect_server("127.0.0.1",5002);

	//Si conecto, informo
	if(SERVIDOR_MEMORIA > 0){
		printf("Connect Memoria\n");
	}
}

void connect_server_filesystem(){
    //Conexion al servidor FileSystem
	SERVIDOR_FILESYSTEM = connect_server("127.0.0.1",5003);

	//Si conecto, informo
	if(SERVIDOR_FILESYSTEM > 0){
		printf("Connect File System\n");
	}
}

void procesarPCB(void* args){
	Program* aProgram;

	while(true){
		aProgram = (Program*) queue_sync_pop(QUEUE_PCB);

		printf("Nuevo proceso PCB\n");
		printf("El pid del proceso es: %d \n", PID_PCB);
		PCB newPCB = PCB_new(PID_PCB, 0, 0, 0, 0, 0, 0);

		//Envio el PID a la consola
		serializar_path(aProgram->ID_Consola, PID_PCB, 4, "PID");

		//Muestro el PID Del proceso
		print_PCB(newPCB);

		PID_PCB++;
	}
}

void server(void* args){
	// Variables para el servidor
	fd_set master;   	// conjunto maestro de descriptores de fichero
	fd_set read_fds; 	// conjunto temporal de descriptores de fichero para select()
	uint32_t fdmax;			// número máximo de descriptores de fichero
	int i;				// variable para el for
	FD_ZERO(&master);	// borra los conjuntos maestro
	FD_ZERO(&read_fds);	// borra los conjuntos temporal

	//Creacion del servidor consola
	SERVIDOR_KERNEL = build_server(5010, 10);

	//El socket esta listo para escuchar
	if(SERVIDOR_KERNEL > 0){
		printf("Servidor escuchando\n");
	}

	// añadir listener al conjunto maestro
	FD_SET(SERVIDOR_KERNEL, &master);

	// seguir la pista del descriptor de fichero mayor
	fdmax = SERVIDOR_KERNEL; // por ahora es éste

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
				if (i == SERVIDOR_KERNEL) {
					// acepto una nueva conexion
					uint32_t newfd = accept_conexion(SERVIDOR_KERNEL);
					FD_SET(newfd, &master); // añadir al conjunto maestro
					if (newfd > fdmax) {    // actualizar el máximo
						fdmax = newfd;
					}
				} else {
					//Recibo los datos
					DatosRecibidos *buffer = deserializar_path(i);

					// gestionar datos de un cliente
					if(buffer <= 0){
						FD_CLR(i, &master); // eliminar del conjunto maestro
					}else {
						//Muestro los datos
						printf("Me llegaron %d bytes con %s\n", buffer->bytesRecibidos, buffer->datos);

						Program * auxProgram = malloc(sizeof(Program));
						auxProgram->ID_Consola = i;
						auxProgram->Path = buffer->datos;
						queue_sync_push(QUEUE_PCB, auxProgram);

						//Manda la info a la memoria
						send_data(SERVIDOR_MEMORIA, buffer->datos, buffer->bytesRecibidos);
						//Manda la info al FS
						send_data(SERVIDOR_FILESYSTEM, buffer->datos, buffer->bytesRecibidos);
						//Manda la info a todas las cpu
						massive_send(fdmax, &master, buffer, i, SERVIDOR_KERNEL);
					}
				}
			}
		}
	}
}

void consola_kernel(void* args){
	while(true) {
		t_Consola consola = leerComandos();
		consola.kernel = SERVIDOR_KERNEL;
		if (!strcmp(consola.comando, "exit"))
			exit(0);
		else if (!strcmp(consola.comando, "clean"))
			system("clear");
		else
			printf("Comando incorrecto. Pruebe con: exit | clean\n");
	}
}

void* queue_sync_pop(t_queue* self) {
	sem_wait(&SEM_PCB);
	pthread_mutex_lock(&mutexPCB);
	void* elem = queue_pop(self);
	pthread_mutex_unlock(&mutexPCB);
	//Semaforo de multiprogramacion, tiene que ir cuando se finaliza un PCB asi lo deja entrar otro en la cola.
	sem_post(&SEM_MULTIPROGRAMACION);
	return elem;
}

void queue_sync_push(t_queue* self, void* element){
	sem_wait(&SEM_MULTIPROGRAMACION);
	pthread_mutex_lock(&mutexPCB);
	queue_push(self, element);
	pthread_mutex_unlock(&mutexPCB);
	sem_post(&SEM_PCB);
}
