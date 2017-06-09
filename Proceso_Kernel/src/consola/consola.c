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
	free(mensaje);
	return consola;
}

void limpiarBufferDeEntrada() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF) { }
}

void list_process(t_list * LIST_READY){
	uint32_t elemn;
	uint32_t cant_procesos = list_size(LIST_READY);
	PCB_t* unPCB;

	printf("Hay %d procesos en ready \n", cant_procesos);

	for(elemn = 0;elemn < cant_procesos; elemn++){
		unPCB = list_get(LIST_READY,elemn);
		printf("El PID en la posicion %d es: %d \n",elemn,unPCB->PID);
	}
}

void list_console(t_list * LIST_CONSOLE){
	uint32_t elemn;
	uint32_t cant_programs = list_size(LIST_CONSOLE);
	Program* aProgram;

	printf("Hay %d programas en las consolas \n", cant_programs);

	for(elemn = 0;elemn < cant_programs; elemn++){
		aProgram = list_get(LIST_CONSOLE,elemn);
		printf("En la posicion %d: Consola: %d , PID: %d \n",elemn,aProgram->ID_Consola,aProgram->PID);
	}
}

void list_new(t_queue * QUEUE_NEW){
	uint32_t elemn;
	uint32_t cant_new = queue_size(QUEUE_NEW);
	t_queue * QUEUE_AUX = queue_create();
	QUEUE_AUX = QUEUE_NEW;
	PCB_t* unPCB;

	printf("Hay %d procesos en new \n", cant_new);

	for(elemn = 0;elemn < cant_new; elemn++){
		unPCB = (PCB_t*) queue_pop(QUEUE_AUX);
		printf("El PID en la posicion %d es: %d \n",elemn,unPCB->PID);
	}
	//queue_destroy(QUEUE_AUX);
}

void kill_process(t_list * lista,uint32_t id_process){
	//matar el proceso
	PCB_t* unPCB = list_remove(lista, id_process);
	PCB_free(unPCB);
	//informar a la consola que se murio el proceso
}

void status_process(t_list * lista,uint32_t id_process){
	PCB_t* unPCB = list_get(lista, id_process);
	print_PCB(unPCB);
	PCB_free(unPCB);
}
