#include "programa.h"

Program Program_new(int32_t ID_Consola, char* Path){
	Program aProgram;
	aProgram.ID_Consola = ID_Consola;
	aProgram.Path = Path;
	return aProgram;
}

void Program_free(Program *this){
	free(this);
}

void print_Program(Program auxProgram){
	printf("PID: %d\n",auxProgram.ID_Consola);
	printf("ProgramCounter: %s\n",auxProgram.Path);
}
