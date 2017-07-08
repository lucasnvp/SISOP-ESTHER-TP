#include "Proceso CPU.h"

AnSISOP_funciones functions = { .AnSISOP_definirVariable = ansi_definirVariable,
		.AnSISOP_obtenerPosicionVariable = obtenerPosicionVariable,
		.AnSISOP_dereferenciar = desreferenciar, .AnSISOP_asignar = asignar,
		.AnSISOP_finalizar = finalizarProceso,

};

AnSISOP_kernel kernel_functions = {

.AnSISOP_wait = kernel_wait };  //Syscalls

static const char* PROGRAMA = "begin\n"
		"variables a, b\n";


int main(void) {

	programa = strdup(PROGRAMA); //copia el programa entero en esa variable, lo hace el Kernel, despues sacarlo

	pcbActivo = PCB_new_pointer(1, 3, metadata_desde_literal(programa));

	tamanio_pagina = 256;

	//Leemos configuracion
	config = load_config(PATH_CONFIG);
	print_config(config);

	//Inicializar Log
	init_log(PATH_LOG);
	log_info(log_Console, "Nueva CPU");

	//Conexion al kernel
	connect_server_kernel();

	//Conexion a memoria
	//connect_server_memoria();

	while (true) {

		//Quedo a la espera de recibir un PCB del Kernel
		//deserializar_pcb(kernel, pcbActivo);

		//Proceso de ejecucion de Primitivas Ansisop
		ejecutar();

		print_PCB(pcbActivo);

		//Envio el mensaje al kernel de que finalizo la rafaga correctamente
		//serializar_int(kernel, FIN_CORRECTO);

		//Envio a kernel PCB actualizado
		//serializar_pcb(kernel, pcbActivo);

	}

	return EXIT_SUCCESS;

}

void ejecutar() {

	printf("Ejecutando en CPU\n");

	printf("Tamanio de pagina %d\n", tamanio_pagina);

	while (!codigoFinalizado()) {

		uint32_t pc = pcbActivo->ProgramCounter;

		t_metadata_program * ctp = pcbActivo->CodeTagsPointer;

		//instancio para utilizar
		t_puntero_instruccion start = ctp->instrucciones_serializado[pc].start;
		//instancio para utilizar
		t_size offset = ctp->instrucciones_serializado[pc].offset; //que me devuelva la siguiente linea la memoria

		//solicito a memoria la instruccion.
		char* const instruccion = solicitarInstruccionAMemoria(PROGRAMA, start,
				offset); //falta la pagina en donde esta la instruccion.

		analizadorLinea(instruccion, &functions, &kernel_functions); //ejecuta las primitivas

		free(instruccion);

		pcbActivo->ProgramCounter++;

	}
}

char* const solicitarInstruccionAMemoria(char* prgrama,
		t_puntero_instruccion offset, t_size size) {

	//Serializo la estructura para enviar a Memoria

	//Quedo a la espera de deserializar la instruccion de Memoria.
	char *aRetornar = calloc(1, 100);
	memcpy(aRetornar, prgrama + offset, size);
	return aRetornar;

}

void connect_server_kernel() {
//Conexion al kernel
	kernel = connect_server(config.IP_KERNEL, config.PUERTO_KERNEL);
	if (kernel > 0) {
		printf("Kernel conectado, Estoy escuchando\n");
		serializar_int(kernel, HANDSHAKE_CPU_KERNEL);

	}
}

void connect_server_memoria() {
//Conexion a memoria
	memoria = connect_server(config.IP_MEMORIA, config.PUERTO_MEMORIA);
	if (memoria > 0) {
		printf("Memoria Conectada\n");
		serializar_int(memoria, HANDSHAKE_CPU_MEMORIA);
		tamanio_pagina = deserializar_int(memoria);

	}
}

void connection_handler(uint32_t socket, uint32_t command) {

	switch (command) {
	case 1: {
		break;
	}
	default: {
		printf("Error de comando\n");
		break;
	}
	}

	return;
}

void init_log(char* pathLog) {
	mkdir("/home/utnso/Blacklist/Logs", 0755);
	log_Console = log_create(pathLog, "CPU", true, LOG_LEVEL_INFO);
	log_CPU = log_create(pathLog, "CPU", false, LOG_LEVEL_INFO);
}

t_puntero ansi_definirVariable(t_nombre_variable identificador_variable) {

	//printf("Ya tiene la ultima variable de la linea");

	printf("Se define la variable %c\n", identificador_variable);

	//Le pido al PCB la cantidad de paginas que tiene el codigo y le sumo uno que es la siguiente pagina.
	uint32_t primerPagina_Stack = pcbActivo->PageCode + 1;

	t_puntero posicion_Variable;

	//Si el registro que tiene el stack es nulo creo el primer registro.
	if (queue_is_empty(pcbActivo->StackPointer)) {

		//Creo la nueva variable para cargarla en el primer registro
		VARIABLE_T *nueva_var = variable_new(identificador_variable,
				primerPagina_Stack, 0, sizeof(int));

		//Creo una lista de variables temporal para agregar a la linea del stack.
		t_queue *variablesTemp = queue_create();

		//Agrego la variable creada previamente
		queue_push(variablesTemp, nueva_var);

		//Creo la linea de stack pasandole como parametro la lista creada previamente
		STACKPOINTER_T* lineaSP = stack_new(NULL, variablesTemp, NULL, NULL);

		//Agrego al stack la linea creada recientemente
		queue_push(pcbActivo->StackPointer, lineaSP);

		//Calculo la posicion es la pagina de la ultima variable por su tamanio + el offset de donde comienza la variable.
		posicion_Variable = (nueva_var->pagina * tamanio_pagina)
				+ nueva_var->offset;

		//Si no estaba vacio le pido a la ultima linea, su ultima variable y trabajo con ella
	} else {

		//Obtengo la ultima linea de stack
		STACKPOINTER_T *lineaSP = queue_pop(pcbActivo->StackPointer);

		//Le pido a la ultima linea la ultima variable que tiene
		VARIABLE_T *ultimaVariable = queue_pop(lineaSP->Variables);

		//Calculo el espacio ocupado sumando el comienzo + el tamanio del ultimo valor cargado.
		int espacioOcupado = ultimaVariable->offset + ultimaVariable->size;

		//Calculo e espacio disponible restando al tamanio de la pagina el espacio ocupado.
		int espacioDisponiblePagina = tamanio_pagina - espacioOcupado;

		if (espacioDisponiblePagina >= sizeof(int)) {

			//Si el espacio disponible es mayor al tamano de lo que quiero guardar.

			//Creo la nueva variable para cargarla en la ultima linea insertada en la misma pagina
			VARIABLE_T *nueva_var = variable_new(identificador_variable,
					ultimaVariable->pagina, espacioOcupado, sizeof(int));

			//Calculo la posicion es la pagina de la ultima variable por su tamanio + el offset de donde comienza la variable.
			posicion_Variable = (nueva_var->pagina * tamanio_pagina)
					+ nueva_var->offset;

		} else {

			//Si el espacio disponible no es mayor al tamanio de lo que quiero guardar, lo pongo en la pagina siguiente.

			//Creo la nueva variable para cargarla en la ultima linea insertada en la siguiente pagina
			VARIABLE_T *nueva_var = variable_new(identificador_variable,
					ultimaVariable->pagina + 1, espacioOcupado, sizeof(int));

			//Agrego a las variables de la ultima linea la nueva variable
			queue_push(lineaSP->Variables, nueva_var);

			//Calculo la posicion es la pagina de la ultima variable por su tamanio + el offset de donde comienza la variable.
			posicion_Variable = (nueva_var->pagina * tamanio_pagina)
					+ nueva_var->offset;

		}

	}

	return posicion_Variable;
}

