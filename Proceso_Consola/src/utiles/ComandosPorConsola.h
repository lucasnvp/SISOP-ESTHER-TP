#ifndef SRC_UTILES_COMANDOSPORCONSOLA_H_
#define SRC_UTILES_COMANDOSPORCONSOLA_H_
#include "../serializador/serializador.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	int kernel;
	char* comando;
	char* argumento;
} t_Consola;

pthread_mutex_t sem_consola;

void limpiarBufferDeEntrada();
t_Consola leerComandos();
void crearHiloConsola(t_Consola* consola);

void limpiarBufferDeEntrada() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF) { }
}

t_Consola leerComandos() {
	//Mensaje
	char* mensaje = (char *) malloc(sizeof(char) * 1000);
	t_Consola consola;
	consola.kernel = 0;
	consola.comando = "";
	consola.argumento = "";
	scanf("%[^\n]s", mensaje);
	if (strcmp(mensaje, "")) {
		consola.comando = strtok(mensaje, " ");
		consola.argumento = strtok(NULL, " ");
	}
	limpiarBufferDeEntrada();
	free(mensaje);
	return consola;
}

void crearHiloConsola(t_Consola* consola) {
	t_Consola* param = consola;

	//Ejecuto el comando run en el servidor
	serializar_data(param->kernel, 1, 4, "run");
	//Le envio el path
	serializar_data(param->kernel, 2, strlen(param->argumento), param->argumento);

	//Recibo los datos
	uint32_t PID_PCB = deserializar_int(consola->kernel);

	//Muestro los datos
	pthread_mutex_lock(&sem_consola);
	printf("El PID del programa es: %d \n\n> ", PID_PCB);
	fflush(stdout);
	pthread_mutex_unlock(&sem_consola);

	pthread_exit(NULL);
}

#endif /* SRC_UTILES_COMANDOSPORCONSOLA_H_ */
