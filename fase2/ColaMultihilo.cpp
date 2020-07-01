#include "ColaMultihilo.h"
#include <stdlib.h>
#include <stdio.h>

ColaMultihilo::ColaMultihilo(){
    elements = 0;
    pthread_mutex_init(&mutex, NULL);
}

void ColaMultihilo::push(client_vw_t* dato){
    pthread_mutex_lock(&mutex);
    /*client_vw_t* client_view = (client_vw_t*)malloc(sizeof(client_vw_t));
    client_view->tipo_nave = dato->tipo_nave;
    client_view->serial = dato->serial;
    client_view->x = dato->x;
    client_view->y = dato->y;*/
    printf("PUSH EN COLA\n");
    printf("typ: %d\n", dato->tipo_nave);
    printf("ser: %d\n", dato->serial);
    printf("X: %d\n", dato->x);
    printf("Y: %d\n", dato->y);
    printf("----------------------\n");
    cola.push(dato);
    //free(dato);
    elements++;
    pthread_mutex_unlock(&mutex);
}

client_vw_t* ColaMultihilo::pop(){
    pthread_mutex_lock(&mutex);
    client_vw_t* data;
    if(elements > 0){
        data = cola.front();
        cola.pop();
        elements--;
    }
    pthread_mutex_unlock(&mutex);

    printf("POP EN COLA\n");
    printf("typ: %d\n", data->tipo_nave);
    printf("ser: %d\n", data->serial);
    printf("X: %d\n", data->x);
    printf("Y: %d\n", data->y);
    printf("----------------------\n");


    return data;
}

bool ColaMultihilo::estaVacia(){
    return elements;
}
