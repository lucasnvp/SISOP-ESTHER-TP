#ifndef PROGRAMA_H_
#define PROGRAMA_H_

#include <inttypes.h>

typedef struct ProgramType{
	uint32_t ID_Consola;
	uint32_t PID;
} Program;

Program* Program_new(uint32_t ID_Consola, uint32_t PID);
void Program_free(Program *this);
void print_Program(Program auxProgram);

#endif /* PROGRAMA_H_ */
