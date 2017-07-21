#include "programa.h"

Program* Program_new(uint32_t ID_Consola, uint32_t PID, char* InfoPath){
	Program* aProgram = malloc(sizeof(Program));
	aProgram->ID_Consola = ID_Consola;
	aProgram->PID = PID;
	aProgram->InfoPath = strdup(InfoPath);
	return aProgram;
}

void Program_free(Program *this){
	free(this);
}

void InfoPath_free(Program *this){
	free(this->InfoPath);
}

void print_Program(Program auxProgram){
	printf("El socket de la consola es: %d\n",auxProgram.ID_Consola);
	printf("El PID asociado es: %d\n",auxProgram.PID);
	printf("La informacion del path es: %s\n", auxProgram.InfoPath);
}

Program* Search_Program_By_PID(t_list * LIST_CONSOLAS, uint32_t PID){
	int equal_pid(Program* program, uint32_t PID){
		return (program->PID == PID);
	}

	return list_find(LIST_CONSOLAS, (void*) equal_pid);
}
