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
	PCB * unPCB;

	printf("Se estan ejecutando %d procesos \n", cant_procesos);

	for(elemn = 0;elemn < cant_procesos; elemn++){
		unPCB = list_get(LIST_READY,elemn);
		printf("El PID en la posicion %d es: %d \n",elemn,unPCB->PID);
		//free(unPCB);
	}
}

void kill_process(t_list * lista,uint32_t id_process){
	//matar el proceso
	PCB * unPCB = list_remove(lista, id_process);
	PCB_free(unPCB);
	//informar a la consola que se murio el proceso
}

void status_process(t_list * lista,uint32_t id_process){
	PCB *unPCB = list_get(lista, id_process);
	print_PCB(unPCB);
	PCB_free(unPCB);
}
