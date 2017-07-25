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
#include <semaphore.h>

#define N_FRAME 0
#define N_PID 1
#define N_PAGINA 2
#define BLOQUEADO 150
#define SOY_KERNEL 100
//#define MARCOS 100
//#define MARCO_SIZE 256
//#define ENTRADAS_CACHE 15
#define CACHE_X_PROCESO 3

uint32_t servidor;

Type_Config config;

sem_t SEM_hiloServidor; //Para que la cola de PCB se suspenda si no tiene entradas

void * bloque_Memoria;

typedef struct estructuraPaginacionInversa{
    uint32_t ** matriz;
    uint32_t filas;
} t_EstructuraPaginacionInversa;

t_EstructuraPaginacionInversa tablaEPI;

typedef struct{
    uint32_t PID;
    uint32_t nPagina;
    char* contenido;
}t_MemoriaCache;

typedef struct{
    t_MemoriaCache memoriaCache;
    uint32_t tiempoEnCache;
}t_cacheHandler;

t_cacheHandler * adminCache;

char* PATH_CONFIG = "/home/utnso/Blacklist/tp-2017-1c-Blacklist/Proceso_Memoria/src/config/config.txt";

uint32_t inicializarPrograma(uint32_t PID, uint32_t cantPaginas);
void* solicitarBytesPagina(uint32_t PID,uint32_t pagina, uint32_t offset, uint32_t size);
uint32_t almacenarBytesPagina(uint32_t PID,uint32_t pagina, uint32_t offset,uint32_t size, void * buffer);
uint32_t asignarPaginasAProceso(uint32_t PID, uint32_t cantPaginas);
uint32_t liberarPaginaDeUnProceso(uint32_t PID, uint32_t pagina); //Falta Hacer
uint32_t finalizarPrograma(uint32_t PID);
//-----------------------OTRAS FUNCIONES: MEMORIA--------------------------//
void inicializarMemoria();
uint32_t paginasQueOcupaProceso(uint32_t PID);
//-----------------------FUNCIONES: EPI--------------------------//
void inicializarTablaEPI();
uint32_t agregarDatosTablaEPI(uint32_t PID,uint32_t nPagina);
uint32_t borrarDatosTablaEPI(uint32_t PID);
uint32_t framesDisponibles();
void imprimirEPI();
void impirmirEPIaccediendoAMemoria(uint32_t inicio,uint32_t fin);
uint32_t obtenerUltimaPaginaUtilizada(uint32_t PID);
uint32_t obtenerFrame(uint32_t PID, uint32_t pag);
//-----------------------FUNCIONES: CACHE--------------------------//
void inicializarCache();
uint32_t estaLaPaginaEnCache(uint32_t PID,uint32_t nPagina);
void * solicitarBytesPaginaCache(uint32_t PID,uint32_t pagina, uint32_t offset, uint32_t size);
void incrementarEnUnoTiempoEnCache();
void imprimirCache();
void actualizoCache(uint32_t PID,uint32_t pagina,uint32_t nFrame);
void quitarProgramaDeCache(uint32_t PID);
void borrarCache();
//-----------------------OTRAS FUNCIONES: USO GENERAL----------------------//
void consola();
void hiloConexion(uint32_t * newfd);
void connection_handler(uint32_t socket, uint32_t command);
void lanzoHiloConsolaComandos();
void inicializoServidor();
void limpiarBufferDeEntrada();
