#include "ManejoDeNiveles.h"

#include <string>
#include <time.h>
#include <ctime>

#include "Nivel1.h"
#include "Nivel2.h"
#include "Nivel3.h"

ManejoDeNiveles::ManejoDeNiveles(){
    cargarNiveles();
}

void ManejoDeNiveles::procesar_servidor(){

    list<Nivel*>::iterator nivel;
    for(nivel = niveles.begin(); nivel != niveles.end(); nivel++){

        (*nivel)->cargarNivel();
        if((*nivel)->procesar_servidor()) {
            cerrar_niveles(niveles);
            break;
        }
        (*nivel)->finalizar();
        delay(5);
        (*nivel)->cerrar();
    }
}

void ManejoDeNiveles::procesar_cliente(){

    list<Nivel*>::iterator nivel;
    for(nivel = niveles.begin(); nivel != niveles.end(); nivel++){

        (*nivel)->cargarNivel();
        if((*nivel)->procesar_cliente()) {
            cerrar_niveles(niveles);
            break;
        }
        (*nivel)->finalizar();
        delay(5);
        (*nivel)->cerrar();
    }
}

void ManejoDeNiveles::cerrar_niveles(list<Nivel*> niveles) {
    list<Nivel*>::iterator nivel;
    for(nivel = niveles.begin(); nivel != niveles.end(); nivel++){
        (*nivel)->cerrar();
    }
}

void ManejoDeNiveles::delay(int seg){
    for(int i = time(NULL) + seg; time(NULL) != i; time(NULL));
}

void ManejoDeNiveles::cargarNiveles(){

    /*Nivel1* mapa1 = new Nivel1();
    Nivel2* mapa2 = new Nivel2();
    Nivel3* mapa3 = new Nivel3();

    niveles.push_back(mapa1);
    niveles.push_back(mapa2);
    niveles.push_back(mapa3);*/
}
