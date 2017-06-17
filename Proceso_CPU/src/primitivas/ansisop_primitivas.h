#ifndef SRC_PRIMITIVAS_ANSISOP_PRIMITIVAS_H_
#define SRC_PRIMITIVAS_ANSISOP_PRIMITIVAS_H_

#include <parser/parser.h>

void finalizarProceso(); //funcion ansisop del kernel para finalizar el proceso
bool codigoFinalizado(); //funcion para recorrer el codigo recibido del pcb
void kernel_wait(t_nombre_semaforo sem);

#endif /* SRC_PRIMITIVAS_ANSISOP_PRIMITIVAS_H_ */
