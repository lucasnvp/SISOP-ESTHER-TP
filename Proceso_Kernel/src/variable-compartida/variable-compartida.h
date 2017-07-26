#ifndef VARIABLE_COMPARTIDA_H_
#define VARIABLE_COMPARTIDA_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

#include <commons/collections/list.h>
#include <commons/log.h>

typedef struct VariableCompartidaType{
	char* ID;
	uint32_t valor;
} Variable_Comparitda_t;

t_list * LIST_VARIABLES_COMPARTIDAS;

Variable_Comparitda_t* variable_compartida_new(char* ID, uint32_t valor);
void variable_compartida_destroy(Variable_Comparitda_t* this);

void init_variables_compartidas(char** variablesCompartidas);

void set_variable_compartida(char* ID, uint32_t value);
uint32_t get_variable_compartida(char* ID);

void print_variables_comparitdas(t_log* log_Kernel);

#endif /* VARIABLE-COMPARTIDA_H_ */
