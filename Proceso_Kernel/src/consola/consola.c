#include "consola.h"

t_Consola leerComandos() {
	//Mensaje
	char* mensaje = (char *) malloc(sizeof(char) * 256);
	t_Consola consola;
	consola.kernel = 0;
	consola.comando = "";
	consola.argumento = "";
	//printf("> ");
	scanf("%[^\n]s", mensaje);
	if (strcmp(mensaje, "")) {
		consola.comando = strtok(mensaje, " ");
		consola.argumento = strtok(NULL, " ");
	}
	limpiarBufferDeEntrada();
	//free(mensaje);
	return consola;
}

void limpiarBufferDeEntrada() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF) { }
}

void list_console(t_list * LIST_CONSOLE){
	uint32_t elemn;
	uint32_t cant_programs = list_size(LIST_CONSOLE);
	Program* aProgram;

	printf("Hay %d programas en consolas \n", cant_programs);

	for(elemn = 0;elemn < cant_programs; elemn++){
		aProgram = list_get(LIST_CONSOLE,elemn);
		printf("En la posicion %d: Consola: %d , PID: %d \n",elemn,aProgram->ID_Consola,aProgram->PID);
	}
}

void list_queue(t_queue * QUEUE, uint32_t mensaje){
	uint32_t elemn;
	uint32_t cant = queue_size(QUEUE);
	PCB_t* unPCB;

	switch(mensaje){
		case 1:{
			printf("Hay %d procesos en new \n", cant);
			break;
		}
		case 2:{
			printf("Hay %d procesos en ready \n", cant);
			break;
		}
		case 3:{
			printf("Hay %d procesos en ejecucion \n", cant);
			break;
		}
		case 4:{
			printf("Hay %d procesos en exit \n", cant);
			break;
		}
		default:{
			printf("Error de comando\n");
			break;
		}
	}

	for(elemn = 0;elemn < cant; elemn++){
		unPCB = (PCB_t*) queue_pop(QUEUE);
		printf("El PID en la posicion %d es: %d \n",elemn,unPCB->PID);
		queue_push(QUEUE, unPCB);
	}
}

void list_new(t_queue * QUEUE_NEW){
	uint32_t elemn;
	uint32_t cant_new = queue_size(QUEUE_NEW);
	PCB_t* unPCB;

	printf("Hay %d procesos en new \n", cant_new);

	for(elemn = 0;elemn < cant_new; elemn++){
		unPCB = (PCB_t*) queue_pop(QUEUE_NEW);
		printf("El PID en la posicion %d es: %d \n",elemn,unPCB->PID);
		queue_push(QUEUE_NEW, unPCB);
	}
}

void list_ready(t_queue * QUEUE_READY){
	uint32_t elemn;
	uint32_t cant = queue_size(QUEUE_READY);
	PCB_t* unPCB;

	printf("Hay %d procesos en ready \n", cant);

	for(elemn = 0;elemn < cant; elemn++){
		unPCB = (PCB_t*) queue_pop(QUEUE_READY);
		printf("El PID en la posicion %d es: %d \n",elemn,unPCB->PID);
		queue_push(QUEUE_READY, unPCB);
	}
}

void kill_process(t_queue * queue,uint32_t pid){
	uint32_t elemn;
	uint32_t cant = queue_size(queue);
	PCB_t* unPCB;
	for(elemn = 0;elemn < cant; elemn++){
		unPCB = (PCB_t*) queue_pop(queue);
		if(unPCB->PID != pid){
			queue_push(queue, unPCB);
		}
	}
	//informar a la consola que se murio el proceso
	//Buscar la consola asociada al PID
//	serializar_int(consola,unPCB->PID);
//	serializar_int(consola,-10);
}

void status_process(t_queue * queue,uint32_t pid){
	uint32_t elemn;
	uint32_t cant = queue_size(queue);
	PCB_t* unPCB;
	for(elemn = 0;elemn < cant; elemn++){
		unPCB = (PCB_t*) queue_pop(queue);
		if(unPCB->PID == pid){
			print_PCB(unPCB);
		}
		queue_push(queue, unPCB);
	}
}
