#include "Kernel.h"

int main(void) {
	puts("Proceso Kernel");

	//Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);

	//Inicializar Log
	init_log(PATH_LOG);

	// Variables hilos
	pthread_t thread_programa;
	pthread_t thread_server;
	pthread_t thread_consola;
	pthread_t thread_planificador;
	pthread_t thread_ejecucion;

	QUEUE_PCB = queue_create();
	QUEUE_NEW = queue_create();
	QUEUE_EXIT = queue_create();
	QUEUE_CPUS = queue_create();
	QUEUE_READY = queue_create();
	QUEUE_EXEC = queue_create();
	LIST_CONSOLAS = list_create();

	pthread_mutex_init(&mutexPCB, NULL);	//Inicializo el mutex
	sem_init(&SEM_MULTIPROGRAMACION,0,config.GRADO_MULTIPROG); 	//Semaforo de multi programacion
	sem_init(&SEM_PCB,0,0);	//Iniciazilo el semaforo de la cola de PCB
	sem_init(&SEM_READY,0,0); //Avisa cuando ingresa un PCB a NEW
	sem_init(&SEM_STOP_PLANNING,0,1); //Semaforo para detener la planificacion
	sem_init(&SEM_COMMAND,0,0);
	sem_init(&SEM_CPU,0,0); //Empieza a planificar si hay una cpu conectada
	sem_init(&SEM_CPU_DISPONIBLE,0,0); //CPUs disponibles
	sem_init(&SEM_EXECUTE,0,0);	//Hay procesos para ejecutar

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

	//Hilo que ejecuta procesos
	pthread_create(&thread_ejecucion,NULL,(void*) ejecutar, NULL);

	pthread_join(thread_server, (void**) NULL);
	pthread_join(thread_programa, (void**) NULL);
	pthread_join(thread_planificador, (void**) NULL);
	pthread_join(thread_ejecucion, (void**) NULL);
	pthread_join(thread_consola, (void**) NULL);

	return EXIT_SUCCESS;
}

void connect_server_memoria(){
    //Conexion al servidor FileSystem
	//SERVIDOR_MEMORIA = connect_server("127.0.0.1",5002);
	SERVIDOR_MEMORIA = connect_server(config.IP_MEMORIA,config.PUERTO_MEMORIA);

	//Si conecto, informo
	if(SERVIDOR_MEMORIA > 1){
		log_info(log_Kernel, "Connect Memoria");
	} else{
		log_warning(log_Kernel, "No se puedo conectar al servidor de Memoria");
	}
}

void connect_server_filesystem(){
    //Conexion al servidor FileSystem
	SERVIDOR_FILESYSTEM = connect_server(config.IP_FS,config.PUERTO_FS);

	//Si conecto, informo
	if(SERVIDOR_FILESYSTEM > 1){
		log_info(log_Kernel,"Connect File System");
	} else{
		log_warning(log_Kernel, "No se puedo conectar al servidor de File System");
	}
}

void procesarPCB(void* args){
	Program* aProgram;

	while(true){
		aProgram = (Program*) queue_sync_pop(QUEUE_PCB);

		log_info(log_Kernel, "Nuevo proceso PCB");
		log_info(log_Kernel, "El pid del proceso es: %d", PID_PCB);

		aProgram->PID = PID_PCB;	//Asigno el PID a la consola
		list_add(LIST_CONSOLAS,aProgram);	//Almaceno el socket de la consola y el PID

		PCB_t* newPCB = PCB_new_pointer(PID_PCB, 0, 0, 0, 0, 0, 0);

		//Agrego el pcb a la lista de new
		queue_push(QUEUE_NEW, newPCB);
		//Aviso que hay un nuevo PCB
		sem_post(&SEM_READY);

		//Envio el PID a la consola
		serializar_int(aProgram->ID_Consola, PID_PCB);

		//Muestro el PID Del proceso
		//print_PCB(newPCB);

		PID_PCB++;
	}
}

void planificador(void* args){
	while(true){
		//El planificador no empieza a planificar hasta que no tenga una cpu conectada.
		sem_wait(&SEM_CPU);
		//Semaforo de multiprogramacion, detiene el ingreso de PCBs a la lista de READYs
		sem_wait(&SEM_MULTIPROGRAMACION);
		//Levanto el signal de un nuevo PCB
		sem_wait(&SEM_READY);
		//Semaforo para parar la planificacion
		sem_wait(&SEM_STOP_PLANNING);
		//Informo que ingresa un PCB al planificador
		log_info(log_Kernel,"Ingreso un PCB al panificador");
		//Sacar un PCB de la cola de NEWs
		PCB_t* element = (PCB_t*) queue_pop(QUEUE_NEW);
		//Agregar un PCB a la cola de READYs
		queue_push(QUEUE_READY, element);
		//Ejecutar proceso
		sem_post(&SEM_EXECUTE);
		//vuelvo a liberar la planificacion
		sem_post(&SEM_STOP_PLANNING);
		//Sigue un CPU conectada
		sem_post(&SEM_CPU);
	}
}

void ejecutar(void* args){
	while(true){
		//Hay procesos para ejecutar
		sem_wait(&SEM_EXECUTE);
		//Hay una CPU disponible
		sem_wait(&SEM_CPU_DISPONIBLE);
		//Busco una CPU DISPONIBLE
		CPU_t* cpu = buscar_CPU_Disponible(QUEUE_CPUS);
		//Saco un PCB de la pila de ready
		PCB_t* pcb = (PCB_t*) queue_pop(QUEUE_READY);
		//Asigno el PCB a la cpu
		AsignarPCB(cpu, pcb);
		//Envio a ejecutar el pcb
		serializar_pcb(cpu->CPU_ID, pcb);
		//Agrego la CPU asignada a la pila de CPUs
		queue_push(QUEUE_CPUS,cpu);
		//Agrego el PCB a la pila de Ejecucion
		queue_push(QUEUE_EXEC,pcb);
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
	SERVIDOR_KERNEL = build_server(config.PUERTO_KERNEL, config.CANTCONEXIONES);
	//SERVIDOR_KERNEL = build_server(5010, 10);

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
		case NUEVO_PROCESO:{
			//printf("Nuevo Programa\n");
			log_info(log_Console,"Nuevo Programa");
			t_SerialString* PATH = malloc(sizeof(t_SerialString));
			deserializar_string(socket, PATH);
			log_info(log_Kernel,"El nuevo programa ocupa %d bytes", PATH->sizeString);
			//Preguntar a memoria si hay lugar para almacenarlo
				//Si tiene lugar enviarlo
				//Si no tiene exit run
			//Libero el programa en el kernel
			free(PATH->dataString);
			free(PATH);
			//Almacenar la consola
			Program* NewProgram = Program_new(socket, 0);
			queue_sync_push(QUEUE_PCB, NewProgram);
			break;
		}
		case NUEVA_CONEXION_CPU:{
			//Nueva conexion de CPU
			log_info(log_Kernel,"Nueva CPU");
			//Nueva CPU
			CPU_t* newCPU = CPU_new(socket);
			//Envio la CPU a la pila de CPUs
			queue_push(QUEUE_CPUS, newCPU);
			//Habilito el semaforo de CPU activas para planificar.
			sem_post(&SEM_CPU);
			//Hay CPU disponible
			sem_post(&SEM_CPU_DISPONIBLE);
			break;
		}
		case FIN_EJECUCION_CPU:{
			//Finalizacion de ejecucion de rafaga de CPU
			log_info(log_Kernel,"Finalizacion de ejecucion de CPU");
			//PCB a deserializar
			PCB_t* pcbRecibido = malloc(sizeof(PCB_t));
			deserializar_pcb(socket, pcbRecibido);
			//Buscar el PCB en la cola de Exec.
			PCB_t* pcbExec = buscar_PCB(pcbRecibido->PID, QUEUE_EXEC);
			//Buscar la cpu
			CPU_t* cpu = buscar_CPU(socket, QUEUE_CPUS);
			//Libero la cpu
			LiberarCPU(cpu);
			//Agrego la cpu disponible a la pila
			queue_push(QUEUE_CPUS,cpu);
			//Libero el pcb en ejecucion.
			PCB_free(pcbExec);
			//Envio el PCB finalizado a la pila de exit
			queue_push(QUEUE_EXIT,pcbRecibido);
			//Libero el semaforo de multiprogramacion por finalizar un programa
			sem_post(&SEM_MULTIPROGRAMACION);
			//CPU lista para nuevo programa
			sem_post(&SEM_CPU_DISPONIBLE);
			break;
		}
		default:{
			printf("Error de comando\n");
			break;
		}
	}

	return;
}

void consola_kernel(void* args){
	while(true) {
		t_Consola consola = leerComandos();
		consola.kernel = SERVIDOR_KERNEL;
		if (!strcmp(consola.comando, "exit")){
			exit(0);
			//Desconectar todo y esperar la reconexion.
		}
		else if (!strcmp(consola.comando, "clean")){
			system("clear");
		}
		else if (!strcmp(consola.comando, "list")){
			list_queue(QUEUE_NEW, 1);
			list_queue(QUEUE_READY, 2);
			list_queue(QUEUE_EXEC, 3);
			list_queue(QUEUE_EXIT, 4);
		}
		else if (!strcmp(consola.comando, "list_ready")){
			list_ready(QUEUE_READY);
		}
		else if (!strcmp(consola.comando, "list_consolas")){
			list_console(LIST_CONSOLAS);
		}
		else if (!strcmp(consola.comando, "list_new")){
			list_new(QUEUE_NEW);
		}
		else if (!strcmp(consola.comando, "stop")){
			sem_wait(&SEM_STOP_PLANNING);
			log_info(log_Console,"Se detuvo la planificacion");
		}
		else if (!strcmp(consola.comando, "start")){
			sem_post(&SEM_STOP_PLANNING);
			log_info(log_Console,"Se reanudo la planificacion");
		}
		else if (!strcmp(consola.comando, "status")){
			if (consola.argumento == NULL)
				printf("Falta el argumento de la funcion %s\n", consola.comando);
			else {
				uint32_t nroProceso = atoi(consola.argumento);
				status_process(QUEUE_READY,nroProceso);
			}
		}
		else if (!strcmp(consola.comando, "kill")){
			if (consola.argumento == NULL)
				printf("Falta el argumento de la funcion %s\n", consola.comando);
			else {
				uint32_t nroProceso = atoi(consola.argumento);
				kill_process(QUEUE_READY,nroProceso);
				sem_post(&SEM_MULTIPROGRAMACION);
			}
		}
		else
			printf("Comando incorrecto. Pruebe con: exit | clean | list | stop | start | status | kill \n");
	}
}

void init_log(char* pathLog){
	mkdir("/home/utnso/Blacklist/Logs",0755);
	log_Console = log_create(pathLog, "Kernel", true, LOG_LEVEL_INFO);
	log_Kernel = log_create(pathLog, "Kernel", false, LOG_LEVEL_INFO);
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
