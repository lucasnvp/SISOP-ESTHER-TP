#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "config/config_Memoria.h"
#include "servidor/servidor.h"

char* PATH_CONFIG = "../src/config/config.txt";
Type_Config config;

#include "Listash.h"

#define TAM_BLOQUE 512
#define TAM_PAGINA 512
#define CANTCONECIONES 10 	// Si quiero el maximo de conexiones posibles en el sockect reemplazar por 'SOMAXCONN'
#define TRUE 1
#define FALSE 0

//Estos define que defino ahora luego se obtienen del config file
#define MARCOS 4
#define MARCO_SIZE 512




Lista listaMemoriaLibre = NULL;
pNodo p;

Lista listaMemoriaOcupada = NULL;
pNodo q;

void * memoria; //MARCOS * MARCOS_SIZE



struct heapMetadata{
    int size;
	bool isFree;
};

//Creo esta estructura para guardar los datos con su tamanio por que si no no se puede saber con un sizeof por ser void
struct datosStruct{
    void * dato;
    unsigned int tamDatos;
    int pid;

};

struct estructuraPaginacionInversa{
    int ** matriz;
    int filas;
    int columnas;

};
typedef struct estructuraPaginacionInversa tEstructuraPaginacionInversa;

tEstructuraPaginacionInversa EPI;

typedef struct heapMetadata metadata;

typedef struct datosStruct tDato;



void * nuevoBloqueDeMemoria();
metadata obtengoHeapMetadata(void * pagina,int posicionDeArranque);
void dividoMemoria(void * memoria);
bool puedoAlojarDatosEnUnaPagina(metadata memoria, int tamDatos);
void * agregarDatosABloqueDeMemoria(void * memoria, tDato datos);
tDato creoDato(void * dato,unsigned int tamDatos);
bool puedoAlojarDatos(void * memoria, int tamDatos);
bool consola();
tDato obtenerMemoria(void * memoria,int PID);

int main(void){
    puts("Proceso Memoria");

    //Configuracion inicial
	config = load_config(PATH_CONFIG);
	print_config(config);

    // variables para el servidor
	int fdmax;        // número máximo de descriptores de fichero
	fd_set master;   // conjunto maestro de descriptores de fichero

	//Creacion del servidor
	int servidor = build_server(config.PUERTO);

	//El socket esta listo para escuchar
	if(servidor > 0){
		printf("Servidor Memoria Escuchando\n");
	}

	// Seteo la cantidad de conexiones
	set_listen(servidor, config.CANTCONEXIONES);

	// seguir la pista del descriptor de fichero mayor
	fdmax = servidor; // por ahora es éste

	// bucle principal
	while(1) {
		if(fdmax == servidor){
			// acepto una nueva conexion
			fdmax = accept_conexion(servidor, &master, fdmax);
		}else{
			DatosRecibidos * buffer = recive_data(fdmax);
			// gestionar datos de un cliente
			if(buffer <= 0){
				fdmax = servidor; // eliminar del conjunto maestro
			}
		}
	}

    return EXIT_SUCCESS;
}

void * nuevoBloqueDeMemoria()//Inicializo memora
	{
		void * memoria = malloc(MARCOS*MARCO_SIZE);

	    dividoMemoria(memoria); //Divido el bloque en paginas de 512 con indicadores de inicio y fin de pagina

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

	          int  nPagina=(((nodo->posMemHeap)+sizeof(metaData)- memoria)/512)-1;
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
	            programa = calloc(programa,tamPrograma*sizeof(void*));
	            if(programa!='\0'){
	           memmove((void *)(programa+tamProgramaAnterior),(void *)(EPI.matriz[i][3]+sizeof(heap)),heap.size);

	            }
	        }
	    }
	    dato.dato=programa; // aca hay que hacer un memcpy
	    dato.tamDatos=tamPrograma;

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

	    while (i<MARCOS){

		metadata inicio;
		inicio.size=MARCO_SIZE-(sizeof(metadata)*2);
		inicio.isFree=TRUE;

		memcpy(memoria + (i*MARCO_SIZE),&inicio,sizeof(metadata));
	    Insertar(&listaMemoriaLibre,((memoria + (i*MARCO_SIZE))),0);//Agrega a listaDeMemoriaLibrea posiciones de los heaps con memoria libre

		metadata fin;
	    fin.size=0;
	    fin.isFree=TRUE;
	    memcpy(memoria+(((i+1)*MARCO_SIZE)-sizeof(metadata)),&fin,sizeof(metadata));
	    Insertar(&listaMemoriaOcupada,memoria+((i+1)*MARCO_SIZE)-sizeof(metadata),0);
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

