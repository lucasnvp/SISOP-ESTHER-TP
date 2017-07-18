#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "config/config_Memoria.h"
#include "servidor/servidor.h"
#include "serializador/serializador.h"
#include <pthread.h>

#define N_FRAME 0
#define N_PID 1
#define N_PAGINA 2

#define MARCOS 100
#define MARCO_SIZE 256
#define ENTRADAS_CACHE 15
#define CACHE_X_PROCESO 3

uint32_t servidor;

Type_Config config;


void * bloque_Memoria;



typedef struct estructuraPaginacionInversa{
    int ** matriz;
    int filas;
}t_EstructuraPaginacionInversa;



t_EstructuraPaginacionInversa tablaEPI;

typedef struct{
    int PID;
    int nPagina;
    char contenido[MARCO_SIZE];
}t_MemoriaCache;



typedef struct{
    t_MemoriaCache memoriaCache;
    int tiempoEnCache;
}t_cacheHandler;

t_cacheHandler adminCache[ENTRADAS_CACHE];



char* PATH_CONFIG = "/home/utnso/Blacklist/tp-2017-1c-Blacklist/Proceso_Memoria/src/config/config.txt";




int inicializarPrograma(int PID, int cantPaginas);
void* solicitarBytesPagina(int PID,int pagina, int offset, int size);
int almacenarBytesPagina(int PID,int pagina, int offset,int size, void * buffer);
int asignarPaginasAProceso(int PID, int cantPaginas);
int liberarPaginaDeUnProceso(int PID, int pagina); //Falta Hacer
int finalizarPrograma(int PID);
//-----------------------OTRAS FUNCIONES: MEMORIA--------------------------//
void inicializarMemoria();
//-----------------------FUNCIONES: EPI--------------------------//
void inicializarTablaEPI();
int agregarDatosTablaEPI(int PID,int nPagina);
int borrarDatosTablaEPI(int PID);
int framesDisponibles();
void imprimirEPI();
void impirmirEPIaccediendoAMemoria(int inicio,int fin);

//-----------------------FUNCIONES: CACHE--------------------------//
void inicializarCache();
int estaLaPaginaEnCache(int PID,int nPagina);
void * solicitarBytesPaginaCache(int PID,int pagina, int offset, int size);
void incrementarEnUnoTiempoEnCache();
void imprimirCache();
void actualizoCache(int PID,int pagina,int nFrame);
int quitarProgramaDeCache(int PID);
//-----------------------OTRAS FUNCIONES: USO GENERAL----------------------//
bool consola();
void crearHilo(uint32_t * newfd);








