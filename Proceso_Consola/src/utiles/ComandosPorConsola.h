#ifndef SRC_UTILES_COMANDOSPORCONSOLA_H_
#define SRC_UTILES_COMANDOSPORCONSOLA_H_
#include "../serializador/serializador.h"
#include <stdio.h>

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

void crearHiloConsola(t_Consola* consola) {
	//Serializo el path
	serializar_path(consola->kernel, 2, strlen(consola->argumento), consola->argumento);

	//Recibo los datos
	DatosRecibidos *buffer = deserializar_path(consola->kernel);

	//Muestro los datos
	printf("Me llegaron %d bytes con %s\n", buffer->bytesRecibidos, buffer->datos);
}

#endif /* SRC_UTILES_COMANDOSPORCONSOLA_H_ */
