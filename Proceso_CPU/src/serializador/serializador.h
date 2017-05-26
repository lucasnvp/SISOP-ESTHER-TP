#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "../servidor/servidor.h"

typedef struct HeaderType{
    uint32_t id;
    uint32_t dimension;
} Header;

void printHeader(Header aux){
    printf("Id: %d\n",aux.id);
    printf("Dimension: %d\n",aux.dimension);
}

void serializar(int client, uint32_t id, uint32_t dimension, void* buff){
    //Cabezara de la informacion
    Header myHeader;
    myHeader.dimension = dimension;
    myHeader.id = id;

    //Preparo el mensaje
    void *ENVIAR = malloc(myHeader.dimension + sizeof(myHeader));

    memcpy(ENVIAR,&myHeader,sizeof(myHeader));
    memcpy(ENVIAR+sizeof(myHeader),buff,myHeader.dimension);

    //Envio el mensaje
    send_data(client, ENVIAR, sizeof(myHeader) + myHeader.dimension);

    free(ENVIAR);
}

void* deserializar(int servidor){
    //Cabecera auxiliar
    Header aux;

    //Recibo el header
    recive_data(servidor,&aux,sizeof(Header));
    void* data = malloc(aux.dimension);

    //Recibo la informacion
    recive_data(servidor,data,aux.dimension);

    //Muestro el header que me llego
    printHeader(aux);

    return data;
}
