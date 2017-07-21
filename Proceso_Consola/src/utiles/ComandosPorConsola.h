#ifndef SRC_UTILES_COMANDOSPORCONSOLA_H_
#define SRC_UTILES_COMANDOSPORCONSOLA_H_
#include "../serializador/serializador.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <commons/log.h>

#define COMANDO_RUN 1

typedef struct {
	uint32_t kernel;
	char* comando;
	char* argumento;
} t_Consola;

pthread_mutex_t sem_consola;
t_log* log_Consola;

void limpiarBufferDeEntrada();
char* substr(char* cadena, uint32_t comienzo, uint32_t longitud);
void imprimirError(uint32_t codigoError);
void imprimirEstadisticas(uint32_t PID, time_t inicio, time_t fin, uint32_t cantLineas);
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
		log_error(log_Consola, "No se pudieron reservar recursos para ejecutar el programa.");
		printf("No se pudieron reservar recursos para ejecutar el programa.\n\n> ");
		break;
	case -2:
		log_error(log_Consola, "El programa intento acceder a un archivo que no existe.");
		printf("El programa intento acceder a un archivo que no existe.\n\n> ");
		break;
	case -3:
		log_error(log_Consola, "El programa intento leer un archivo sin permisos.");
		printf("El programa intento leer un archivo sin permisos.\n\n> ");
		break;
	case -4:
		log_error(log_Consola, "El programa intento escribir un archivo sin permisos.");
		printf("El programa intento escribir un archivo sin permisos.\n\n> ");
		break;
	case -5:
		log_error(log_Consola, "Excepcion de memoria.");
		printf("Excepcion de memoria.\n\n> ");
		break;
	case -6:
		log_error(log_Consola, "Finalizado a traves de desconexion de consola.");
		printf("Finalizado a traves de desconexion de consola.\n\n> ");
		break;
	case -7:
		log_error(log_Consola, "Finalizado a traves del comando Finalizar Programa de la consola.");
		printf("Finalizado a traves del comando Finalizar Programa de la consola.\n\n> ");
		break;
	case -8:
		log_error(log_Consola, "Se intento reservar mas memoria que el tamaño de una pagina.");
		printf("Se intento reservar mas memoria que el tamaño de una pagina\n\n> ");
		break;
	case -9:
		log_error(log_Consola, "No se pueden asignar mas paginas al proceso.");
		printf("No se pueden asignar mas paginas al proceso\n\n> ");
		break;
	case -10:
		log_error(log_Consola, "Finalizado a traves del comando Finalizar Programa del kernel.");
		printf("Finalizado a traves del comando Finalizar Programa del kernel.\n\n> ");
		break;
	case -20:
		log_error(log_Consola, "Error sin definicion.");
		printf("Error sin definicion\n\n> ");
		break;
	}
	fflush(stdout);

	pthread_mutex_unlock(&sem_consola);
}

void imprimirEstadisticas(uint32_t PID, time_t inicio, time_t fin, uint32_t cantLineas) {
	char* diferencia = (char *) malloc(sizeof(char) * 128);
	diferencia = calcularDiferencia(inicio, fin);

	pthread_mutex_lock(&sem_consola);
	printf("\n*********************** ESTADISTICAS %i ***********************\n", PID);
	log_info(log_Consola, "*********************** ESTADISTICAS %i ***********************", PID);

	struct tm *tlocali = localtime(&inicio);
	char* strInicio = (char *) malloc(sizeof(char) * 128);
	strftime(strInicio, 128, "%d/%m/%y %H:%M:%S", tlocali);

	printf("Fecha y hora de inicio de ejecucion: %s\n", strInicio);
	log_info(log_Consola, "Fecha y hora de inicio de ejecucion: %s", strInicio);

	struct tm *tlocalf = localtime(&fin);
	char* strFin = (char *) malloc(sizeof(char) * 128);
	strftime(strFin, 128, "%d/%m/%y %H:%M:%S", tlocalf);

	printf("Fecha y hora de fin de ejecucion: %s\n", strFin);
	log_info(log_Consola, "Fecha y hora de fin de ejecucion: %s", strFin);

	printf("Cantidad de impresiones por pantalla: %i\n", cantLineas);
	printf("Tiempo total de ejecucion: %s\n", diferencia);
	printf("*****************************************************************\n\n> ");
	log_info(log_Consola, "Cantidad de impresiones por pantalla: %i", cantLineas);
	log_info(log_Consola, "Tiempo total de ejecucion: %s", diferencia);
	log_info(log_Consola, "*****************************************************************");

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
	strcpy(mensaje, "");
	scanf("%[^\n]s", mensaje);
	if (strcmp(mensaje, "")) {
		log_info(log_Consola, "Leida linea de comandos del usuario: %s.", mensaje);
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
		char* aux = (char *) malloc(sizeof(char) * strlen(param->argumento) + 3);
		strcpy(aux, "./");
		strcat(aux, param->argumento);
		strcpy(param->argumento, aux);
		free(aux);
	}

	log_info(log_Consola, "Archivo a procesar: %s.", param->argumento);
	FILE *archivo = fopen(param->argumento, "r");

	if (!archivo) {
		perror("Error");
		printf("No se pudo abrir el archivo %s\n\n> ", param->argumento);
		log_error(log_Consola, "No se pudo abrir el archivo %s.", param->argumento);
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
	serializar_int(param->kernel, COMANDO_RUN);

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
		printf("El PID del programa es: %d\n\n> ", PID_PCB);
		log_info(log_Consola, "El PID del programa es: %d.", PID_PCB);
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
						t_SerialString* lineaDatos = (t_SerialString *) malloc(sizeof(t_SerialString));
						deserializar_string(param->kernel, lineaDatos);
						pthread_mutex_lock(&sem_consola);
						printf("\n***** PID %i (%s) *****\n", PID_Actual, param->argumento);
						printf("%s\n\n> ", lineaDatos->dataString);
						log_info(log_Consola, "***** PID %i (%s) *****", PID_Actual, param->argumento);
						log_info(log_Consola, "%s", lineaDatos->dataString);
						fflush(stdout);
						pthread_mutex_unlock(&sem_consola);
						contadorLineas++;
						free(lineaDatos);
					}
					else if (Codigo_ID == 2) {
						//Finalizar programa
						tiempoFin = time(0);
						imprimirEstadisticas(PID_Actual, tiempoInicio, tiempoFin, contadorLineas);
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
