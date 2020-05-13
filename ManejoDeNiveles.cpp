#include "ManejoDeNiveles.h"

#include <string>
//#include <list>
#include "Nivel1.h"
//#include "Nivel2.h"

ManejoDeNiveles::ManejoDeNiveles(){
    Nivel1* mapa1 = new Nivel1();
    //Nivel2* mapa2 = new Nivel2();

    niveles.push_back(mapa1);
    //niveles.push_back(mapa2);
}

void ManejoDeNiveles::procesar(){

    list<Nivel*>::iterator nivel;
    for(nivel = niveles.begin(); nivel != niveles.end(); nivel++){
        (*nivel)->cargarNivel();
        (*nivel)->procesar();
        (*nivel)->cerrar();
    }
}
