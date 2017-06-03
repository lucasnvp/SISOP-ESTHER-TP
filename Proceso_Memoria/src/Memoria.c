#include "Memoria.h"


int main(void){
    puts("Proceso Memoria");

    //Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);

	//Creacion del servidor
	uint32_t servidor = build_server(config.PUERTO, config.CANTCONEXIONES);

	//El socket esta listo para escuchar
	if(servidor > 0){
		printf("Servidor Memoria Escuchando\n");
	}

	// bucle principal
	while(1) {
		// acepto una nueva conexion
		uint32_t newfd = accept_conexion(servidor);
		if(newfd){
			pthread_t* hiloConsola = (pthread_t *) malloc(sizeof(pthread_t));
			pthread_create(hiloConsola, NULL, (void*) crearHilo, (void*) &newfd);

		}
	}
	return EXIT_SUCCESS;
}


void crearHilo(uint32_t * newfd){


	uint32_t command = deserializar_int(newfd);


	connection_handler(newfd,command);

}


void * nuevoBloqueDeMemoria()//Inicializo memora
	{
		void * memoria = malloc(config.MARCOS*config.MARCO_SIZE);

	    dividoMemoria(memoria); //Divido el cant de marcos y marcossize con indicadores de inicio y fin de pagina

	    inicializoEPI();

		return memoria;
	}

	void * agregarDatosABloqueDeMemoria(void * memoria, tDato d)
	{
		/*Verifico condiciones:: 1 - Existe bloque que puede alojar todo

	                             2 - Se puede alojar todo en diferentes bloques

	                             3 - No hay suficiente espacio */


	    int i = 0;
	    int j;
	    int tamDatos = d.tamDatos;

	    void * datos = d.dato;

	    int dirEspacioDisponible;
	    int cuantoPuedoCopiar;
	    int espacioDisponible;

	    int tamMetaAntesModificacion;
	    metadata metaData;

	    int posMem;
	    pNodo nodo;

	    int yaCopie=FALSE;
	    int yaBorre=FALSE;

	    tamDatos = d.tamDatos;
	    datos = d.dato;
	    while (nodo && tamDatos>0)//avanzo segun la lista de memoria disponible
	    {

	        nodo = obtenerPrimerNodo(listaMemoriaLibre);
	        metaData = obtengoHeapMetadata(memoria,nodo->posMemHeap);
	        tamMetaAntesModificacion = metaData.size;
	        posMem=nodo->posMemHeap;


	        if (puedoAlojarDatos(memoria,tamDatos) && tamDatos>0)
	        {

	            if(tamDatos<=metaData.size) cuantoPuedoCopiar =tamDatos;
	            if(tamDatos>metaData.size) cuantoPuedoCopiar =metaData.size;

	            metaData.isFree = FALSE;
	            metaData.size = cuantoPuedoCopiar;
	            memcpy(nodo->posMemHeap,&metaData,sizeof(metaData));

	            memcpy((nodo->posMemHeap)+sizeof(metaData),datos,cuantoPuedoCopiar); //Copio los datos que me llegan

	          int  nPagina=(((nodo->posMemHeap)+sizeof(metaData)- memoria)/config.MARCO_SIZE); //Aca antes iba 512
	          if (nPagina==-1) nPagina+=1;


	            agregaDatoEPI(d.pid,nPagina,nodo);

	             //Actualizo el metadata de memoria a ocupado y los bytes ocuados
	            Insertar(&listaMemoriaOcupada,nodo->posMemHeap,d.pid);



	            Borrar(&listaMemoriaLibre,nodo->posMemHeap);





	           for (j=0;j<7 && yaBorre==FALSE;j++)//validacion para que no se pise con otros heapMetadatas. siguiente al copiado de datos
	            {

	                if (existeNodo(&listaMemoriaOcupada,j+posMem+cuantoPuedoCopiar+sizeof(metaData))) //si esxiste algun heap desde donde copio los datos en los proximos 8 bytes no agrega ningun heap ya que pisaria el otro. Queda memoria basura
	                {
	                    yaBorre=TRUE;
	                }
	            }

	            if (yaBorre==FALSE)
	            {
	                metaData.isFree=TRUE;
	                metaData.size = tamMetaAntesModificacion - cuantoPuedoCopiar - sizeof(metaData);
	                memcpy(posMem+sizeof(metaData)+cuantoPuedoCopiar,&metaData,sizeof(metaData));


	            Insertar(&listaMemoriaLibre,posMem+sizeof(metaData)+cuantoPuedoCopiar,0);


	            yaBorre=TRUE;

	            }



	            }

	        datos=datos + cuantoPuedoCopiar;
	        tamDatos=tamDatos - cuantoPuedoCopiar;
	        yaCopie=FALSE;
	        yaBorre=FALSE;

	    }

	return memoria;

	    }

	tDato obtenerMemoria(void * memoria,int PID){
	    int i=0;
	    void *programa;
	    metadata heap;
	    int tamPrograma=0;
	    int tamProgramaAnterior=0;
	    tDato dato;
	    for(i;i<EPI.filas;i++){
	        if (EPI.matriz[i][1]==PID){

	            heap = obtengoHeapMetadata(memoria,EPI.matriz[i][3]);

	            tamProgramaAnterior=tamPrograma;
	            tamPrograma+=heap.size;
	            programa = calloc(programa,tamPrograma*sizeof(void*));//TODO aca va un realloc pero no funciona, fijarse que no reserva espacio
	            if(programa!='\0'){

	           memmove((void *)(programa+tamProgramaAnterior),(void *)(EPI.matriz[i][3]+sizeof(heap)),heap.size);

	            }
	        }
	    }
	    dato.dato=programa; // TODO aca hay que hacer un memcpy
	    dato.tamDatos=tamPrograma;

	    return dato;
	}
	tDato obtenerMemoriaReducida(void * memoria,int PID,int posArranque,int desplazamiento){
	    int i=0;
	    void *programa;
	    metadata heap;

	    tDato dato;
	    for(i;i<EPI.filas;i++){
	        if (EPI.matriz[i][1]==PID){

	            heap = obtengoHeapMetadata(memoria,EPI.matriz[i][3]);



	            programa = calloc(programa,desplazamiento*sizeof(void*));
	            if(programa!='\0'){
	           memmove((void *)(programa),(void *)(EPI.matriz[i][3]+sizeof(heap)+posArranque),desplazamiento);

	            }
	        }
	    }
	    dato.dato=programa; // aca hay que hacer un memcpy
	    dato.tamDatos=desplazamiento;
	    return dato;


	}

	metadata obtengoHeapMetadata(void * pagina,int posicionDeArranque){
	   metadata b;

	    void *p = (void *)posicionDeArranque;

	    memcpy(&b,p,8);

	    return b;
	}


	void dividoMemoria(void * memoria){

	    int i=0;

	    while (i<(config.MARCOS)){

		metadata inicio;
		inicio.size=config.MARCO_SIZE-(sizeof(metadata)*2);
		inicio.isFree=TRUE;

		memcpy(memoria + (i*config.MARCO_SIZE),&inicio,sizeof(metadata));
	    Insertar(&listaMemoriaLibre,((memoria + (i*config.MARCO_SIZE))),0);//Agrega a listaDeMemoriaLibrea posiciones de los heaps con memoria libre

		metadata fin;
	    fin.size=0;
	    fin.isFree=TRUE;
	    memcpy(memoria+(((i+1)*config.MARCO_SIZE)-sizeof(metadata)),&fin,sizeof(metadata));
	    Insertar(&listaMemoriaOcupada,memoria+((i+1)*config.MARCO_SIZE)-sizeof(metadata),0);
	    i++;
	    }

	}

	bool puedoAlojarDatosEnUnaPagina(metadata memoria, int tamDatos){

	    return memoria.isFree && memoria.size>=tamDatos;

	}

	bool puedoAlojarDatos(void * memoria, int tamDatos){

	    pNodo n = obtenerPrimerNodo(listaMemoriaLibre);
	    int sumador=0;
	    metadata meta;
	    while (n){
	        meta = obtengoHeapMetadata(memoria,n->posMemHeap);
	        sumador+=meta.size;
	        n=n->siguiente;
	    }
	    if (sumador>=tamDatos+sizeof(meta)) return TRUE;

	    return FALSE;
	}
	void inicializoEPI(){

	EPI.filas=0;
	EPI.columnas=3;


	}
	void agregaDatoEPI(int PID,int nPagina,pNodo nodo){

	    int *aux;
	    aux = (int**)realloc(EPI.matriz,sizeof(int)*(EPI.filas+1));

	    if (aux!='\0'){
	    //memcpy(EPI.matriz,&aux,sizeof(int)*(EPI.filas+1));
	    aux[EPI.filas] =(int**) malloc(sizeof(int)*4);
	    EPI.matriz=aux;
	    memcpy(&EPI.matriz[EPI.filas][0],&EPI.filas,sizeof(EPI.filas));

	    memcpy(&EPI.matriz[EPI.filas][1],&PID,sizeof(PID));
	    memcpy(&EPI.matriz[EPI.filas][2],&nPagina,sizeof(nPagina));
	    memcpy(&EPI.matriz[EPI.filas][3],&nodo->posMemHeap,sizeof(nodo->posMemHeap));

	    EPI.filas+=1;
	    }
	}


	tDato creoDato(void * dato,unsigned int tamDatos){
	    tDato d;

	    d.dato=dato;
	    d.tamDatos=tamDatos;

	    return d;
	}

	void * borrarDatosMemoria(int PID){//todo FALTA TERMINAR

		/* -

		 * -Borrar en momoria segun posicion inicial que devuelve tabla y pararme para obtener heapMetada y saber cuanto borrar
		 * -Eliminar el nodo de memoria ocupada, agregar el nodo de memoria libre
		 * -eliminar de tabla de paginacion la entrada del programa en cuestion
		 */


		void * programa = buscarEnEPI(PID);
		metadata meta;

		meta = obtengoHeapMetadata(memoria,PID);


		int memoriaABorrar = meta.size;

		Borrar(listaMemoriaOcupada,programa);

		meta.size-=memoriaABorrar;
		meta.isFree=true;

		memcpy(memoria,&meta,sizeof(meta));

		void Insertar(listaDeMemoriaLibre,memoria,PID);

		while(programa!=NULL)
		{

			//char ** nuevaTabla = reorganizarTabla(PID);

		}

	}

	/*todo char ** reorganizarTabla(int PID);
	{

	}*/
	void * buscarEnEPI(int PID)
	{
		int i;
		for(i=0;i<EPI.filas;i++)
		{
			if (PID==EPI.matriz[i][1])
			{
				return EPI.matriz[i][3];
			}
		}
		return NULL;

	}

	void dump(){
	    BorrarLista(&listaMemoriaLibre);
	    BorrarLista(&listaMemoriaOcupada);
	    free(memoria);
	}

	bool consola(){
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
	        else if (!strcmp(comando, "dump")){
	             dump();
				}
			else
				printf("Comando incorrecto. Pruebe run | stop | exit | clean\n");
			free(mensaje);
			limpiarBufferDeEntrada();
		}
	}
	void limpiarBufferDeEntrada() {
		char c;
		while ((c = getchar()) != '\n' && c != EOF) { }
	}
	void connection_handler(uint32_t socket, uint32_t command){
		switch(command){
			case 5: //CPU me pide una instruccion
				serializar_int(socket,5);//Le respondo que le doy memoria => voy a esperar un mensaje que me va a decir PID,Inicio y offset para poder devolver
				t_SerialString* PATH = malloc(sizeof(t_SerialString));
				deserializar_string(socket, PATH);
				char * PID = memcpy((void*)PID,(void*)PATH->dataString,4);//Por convencion los primeros 4 bits son de PID
				char * posInicio = memcpy((void*)posInicio,(void*)PATH->dataString+4,4);
				char * offSet = memcpy((void*)deserializar_string,(void*)PATH->dataString+8,4);

				int IPID = convertirCharAInt(PID, 4);
				int IposInicio = convertirCharAInt(posInicio,4);
				int IoffSet = convertirCharAInt(offSet, 4);

				tDato instruccion = obtenerMemoriaReducida(memoria,IPID,IposInicio,IoffSet);
				t_SerialString * instruccionConvertida;
				instruccionConvertida = malloc(sizeof(t_SerialString));
				instruccionConvertida->dataString = malloc(sizeof(char*)*instruccion.tamDatos);
				memcpy(&instruccionConvertida->dataString ,instruccion.dato,instruccion.tamDatos);
				instruccionConvertida->sizeString=instruccion.tamDatos;

				serializar_string(socket,instruccionConvertida);


				break;
			default:
				printf("Error de comando\n");
				break;
			}
	}

	int convertirCharAInt(char * numero,int tamChar){
		int i;
		int multiplicadorBase = 1;
		int numeroInt=0;
		tamChar -= 1;
		while (tamChar>=0)
		{
			numeroInt+= numero[tamChar]*multiplicadorBase;
			tamChar--;
			multiplicadorBase*=10;
		}
		return numeroInt;
	}


