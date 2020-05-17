#include "ManejoDeNiveles.h"

#include <string>
#include <unistd.h>
#include "Nivel1.h"
#include "Nivel2.h"


ManejoDeNiveles::ManejoDeNiveles(){
    Nivel1* mapa1 = new Nivel1();
    Nivel2* mapa2 = new Nivel2();

    niveles.push_back(mapa1);
    niveles.push_back(mapa2);

    fin_nivel1.loadFromFile("sprites/niveles/nivel1/final_nivel1.png");
}

void ManejoDeNiveles::procesar(){

    list<Nivel*>::iterator nivel;
    for(nivel = niveles.begin(); nivel != niveles.end(); nivel++){
        (*nivel)->cargarNivel();
        (*nivel)->procesar();
        fin_nivel1.render(0,0);
        usleep(500);
        (*nivel)->cerrar();
    }
}
