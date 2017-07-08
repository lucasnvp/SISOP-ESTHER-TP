#include "Memoria.h"

int main(void) {
	puts("Proceso Memoria");

	//Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);

	inicializarMemoria();

	/*inicializarPrograma(1,2);
	 inicializarPrograma(2,2);
	 inicializarPrograma(3,4);
	 almacenarBytesPagina(1,0,0,5,"hola");
	 almacenarBytesPagina(1,0,4,6," pepe");
	 almacenarBytesPagina(2,0,0,5,"chau");
	 almacenarBytesPagina(3,0,0,5,"1");
	 almacenarBytesPagina(3,1,0,5,"2");
	 almacenarBytesPagina(3,2,0,5,"3");
	 almacenarBytesPagina(3,3,0,5,"4");
	 solicitarBytesPagina(1,0,0,5);
	 solicitarBytesPagina(2,0,0,5);
	 solicitarBytesPagina(3,0,0,5);
	 solicitarBytesPagina(3,1,0,5);
	 solicitarBytesPagina(3,2,0,5);
	 solicitarBytesPagina(3,3,0,5);
	 solicitarBytesPagina(1,0,0,11);
	 finalizarPrograma(1);
	 almacenarBytesPagina(2,1,0,7,"prueba");
	 solicitarBytesPagina(2,1,0,7);



	 impirmirEPIaccediendoAMemoria(0,40);

	 printf("\n");

	 imprimirCache();*/

	/*fd_set master;   	// conjunto maestro de descriptores de fichero
	fd_set read_fds; // conjunto temporal de descriptores de fichero para select()
	uint32_t fdmax;			// número máximo de descriptores de fichero
	int i;				// variable para el for
	FD_ZERO(&master);	// borra los conjuntos maestro
	FD_ZERO(&read_fds);	// borra los conjuntos temporal

	//Creacion del servidor
	servidor = build_server(config.PUERTO, config.CANTCONEXIONES);
	//El socket esta listo para escuchar
	if (servidor > 0) {
		printf("Servidor Memoria Escuchando\n");
	}
	FD_SET(servidor, &master);
	// seguir la pista del descriptor de fichero mayor
	fdmax = servidor; // por ahora es éste
	// bucle principal
	while (1) {
		// acepto una nueva conexion

		for (i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &read_fds)) { // ¡¡tenemos datos!!
				if (i == servidor) {
					// acepto una nueva conexion*/
					uint32_t newfd = accept_conexion(servidor);
					/*FD_SET(newfd, &master); // añadir al conjunto maestro
					if (newfd > fdmax) {    // actualizar el máximo
						fdmax = newfd;
					}*/
					pthread_t* hiloConsola = (pthread_t *) malloc(
							sizeof(pthread_t));
					pthread_create(hiloConsola, NULL, (void*) crearHilo,
							(void*) &newfd);
				/*} else {
					//Recibo el comando
					uint32_t command = deserializar_int(i);

					// gestionar datos de un cliente
					if (command <= 0) {
						close(i); // Close conexion
						FD_CLR(i, &master); // eliminar del conjunto maestro
					} else {
						connection_handler(i, command);
					}
				}
			}
		}
	}*/
	return EXIT_SUCCESS;

}

void inicializarMemoria() {
	inicializarTablaEPI();
	bloque_Memoria = malloc(MARCOS * MARCO_SIZE);

	int i;
	int cantMarcosOcupaMemoriaAdm =
			((sizeof(int) * 3 * MARCOS) + MARCO_SIZE - 1) / MARCO_SIZE;
	int aux[3];
	aux[N_PID] = -1;

	//Inicializo en memoria tabla que va a dar informacion de los procesos de sistema
	for (i = 0; i < cantMarcosOcupaMemoriaAdm; i++) {
		aux[N_PAGINA] = i;
		aux[N_FRAME] = i;
		memcpy(bloque_Memoria + (i * sizeof(aux)), &aux, sizeof(aux));

	}
	aux[N_PAGINA] = 0;
	aux[N_PID] = 0;
	//Inicializo en memoria siguiente parte de tabla que va a dar informacion de los procesos de usuario
	for (i = 0; i < MARCOS - cantMarcosOcupaMemoriaAdm; i++) {
		aux[N_FRAME] = i + cantMarcosOcupaMemoriaAdm;
		memcpy(
				(bloque_Memoria + cantMarcosOcupaMemoriaAdm * sizeof(aux))
						+ (i * sizeof(aux)), &aux, sizeof(aux)); //TODO: no se esta copiando bien con ceros las cosas

	}
	for (i = 0; i < cantMarcosOcupaMemoriaAdm; i++) {
		agregarDatosTablaEPI(-1, i);
	}

}
void inicializarTablaEPI() {
	tablaEPI.filas = 0;
	tablaEPI.matriz = malloc(sizeof(tablaEPI.matriz));

}
int agregarDatosTablaEPI(int PID, int nPagina) {
	int pudeEscribirTabla = 0;
	int i;
	int cantMarcosOcupaMemoriaAdm = ((sizeof(int*) * 3 * MARCOS) + MARCO_SIZE
			- 1) / MARCO_SIZE;
	int info[3];
	info[N_PID] = PID;
	info[N_PAGINA] = nPagina;
	//Busco si hay marcos Libres
	for (i = 0; i < tablaEPI.filas && !pudeEscribirTabla; i++) {
		if (tablaEPI.matriz[i][N_PID] == 0
				&& tablaEPI.matriz[i][N_PAGINA] == 0) {
			memcpy(&tablaEPI.matriz[i][N_PID], &PID, sizeof(PID));
			memcpy(&tablaEPI.matriz[i][N_PAGINA], &nPagina, sizeof(nPagina));

			//Copio En memoria la info de la estructura administrativa.
			info[N_FRAME] = tablaEPI.matriz[i][N_FRAME];
			memcpy(bloque_Memoria + (info[N_FRAME]) * sizeof(info), &info,
					sizeof(info));

			pudeEscribirTabla = 1;
		}
	}
	if (!pudeEscribirTabla && tablaEPI.filas < MARCOS) //Esto lo voy a tener que borrar
	{
		int **aux;
		aux = realloc(tablaEPI.matriz, sizeof(int) * (tablaEPI.filas + 1));

		if (aux != '\0') {

			aux[tablaEPI.filas] = (int*) malloc(
					sizeof(aux[tablaEPI.filas]) * 3);
			memcpy(&tablaEPI.matriz, &aux, sizeof(aux));

			memcpy(&tablaEPI.matriz[tablaEPI.filas][N_FRAME], &tablaEPI.filas,
					sizeof(tablaEPI.filas));
			memcpy(&tablaEPI.matriz[tablaEPI.filas][N_PID], &PID, sizeof(PID));
			memcpy(&tablaEPI.matriz[tablaEPI.filas][N_PAGINA], &nPagina,
					sizeof(nPagina));

			//Copio En memoria la info de la estructura administrativa.
			info[N_FRAME] = tablaEPI.matriz[tablaEPI.filas][N_FRAME];
			memcpy(bloque_Memoria + (info[N_FRAME]) * sizeof(info), &info,
					sizeof(info));

			tablaEPI.filas += 1;
			pudeEscribirTabla = 1;
		}

	}

	return pudeEscribirTabla;
}

int borrarDatosTablaEPI(int PID) {

	int i;

	int cantMarcosOcupaMemoriaAdm = ((sizeof(int*) * 3 * MARCOS) + MARCO_SIZE
			- 1) / MARCO_SIZE;
	int info[3];
	info[N_PID] = 0;
	info[N_PAGINA] = 0;

	int pudeBorrar = 0;

	for (i = 0; i < tablaEPI.filas; i++) {
		if (PID == tablaEPI.matriz[i][N_PID]) {
			tablaEPI.matriz[i][N_PID] = 0;
			tablaEPI.matriz[i][N_PAGINA] = 0;

			info[N_FRAME] = tablaEPI.matriz[i][N_FRAME];

			memcpy(bloque_Memoria + (info[N_FRAME]) * sizeof(info), &info,
					sizeof(info));

			pudeBorrar = 1;
		}
	}

	return pudeBorrar;
}

int framesDisponibles() {

	int i;
	int framesDisponibles = 0;
	//Busco si hay marcos Libres
	for (i = 0; i < tablaEPI.filas; i++) {
		if (tablaEPI.matriz[i][N_PID] == 0
				&& tablaEPI.matriz[i][N_PAGINA] == 0) {
			framesDisponibles++;
		}
	}
	return (MARCOS - tablaEPI.filas + framesDisponibles);
}

void imprimirEPI() {
	int i;
	for (i = 0; i < tablaEPI.filas; i++) {
		printf("#FRAME:%d  PID:%d  #PAGINA:%d \n", tablaEPI.matriz[i][N_FRAME],
				tablaEPI.matriz[i][N_PID], tablaEPI.matriz[i][N_PAGINA]);
	}

	printf("Los frames disponibles son: %d", framesDisponibles());
}

void impirmirEPIaccediendoAMemoria(int inicio, int fin) {
	int a[3];
	int i;
	if (fin < MARCOS) {
		for (i = inicio; i <= fin; i++) {
			memcpy(&a, bloque_Memoria + 12 * i, sizeof(int) * 3);

			printf("\nIMPRIMIENDO DESDE MEMORIA EPI FRAME:%d PID:%d PAGINA:%d",
					a[0], a[1], a[2]);
		}
	}

}
int inicializarPrograma(int PID, int cantPaginas) {

	int i;
	int pudeInicializar;
	for (i = 0; i < cantPaginas; i++) {
		pudeInicializar = agregarDatosTablaEPI(PID, i);
	}

	return pudeInicializar;
}

int asignarPaginasAProceso(int PID, int cantPaginas) {

	int pudeInicializar = 0;
	if (framesDisponibles() >= cantPaginas) {
		int ultimaPagina = obtenerUltimaPaginaUtilizada(PID);

		int i;

		for (i = ultimaPagina + 1; i < cantPaginas + ultimaPagina + 1; i++) {
			pudeInicializar = agregarDatosTablaEPI(PID, i);
		}
	}
	return pudeInicializar;

}

int obtenerUltimaPaginaUtilizada( PID) {

	int i;
	int ultimaPaginaUtilizada = 0;
	int cantMarcosOcupaMemoriaAdm = ((sizeof(int*) * 3 * MARCOS) + MARCO_SIZE
			- 1) / MARCO_SIZE;
	int info[3];
	info[N_PID] = 0;
	info[N_PAGINA] = 0;

	for (i = cantMarcosOcupaMemoriaAdm; i < tablaEPI.filas; i++) {
		if (PID == tablaEPI.matriz[i][N_PID]) {
			ultimaPaginaUtilizada = tablaEPI.matriz[i][N_PAGINA];
		}
	}

	return ultimaPaginaUtilizada;;
}

int almacenarBytesPagina(int PID, int pagina, int offset, int size,
		void * buffer) //TODO: Si ya esta en cache, actualizar contenido
{
	int cantMarcosOcupaMemoriaAdm = ((sizeof(int*) * 3 * MARCOS) + MARCO_SIZE
			- 1) / MARCO_SIZE;
	int encontrePagina = 0;
	int i;

	for (i = cantMarcosOcupaMemoriaAdm; i < tablaEPI.filas && !encontrePagina;
			i++) {
		if (PID == tablaEPI.matriz[i][N_PID]
				&& pagina == tablaEPI.matriz[i][N_PAGINA]) {
			memcpy(
					bloque_Memoria + tablaEPI.matriz[i][N_FRAME] * MARCO_SIZE
							+ offset, buffer, size);
			encontrePagina = 1;
			if (estaLaPaginaEnCache(PID, pagina)) {
				actualizoCache(PID, pagina, tablaEPI.matriz[i][N_FRAME]);
			}
		}

	}
	return encontrePagina;
}
void* solicitarBytesPagina(int PID, int pagina, int offset, int size) {
	return solicitarBytesPaginaCache(PID, pagina, offset, size);
}
int finalizarPrograma(int PID) {
	quitarProgramaDeCache(PID);
	return borrarDatosTablaEPI(PID);
}

void inicializarCache() {
	int i;

	for (i = 0; i < ENTRADAS_CACHE; i++) {
		adminCache[i].tiempoEnCache = 0;
		adminCache[i].memoriaCache.nPagina = 0;
		adminCache[i].memoriaCache.PID = 0;

	}
}
void incrementarEnUnoTiempoEnCache() {
	int i;

	for (i = 0; i < ENTRADAS_CACHE; i++) {
		if (adminCache[i].memoriaCache.PID != 0)
			adminCache[i].tiempoEnCache += 1;
	}
}
void * solicitarBytesPaginaCache(int PID, int pagina, int offset, int size) {
	int i;
	void * aux;
	aux = '\0';
	int encontreEnCache = 0;
	int cantMarcosOcupaMemoriaAdm = ((sizeof(int*) * 3 * MARCOS) + MARCO_SIZE
			- 1) / MARCO_SIZE;
	int encontrePagina = 0;
	incrementarEnUnoTiempoEnCache();

	for (i = 0; i < ENTRADAS_CACHE && !encontreEnCache; i++) //Busco en cache primero
			{
		if (adminCache[i].memoriaCache.nPagina == pagina
				&& adminCache[i].memoriaCache.PID == PID) {
			aux = malloc(size);
			memcpy(aux, &adminCache[i].memoriaCache.contenido[offset], size);
			encontreEnCache = 1;
		}
	}
	//Busco en memoria y lo traigo a cache
	for (i = cantMarcosOcupaMemoriaAdm;
			i < tablaEPI.filas && !encontreEnCache && !encontrePagina; i++) {
		if (PID == tablaEPI.matriz[i][N_PID]
				&& pagina == tablaEPI.matriz[i][N_PAGINA]) {
			aux = malloc(size);
			memcpy(aux,
					bloque_Memoria + tablaEPI.matriz[i][N_FRAME] * MARCO_SIZE
							+ offset, size);
			encontrePagina = 1;
			actualizoCache(PID, pagina, tablaEPI.matriz[i][N_FRAME]);
		}
	}

	return aux;
}
void actualizoCache(int PID, int pagina, int nFrame) {
	int i;
	int encontreEnCache = 0;
	int entradasCachePorPID = 0;
	int entraMasVieja = 0;
	int primeraEntradaVacia = -1;
	for (i = 0; i < ENTRADAS_CACHE; i++) //Reemplazo contenido viejo de la cache por nuevo si ya existe la pagina
			{
		if (adminCache[i].memoriaCache.PID == 0
				&& adminCache[i].memoriaCache.nPagina == 0
				&& primeraEntradaVacia == -1) {
			primeraEntradaVacia = i;

		}
		if (adminCache[i].memoriaCache.PID == PID) {
			entradasCachePorPID++;
		}
		if (adminCache[i].memoriaCache.nPagina == pagina
				&& adminCache[i].memoriaCache.PID == PID) {

			memcpy(&adminCache[i].memoriaCache.contenido,
					bloque_Memoria + nFrame * MARCO_SIZE, MARCO_SIZE);
			adminCache[i].tiempoEnCache = 0;
		}
	}
	if (entradasCachePorPID == CACHE_X_PROCESO) {
		for (i = 0; i < ENTRADAS_CACHE; i++) {
			if (adminCache[i].memoriaCache.PID == PID) {
				if (entraMasVieja < adminCache[i].tiempoEnCache) {
					entraMasVieja = i;
				}
			}
		}
		adminCache[entraMasVieja].memoriaCache.nPagina = pagina;

		memcpy(&adminCache[entraMasVieja].memoriaCache.contenido,
				bloque_Memoria + nFrame * MARCO_SIZE, MARCO_SIZE);
		adminCache[entraMasVieja].tiempoEnCache = 0;
	}
	if (entradasCachePorPID < CACHE_X_PROCESO) {

		if (primeraEntradaVacia != -1) {

			adminCache[primeraEntradaVacia].memoriaCache.nPagina = pagina;
			adminCache[primeraEntradaVacia].memoriaCache.PID = PID;
			memcpy(&adminCache[primeraEntradaVacia].memoriaCache.contenido,
					bloque_Memoria + (nFrame * MARCO_SIZE), MARCO_SIZE);
			adminCache[primeraEntradaVacia].tiempoEnCache = 0;
		} else {

			for (i = 0; i < ENTRADAS_CACHE; i++) {

				if (entraMasVieja < adminCache[i].tiempoEnCache) {
					entraMasVieja = i;
				}
			}
			adminCache[entraMasVieja].memoriaCache.nPagina = pagina;
			adminCache[entraMasVieja].memoriaCache.PID = PID;
			memcpy(&adminCache[entraMasVieja].memoriaCache.contenido,
					bloque_Memoria + nFrame * MARCO_SIZE, MARCO_SIZE);
			adminCache[entraMasVieja].tiempoEnCache = 0;
		}
	}
}
void imprimirCache() {
	int i;
	printf("MEMORIA CACHE \n");
	for (i = 0; i < ENTRADAS_CACHE; i++) {
		printf("PID:%d NPAGINA:%d VIGENCIA_EN_CACHE:%d CONTENIDO:%s\n",
				adminCache[i].memoriaCache.PID,
				adminCache[i].memoriaCache.nPagina, adminCache[i].tiempoEnCache,
				adminCache[i].memoriaCache.contenido);
	}
}
int estaLaPaginaEnCache(int PID, int nPagina) {
	int i;
	int estaEnCache = 0;
	for (i = 0; i < ENTRADAS_CACHE && !estaEnCache; i++) {

		if (adminCache[i].memoriaCache.PID == PID
				&& adminCache[i].memoriaCache.nPagina == nPagina) {
			estaEnCache = 1;
		}
	}
	return estaEnCache;
}
int quitarProgramaDeCache(int PID) {
	int i;
	for (i = 0; i < ENTRADAS_CACHE; i++) {
		if (adminCache[i].memoriaCache.PID == PID) {
			adminCache[i].memoriaCache.PID = 0;
			adminCache[i].memoriaCache.nPagina = 0;
			adminCache[i].tiempoEnCache = 0;
		}
	}

}

void crearHilo(uint32_t * newfd) {

	uint32_t command = deserializar_int(newfd);

	connection_handler(newfd, command);

}

bool consola() {
	int salir = 0;
	while (!salir) {
		//Mensaje
		char* mensaje = (char *) malloc(sizeof(char) * 1000);
		printf("> ");
		scanf("%[^\n]s", mensaje);
		char* comando = strtok(mensaje, " ");
		char* argumento = strtok(NULL, " ");

		if (!strcmp(comando, "exit"))
			salir = 1;
		else if (!strcmp(comando, "clean"))
			system("clear");
		else if (!strcmp(comando, "dump")) {

		} else
			printf("Comando incorrecto. Pruebe run | stop | exit | clean\n");
		free(mensaje);
		limpiarBufferDeEntrada();
	}
}
void limpiarBufferDeEntrada() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {
	}
}
void connection_handler(uint32_t socket, uint32_t command) {
	switch (command) {
	case 5: { //CPU me pide una instruccion
		serializar_int(socket, MARCO_SIZE); //Le respondo que le doy memoria => voy a esperar un mensaje que me va a decir PID,Inicio y offset para poder devolver
		/*t_SerialString* PATH = malloc(sizeof(t_SerialString));
		deserializar_string(socket, PATH);
		char * PID = memcpy((void*) PID, (void*) PATH->dataString, 4); //Por convencion los primeros 4 bits son de PID
		char * posInicio = memcpy((void*) posInicio,
				(void*) PATH->dataString + 4, 4);
		char * offSet = memcpy((void*) deserializar_string,
				(void*) PATH->dataString + 8, 4);

		int IPID = convertirCharAInt(PID, 4);
		int IposInicio = convertirCharAInt(posInicio, 4);
		int IoffSet = convertirCharAInt(offSet, 4);

		/*tDato instruccion = obtenerMemoriaReducida(memoria,IPID,IposInicio,IoffSet);
		 t_SerialString * instruccionConvertida;
		 instruccionConvertida = malloc(sizeof(t_SerialString));
		 instruccionConvertida->dataString = malloc(sizeof(char*)*instruccion.tamDatos);
		 memcpy(&instruccionConvertida->dataString ,instruccion.dato,instruccion.tamDatos);
		 instruccionConvertida->sizeString=instruccion.tamDatos;

		 serializar_string(socket,instruccionConvertida);*/

		break;
	}
	/**case 10: { //KERNEL me pregunta si hay memoria
		int cantidadDePaginas = convretirBitsAPAginas();
		int hayMemoria = hayMemoria(cantidadDePaginas);
		serializar_int(socket, hayMemoria);
	}
	case 15: { //KERNEL me pregunta si hay memoria
		uint32_t PID;
		deserializar_int(socket,PID);
		char * datos;
		deserializar_string(socket,&datos);
		asignarPaginasAProceso(PID,sizeof(datos)/MARCO_SIZE);

		serializar_int(socket, hayMemoria);
	}*/
	default:
		printf("Error de comando\n");
		break;
	}
}

int convertirCharAInt(char * numero, int tamChar) {
	int i;
	int multiplicadorBase = 1;
	int numeroInt = 0;
	tamChar -= 1;
	while (tamChar >= 0) {
		numeroInt += numero[tamChar] * multiplicadorBase;
		tamChar--;
		multiplicadorBase *= 10;
	}
	return numeroInt;
}

