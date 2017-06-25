#ifndef SRC_UTILES_COMANDOSPORCONSOLA_H_
#define SRC_UTILES_COMANDOSPORCONSOLA_H_
#include "../serializador/serializador.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	uint32_t kernel;
	char* comando;
	char* argumento;
} t_Consola;

pthread_mutex_t sem_consola;

void limpiarBufferDeEntrada();
char* substr(char* cadena, int comienzo, int longitud);
t_Consola* leerComandos();
void crearHiloConsola(t_Consola* consola);

void limpiarBufferDeEntrada() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {
	}
}

char* substr(char* cadena, int comienzo, int longitud) {
	char *nuevo = (char*) malloc(sizeof(char) * (longitud + 1));
	nuevo[longitud] = '\0';
	strncpy(nuevo, cadena + comienzo, longitud);

	return nuevo;
}

t_Consola* leerComandos() {
	//Mensaje
	char* mensaje = (char *) malloc(sizeof(char) * 2000);
	t_Consola* consola = (t_Consola *) malloc(sizeof(t_Consola));
	consola->kernel = 0;
	consola->comando = "";
	consola->argumento = "";
	scanf("%[^\n]s", mensaje);
	if (strcmp(mensaje, "")) {
		consola->comando = strtok(mensaje, " ");
		consola->argumento = strtok(NULL, " ");
	}
	limpiarBufferDeEntrada();
	//free(mensaje);
	return consola;
}

void crearHiloConsola(t_Consola* consola) {
	t_Consola* param = consola;

	if (strcmp(substr(param->argumento, 0, 1), "/")
			&& strcmp(substr(param->argumento, 0, 1), ".")) {
		//Archivo en directorio local
		char* aux = (char *) malloc(
				sizeof(char) * strlen(param->argumento) + 3);
		strcpy(aux, "./");
		strcat(aux, param->argumento);
		strcpy(param->argumento, aux);
		free(aux);
	}

	FILE *archivo = fopen(param->argumento, "r");

	if (!archivo) {
		perror("Error");
		printf("No se pudo abrir el archivo %s\n\n> ", param->argumento);
		fflush(stdout);
		pthread_exit(NULL);
	}

	char* caracteres = (char *) malloc(sizeof(char) * 1000);
	char* contenido = (char *) malloc(sizeof(char) * 10000);

	while (!feof(archivo)) {
		strcpy(caracteres, "");
		fgets(caracteres, 1000, archivo);
		if (strlen(contenido))
			strcat(contenido, caracteres);
		else
			strcpy(contenido, caracteres);
	}

	free(caracteres);
	fclose(archivo);

	//Ejecuto el comando run en el servidor
	serializar_int(param->kernel, 1);

	//Le envio el archivo completo
	t_SerialString* path = malloc(sizeof(t_SerialString));
	path->sizeString = strlen(contenido) - 1;
	path->dataString = malloc(path->sizeString);
	path->dataString = contenido;
	serializar_string(param->kernel, path);
	free(contenido);

	//Recibo los datos
	uint32_t PID_PCB = deserializar_int(param->kernel);

	//Muestro los datos
	pthread_mutex_lock(&sem_consola);
	printf("El PID del programa es: %d \n\n> ", PID_PCB);
	fflush(stdout);
	pthread_mutex_unlock(&sem_consola);

	pthread_exit(NULL);
}

#endif /* SRC_UTILES_COMANDOSPORCONSOLA_H_ */
