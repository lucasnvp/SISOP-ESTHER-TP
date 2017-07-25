#include "Memoria.h"

int main(void) {
	puts("Proceso Memoria");

	//Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);

	inicializarMemoria();

	/*inicializarPrograma(1, 1);
	inicializarPrograma(2, 1);
	inicializarPrograma(3, 4);
	almacenarBytesPagina(1, 0, 0, 5, "hola");
	almacenarBytesPagina(1, 0, 4, 6, " pepe");
	almacenarBytesPagina(2, 0, 0, 5, "chau");
	almacenarBytesPagina(3, 0, 0, 5, "1");
	almacenarBytesPagina(3, 1, 0, 5, "2");
	almacenarBytesPagina(3, 2, 0, 5, "3");
	almacenarBytesPagina(3, 3, 0, 5, "4");
	printf("\n");
	imprimirCache();*/

	lanzoHiloConsolaComandos();
	sem_init(&SEM_hiloServidor, 0, 0);
	inicializoServidor();

	return EXIT_SUCCESS;
}

void inicializarMemoria() {

	inicializarTablaEPI();
	inicializarCache();
	bloque_Memoria = malloc(config.MARCOS * config.MARCO_SIZE);

	uint32_t i;
	uint32_t cantMarcosOcupaMemoriaAdm = ((sizeof(uint32_t) * 3 * config.MARCOS) + config.MARCO_SIZE - 1) / config.MARCO_SIZE;
	uint32_t aux[3];
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
	for (i = 0; i < config.MARCOS - cantMarcosOcupaMemoriaAdm; i++) {
		aux[N_FRAME] = i + cantMarcosOcupaMemoriaAdm;
		memcpy((bloque_Memoria + cantMarcosOcupaMemoriaAdm * sizeof(aux)) + (i * sizeof(aux)), &aux, sizeof(aux)); //TODO: no se esta copiando bien con ceros las cosas
	}

	for (i = 0; i < cantMarcosOcupaMemoriaAdm; i++)
		agregarDatosTablaEPI(-1, i);
}

void inicializarTablaEPI() {
	tablaEPI.filas = 0;
	tablaEPI.matriz = malloc(sizeof(tablaEPI.matriz));
}

uint32_t agregarDatosTablaEPI(uint32_t PID, uint32_t nPagina) {
	uint32_t pudeEscribirTabla = 0;
	uint32_t i;
//uint32_t cantMarcosOcupaMemoriaAdm = ((sizeof(uint32_t*) * 3 * config.MARCOS) + config.MARCO_SIZE - 1) / config.MARCO_SIZE;
	uint32_t info[3];
	info[N_PID] = PID;
	info[N_PAGINA] = nPagina;

    //Busco si hay marcos Libres
	for (i = 0; i < tablaEPI.filas && !pudeEscribirTabla; i++) {
		if (tablaEPI.matriz[i][N_PID] == 0 && tablaEPI.matriz[i][N_PAGINA] == 0) {
			memcpy(&tablaEPI.matriz[i][N_PID], &PID, sizeof(PID));
			memcpy(&tablaEPI.matriz[i][N_PAGINA], &nPagina, sizeof(nPagina));

			//Copio En memoria la info de la estructura administrativa.
			info[N_FRAME] = tablaEPI.matriz[i][N_FRAME];
			memcpy(bloque_Memoria + (info[N_FRAME]) * sizeof(info), &info, sizeof(info));

			pudeEscribirTabla = 1;
		}
	}

	if (!pudeEscribirTabla && tablaEPI.filas < config.MARCOS) { //Esto lo voy a tener que borrar
		uint32_t **aux;
		aux = realloc(tablaEPI.matriz, sizeof(uint32_t) * (tablaEPI.filas + 1));

		if (aux != '\0') {
			aux[tablaEPI.filas] = (uint32_t*) malloc(sizeof(aux[tablaEPI.filas]) * 3);
			memcpy(&tablaEPI.matriz, &aux, sizeof(aux));

			memcpy(&tablaEPI.matriz[tablaEPI.filas][N_FRAME], &tablaEPI.filas, sizeof(tablaEPI.filas));
			memcpy(&tablaEPI.matriz[tablaEPI.filas][N_PID], &PID, sizeof(PID));
			memcpy(&tablaEPI.matriz[tablaEPI.filas][N_PAGINA], &nPagina, sizeof(nPagina));

			//Copio En memoria la info de la estructura administrativa.
			info[N_FRAME] = tablaEPI.matriz[tablaEPI.filas][N_FRAME];
			memcpy(bloque_Memoria + (info[N_FRAME]) * sizeof(info), &info, sizeof(info));

			tablaEPI.filas += 1;
			pudeEscribirTabla = 1;
		}
	}

	return pudeEscribirTabla;
}

uint32_t borrarDatosTablaEPI(uint32_t PID) {

	uint32_t i;

	uint32_t info[3];
	info[N_PID] = 0;
	info[N_PAGINA] = 0;

	uint32_t pudeBorrar = 0;

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

uint32_t framesDisponibles() {

	uint32_t i;
	uint32_t framesDisponibles = 0;
//Busco si hay marcos Libres
	for (i = 0; i < tablaEPI.filas; i++) {
		if (tablaEPI.matriz[i][N_PID] == 0
				&& tablaEPI.matriz[i][N_PAGINA] == 0) {
			framesDisponibles++;
		}
	}
	return (config.MARCOS - tablaEPI.filas + framesDisponibles);
}

void imprimirEPI() {
	uint32_t i;
	for (i = 0; i < tablaEPI.filas; i++) {
		printf("#FRAME:%d  PID:%d  #PAGINA:%d \n", tablaEPI.matriz[i][N_FRAME],
				tablaEPI.matriz[i][N_PID], tablaEPI.matriz[i][N_PAGINA]);
	}

	printf("quedan %d frames disponibles\n\n", framesDisponibles());
}

void impirmirEPIaccediendoAMemoria(uint32_t inicio, uint32_t fin) {
	uint32_t a[3];
	uint32_t i;
	if (fin < config.MARCOS) {
		for (i = inicio; i <= fin; i++) {
			memcpy(&a, bloque_Memoria + 12 * i, sizeof(uint32_t) * 3);

			printf("\nIMPRIMIENDO DESDE MEMORIA EPI FRAME:%d PID:%d PAGINA:%d",
					a[0], a[1], a[2]);
		}
	}
	printf("\n");
}
uint32_t inicializarPrograma(uint32_t PID, uint32_t cantPaginas) {

	uint32_t i;
	uint32_t pudeInicializar;
	for (i = 0; i < cantPaginas; i++) {
		pudeInicializar = agregarDatosTablaEPI(PID, i);
	}

	return pudeInicializar;
}

uint32_t asignarPaginasAProceso(uint32_t PID, uint32_t cantPaginas) {

	uint32_t pudeInicializar = 0;
	if (framesDisponibles() >= cantPaginas) {
		uint32_t ultimaPagina = obtenerUltimaPaginaUtilizada(PID);

		uint32_t i;

		for (i = ultimaPagina + 1; i < cantPaginas + ultimaPagina + 1; i++) {
			pudeInicializar = agregarDatosTablaEPI(PID, i);
		}
	}
	return pudeInicializar;

}

uint32_t obtenerUltimaPaginaUtilizada(uint32_t PID) {

	uint32_t i;
	uint32_t ultimaPaginaUtilizada = 0;
	uint32_t cantMarcosOcupaMemoriaAdm = ((sizeof(uint32_t*) * 3 * config.MARCOS) + config.MARCO_SIZE - 1) / config.MARCO_SIZE;
//uint32_t info[3];
//info[N_PID] = 0;
//info[N_PAGINA] = 0;

	for (i = cantMarcosOcupaMemoriaAdm; i < tablaEPI.filas; i++) {
		if (PID == tablaEPI.matriz[i][N_PID]) {
			ultimaPaginaUtilizada = tablaEPI.matriz[i][N_PAGINA];
		}
	}

	return ultimaPaginaUtilizada;;
}

uint32_t almacenarBytesPagina(uint32_t PID, uint32_t pagina, uint32_t offset, uint32_t size,
		void * buffer) //TODO: Si ya esta en cache, actualizar contenido
{
//uint32_t cantMarcosOcupaMemoriaAdm = ((sizeof(uint32_t*) * 3 * config.MARCOS) + config.MARCO_SIZE - 1) / config.MARCO_SIZE;
	uint32_t encontrePagina = 0;
	uint32_t frame;

	frame = obtenerFrame(PID, pagina);

	memcpy(
			bloque_Memoria + tablaEPI.matriz[frame][N_FRAME] * config.MARCO_SIZE
					+ offset, buffer, size);
	encontrePagina = 1;
	if (estaLaPaginaEnCache(PID, pagina)) {
		actualizoCache(PID, pagina, tablaEPI.matriz[frame][N_FRAME]);
	}

	return encontrePagina;
}
void* solicitarBytesPagina(uint32_t PID, uint32_t pagina, uint32_t offset, uint32_t size) {
	return solicitarBytesPaginaCache(PID, pagina, offset, size);
}
uint32_t finalizarPrograma(uint32_t PID) {
	quitarProgramaDeCache(PID);
	return borrarDatosTablaEPI(PID);
}

uint32_t paginasQueOcupaProceso(uint32_t PID) {
	uint32_t i;
	uint32_t paginas = 0;
	for (i = 0; i < tablaEPI.filas; i++) {
		if (tablaEPI.matriz[i][N_PID] == PID) {
			paginas++;
		}

	}
	return paginas;
}

void inicializarCache() {
	adminCache = malloc(sizeof(t_cacheHandler) * config.ENTRADAS_CACHE);
	uint32_t i;
	for (i = 0; i < config.ENTRADAS_CACHE; i++) {
		adminCache[i].tiempoEnCache = 0;
		adminCache[i].memoriaCache.nPagina = 0;
		adminCache[i].memoriaCache.PID = 0;
		//TODO mepa que aca falta inicializar adminCache[i].memoriaCache.contenido que es un char*
	}
}

void incrementarEnUnoTiempoEnCache() {
	uint32_t i;

	for (i = 0; i < config.ENTRADAS_CACHE; i++) {
		if (adminCache[i].memoriaCache.PID != 0)
			adminCache[i].tiempoEnCache += 1;
	}
}

void * solicitarBytesPaginaCache(uint32_t PID, uint32_t pagina, uint32_t offset, uint32_t size) {
	uint32_t i;
	void * aux;
	aux = '\0';
	uint32_t encontreEnCache = 0;
//uint32_t cantMarcosOcupaMemoriaAdm = ((sizeof(uint32_t*) * 3 * config.MARCOS) + config.MARCO_SIZE - 1) / config.MARCO_SIZE;
	uint32_t encontrePagina = 0;
	incrementarEnUnoTiempoEnCache();

	for (i = 0; i < config.ENTRADAS_CACHE && !encontreEnCache; i++) //Busco en cache primero
			{
		if (adminCache[i].memoriaCache.nPagina == pagina
				&& adminCache[i].memoriaCache.PID == PID) {
			aux = malloc(size);
			memcpy(aux, &adminCache[i].memoriaCache.contenido[offset], size);
			encontreEnCache = 1;
		}
	}

	if (!encontreEnCache) {
		uint32_t frame = obtenerFrame(PID, pagina);
		aux = malloc(size);
		memcpy(aux,
				bloque_Memoria + tablaEPI.matriz[frame][N_FRAME] * config.MARCO_SIZE + offset, size);
		encontrePagina = 1;
		actualizoCache(PID, pagina, tablaEPI.matriz[frame][N_FRAME]);
	}
	/*printf("el frame traido por la funcion es:%d\n",frame);*/

//Busco en memoria y lo traigo a cache VIEJO
	/*for (i = cantMarcosOcupaMemoriaAdm;
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
	 }*/

	return aux;
}
void actualizoCache(uint32_t PID, uint32_t pagina, uint32_t nFrame) {
	uint32_t i;
//uint32_t encontreEnCache = 0;
	uint32_t entradasCachePorPID = 0;
	uint32_t entraMasVieja = 0;
	uint32_t primeraEntradaVacia = -1;
	for (i = 0; i < config.ENTRADAS_CACHE; i++) //Reemplazo contenido viejo de la cache por nuevo si ya existe la pagina
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

			memcpy(&adminCache[i].memoriaCache.contenido, bloque_Memoria + nFrame * config.MARCO_SIZE, config.MARCO_SIZE);
			adminCache[i].tiempoEnCache = 0;
		}
	}
	if (entradasCachePorPID == CACHE_X_PROCESO) {
		for (i = 0; i < config.ENTRADAS_CACHE; i++) {
			if (adminCache[i].memoriaCache.PID == PID) {
				if (entraMasVieja < adminCache[i].tiempoEnCache) {
					entraMasVieja = i;
				}
			}
		}
		adminCache[entraMasVieja].memoriaCache.nPagina = pagina;

		memcpy(&adminCache[entraMasVieja].memoriaCache.contenido, bloque_Memoria + nFrame * config.MARCO_SIZE, config.MARCO_SIZE);
		adminCache[entraMasVieja].tiempoEnCache = 0;
	}
	if (entradasCachePorPID < CACHE_X_PROCESO) {

		if (primeraEntradaVacia != -1) {

			adminCache[primeraEntradaVacia].memoriaCache.nPagina = pagina;
			adminCache[primeraEntradaVacia].memoriaCache.PID = PID;
			memcpy(&adminCache[primeraEntradaVacia].memoriaCache.contenido,	bloque_Memoria + (nFrame * config.MARCO_SIZE), config.MARCO_SIZE);
			adminCache[primeraEntradaVacia].tiempoEnCache = 0;
		} else {

			for (i = 0; i < config.ENTRADAS_CACHE; i++) {

				if (entraMasVieja < adminCache[i].tiempoEnCache) {
					entraMasVieja = i;
				}
			}
			adminCache[entraMasVieja].memoriaCache.nPagina = pagina;
			adminCache[entraMasVieja].memoriaCache.PID = PID;
			memcpy(&adminCache[entraMasVieja].memoriaCache.contenido, bloque_Memoria + nFrame * config.MARCO_SIZE, config.MARCO_SIZE);
			adminCache[entraMasVieja].tiempoEnCache = 0;
		}
	}
}
void borrarCache() {
	uint32_t i;
	for (i = 0; i < config.ENTRADAS_CACHE; i++) {

		adminCache[i].memoriaCache.PID = 0;
		adminCache[i].memoriaCache.nPagina = 0;
		adminCache[i].tiempoEnCache = 0;
		adminCache[i].memoriaCache.contenido = NULL;
	}
}

void imprimirCache() {
	uint32_t i;
	printf("MEMORIA CACHE \n");
	for (i = 0; i < config.ENTRADAS_CACHE; i++) {
		printf("PID:%d NPAGINA:%d VIGENCIA_EN_CACHE:%d CONTENIDO:%s\n",
				adminCache[i].memoriaCache.PID,
				adminCache[i].memoriaCache.nPagina, adminCache[i].tiempoEnCache,
				adminCache[i].memoriaCache.contenido);
	}
	printf("\n");
}
uint32_t estaLaPaginaEnCache(uint32_t PID, uint32_t nPagina) {
	uint32_t i;
	uint32_t estaEnCache = 0;
	for (i = 0; i < config.ENTRADAS_CACHE && !estaEnCache; i++) {

		if (adminCache[i].memoriaCache.PID == PID
				&& adminCache[i].memoriaCache.nPagina == nPagina) {
			estaEnCache = 1;
		}
	}
	return estaEnCache;
}

void quitarProgramaDeCache(uint32_t PID) {
	uint32_t i;
	for (i = 0; i < config.ENTRADAS_CACHE; i++) {
		if (adminCache[i].memoriaCache.PID == PID) {
			adminCache[i].memoriaCache.PID = 0;
			adminCache[i].memoriaCache.nPagina = 0;
			adminCache[i].tiempoEnCache = 0;
		}
	}
}

void lanzoHiloConsolaComandos() {
	pthread_t* cons = (pthread_t *) malloc(sizeof(pthread_t));
	pthread_create(cons, NULL, (void*) consola, NULL);
	//free(cons);
}

void consola() {
//Mensaje
	char* mensaje = (char *) malloc(sizeof(char) * 1000);
	uint32_t salir = 0;

	while (!salir) {
		printf("> ");
		scanf("%[^\n]s", mensaje);
		if (strcmp(mensaje, "")) {
			char* comando = strtok(mensaje, " ");
			char* argumento = strtok(NULL, " ");
			char* argumento2 = strtok(NULL, " ");
			uint32_t numero;
			if (!strcmp(comando, "exit"))
				salir = 1;
			else if (!strcmp(comando, "clean"))
				system("clear");
			else if (!strcmp(comando, "size") && !strcmp(argumento, "mem")) {

				uint32_t fd = framesDisponibles();

				uint32_t fo = config.MARCOS - fd;

				printf("El tamaño de memoria es de %d Frames \n", config.MARCOS);
				printf("Hay %d Frames ocupados\n", fo);
				printf("Hay %d Frames libres\n", fd);
			} else if (!strcmp(comando, "size") && !strcmp(argumento, "PID")) {
				if (argumento2 == NULL) {
					printf("Falta el numero de PID\n ");
				}

				if (argumento2 != NULL) {
//					numero = convertirCharAInt(argumento2, strlen(argumento2));
					numero = atoi(argumento2);
					uint32_t cantPaginas = paginasQueOcupaProceso(numero);

					if (cantPaginas == 0)
						printf("El proceso con PID %d no existe en memoria \n",
								numero);
					else {

						printf(
								"El proceso con PID %d ocupa %d paginas de memoria \n",
								numero, cantPaginas);
					}
				}

			} else if (!strcmp(comando, "flush") && !strcmp(argumento, "cache")) //TODO: Falta
					{
				borrarCache();
			} else if (!strcmp(comando, "dump") && !strcmp(argumento, "cache")) //TODO: Falta
					{
				imprimirCache();
			} else if (!strcmp(comando, "dump")
					&& !strcmp(argumento, "estructMem")) //TODO: Falta
							{
				imprimirEPI();
			} else if (!strcmp(comando, "dump")
					&& !strcmp(argumento, "contMem")) //TODO: Falta
							{

				if (argumento2 != NULL) {
//					numero = convertirCharAInt(argumento2, strlen(argumento2));
					numero = atoi(argumento2);
					uint32_t i;
//uint32_t paginas = 0;
					uint32_t encontreProceso = 0;
					char * contenido;
					for (i = 0; i < tablaEPI.filas; i++) {
						if (tablaEPI.matriz[i][N_PID] == numero) {
							encontreProceso = 1;
							contenido = solicitarBytesPagina(numero,
									tablaEPI.matriz[i][N_PAGINA], 0,
									config.MARCO_SIZE);
							printf(
									"En el frame %d, pagina %d y PID %d el contenido es:%s \n",
									tablaEPI.matriz[i][N_FRAME],
									tablaEPI.matriz[i][N_PAGINA], numero,
									contenido);
						}

					}
					if (!encontreProceso) {
						printf("El proceso con PID %d no existe en memoria \n",
								numero);
					}
				} else {
					uint32_t i;
//uint32_t paginas = 0;
					char * contenido;
					uint32_t cantMarcosOcupaMemoriaAdm = ((sizeof(uint32_t*) * 3 * config.MARCOS) + config.MARCO_SIZE - 1) / config.MARCO_SIZE;

					for (i = 0; i < cantMarcosOcupaMemoriaAdm; i++) {

						printf(
								"En el frame %d, pagina %d y PID %d el contenido es: MEMORIA ADMINISTRATIVA \n",
								tablaEPI.matriz[i][N_FRAME],
								tablaEPI.matriz[i][N_PAGINA],
								tablaEPI.matriz[i][N_PID]);
					}
					for (i = cantMarcosOcupaMemoriaAdm; i < tablaEPI.filas;
							i++) {
						contenido = solicitarBytesPagina(
								tablaEPI.matriz[i][N_PID],
								tablaEPI.matriz[i][N_PAGINA], 0, config.MARCO_SIZE);
						printf(
								"En el frame %d, pagina %d y PID %d el contenido es:%s \n",
								tablaEPI.matriz[i][N_FRAME],
								tablaEPI.matriz[i][N_PAGINA],
								tablaEPI.matriz[i][N_PID], contenido);
					}

				}
			}

			else
				printf(
						"Comando incorrecto. Pruebe retardo milisegundos | dump cache | dump estructMem | dump contMem PID | flush cache | size mem | size PID | exit | clean\n");

			argumento2 = NULL;
		}
		strcpy(mensaje, "");
		limpiarBufferDeEntrada();
	}
	free(mensaje);
	pthread_exit(NULL);
}

void limpiarBufferDeEntrada() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {
	}
}

void hiloConexion(uint32_t * newfd) {
	uint32_t command;

	while (1) {

		command = deserializar_int(newfd);

		if (command == KERNEL && kernelConectado) {
			while (deserializar_int(newfd))
				command = -1;
			printf("ERROR: No se puede conectar mas de un kernel");
			pthread_exit(NULL);

		}
		if (command == KERNEL && !kernelConectado) {

			kernelConectado = 1;

		}

		if (command > 0 && command != KERNEL && command != BLOQUEADO) //command!=BLOQUEADO &&
		{

			connection_handler(newfd, command);

		}

		if (command <= 0) {
			conexionesActivas--;
			pthread_exit(NULL);
		}

		//sem_wait(&SEM_hiloServidor);

	}

}

void inicializoServidor() {
	kernelConectado = 0;
	conexionesActivas = 0;
	servidor = build_server(config.PUERTO, config.CANTCONEXIONES);

	//El socket esta listo para escuchar
	if (servidor > 0) {
		printf("Servidor Memoria Escuchando\n");
	}
//uint32_t yaAcepteKernel = 0;
	while (1) {
		uint32_t newfd;

		newfd = accept_conexion(servidor);
		if (newfd && conexionesActivas <= config.CANTCONEXIONES) {
			conexionesActivas++;
			pthread_t* hilo = (pthread_t *) malloc(sizeof(pthread_t));
			pthread_create(hilo, NULL, (void*) hiloConexion, (void*) newfd);
			//free(hilo);
		}
	}
}

void connection_handler(uint32_t socket, uint32_t command) {

	switch (command) {

	case 100: {
		printf("Conectado con KERNEL \n");

		break;
	}
	case 3: {

		uint32_t hayMemoria = 0;
		uint32_t memoriaRequerida = 0;
		uint32_t cantPaginas;
		memoriaRequerida = deserializar_int(socket);
		if (memoriaRequerida % config.MARCO_SIZE != 0)
			cantPaginas = (memoriaRequerida / config.MARCO_SIZE) + 1;
		else
			cantPaginas = (memoriaRequerida / config.MARCO_SIZE);

		if (cantPaginas <= framesDisponibles()) {
			hayMemoria = 1;
			//printf("Me pidieron memoria, y hay.");
		}
		serializar_int(socket, hayMemoria);
		break;

	}
	case 4: {
		//printf("Voy a reservar ese espacio\n");
		t_SerialString* cadena = malloc(sizeof(t_SerialString));
		uint32_t largoCadena;
		uint32_t cantPaginas;
		uint32_t i;

		uint32_t PID = deserializar_int(socket);
		//printf("El pid que voy a asignar es: %d\n",PID);
		deserializar_string(socket, cadena);

		largoCadena = cadena->sizeString;

		//printf("El largo de la cadena es: %d\n",largoCadena);

		if (largoCadena > 0) {
			if (largoCadena % config.MARCO_SIZE != 0)
				cantPaginas = (largoCadena / config.MARCO_SIZE) + 1;
			else
				cantPaginas = (largoCadena / config.MARCO_SIZE);

			inicializarPrograma(PID, cantPaginas);

			for (i = 0; i < cantPaginas; i++)
				almacenarBytesPagina(PID, i, 0, config.MARCO_SIZE,
						cadena->dataString + i * config.MARCO_SIZE);
		}
		free(cadena);
		serializar_int(socket, cantPaginas);
		//solicitarBytesPagina(PID,0,0,largoCadena);
		//printf("La cadena es: %s\n",cadena->dataString);
		break;

	}
	case 5: { //CPU me pide una instruccion
		serializar_int(socket, config.MARCO_SIZE); //Le respondo que le doy memoria => voy a esperar un mensaje que me va a decir PID,Inicio y offset para poder devolver
		printf("Conectado con CPU \n\n> ");
		fflush(stdout);

		break;
	}

	case 6: {

		t_pedido_memoria *nuevoPedido = deserializar_pedido_memoria(servidor);

		char* instruccion = solicitarBytesPagina(nuevoPedido->id,
				nuevoPedido->pagina, nuevoPedido->offset, nuevoPedido->size);

		t_SerialString *envio;

		envio->dataString = instruccion;
		envio->sizeString = strlen(instruccion);

		serializar_string(servidor, envio);

		break;
	}

	default: {
		pthread_exit(NULL);
		//printf("Ningun Comando\n");
		// sem_wait(&SEM_hiloServidor);
		break;
	}

	}

	return;
}

//uint32_t convertirCharAInt(char * numero, uint32_t tamChar) {
//	uint32_t i;
//	uint32_t multiplicadorBase = 1;
//	uint32_t numeroInt = 0;
//	tamChar -= 1;
//	while (tamChar >= 0) {
//		numeroInt += (numero[tamChar] - '0') * multiplicadorBase;
//		tamChar--;
//		multiplicadorBase *= 10;
//	}
//	return numeroInt;
//}

uint32_t hash(uint32_t x, uint32_t y) {

	x--;

	uint32_t r = ((x + y) * (x + y + 1)) / 4;

	while (r > config.MARCOS)

		r -= config.MARCOS;

	return r;

}

uint32_t obtenerFrame(uint32_t PID, uint32_t pag) {

	uint32_t frame = hash(PID, pag);

	while (tablaEPI.matriz[frame][N_PID] != PID
			|| tablaEPI.matriz[frame][N_PAGINA] != pag)
		frame++;

	return frame;

}
