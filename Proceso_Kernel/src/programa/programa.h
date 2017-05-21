#ifndef PROGRAMA_H_
#define PROGRAMA_H_

#include <inttypes.h>

typedef struct ProgramType{
	int32_t ID_Consola;
	char* Path;
} Program;

Program Program_new(int32_t ID_Consola, char* Path);
void Program_free(Program *this);
void print_Program(Program auxProgram);

#endif /* PROGRAMA_H_ */
