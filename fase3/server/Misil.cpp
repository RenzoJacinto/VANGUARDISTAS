#include "Misil.h"

Misil::Misil( int x_parm, int y_parm, int id_parm, int danio){
    alto = MISIL_HEIGHT;
    ancho = MISIL_WIDTH;
    id = id_parm;

    x = x_parm;
    y = y_parm;
    vel = MISIL_VEL;

    damage = danio;
}

int Misil::mover(vector<NaveEnemiga*> enemigos, int renderizados){
    x+=vel;
    if(x >= SCREEN_WIDTH) return -1;
    for(int i=0; i<renderizados; i++){
        if ( enemigos[i]->impacto_misil(x, y, MISIL_WIDTH, MISIL_HEIGHT, damage) ) return i+4;
    }
    return -2;
}

int Misil::mover(vector<NaveJugador*> jugadores){
    x-=vel;
    if(x < 0) return -1;
    int cant_jug = jugadores.size();
    printf("colision misil jugador, size: %d\n", cant_jug);
    for(int i=0; i<cant_jug; i++){
        printf("colision misil jugador, id: %d\n", jugadores[i]->get_id());
        if ( jugadores[i]->impacto_misil(x, y, MISIL_WIDTH, MISIL_HEIGHT, damage) ) return i;
        printf("check ok\n");
    }
    printf("check ok\n");
    return -2;
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

