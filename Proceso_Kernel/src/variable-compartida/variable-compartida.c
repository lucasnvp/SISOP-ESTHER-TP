#include "variable-compartida.h"

Variable_Comparitda_t* variable_compartida_new(char* ID, uint32_t valor){
	Variable_Comparitda_t* varShare = malloc(sizeof(Variable_Comparitda_t));
	varShare->ID = strdup(ID);
	varShare->valor = valor;
	return varShare;
}

void variable_compartida_destroy(Variable_Comparitda_t* this){
	free(this->ID);
	free(this);
}

void init_variables_compartidas(char** variablesCompartidas){
	LIST_VARIABLES_COMPARTIDAS = list_create();
	int i;
	for(i=0 ; variablesCompartidas[i] != NULL ; i++){
		Variable_Comparitda_t* varShare = variable_compartida_new(variablesCompartidas[i], 0);
		list_add(LIST_VARIABLES_COMPARTIDAS, varShare);
	}
}

void print_variables_comparitdas(t_log* log_Kernel){
	log_info(log_Kernel, "Init Variables Compartidas");
	void print_element_sharevar(void* element){
		Variable_Comparitda_t* varShare = element;
		log_info(log_Kernel, "ID: %s", varShare->ID);
		log_info(log_Kernel, "Valor: %d", varShare->valor);
	}

	list_iterate(LIST_VARIABLES_COMPARTIDAS, print_element_sharevar);
}

void set_variable_compartida(char* ID, uint32_t value){
	bool remove_element_var(void* element){
		Variable_Comparitda_t* var = element;
		return strcmp(ID, var->ID) == 0;
	}

	Variable_Comparitda_t* varShare = list_remove_by_condition(LIST_VARIABLES_COMPARTIDAS, remove_element_var);
	varShare->valor = value;
	list_add(LIST_VARIABLES_COMPARTIDAS, varShare);
}

uint32_t get_variable_compartida(char* ID){
	bool find_element_var(void* element){
		Variable_Comparitda_t* var = element;
		return strcmp(ID, var->ID) == 0;
	}

	Variable_Comparitda_t* varShare = list_find(LIST_VARIABLES_COMPARTIDAS, find_element_var);
	return varShare->valor;
}
