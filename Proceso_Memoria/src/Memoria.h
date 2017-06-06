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

#define N_FRAME 0
#define N_PID 1
#define N_PAGINA 2



//-----------------------FUNCIONES MEMORIA--------------------------------//
int inicializarPrograma(int PID, int cantPaginas);
char* solicitarBytesPagina(int pid,int pagina, int offset, int size,char** buffer);
int almacenarBytesPagina(int pid,int pagina, int offset,int size, char* buffer);
int asignarPaginasAProceso(int PID, int cantPaginas);
int finalizarPrograma(int pid);

//-----------------------OTRAS FUNCIONES: EPI------------------------------//
void inicializarTablaEPI();
int agregarDatosTablaEPI(int PID,int nPagina); //1 si pudo escribir, 0 si no.
void borrarDatosTablaEPI(int PID);
int framesDisponibles();
void imprimirEPI();
void impirmirEPIaccediendoAMemoria();
//-----------------------OTRAS FUNCIONES: MEMORIA--------------------------//
void inicializarMemoria();

//-----------------------OTRAS FUNCIONES: USO GENERAL----------------------//
bool consola();
void crearHilo(uint32_t * newfd);


void * bloque_Memoria;

struct estructuraPaginacionInversa{
    int ** matriz;
    int filas;
};
typedef struct estructuraPaginacionInversa * t_EstructuraPaginacionInversa;

t_EstructuraPaginacionInversa tablaEPI;



char* PATH_CONFIG = "../src/config/config.txt";

Type_Config config;






