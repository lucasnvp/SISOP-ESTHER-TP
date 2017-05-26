#include "Proceso CPU.h"
#include <stdio.h>
#include <stdlib.h>
#include "serializador/serializador.h"
#include "config/config_CPU.h"
#include "pcb/pcb.h"

char* PATH_CONFIG = "../src/config/config.txt";
Type_Config config;

// ------------------------ VARIABLES GLOBALES ---------------------------------

uint32_t idCpu; // Identificador de CPU.

int main(void) {
	puts("Proceso CPU");

	idCpu = 1;

	//Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);

	//Conexion al servidor
	int kernel = connect_server(config.IP_KERNEL, config.PUERTO_KERNEL);

	//Si conecto, informo
	if (kernel > 0) {
		printf("Estoy escuchando\n");
	}

	char* buff;
	while (1) {
		//Recibo los datos
		recive_data(kernel,buff,sizeof(buff));
		enviarMensaje("Hola Kernel Serializado",0,1,kernel);

	}

	return EXIT_SUCCESS;
}


//Funcion para enviar un mensaje al Kernel con un literal

void enviarMensaje(char* mensaje, int valor, int idMje, int socket) {

	t_STR_INTERCAMBIO str_mensaje = incializarIntercambio();
	str_mensaje.socket_server = socket;
	str_mensaje.identificador_cpu = idCpu;
	str_mensaje.size_str = sizeof(t_STR_INTERCAMBIO);

	if (idMje == 1) { //IMPRIMIR TEXTO
		str_mensaje.id_MSJ = IMPRIMIR_TEXTO;
		serializar(str_mensaje.socket_server,str_mensaje.id_MSJ ,str_mensaje.size_str, str_mensaje.texto);
	} else if (idMje == 2) { //IMPRIMIR VALOR
		str_mensaje.id_MSJ = IMPRIMIR_VALOR;
		str_mensaje.valor = valor;
	}
	serializar(str_mensaje.socket_server,1,str_mensaje.size_str, str_mensaje.texto);
}

t_STR_INTERCAMBIO incializarIntercambio(){

	t_STR_INTERCAMBIO str;

	str.buffer = "\0";
	str.codigo = "\0";
	str.id_var_comp = "\0";
	str.mensaje = "\0";
	str.texto = "\0";
	str.PID = 0;
	str.OK = 0;
	str.base_bytes = 0;
	str.identificador_cpu = 0;
	str.indice = 0;
	str.offset_bytes = 0;
	str.tamanio_bytes = 0;
	str.valor = 0;
	return str;

}



