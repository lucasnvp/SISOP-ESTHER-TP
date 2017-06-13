#include "Memoria.h"


int main(void){
    puts("Proceso Memoria");

    //Configuracion inicial
		//config = load_config(PATH_CONFIG);
	 	//print_config(config);

	 	inicializarMemoria();


		inicializarPrograma(1,3);
		inicializarPrograma(2,1);
		asignarPaginasAProceso(1,2);


		//imprimirEPI();

		impirmirEPIaccediendoAMemoria();


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

void inicializarMemoria()
{
    inicializarTablaEPI();
    bloque_Memoria = malloc(MARCOS*MARCO_SIZE);

    int i;
    int cantMarcosOcupaMemoriaAdm = ((sizeof(int*)*3*MARCOS)+MARCO_SIZE-1)/MARCO_SIZE;
    int aux[3];
    aux[N_PID]=-1;

    //Inicializo en memoria tabla que va a dar informacion de los procesos de sistema
    for(i=0;i<cantMarcosOcupaMemoriaAdm;i++)
    {
        aux[N_PAGINA]=i;
        aux[N_FRAME]=i;
        memcpy(bloque_Memoria +(i*sizeof(aux)),&aux,sizeof(aux));

    }
    aux[N_PAGINA]=0;
    aux[N_PID]=0;
    //Inicializo en memoria siguiente parte de tabla que va a dar informacion de los procesos de usuario
    for(i=0;i<MARCOS-cantMarcosOcupaMemoriaAdm;i++)
    {
        aux[N_FRAME]=i+cantMarcosOcupaMemoriaAdm;
        memcpy((bloque_Memoria+cantMarcosOcupaMemoriaAdm*sizeof(aux)) +(i*sizeof(aux)),&aux,sizeof(aux));//TODO: no se esta copiando bien con ceros las cosas

    }
    for(i=0;i<cantMarcosOcupaMemoriaAdm;i++)
    {
        agregarDatosTablaEPI(-1,i);
    }

}
void inicializarTablaEPI()
{
    tablaEPI.filas=0;
    tablaEPI.matriz = malloc(sizeof(int *));

}
int agregarDatosTablaEPI(int PID,int nPagina){
    int pudeEscribirTabla = 0;
    int i;

    int info[3];
    info[N_PID]=PID;
    info[N_PAGINA]=nPagina;
    //Busco si hay marcos Libres
    for (i=0;i<tablaEPI.filas && !pudeEscribirTabla ;i++)
    {
        if (tablaEPI.matriz[i][N_PID]==0 && tablaEPI.matriz[i][N_PAGINA]==0)
        {
            memcpy(&tablaEPI.matriz[i][N_PID],&PID,sizeof(PID));
            memcpy(&tablaEPI.matriz[i][N_PAGINA],&nPagina,sizeof(nPagina));

            //Copio En memoria la info de la estructura administrativa.
            info[N_FRAME]=tablaEPI.matriz[i][N_FRAME];
            memcpy(bloque_Memoria + (info[N_FRAME])*sizeof(info),&info,sizeof(info));


            pudeEscribirTabla = 1;
        }
    }
    if (!pudeEscribirTabla && tablaEPI.filas<MARCOS)//Esto lo voy a tener que borrar
    {
        int **aux;
        aux = realloc(tablaEPI.matriz,sizeof(int)*(tablaEPI.filas+1));

        if (aux!='\0')
            {

        aux[tablaEPI.filas] =(int*) malloc(sizeof(aux[tablaEPI.filas])*3);
        memcpy(&tablaEPI.matriz,&aux,sizeof(aux));

        memcpy(&tablaEPI.matriz[tablaEPI.filas][N_FRAME],&tablaEPI.filas,sizeof(tablaEPI.filas));
        memcpy(&tablaEPI.matriz[tablaEPI.filas][N_PID],&PID,sizeof(PID));
        memcpy(&tablaEPI.matriz[tablaEPI.filas][N_PAGINA],&nPagina,sizeof(nPagina));

        //Copio En memoria la info de la estructura administrativa.
        info[N_FRAME]=tablaEPI.matriz[tablaEPI.filas][N_FRAME];
        memcpy(bloque_Memoria + (info[N_FRAME])*sizeof(info),&info,sizeof(info));


        tablaEPI.filas+=1;
        pudeEscribirTabla=1;
            }

        }

  return pudeEscribirTabla;
    }



void borrarDatosTablaEPI(int PID){

    int i;

    int cantMarcosOcupaMemoriaAdm = ((sizeof(int*)*3*MARCOS)+MARCO_SIZE-1)/MARCO_SIZE;
    int info[3];
    info[N_PID]=0;
    info[N_PAGINA]=0;

    for (i=0;i<tablaEPI.filas;i++)
    {
        if (PID == tablaEPI.matriz[i][N_PID])
        {
            tablaEPI.matriz[i][N_PID]=0;
            tablaEPI.matriz[i][N_PAGINA]=0;

        info[N_FRAME]=tablaEPI.matriz[i][N_FRAME];
        memcpy(bloque_Memoria + (info[N_FRAME])*sizeof(info),&info,sizeof(info));
        }
    }

    return;
}

int framesDisponibles()
{


    int i;
    int framesDisponibles=0;
    //Busco si hay marcos Libres
    for (i=0;i<tablaEPI.filas;i++)
    {
        if (tablaEPI.matriz[i][N_PID]==0 && tablaEPI.matriz[i][N_PAGINA]==0)
        {
            framesDisponibles++;
        }
    }
    return (MARCOS-tablaEPI.filas+framesDisponibles);
}



void imprimirEPI()
{
    int i;
    for (i=0;i<tablaEPI.filas;i++)
    {
        printf("#FRAME:%d  PID:%d  #PAGINA:%d \n" ,tablaEPI.matriz[i][N_FRAME],tablaEPI.matriz[i][N_PID],tablaEPI.matriz[i][N_PAGINA]);
    }

    printf("Los frames disponibles son: %d",framesDisponibles());
}

void impirmirEPIaccediendoAMemoria()
{
int a[3];
    int i;

    for(i=0;i<MARCOS;i++)
    {
        memcpy(&a,bloque_Memoria+12*i,sizeof(int)*3);

    printf("\nIMPRIMIENDO DESDE MEMORIA EPI FRAME:%d PID:%d PAGINA:%d",a[0],a[1],a[2]);
    }

}
int inicializarPrograma(int PID, int cantPaginas){

    int i;
    int pudeInicializar;
    for (i=0;i<cantPaginas;i++)
    {
       pudeInicializar = agregarDatosTablaEPI(PID,i);
    }

    return pudeInicializar;
}

int asignarPaginasAProceso(int PID, int cantPaginas){

int pudeInicializar=0;
if (framesDisponibles()>=cantPaginas)
{
    int ultimaPagina = obtenerUltimaPaginaUtilizada(PID);

    int i;

    for (i=ultimaPagina+1;i<cantPaginas+ultimaPagina+1;i++)
    {
       pudeInicializar = agregarDatosTablaEPI(PID,i);
    }
}
    return pudeInicializar;

}

int obtenerUltimaPaginaUtilizada(PID)
{


    int i;
    int ultimaPaginaUtilizada=0;
    int cantMarcosOcupaMemoriaAdm = ((sizeof(int*)*3*MARCOS)+MARCO_SIZE-1)/MARCO_SIZE;
    int info[3];
    info[N_PID]=0;
    info[N_PAGINA]=0;

    for (i=cantMarcosOcupaMemoriaAdm;i<tablaEPI.filas;i++)
    {
        if (PID == tablaEPI.matriz[i][N_PID])
        {
            ultimaPaginaUtilizada=tablaEPI.matriz[i][N_PAGINA];
        }
    }

    return ultimaPaginaUtilizada;;
}


void crearHilo(uint32_t * newfd){


	uint32_t command = deserializar_int(newfd);


	connection_handler(newfd,command);

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

			/*tDato instruccion = obtenerMemoriaReducida(memoria,IPID,IposInicio,IoffSet);
			t_SerialString * instruccionConvertida;
			instruccionConvertida = malloc(sizeof(t_SerialString));
			instruccionConvertida->dataString = malloc(sizeof(char*)*instruccion.tamDatos);
			memcpy(&instruccionConvertida->dataString ,instruccion.dato,instruccion.tamDatos);
			instruccionConvertida->sizeString=instruccion.tamDatos;

			serializar_string(socket,instruccionConvertida);*/


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


