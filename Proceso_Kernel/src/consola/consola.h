#ifndef SRC_UTILES_COMANDOSPORCONSOLA_H_
#define SRC_UTILES_COMANDOSPORCONSOLA_H_

#include <stdio.h>
#include <inttypes.h>

typedef struct {
	uint32_t kernel;
	char* comando;
	char* argumento;
} t_Consola;

t_Consola leerComandos();
void limpiarBufferDeEntrada();

#endif /* SRC_UTILES_COMANDOSPORCONSOLA_H_ */
