#include "Misil.h"

Misil::Misil( int x_parm, int y_parm, int id_parm){
    alto = MISIL_HEIGHT;
    ancho = MISIL_WIDTH;
    id = id_parm * 10;

    x = x_parm;
    y = y_parm;
    vel = MISIL_VEL;

}

int Misil::mover(vector<NaveEnemiga*> enemigos, int renderizados){
    x+=vel;
    if(x >= SCREEN_WIDTH) return -1;
    for(int i=0; i<renderizados; i++){
        if( enemigos[i]->impacto_misil(x, y, MISIL_WIDTH, MISIL_HEIGHT) ) return i+4;
    }
    return 0;
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

