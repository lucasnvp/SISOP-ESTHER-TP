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
char* substr(char* cadena, uint32_t comienzo, uint32_t longitud);
void imprimirError(uint32_t codigoError);
void imprimirEstadisticas(time_t inicio, time_t fin, uint32_t cantLineas);
char* calcularDiferencia(time_t inicio, time_t fin);
t_Consola* leerComandos();
void crearHiloConsola(t_Consola* consola);

void limpiarBufferDeEntrada() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {
	}
}

char* substr(char* cadena, uint32_t comienzo, uint32_t longitud) {
	char *nuevo = (char*) malloc(sizeof(char) * (longitud + 1));
	nuevo[longitud] = '\0';
	strncpy(nuevo, cadena + comienzo, longitud);

	return nuevo;
}

void imprimirError(uint32_t codigoError) {
	pthread_mutex_lock(&sem_consola);

	switch (codigoError) {
	case -1:
		printf(
				"No se pudieron reservar recursos para ejecutar el programa.\n\n> ");
		break;
	case -2:
		printf("El programa intento acceder a un archivo que no existe.\n\n> ");
		break;
	case -3:
		printf("El programa intento leer un archivo sin permisos.\n\n> ");
		break;
	case -4:
		printf("El programa intento escribir un archivo sin permisos.\n\n> ");
		break;
	case -5:
		printf("Excepcion de memoria.\n\n> ");
		break;
	case -6:
		printf("Finalizado a traves de desconexion de consola.\n\n> ");
		break;
	case -7:
		printf(
				"Finalizado a traves del comando Finalizar Programa de la consola.\n\n> ");
		break;
	case -8:
		printf(
				"Se intento reservar mas memoria que el tamaño de una pagina\n\n> ");
		break;
	case -9:
		printf("No se pueden asignar mas paginas al proceso\n\n> ");
		break;
	case -20:
		printf("Error sin definicion\n\n> ");
		break;
	}
	fflush(stdout);

	pthread_mutex_unlock(&sem_consola);
}

void imprimirEstadisticas(time_t inicio, time_t fin, uint32_t cantLineas) {
	char* diferencia = (char *) malloc(sizeof(char) * 128);
	diferencia = calcularDiferencia(inicio, fin);

	pthread_mutex_lock(&sem_consola);
	printf("\n*********************** ESTADISTICAS ***********************\n");

	struct tm *tlocali = localtime(&inicio);
	char* strInicio = (char *) malloc(sizeof(char) * 128);
	strftime(strInicio, 128, "%d/%m/%y %H:%M:%S", tlocali);

	printf("Fecha y hora de inicio de ejecucion: %s\n", strInicio);

	struct tm *tlocalf = localtime(&fin);
	char* strFin = (char *) malloc(sizeof(char) * 128);
	strftime(strFin, 128, "%d/%m/%y %H:%M:%S", tlocalf);

	printf("Fecha y hora de fin de ejecucion: %s\n", strFin);

	printf("Cantidad de impresiones por pantalla: %i\n", cantLineas);
	printf("Tiempo total de ejecucion: %s\n", diferencia);
	printf("************************************************************\n\n> ");

	fflush(stdout);
	pthread_mutex_unlock(&sem_consola);

	free(diferencia);
	free(strInicio);
	free(strFin);
}

char* calcularDiferencia(time_t inicio, time_t fin) {
	uint32_t diferencia = (uint32_t) difftime(fin, inicio);
	uint32_t horas = (diferencia / 60) / 60;
	uint32_t minutos = (diferencia / 60) % 60;
	uint32_t segundos = diferencia % 60;

	char* strRetorno = (char *) malloc(sizeof(char) * 128);
	strcpy(strRetorno, "");

	if (horas > 0) {
		char* strHoras = (char *) malloc(sizeof(char) * 3);
		sprintf(strHoras, "%d", horas);
		strcat(strRetorno, strHoras);
		free(strHoras);
		if (horas > 1)
			strcat(strRetorno, " horas ");
		else
			strcat(strRetorno, " hora ");
	}

	if (minutos > 0) {
		char* strMinutos = (char *) malloc(sizeof(char) * 3);
		sprintf(strMinutos, "%d", minutos);
		strcat(strRetorno, strMinutos);
		free(strMinutos);
		if (minutos > 1)
			strcat(strRetorno, " minutos ");
		else
			strcat(strRetorno, " minuto ");
	}

	if (segundos > 0) {
		char* strSegundos = (char *) malloc(sizeof(char) * 3);
		sprintf(strSegundos, "%d", segundos);
		strcat(strRetorno, strSegundos);
		free(strSegundos);
		if (segundos > 1)
			strcat(strRetorno, " segundos");
		else
			strcat(strRetorno, " segundo");
	}

	return strRetorno;
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
	uint32_t PID_Actual;
	uint32_t Codigo_ID;

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

	if (PID_PCB > 0) {
		//Muestro los datos
		pthread_mutex_lock(&sem_consola);
		printf("El PID del programa es: %d \n\n> ", PID_PCB);
		fflush(stdout);
		pthread_mutex_unlock(&sem_consola);

		bool salidaBuclePpal = false;
		uint32_t contadorLineas = 0;
		time_t tiempoInicio = time(0);
		time_t tiempoFin;

		while(!salidaBuclePpal) {
			//Recibo datos
			PID_Actual = deserializar_int(param->kernel);

			if (PID_Actual == PID_PCB) {
				Codigo_ID = deserializar_int(param->kernel);

				if (Codigo_ID > 0) {
					if (Codigo_ID == 1) {
						//Imprimir texto en pantalla
						pthread_mutex_lock(&sem_consola);
						//TODO: Logica de impresion
						pthread_mutex_unlock(&sem_consola);
						contadorLineas++;
					}
					else if (Codigo_ID == 2) {
						//Finalizar programa
						tiempoFin = time(0);
						imprimirEstadisticas(tiempoInicio, tiempoFin, contadorLineas);
						salidaBuclePpal = true;
					}
				}
				else {
					imprimirError(PID_PCB);
					salidaBuclePpal = true;
				}
			}
		}
	} else
		imprimirError(PID_PCB);

	pthread_exit(NULL);
}

#endif /* SRC_UTILES_COMANDOSPORCONSOLA_H_ */
