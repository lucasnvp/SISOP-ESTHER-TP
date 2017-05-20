#include "Kernel.h"

int main(void) {
	puts("Proceso Kernel");

	//Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);

	// Variables hilos
	pthread_t thread_programa;
	pthread_t thread_server;
	pthread_t thread_consola;
	pthread_t thread_planificador;

	QUEUE_PCB = queue_create();
	QUEUE_NEW = queue_create();
	LIST_READY = list_create();

	pthread_mutex_init(&mutexPCB, NULL);	//Inicializo el mutex
	sem_init(&SEM_MULTIPROGRAMACION,0,2); 	//Semaforo de multi programacion
	sem_init(&SEM_PCB,0,0);	//Iniciazilo el semaforo de la cola de PCB
	sem_init(&SEM_READY,0,0); //Avisa cuando ingresa un PCB a NEW
	sem_init(&SEM_STOP_PLANNING,0,1); //Semaforo para detener la planificacion

	//Conexion al servidor FileSystem
	connect_server_memoria();

	//Conexion al servidor FileSystem
	connect_server_filesystem();

	//Creo el hilo del servidor
	pthread_create(&thread_server,NULL,(void*) server,"Servidor");

	//Hilo de consola
	pthread_create(&thread_consola,NULL,(void*) consola_kernel,"Consola");

	//Hilo que procesa los PCB mandandolos a la cola de NEWs
	pthread_create(&thread_programa,NULL,(void*) procesarPCB,NULL);

	//Hilo que planifica el paso de NEWs a READYs dependiendo del grado de multiprogramacion
	pthread_create(&thread_planificador,NULL,(void*) planificador, NULL);

	pthread_join(thread_server, (void**) NULL);
	pthread_join(thread_programa, (void**) NULL);
	pthread_join(thread_planificador, (void**) NULL);
	pthread_join(thread_consola, (void**) NULL);

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

		uint32_t idConsola = aProgram->ID_Consola;

		PCB * newPCB = PCB_new_pointer(idConsola,PID_PCB, 0, 0, 0, 0, 0, 0);

		//Agrego el pcb a la lista de new
		queue_push(QUEUE_NEW, newPCB);
		//Aviso que hay un nuevo PCB
		sem_post(&SEM_READY);

		//Envio el PID a la consola
		serializar_path(aProgram->ID_Consola, PID_PCB, 4, "PID");

		//Muestro el PID Del proceso
		print_PCB(newPCB);

		PID_PCB++;
	}
}

void planificador(void* args){
	while(true){
		//Semaforo de multiprogramacion, detiene el ingreso de PCBs a la lista de READYs
		sem_wait(&SEM_MULTIPROGRAMACION);
		//Levanto el signal de un nuevo PCB
		sem_wait(&SEM_READY);
		//Semaforo para parar la planificacion
		sem_wait(&SEM_STOP_PLANNING);
		//Informo que ingresa un PCB al planificador
		printf("Ingreso un PCB al panificador \n");
		//Sacar un PCB de la cola de NEWs
		PCB* element = (PCB*) queue_pop(QUEUE_NEW);
		//Agregar un PCB a la lista de READYs
		list_add(LIST_READY,element);
		//vuelvo a liberar la planificacion
		sem_post(&SEM_STOP_PLANNING);
	}

	//Preguntar si algun CPU esta disponible
	//Mandar el PCB a la CPU
	//Respuesta de la CPU
	//Semaforo de multiprogramacion, tiene que ir cuando se finaliza un PCB asi deja entrar otro en la lista.

	//sem_post(&SEM_MULTIPROGRAMACION);
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
		else if (!strcmp(consola.comando, "list"))
			list_process(LIST_READY);
		else if (!strcmp(consola.comando, "stop"))
			sem_wait(&SEM_STOP_PLANNING);
		else if (!strcmp(consola.comando, "start"))
			sem_post(&SEM_STOP_PLANNING);
		else if (!strcmp(consola.comando, "status"))
			if (consola.argumento == NULL)
				printf("Falta el argumento de la funcion %s\n", consola.comando);
			else {
				uint32_t nroProceso = atoi(consola.argumento);
				status_process(LIST_READY,nroProceso);
			}
		else if (!strcmp(consola.comando, "kill"))
			if (consola.argumento == NULL)
				printf("Falta el argumento de la funcion %s\n", consola.comando);
			else {
				uint32_t nroProceso = atoi(consola.argumento);
				kill_process(LIST_READY,nroProceso);
			}
		else
			printf("Comando incorrecto. Pruebe con: exit | clean | list | stop | start | status | kill \n");
	}
}

void* queue_sync_pop(t_queue* self) {
	sem_wait(&SEM_PCB);
	pthread_mutex_lock(&mutexPCB);
	void* elem = queue_pop(self);
	pthread_mutex_unlock(&mutexPCB);
	return elem;
}

void queue_sync_push(t_queue* self, void* element){
	pthread_mutex_lock(&mutexPCB);
	queue_push(self, element);
	pthread_mutex_unlock(&mutexPCB);
	sem_post(&SEM_PCB);
}
