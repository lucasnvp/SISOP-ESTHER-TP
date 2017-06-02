#include "programa.h"

Program* Program_new(uint32_t ID_Consola, uint32_t PID){
	Program* aProgram = malloc(sizeof(Program));
	aProgram->ID_Consola = ID_Consola;
	aProgram->PID = PID;
	return aProgram;
}

void Program_free(Program *this){
	free(this);
}

void print_Program(Program auxProgram){
	printf("El socket de la consola es: %d\n",auxProgram.ID_Consola);
	printf("El PID asociado es: %d\n",auxProgram.PID);
}
