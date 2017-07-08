#include "ansisop_primitivas.h"

static const int CONTENIDO_VARIABLE = 20;
static const int POSICION_MEMORIA = 0x10;

bool termino = false;

t_valor_variable desreferenciar(t_puntero puntero) {
	printf("Dereferenciar %d y su valor es: %d\n", puntero, CONTENIDO_VARIABLE);
	return CONTENIDO_VARIABLE;
}

void asignar(t_puntero puntero, t_valor_variable variable) {
	printf("Asignando en %d el valor %d\n", puntero, variable);
}

t_puntero obtenerPosicionVariable(t_nombre_variable variable) {
	printf("Obtener posicion de %c\n", variable);
	return POSICION_MEMORIA;
}

void finalizarProceso() {
	termino = true;
	printf("Finalizar\n");
}

bool codigoFinalizado() {
	return termino;
}

void kernel_wait(t_nombre_semaforo sem) {

	//desarrollar

}

