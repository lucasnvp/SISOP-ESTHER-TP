#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "config/config_Memoria.h"
#include "servidor/servidor.h"
#include "serializador/serializador.h"
#include <pthread.h>

char* PATH_CONFIG = "../src/config/config.txt";

Type_Config config;

#include "Listash.h"

#define TAM_BLOQUE 512
#define TAM_PAGINA 512
#define CANTCONECIONES 10 	// Si quiero el maximo de conexiones posibles en el sockect reemplazar por 'SOMAXCONN'
#define TRUE 1
#define FALSE 0






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
void crearHilo(uint32_t * newfd);
<<<<<<< HEAD
=======
void * buscarEnEPI(int PID);
>>>>>>> b0af0ca4947292a9997d7b4b17a59b1b90d2e52b
