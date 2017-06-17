#include "ansisop_primitivas.h"

bool termino = false;

void finalizarProceso(void) {
	termino = true;
	printf("Finalizar\n");
}

bool codigoFinalizado() {
	return termino;
}

void kernel_wait(t_nombre_semaforo sem) {

}

