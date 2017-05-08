#ifndef SRC_UTILES_COMANDOSPORCONSOLA_H_
#define SRC_UTILES_COMANDOSPORCONSOLA_H_

typedef struct {
	char* comando;
	char* argumento;
} t_Consola;

void limpiarBufferDeEntrada();
t_Consola leerComandos();

void limpiarBufferDeEntrada() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF) { }
}

t_Consola leerComandos() {
	//Mensaje
	char* mensaje = (char *) malloc(sizeof(char) * 1000);
	t_Consola consola;
	printf("> ");
	scanf("%[^\n]s", mensaje);
	consola.comando = strtok(mensaje, " ");
	consola.argumento = strtok(NULL, " ");
	limpiarBufferDeEntrada();
	free(mensaje);
	return consola;
}

#endif /* SRC_UTILES_COMANDOSPORCONSOLA_H_ */
