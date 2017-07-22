#include "Proceso CPU.h"

AnSISOP_funciones functions = { .AnSISOP_definirVariable = ansi_definirVariable,
		.AnSISOP_obtenerPosicionVariable = ansi_obtenerPosicionVariable,
		.AnSISOP_asignar = ansi_asignar, .AnSISOP_dereferenciar =
				ansi_desreferenciar, .AnSISOP_obtenerValorCompartida =
				ansi_obtener_valor_compartida, .AnSISOP_asignarValorCompartida =
				ansi_asignar_valor_compartida, .AnSISOP_irAlLabel =
				ansi_irAlLabel, .AnSISOP_llamarSinRetorno =
				ansi_llamarSinRetorno, .AnSISOP_llamarConRetorno =
				ansi_llamarConRetorno, .AnSISOP_finalizar = ansi_finalizar,
		.AnSISOP_retornar = ansi_retornar,

};

AnSISOP_kernel kernel_functions = {

.AnSISOP_wait = kernel_wait };  //Syscalls

int main(void) {

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

	//Inicializo el PCB
	pcbActivo = PCB_new_pointer(0, 0, NULL);

	//while (true) {

	//Quedo a la espera de recibir un PCB del Kernel
	deserializar_pcb(kernel, pcbActivo);

	log_info(log_Console, "PCB Activo\n");

	print_PCB(pcbActivo);

	//Proceso de ejecucion de Primitivas Ansisop
	//ejecutar();

	print_PCB(pcbActivo);

	char* pedido = solicitarInstruccionAMemoria(1, 0, 25, 16);

	printf("El pedido a memoria es: %s", pedido);

	//Envio el mensaje al kernel de que finalizo la rafaga correctamente
	serializar_int(kernel, FIN_CORRECTO);

	//Envio a kernel PCB actualizado
	serializar_pcb(kernel, pcbActivo);

	//}

	return EXIT_SUCCESS;

}

void ejecutar() {

	log_info(log_Console, "Ejecutando en CPU\n");

	log_info(log_Console, "Tamanio de pagina %d\n", tamanio_pagina);

	while (!codigoFinalizado()) {

		t_metadata_program *ctp = pcbActivo->CodeTagsPointer;

		//Instancio para utilizar
		t_intructions inst =
				ctp->instrucciones_serializado[pcbActivo->ProgramCounter];

		//Sirve para calcular en que pagina esta la instruccion que acaba de traer del PCB
		uint32_t pagina_actual = 0;
		uint32_t offset_actual = inst.offset;

		while (offset_actual > tamanio_pagina) {

			pagina_actual++;

			offset_actual = offset_actual - tamanio_pagina;

		}

		//Solicito a memoria la instruccion.
		char* const instruccion = solicitarInstruccionAMemoria(pcbActivo->PID,
				pagina_actual, inst.start, inst.offset);

		log_info(log_Console, "Instruccion %s\n", instruccion);

		//Ejecuta las primitivas
		//analizadorLinea(instruccion, &functions, &kernel_functions);

		pcbActivo->ProgramCounter++;

	}
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
	deserializar_string(memoria, linea);

	return linea;

	free(pedido);

}

t_valor_variable solicitarValorAMemoria(uint32_t pid, uint32_t pagina,
		uint32_t offset, t_size size) {

	//Creo la estructura para enviar el pedido a memoria
	t_pedido_memoria *pedido = malloc(sizeof(t_pedido_memoria));
	pedido->id = pid;
	pedido->pagina = pagina;
	pedido->offset = offset;
	pedido->size = size;

	//Serializo la estructura para enviar a Memoria

	log_info(log_Console, "Solicitando Dato a Memoria\n");

	//Le envio a memoria que necesito una instruccion.
	serializar_int(memoria, SOLICITUD_INSTRUCCION_MEMORIA);

	//Serializo la estructura para enviar a Memoria
	serializar_pedido_memoria(memoria, pedido);

	//Quedo a al espera de que memoria me envie la instruccion

	t_valor_variable dato = deserializar_int(memoria);

	return dato;

	free(pedido);

}

void enviarDatoAMemoria(uint32_t pagina, uint32_t offset, uint32_t size) {

	//Creo la estructura para enviar el dato a memoria
	t_pedido_memoria *envio = malloc(sizeof(t_pedido_memoria));
	envio->id = pcbActivo->PID;
	envio->pagina = pagina;
	envio->offset = offset;
	envio->size = size;

	//Le envio a memoria que necesito una instruccion.
	serializar_int(memoria, ASIGNAR_VALOR_A_MEMORIA);

	//Serializo la estructura para enviar a Memoria
	serializar_pedido_memoria(memoria, envio);

	log_info(log_Console,
			"Se envia a memoria la actualizacion de una variable\n");

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
	uint32_t primerPagina_Stack = 0;

	t_puntero posicion_Variable;

	VARIABLE_T *nueva_var = variable_new(identificador_variable,
			primerPagina_Stack, 0, sizeof(int));

	//Si el registro que tiene el stack es nulo creo el primer registro.
	if (list_size(pcbActivo->StackPointer) == 0) {

		//Creo una lista de variables temporal para agregar a la linea del stack.
		t_list * variablesTemp = list_create();

		//Agrego la variable creada previamente
		list_add(variablesTemp, nueva_var);

		//Creo la linea de stack pasandole como parametro la lista creada previamente
		STACKPOINTER_T* lineaSP = stack_new(list_create(), variablesTemp,
				list_create(), NULL);

		//Agrego al stack la linea creada recientemente
		list_add(pcbActivo->StackPointer, lineaSP);

		//Si no estaba vacio le pido a la ultima linea, su ultima variable y trabajo con ella
	} else {

		//Obtengo la ultima linea de stack
		STACKPOINTER_T *ultimaLineaSP = list_get(pcbActivo->StackPointer,
				pcbActivo->StackPointer->elements_count - 1);

		//Elimino la ultima linea para despues actualizarla
		list_remove(pcbActivo->StackPointer,
				pcbActivo->StackPointer->elements_count - 1);

		//Le pido a la ultima linea la ultima variable que tiene
		VARIABLE_T *ultimaVariable = list_get(ultimaLineaSP->Variables,
				ultimaLineaSP->Variables->elements_count - 1);

		//Calculo el espacio ocupado sumando el comienzo + el tamanio del ultimo valor cargado.
		int espacioOcupado = ultimaVariable->offset + ultimaVariable->size;

		//Calculo e espacio disponible restando al tamanio de la pagina el espacio ocupado.
		int espacioDisponiblePagina = tamanio_pagina - espacioOcupado;

		if (espacioDisponiblePagina >= sizeof(int)) {

			//Si el espacio disponible es mayor al tamano de lo que quiero guardar.

			//Cargo Pagina y Offset a la nueva variable
			nueva_var->pagina = ultimaVariable->pagina;
			nueva_var->offset = espacioOcupado;

			//Agrego a las variables de la ultima linea la nueva variable
			//queue_push(lineaSP->Variables, nueva_var);
			list_add(ultimaLineaSP->Variables, nueva_var);

			//Agrego al stack la linea creada recientemente
			//push_stack(pcbActivo, lineaSP);
			list_add(pcbActivo->StackPointer, ultimaLineaSP);

		} else {

			//Si el espacio disponible no es mayor al tamanio de lo que quiero guardar, lo pongo en la pagina siguiente.

			//Cargo en pagina siguiente. La cargo en la ultima linea insertada
			nueva_var->pagina = ultimaVariable->pagina + 1;

			//Agrego a las variables de la ultima linea la nueva variable
			//queue_push(lineaSP->Variables, nueva_var);
			list_add(ultimaLineaSP->Variables, nueva_var);

			//Agrego al stack la linea creada recientemente
			list_add(pcbActivo->StackPointer, ultimaLineaSP);

		}

	}

	//Calculo la posicion es la pagina de la ultima variable por su tamanio + el offset de donde comienza la variable.
	posicion_Variable = (nueva_var->pagina * tamanio_pagina)
			+ nueva_var->offset;

	return posicion_Variable;
}

t_puntero ansi_obtenerPosicionVariable(t_nombre_variable identificador_variable) {

	//log_info(log_Console, "Se obtiene la posicion de la variable %c: \n",
	//		identificador_variable);

	//Va a ser la posicion a devolver
	uint32_t posicion;

	//Obtengo el contexto de ejecucion actual, la ultima linea del stack.

	STACKPOINTER_T *ultimaLineaSP = list_get(pcbActivo->StackPointer,
			pcbActivo->StackPointer->elements_count - 1);

	t_list *variables = ultimaLineaSP->Variables;
	t_list *argumentos = ultimaLineaSP->Argumentos;

	uint32_t i;

	//Si es un digito, es un argumento
	if (isdigit(identificador_variable)) {
		//Recorro la lista de argumentos hasta encontrarla:
		for (i = 0; i < argumentos->elements_count; i++) {
			VARIABLE_T *argumento = list_get(argumentos, i);

			if (argumento->id == identificador_variable) {
				posicion = (argumento->pagina * tamanio_pagina)
						+ argumento->offset;
				return posicion;
			}
		}
	} else {

		for (i = 0; i < variables->elements_count; i++) {

			VARIABLE_T *variable = list_get(variables, i);

			if (variable->id == identificador_variable) {

				posicion = (variable->pagina * tamanio_pagina)
						+ variable->offset;

				return posicion;
			}
		}
	}

	return -1;

}

void ansi_asignar(t_puntero direccion_variable, t_valor_variable valor) {

	uint32_t pagina = direccion_variable / tamanio_pagina;
	uint32_t offset = direccion_variable % tamanio_pagina; //el resto de la division
	t_valor_variable value = valor;

	enviarDatoAMemoria(pagina, offset, value);

	log_info(log_Console,
			"Se asigna el valor %d a la variable en la posicion%c:\n", valor,
			direccion_variable);

}

t_valor_variable ansi_desreferenciar(t_puntero direccion_variable) {

	log_info(log_Console,
			"Se obtiene de memoria un dato en base a la su direccion");

	uint32_t pag = direccion_variable / tamanio_pagina;
	uint32_t offset = direccion_variable % tamanio_pagina; //el resto de la division
	uint32_t size = sizeof(int);

	t_valor_variable dato = solicitarValorAMemoria(pcbActivo->PID, pag, offset,
			size);

	return dato;

}

t_valor_variable ansi_obtener_valor_compartida(
		t_nombre_compartida identificador) {

	//Le envio el mensaje al Kernel de que necesito el valor de una variable compartida
	serializar_int(kernel, PEDIDO_VAR_COMPARTIDA);

	t_SerialString* variable = malloc(sizeof(t_SerialString));
	variable->dataString = identificador;
	variable->sizeString = strlen(identificador);

	serializar_string(kernel, variable);

	t_valor_variable valor;

	//Quedo a la espera de recibir el valor del kernel
	valor = deserializar_int(kernel);

	return valor;

	log_info(log_Console,
			"Se solicita a Kernel el valor %d de la variable compartida: %c\n",
			valor, identificador);

	free(variable);
}

t_valor_variable ansi_asignar_valor_compartida(
		t_nombre_compartida identificador, t_valor_variable valor) {

	//Le envio el mensaje al Kernel de que necesito actualizar el valor de una variable compartida
	serializar_int(kernel, ASIGNAR_VAR_COMPARTIDA);

	//Preparo la estructura para serializar el String
	t_SerialString* variable = malloc(sizeof(t_SerialString));
	variable->dataString = identificador;
	variable->sizeString = strlen(identificador);

	//Serializo el string con el nombre de la variable
	serializar_string(kernel, variable);

	//Serializo el nuevo valor que va a tener la variable compartida
	serializar_int(kernel, valor);

	return valor;

	free(variable);

}

void ansi_irAlLabel(t_nombre_etiqueta t_nombre_etiqueta) {

	const char* etiquetas = pcbActivo->CodeTagsPointer->etiquetas;
	const t_size cant_etiquetas =
			pcbActivo->CodeTagsPointer->cantidad_de_etiquetas;

	t_puntero_instruccion posicionEtiqueta = metadata_buscar_etiqueta(
			t_nombre_etiqueta, etiquetas, cant_etiquetas);

	pcbActivo->ProgramCounter = posicionEtiqueta;
	printf("Cambio el PC a: %d\n", posicionEtiqueta);

	log_info(log_Console, "Se ejecuta primitiva irAlLabel para etiqueta: %s\n",
			t_nombre_etiqueta);
	log_info(log_Console, "Cambio el PC a: %s\n", posicionEtiqueta);
}

void ansi_llamarSinRetorno(t_nombre_etiqueta etiqueta) {

	STACKPOINTER_T *lineaSP = stack_new(list_create(), list_create(),
			pcbActivo->ProgramCounter, NULL);

	//Agrego al stack la linea creada recientemente
	list_add(pcbActivo->StackPointer, lineaSP);

	ansi_irAlLabel(etiqueta);

	log_info(log_Console,
			"Se ejecuta la primitiva llamarSinRetorno con la etiqueta: %s\n",
			etiqueta);

}

void ansi_llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero punto_retorno) {

	//Calculo los datos de la nueva variable de retorno para cargarla en el contexto de ejecucion a retornar
	uint32_t pagina = punto_retorno / tamanio_pagina;
	uint32_t offset = punto_retorno % tamanio_pagina;
	uint32_t size = sizeof(int);

	VARIABLE_T *variable = variable_new(NULL, pagina, offset, size);

	//Creo un nuevo contexto de ejecucion
	STACKPOINTER_T *lineaSP = stack_new(list_create(), list_create(),
			pcbActivo->ProgramCounter, variable);

	//Agrego al stack el contexto creado recientemente
	list_add(pcbActivo->StackPointer, lineaSP);

	ansi_irAlLabel(etiqueta);

	log_info(log_Console,
			"Se ejecuta la primitiva llamarConRetorno con la etiqueta: %s\n",
			etiqueta);

}

void ansi_finalizar(void) {

	//Pido el ultimo contexto de ejecucion
	STACKPOINTER_T* lineaSPActual = list_remove(pcbActivo->StackPointer,
			pcbActivo->StackPointer->elements_count - 1);

	//Si no hay quiere decir que el programa finalizo.
	if (list_size(pcbActivo->StackPointer) == 0) {
		termino_codigo = true;
		log_info(log_Console, "El programa ha finalizado\n");

	} else {
		//Si hay le asigno al PC del pcbActivo el valor de Retorno
		pcbActivo->ProgramCounter = lineaSPActual->DireccionDeRetorno;
		log_info(log_Console, "El programa continua en la instruccion: %d\n",
				pcbActivo->ProgramCounter);

	}

	stack_free(lineaSPActual);
}

//Funcion que permite evaluar si el codigo ha finalizado
bool codigoFinalizado() {
	return termino_codigo;
}

//Funcion que permitira indicar que el codigo ha finalizado.
void finalizarCodigo() {

	termino_codigo = true;
}

void ansi_retornar(t_valor_variable retorno) {

	log_info(log_Console,
			"Se ejecuta la primitiva Retornar para el valor: %d\n", retorno);

	//Obtengo registro actual:
	STACKPOINTER_T *lineaSPActual = list_get(pcbActivo->StackPointer,
			pcbActivo->StackPointer->elements_count - 1);

	//Obtengo la direccion a la que voy a retornar en base a lo que me indica la dir de retorno del contexto de ejecucion actual
	t_puntero direccion_retorno = (lineaSPActual->VariableDeRetorno->pagina
			* tamanio_pagina) + lineaSPActual->VariableDeRetorno->offset;

	ansi_asignar(direccion_retorno, retorno);

}

void kernel_wait(t_nombre_semaforo identificador_semaforo) {

	log_info(log_Console,
			"Se ejecuta la primitiva Retornar para el valor: %d\n",
			identificador_semaforo);

	//Le envio al kernel el mensaje del wait.
	serializar_int(kernel, KERNEL_WAIT);

	//Variable para enviar
	t_SerialString* semaforo;

	//Le cargo el dato del semaforo
	semaforo->dataString = identificador_semaforo;
	semaforo->sizeString = strlen(identificador_semaforo);

	//Le envio al kernel el semaforo
	//serializar_string(kernel, semaforo);

	//Deserializo el dato.
	uint32_t retorno = deserializar_int(kernel);

	retorno = true;

	//Si el retorno es True tengo que bloquear el proceso
	if (retorno == true) {
		procesoBloqueado = true;
		log_info(log_Console, "El proceso %d, se encuentra bloqueado: \n",
				pcbActivo->PID);
	}

}

void kernel_signal(t_nombre_semaforo identificador_semaforo) {

	//Le envio al kernel el mensaje del signal.
	serializar_int(kernel, KERNEL_SIGNAL);

	//Variable para enviar
	t_SerialString* semaforo;

	//Le cargo el dato del semaforo
	semaforo->dataString = identificador_semaforo;
	semaforo->sizeString = strlen(identificador_semaforo);

	//Le envio al kernel el semaforo
	serializar_string(kernel, semaforo);

	//Deserializo el dato.
	uint32_t retorno = deserializar_int(kernel);

	retorno = true;

	//Si el retorno es True tengo que bloquear el proceso
	if (retorno == true) {
		log_info(log_Console, "El proceso %d, se desbloqueo: \n",
				pcbActivo->PID);
	}

}

t_descriptor_archivo abrir_archivo(t_direccion_archivo direccion,
		t_banderas flags) {

	serializar_int(kernel, FS_ABRIR);

	//Serializo para enviar la direccion
	t_SerialString* dire;

	//Le cargo el dato de la direccion
	dire->dataString = direccion;
	dire->sizeString = strlen(direccion);

	//Le envio al kernel el semaforo
	serializar_string(kernel, dire);

	char* permisos = concatenarPermisos(flags);

	//Serializo para enviar los permisos
	t_SerialString* envioPermios;

	envioPermios->dataString = permisos;
	envioPermios->sizeString = strlen(permisos);

	serializar_string(kernel, envioPermios);

	uint32_t resultado;

	//Quedo a la espera del resultado del pedido
	resultado = deserializar_int(kernel);

	if (resultado < 0) {

		log_error(log_Console, "Error al intentar abrir archivo en modo %s/n",
				permisos);
		return 0;

	}

	t_descriptor_archivo descriptor = deserializar_int(kernel);

	log_info(log_Console, "El proceso %d ha abierto un achivo en modo %s",
			pcbActivo->PID, permisos);

	return descriptor;
}

void borrar_archivo(t_descriptor_archivo descriptor_archivo) {

	serializar_int(kernel, FS_BORRAR);

	serializar_int(kernel, descriptor_archivo);

	uint32_t resultado;

	//Quedo a la espera del resultado del pedido
	resultado = deserializar_int(kernel);

	if (resultado > 0) {

		log_info(log_Console, "El proceso %d ha borrado el archivo %d/n",
				pcbActivo->PID, descriptor_archivo);
	} else {

		log_error(log_Console, "Error del proceso %d al borrar el archivo %d",
				pcbActivo->PID, descriptor_archivo);

	}

}

void cerrar_archivo(t_descriptor_archivo descriptor_archivo) {

	serializar_int(kernel, FS_CERRAR);

	serializar_int(kernel, descriptor_archivo);

	uint32_t resultado;

	resultado = deserializar_int(kernel);

	if (resultado > 0)
		log_info(log_Console, "El proceso %d ha cerrado un archivo %d\n",
				pcbActivo->PID, descriptor_archivo);
	else {
		log_error(log_Console, "Error del proceso %d al cerrar el archivo %d",
				pcbActivo->PID, descriptor_archivo);

	}

}

void moverCursor_archivo(t_descriptor_archivo descriptor_archivo,
		t_valor_variable posicion) {

	serializar_int(kernel, FS_MOVER_CURSOR);

	serializar_int(kernel, descriptor_archivo);
	serializar_int(kernel, posicion);

	uint32_t resultado = deserializar_int(kernel);

	if (resultado > 0)
		log_info(log_Console,
				"El proceso %d ha movido el cursor del archivo %d en la posicion %d\n",
				pcbActivo->PID, descriptor_archivo, posicion);
	else {
		log_error(log_Console,
				"Error del proceso %d al mover el cursor del archivo %d",
				pcbActivo->PID, descriptor_archivo);

	}

}

void leer_archivo(t_descriptor_archivo descriptor_archivo,
		t_puntero informacion, t_valor_variable tamanio) {

	serializar_int(kernel, FS_LEER);
}

void escribir(t_descriptor_archivo descriptor_archivo, void* informacion,
		t_valor_variable tamanio) {

}

char* concatenarPermisos(t_banderas flags) {
	char *flagsAConcatenar = string_new();
	if (flags.creacion == true) {
		string_append(&flagsAConcatenar, "c");
	}
	if (flags.lectura == true) {
		string_append(&flagsAConcatenar, "r");
	}
	if (flags.escritura == true) {
		string_append(&flagsAConcatenar, "w");
	}

	return flagsAConcatenar;
}

