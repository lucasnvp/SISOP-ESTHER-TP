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

	DatosRecibidos *buffer;
	pthread_mutex_t sem_consola;
	pthread_mutex_init(&sem_consola, NULL);

	//Serializo el path
	serializar_path(param->kernel, 2, strlen(param->argumento), param->argumento);

	//while(1) {
		//Recibo los datos
		buffer = deserializar_path(consola->kernel);

		//Muestro los datos
		pthread_mutex_lock(&sem_consola);
		printf("Me llegaron %d bytes con %s\n\n> ", buffer->bytesRecibidos, buffer->datos);
		fflush(stdout);
		pthread_mutex_unlock(&sem_consola);
	//}
	pthread_exit(NULL);
}

#endif /* SRC_UTILES_COMANDOSPORCONSOLA_H_ */