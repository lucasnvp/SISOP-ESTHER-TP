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

	pcbActivo = PCB_new_pointer(1, 1, metadata_desde_literal(programa));

	//Leemos configuracion
	config = load_config(PATH_CONFIG);
	print_config(config);

	//Inicializar Log
	init_log(PATH_LOG);
	log_info(log_Console, "Nueva CPU\n");

	//Conexion al kernel
	connect_server_kernel();

	//Conexion a memoria
	connect_server_memoria();

	while (true) {

	//Quedo a la espera de recibir un PCB del Kernel
	deserializar_pcb(kernel, pcbActivo);

	log_info(log_Console, "PCB Activo\n");

	print_PCB(pcbActivo);

	//Proceso de ejecucion de Primitivas Ansisop
	ejecutar();

	print_PCB(pcbActivo);

	//Envio el mensaje al kernel de que finalizo la rafaga correctamente
	serializar_int(kernel, FIN_CORRECTO);

	//Envio a kernel PCB actualizado
	serializar_pcb(kernel, pcbActivo);

	}

	return EXIT_SUCCESS;

}

void ejecutar() {

	log_info(log_Console, "Ejecutando en CPU\n");

	log_info(log_Console, "Tamanio de pagina %d\n", tamanio_pagina);

	//while (!codigoFinalizado()) { Lo ejecuto una vez porque la sentencia esta hardcodeada

	uint32_t pc = pcbActivo->ProgramCounter;

	t_metadata_program * ctp = pcbActivo->CodeTagsPointer;

	//Instancio para utilizar
	t_puntero_instruccion start =
			pcbActivo->CodeTagsPointer->instrucciones_serializado[pcbActivo->ProgramCounter].start;

	//Instancio para utilizar
	t_size offset =
			pcbActivo->CodeTagsPointer->instrucciones_serializado[pcbActivo->ProgramCounter].offset; //que me devuelva la siguiente linea la memoria

	//Solicito a memoria la instruccion.
	//char* const instruccion = solicitarInstruccionAMemoria(pcbActivo->PID,
	//		4, start, offset); //falta la pagina en donde esta la instruccion.

	char* const instruccion = "variables a, b, c, d, e \n";

	//Ejecuta las primitivas
	analizadorLinea(instruccion, &functions, &kernel_functions);

	pcbActivo->ProgramCounter++;

	//}
}

char* const solicitarInstruccionAMemoria(uint32_t pid, uint32_t pagina,
		t_puntero_instruccion offset, t_size size) {

	//Creo la estructura para enviar el pedido a memoria
	t_pedido_memoria *pedido = malloc(sizeof(t_pedido_memoria));
	pedido->id = pid;
	pedido->pagina = pagina;
	pedido->offset = offset;
	pedido->size = size;

	//Serializo la estructura para enviar a Memoria

	log_info(log_Console, "Pidiendo Instruccion a Memoria\n");

	//Le envio a memoria que necesito una instruccion.
	serializar_int(memoria, SOLICITUD_INSTRUCCION_MEMORIA);

	//Serializo la estructura para enviar a Memoria
	serializar_pedido_memoria(memoria, pedido);

	//Quedo a al espera de que memoria me envie la instruccion
	t_SerialString* linea = malloc(sizeof(t_SerialString));
	deserializar_string(memoria, linea->dataString);

	return linea;

}

void connect_server_kernel() {
//Conexion al kernel
	kernel = connect_server(config.IP_KERNEL, config.PUERTO_KERNEL);
	if (kernel > 0) {
		log_info(log_Console, "Kernel Conectado\n");
		serializar_int(kernel, HANDSHAKE_CPU_KERNEL);

	}
}

void connect_server_memoria() {
//Conexion a memoria
	memoria = connect_server(config.IP_MEMORIA, config.PUERTO_MEMORIA);
	if (memoria > 0) {
		log_info(log_Console, "Memoria Conectada\n");
		serializar_int(memoria, HANDSHAKE_CPU_MEMORIA);
		tamanio_pagina = deserializar_int(memoria);

	}
}

void init_log(char* pathLog) {
	mkdir("/home/utnso/Blacklist/Logs", 0755);
	log_Console = log_create(pathLog, "CPU", true, LOG_LEVEL_INFO);
	log_CPU = log_create(pathLog, "CPU", false, LOG_LEVEL_INFO);
}

t_puntero ansi_definirVariable(t_nombre_variable identificador_variable) {

	log_info(log_Console, "Se define la variable %c\n", identificador_variable);

	//Le pido al PCB la cantidad de paginas que tiene el codigo y le sumo uno que es la siguiente pagina.
	uint32_t primerPagina_Stack = pcbActivo->PageCode + 1;

	t_puntero posicion_Variable;

	//Si el registro que tiene el stack es nulo creo el primer registro.
	if (list_size(pcbActivo->StackPointer) == 0) {

		//Creo la nueva variable para cargarla en el primer registro
		VARIABLE_T *nueva_var = variable_new(identificador_variable,
				primerPagina_Stack, 0, sizeof(int));

		//Creo una lista de variables temporal para agregar a la linea del stack.
		//t_queue *variablesTemp = queue_create();
		t_list * variablesTemp2 = list_create();

		//Agrego la variable creada previamente
		//queue_push(variablesTemp, nueva_var);
		list_add(variablesTemp2, nueva_var);

		//Creo la linea de stack pasandole como parametro la lista creada previamente
		//STACKPOINTER_T* lineaSP = stack_new(NULL, variablesTemp, NULL, NULL);
		STACKPOINTER_T* lineaSP = stack_new(NULL, variablesTemp2, NULL, NULL);

		//Agrego al stack la linea creada recientemente
		//push_stack(pcbActivo, lineaSP);
		list_add(pcbActivo->StackPointer, lineaSP);

		//Calculo la posicion es la pagina de la ultima variable por su tamanio + el offset de donde comienza la variable.
		posicion_Variable = (nueva_var->pagina * tamanio_pagina)
				+ nueva_var->offset;

		//Si no estaba vacio le pido a la ultima linea, su ultima variable y trabajo con ella
	} else {

		//Obtengo la ultima linea de stack
		//STACKPOINTER_T *lineaSP = pull_stack(pcbActivo);
		STACKPOINTER_T *ultimaLineaSP = list_get(pcbActivo->StackPointer,
				pcbActivo->StackPointer->elements_count - 1);

		//Elimino la ultima linea para despues actualizarla
		list_remove(pcbActivo->StackPointer,
				pcbActivo->StackPointer->elements_count - 1);

		//Le pido a la ultima linea la ultima variable que tiene
		//VARIABLE_T *ultimaVariable = queue_pop(lineaSP->Variables);
		VARIABLE_T *ultimaVariable = list_get(ultimaLineaSP->Variables,
				ultimaLineaSP->Variables->elements_count - 1);

		//La vuelvo a agregar a la lista porque era solo para leerla
		//queue_push(lineaSP->Variables,ultimaVariable);

		//Calculo el espacio ocupado sumando el comienzo + el tamanio del ultimo valor cargado.
		int espacioOcupado = ultimaVariable->offset + ultimaVariable->size;

		//Calculo e espacio disponible restando al tamanio de la pagina el espacio ocupado.
		int espacioDisponiblePagina = tamanio_pagina - espacioOcupado;

		if (espacioDisponiblePagina >= sizeof(int)) {

			//Si el espacio disponible es mayor al tamano de lo que quiero guardar.

			//Creo la nueva variable para cargarla en la ultima linea insertada en la misma pagina
			VARIABLE_T *nueva_var = variable_new(identificador_variable,
					ultimaVariable->pagina, espacioOcupado, sizeof(int));

			//Agrego a las variables de la ultima linea la nueva variable
			//queue_push(lineaSP->Variables, nueva_var);
			list_add(ultimaLineaSP->Variables, nueva_var);

			//Agrego al stack la linea creada recientemente
			//push_stack(pcbActivo, lineaSP);
			list_add(pcbActivo->StackPointer, ultimaLineaSP);

			//Calculo la posicion es la pagina de la ultima variable por su tamanio + el offset de donde comienza la variable.
			posicion_Variable = (nueva_var->pagina * tamanio_pagina)
					+ nueva_var->offset;

		} else {

			//Si el espacio disponible no es mayor al tamanio de lo que quiero guardar, lo pongo en la pagina siguiente.

			//Creo la nueva variable para cargarla en la ultima linea insertada en la siguiente pagina
			VARIABLE_T *nueva_var = variable_new(identificador_variable,
					ultimaVariable->pagina + 1, 0, sizeof(int));

			//Agrego a las variables de la ultima linea la nueva variable
			//queue_push(lineaSP->Variables, nueva_var);
			list_add(ultimaLineaSP->Variables, nueva_var);

			//Agrego al stack la linea creada recientemente
			//push_stack(pcbActivo, lineaSP);
			list_add(pcbActivo->StackPointer, ultimaLineaSP);

			//Calculo la posicion es la pagina de la ultima variable por su tamanio + el offset de donde comienza la variable.
			posicion_Variable = (nueva_var->pagina * tamanio_pagina)
					+ nueva_var->offset;

		}

	}


	return posicion_Variable;
}

