#include "ManejoDeNiveles.h"

#include <string>
#include <time.h>
#include <ctime>

#include "Nivel1.h"
#include "Nivel2.h"
#include "Nivel3.h"

ManejoDeNiveles::ManejoDeNiveles(){}

void ManejoDeNiveles::procesar(position_t* pos){

    list<Nivel*>::iterator nivel;
    actual_nivel = 0;
    for(nivel = niveles.begin(); nivel != niveles.end(); nivel++){

        (*nivel)->cargarNivel();
        if((*nivel)->procesarClient(pos)) {
            cerrar_niveles(niveles);
            break;
        }
        actual_nivel++;
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

    Nivel1* mapa1 = new Nivel1();
    Nivel2* mapa2 = new Nivel2();
    Nivel3* mapa3 = new Nivel3();

    niveles.push_back(mapa1);
    niveles.push_back(mapa2);
    niveles.push_back(mapa3);
}

void ManejoDeNiveles::procesarServer(ColaMultihilo* cola){
    Nivel nivel;
    for(int i=0; i<CANT_NIVELES; i++){
        std::string ac_nivel = "nivel"+std::to_string(i+1);
        nivel.procesarServer(cola, ac_nivel);
    }
}

void ManejoDeNiveles::pushDato(client_vw_t* client_view){
    list<Nivel*>::iterator nivel;
    int i = 0;
    for(nivel = niveles.begin(); nivel != niveles.end(); nivel++){
        if(actual_nivel == i){
            (*nivel)->pushDato(client_view);
            break;
        }
        i++;
    }
}
