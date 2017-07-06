
#include "/home/utnso/Blacklist/so-funcion-hash/src/funciones.c"

#include "Memoria.h"



int main(void) {
	puts("Proceso Memoria");

	//Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);

	inicializarMemoria();

	/*inicializarPrograma(1,2);
	 inicializarPrograma(2,1);
	 inicializarPrograma(3,4);
	 almacenarBytesPagina(1,0,0,5,"hola");
	 almacenarBytesPagina(1,0,4,6," pepe");
	 almacenarBytesPagina(2,0,0,5,"chau");
	 almacenarBytesPagina(3,0,0,5,"1");
	 almacenarBytesPagina(3,1,0,5,"2");
	 almacenarBytesPagina(3,2,0,5,"3");
	 almacenarBytesPagina(3,3,0,5,"4");
	 solicitarBytesPagina(1,0,0,5);
	 solicitarBytesPagina(2,0,0,5);*/
	 /*impirmirEPIaccediendoAMemoria(0,40);
	 printf("\n");
	 imprimirCache();*/
	lanzoHiloConsolaComandos();
	sem_init(&SEM_hiloServidor,0,0);	//Iniciazilo el semaforo de la cola de PCB
	inicializoServidor();
	return EXIT_SUCCESS;

}



void inicializarMemoria() {
	CANTIDAD_DE_MARCOS = MARCOS;
	inicializarOverflow(CANTIDAD_DE_MARCOS);
	inicializarTablaEPI();
	inicializarCache();
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

	printf("quedan %d frames disponibles  \n", framesDisponibles());
	printf("");
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
	printf("\n");
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

int paginasQueOcupaProceso(int PID)
{
	int i;
	int paginas=0;
		for (i = 0; i < tablaEPI.filas; i++) {
			if (tablaEPI.matriz[i][N_PID]==PID)
			{
				paginas++;
			}

		}
		return paginas;
}

void inicializarCache() {

	adminCache = malloc (sizeof(t_cacheHandler)*ENTRADAS_CACHE);
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




	/* if( !encontreEnCache && !encontrePagina)
	{
					//Uso funcio de hash
					int es_pagina_buscada = 1;

					int pos_candidata = calcularPosicion(PID,pagina);

					if(tablaEPI.matriz[pos_candidata][N_PID]!=PID && tablaEPI.matriz[pos_candidata][N_PAGINA]!=pagina)
					{
						agregarSiguienteEnOverflow(pos_candidata, tablaEPI.matriz[pos_candidata][N_FRAME]);
						es_pagina_buscada=0;
					}

					// Inicio del caso de uso
					int pos_definitiva;
					if(es_pagina_buscada) {
						pos_definitiva = pos_candidata;
					}
					else {
						pos_definitiva = buscarEnOverflow(pos_candidata,PID,pagina);
					}
					if(tablaEPI.matriz[pos_candidata][N_PID]!=PID && tablaEPI.matriz[pos_candidata][N_PAGINA]!=pagina)
					{
						pos_definitiva=0;
					}
					for (i = pos_definitiva;i < tablaEPI.filas && !encontreEnCache && !encontrePagina; i++)
						{
							if (PID == tablaEPI.matriz[i][N_PID] && pagina == tablaEPI.matriz[i][N_PAGINA])
							{
								aux = malloc(size);
								memcpy(aux,
										bloque_Memoria + tablaEPI.matriz[i][N_FRAME] * MARCO_SIZE
												+ offset, size);
								encontrePagina = 1;
								actualizoCache(PID, pagina, tablaEPI.matriz[i][N_FRAME]);
							}
						}

	}*/


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
void borrarCache()
{
	int i;
		for (i = 0; i < ENTRADAS_CACHE; i++) {

					adminCache[i].memoriaCache.PID = 0;
					adminCache[i].memoriaCache.nPagina=0;
					adminCache[i].tiempoEnCache=0;
					adminCache[i].memoriaCache.contenido[0]=NULL;
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
	printf("\n");
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
void lanzoHiloConsolaComandos()
{

	 		pthread_t* cons = (pthread_t *) malloc(sizeof(pthread_t));
			pthread_create(cons, NULL, (void*) consola, NULL);
			free(cons);

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
		char* argumento2=strtok(NULL, " ");
		int numero;
		if (!strcmp(comando, "exit"))
			salir = 1;
		else if (!strcmp(comando, "clean"))
			system("clear");
		else if (!strcmp(comando, "size") && !strcmp(argumento, "mem"))
		{

			int fd=framesDisponibles();

			int fo=MARCOS - fd;

			printf("El tamaño de memoria es de %d Frames \n",MARCOS);
			printf("Hay %d Frames ocupados\n",fo);
			 printf("Hay %d Frames libres\n",fd);
		}
		else if (!strcmp(comando, "size") && !strcmp(argumento, "PID"))
		{
			if(argumento2 == NULL)
				{
					printf("Falta el numero de PID\n ");
				}


				if(argumento2!=NULL)
				{
					numero = convertirCharAInt(argumento2,strlen(argumento2));
					int cantPaginas=paginasQueOcupaProceso(numero);

					if(cantPaginas==0)
						printf("El proceso con PID %d no existe en memoria \n",numero);
					else{

					printf("El proceso con PID %d ocupa %d paginas de memoria \n",numero,cantPaginas);
					}
				}

		}
		else if (!strcmp(comando, "flush")&& !strcmp(argumento, "cache"))//TODO: Falta
				{
					 borrarCache();
				}
		else if (!strcmp(comando, "dump")&& !strcmp(argumento, "cache"))//TODO: Falta
		{
			 imprimirCache();
		}
		else if (!strcmp(comando, "dump")&& !strcmp(argumento, "estructMem"))//TODO: Falta
		{
			imprimirEPI();
		}
		else if (!strcmp(comando, "dump")&& !strcmp(argumento, "contMem"))//TODO: Falta
		{

			if(argumento2!=NULL)
			{
			numero = convertirCharAInt(argumento2,strlen(argumento2));
			int i;
			int paginas=0;
			int encontreProceso=0;
			char * contenido;
				for (i = 0; i < tablaEPI.filas; i++)
				{
					if (tablaEPI.matriz[i][N_PID]==numero)
					{
						encontreProceso=1;
						contenido= solicitarBytesPagina(numero,tablaEPI.matriz[i][N_PAGINA],0,MARCO_SIZE);
						printf("En el frame %d, pagina %d y PID %d el contenido es:%s \n",tablaEPI.matriz[i][N_FRAME],tablaEPI.matriz[i][N_PAGINA],numero,contenido);
					}

				}
				if (!encontreProceso)
				{
					printf("El proceso con PID %d no existe en memoria \n",numero);
				}
			}
			else
			{
				int i;
				int paginas=0;
				char * contenido;
				for (i = 0; i < tablaEPI.filas; i++)
				{
					if (tablaEPI.matriz[i][N_PID]!=0)
					{
						contenido= solicitarBytesPagina(tablaEPI.matriz[i][N_PID],tablaEPI.matriz[i][N_PAGINA],0,MARCO_SIZE);
						if (contenido==NULL)
						{
							printf("En el frame %d, pagina %d y PID %d el contenido es: PROCESO DE SO \n",tablaEPI.matriz[i][N_FRAME],tablaEPI.matriz[i][N_PAGINA],tablaEPI.matriz[i][N_PID]);
						}
						else
						{
							printf("En el frame %d, pagina %d y PID %d el contenido es:%s \n",tablaEPI.matriz[i][N_FRAME],tablaEPI.matriz[i][N_PAGINA],tablaEPI.matriz[i][N_PID],contenido);
						}
						}

				}
			}
		}

		else
			printf("Comando incorrecto. Pruebe retardo milisegundos | dump cache | dump estructMem | dump contMem PID | flush cache | size mem | size PID | exit | clean\n");

		free(mensaje);
		limpiarBufferDeEntrada();
		argumento2=NULL;


	}
	pthread_exit(NULL);
}
void limpiarBufferDeEntrada() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF) {
	}
}

/**
void crearHilo(uint32_t * newfd) {
	uint32_t command;
	uint32_t cantidad=deserializar_int(newfd);//Cantidad De Veces que voy a leer.
	int i;
	//for(i=0;i<cantidad;i++){
			command=deserializar_int(newfd);
			connection_handler(newfd, command);
		//}
}
void inicializoServidor() {
	servidor = build_server(config.PUERTO, config.CANTCONEXIONES);
	//El socket esta listo para escuchar
	if (servidor > 0) {
		printf("Servidor Memoria Escuchando\n");
	}
	while (1) {
	uint32_t newfd = accept_conexion(servidor);
	if (existeNodo(&listaConexiones,newfd))
			{
		newfd = accept_conexion(servidor);
				pthread_join(listaConexiones->hilo,0);
			}
	else{
	 		pthread_t* hilo = (pthread_t *) malloc(sizeof(pthread_t));
			pthread_create(hilo, NULL, (void*) crearHilo, (void*) &newfd);
			Insertar(&listaConexiones,hilo,newfd,0);
			//free(hilo);
	 }
	}
}*/

void hiloConexion(uint32_t * newfd) {
	uint32_t command;

		while(1){


			command=deserializar_int(newfd);


			if(command==KERNEL && kernelConectado)
			{
				while(deserializar_int(newfd))
				command=-1;
				printf("ERROR: No se puede conectar mas de un kernel");
				pthread_exit(NULL);

			}
			if(command==KERNEL && !kernelConectado)
			{

				kernelConectado=1;

			}


			if ( command>0 && command!=KERNEL && command!=BLOQUEADO)//command!=BLOQUEADO &&
			{

			connection_handler(newfd, command);

			}

			if(command<=0)
			{
			conexionesActivas--;
			 pthread_exit(NULL);
			}


			//sem_wait(&SEM_hiloServidor);

		}

}



void inicializoServidor() {

	kernelConectado=0;
	conexionesActivas=0;
	servidor = build_server(config.PUERTO, config.CANTCONEXIONES);
	//El socket esta listo para escuchar
	if (servidor > 0) {
		printf("Servidor Memoria Escuchando\n");
	}
	int yaAcepteKernel=0;
	while (1) {
	uint32_t newfd;

			newfd = accept_conexion(servidor);
			if(newfd && conexionesActivas<=config.CANTCONEXIONES){
			conexionesActivas++;
	 		pthread_t* hilo = (pthread_t *) malloc(sizeof(pthread_t));
			pthread_create(hilo, NULL, (void*) hiloConexion, (void*) newfd);
			free(hilo);
			}

	}
}
void connection_handler(uint32_t socket, uint32_t command) {

	switch (command) {

	case 100:{
		printf("Conectado con KERNEL \n");

		break;
	}
	case 3:{

			int hayMemoria=0;
			int memoriaRequerida=0;
			int cantPaginas;
			memoriaRequerida=deserializar_int(socket);
			if(memoriaRequerida%MARCO_SIZE!=0)
				cantPaginas=(memoriaRequerida/MARCO_SIZE)+1;
			else
				cantPaginas=(memoriaRequerida/MARCO_SIZE);

			if (cantPaginas<=framesDisponibles())
			{
				hayMemoria=1;
				 //printf("Me pidieron memoria, y hay.");
			}
			serializar_int(socket,hayMemoria);
			break;

		}
	case 4:{
		        //printf("Voy a reservar ese espacio\n");
		        t_SerialString* cadena = malloc(sizeof(t_SerialString));
				int largoCadena;
				int cantPaginas;
				int i;

				int PID=deserializar_int(socket);
				//printf("El pid que voy a asignar es: %d\n",PID);
				deserializar_string(socket,cadena);


				largoCadena=cadena->sizeString;

				//printf("El largo de la cadena es: %d\n",largoCadena);



				if(largoCadena>0)
				{
					if(largoCadena%MARCO_SIZE!=0)
						cantPaginas=(largoCadena/MARCO_SIZE)+1;
					else
						cantPaginas=(largoCadena/MARCO_SIZE);

					inicializarPrograma(PID,cantPaginas);


					for (i=0;i<cantPaginas;i++)
						almacenarBytesPagina(PID,i,0,MARCO_SIZE,cadena->dataString+i*MARCO_SIZE);
				}
				free(cadena);
				serializar_int(socket,cantPaginas);
				//solicitarBytesPagina(PID,0,0,largoCadena);
				//printf("La cadena es: %s\n",cadena->dataString);
				break;

			}
	case 5: { //CPU me pide una instruccion
		serializar_int(socket, MARCO_SIZE); //Le respondo que le doy memoria => voy a esperar un mensaje que me va a decir PID,Inicio y offset para poder devolver
		printf("Conectado con CPU \n");

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


	default:{
		pthread_exit(NULL);
		 //printf("Ningun Comando\n");
		// sem_wait(&SEM_hiloServidor);
		break;
	}

	}

	return;
}

int convertirCharAInt(char * numero, int tamChar) {
	int i;
	int multiplicadorBase = 1;
	int numeroInt = 0;
	tamChar -= 1;
	while (tamChar >= 0) {
		numeroInt += (numero[tamChar] - '0') * multiplicadorBase;
		tamChar--;
		multiplicadorBase *= 10;
	}
	return numeroInt;
}
