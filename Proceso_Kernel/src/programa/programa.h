#ifndef PROGRAMA_H_
#define PROGRAMA_H_

#include <inttypes.h>
#include <string.h>

#include <commons/collections/list.h>

#include <commons/collections/list.h>

typedef struct ProgramType{
	uint32_t ID_Consola;
	uint32_t PID;
	char* InfoPath;
} Program;

Program* Program_new(uint32_t ID_Consola, uint32_t PID, char* InfoPath);
Program* Search_Program_By_PID(t_list * LIST_CONSOLAS, uint32_t PID);
void Program_free(Program *this);
void InfoPath_free(Program *this);
void print_Program(Program auxProgram);

#endif /* PROGRAMA_H_ */
