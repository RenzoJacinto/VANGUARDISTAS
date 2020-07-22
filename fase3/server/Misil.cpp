#include "Misil.h"

Misil::Misil( int x_parm, int y_parm, int id_parm){
    alto = MISIL_HEIGHT;
    ancho = MISIL_WIDTH;
    id = id_parm * 10;

    x = x_parm;
    y = y_parm;
    vel = MISIL_VEL;

}

bool Misil::mover(vector<NaveEnemiga*> enemigos, int renderizados){
    x+=vel;
    if(x >= SCREEN_WIDTH) return false;
    /*for(int i=0; i<renderizados; i++){
        enemigos[i]->impacto_misil(x, y);

    }*/
    return true;
}

int Misil::getAlto(){
    return alto;
}

int Misil::getAncho(){
    return ancho;
}

int Misil::get_id(){
    return id;
}

int Misil::getVel(){
    return vel;
}

int Misil::getPosX(){
    return x;
}

int Misil::getPosY(){
    return y;
}

