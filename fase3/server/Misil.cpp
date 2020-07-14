#include "Misil.h"

Misil::Misil( int x_parm, int y_parm, int id_parm){
    alto = MISIL_HEIGHT;
    ancho = MISIL_WIDTH;
    id = id_parm;

    x = x_parm;
    y = y_parm;
    vel = MISIL_VEL;

}

bool Misil::mover(){

    x+=vel;
    return (x < SCREEN_HEIGHT && x > 0);
}

int Misil::getAlto(){
    return alto;
}

int Misil::getAncho(){
    return ancho;
}

int Misil::get_id(){
    return id_nave;
}

int Misil::getVel(){
    return vel;
}

int Misil::getPosX(){
    return x;
}

