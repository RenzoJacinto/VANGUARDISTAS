#include "Enemigo3.h"

Enemigo3::Enemigo3(int x, int y){

    disparo = false;

    std::string mensaje =  ">>>> CARGANDO LA NAVE ENEMIGO3 ....";
    logger.info(mensaje.c_str());

    if(crearNave(x, y)){
        mensaje = "Se creo el enemigo3";
        logger.debug(mensaje.c_str());
    }

    disparoTriple = false;

    strcpy(imagenActual, "enemigo3");
    strcpy(imagenEspejo, "enemigo1");

    alto = 80;
    ancho = 80;
    score = 500;
    energia_actual = 100;
    energia_total = 100;
    DISTANCIA_DE_COMBATE = 0;

    vidas = 1;

    damage = 15;

    strcpy(clave, imagenActual);
    radio=alto/2;

    mensaje = "<<<< SE CARGO LA NAVE ENEMIGO3" ;
    logger.info(mensaje.c_str());
    fireRate.iniciar();
}

// intenta chocar al enemigo mas cercano
int Enemigo3::procesarAccion(vector<NaveJugador*> jugadores){
    int nave_seguida = naveDerechaCercana(jugadores);
    if(nave_seguida == -1){
        int a = -1;
        if(mPosX < 600) a = mover(15, 0, jugadores);
        else if(!turret) turret = true;
        if(a != -1) return a;
        int idx = obtenerNaveSeguidaMasCercana(jugadores);
        NaveJugador* nave = jugadores[idx];
        int vy = 0;
        if (abs(nave->getPosX() + 80 - mPosX) < radio){
            if (nave->getPosY() > mPosY ) vy = 1;
            else vy = -1;
        } else if (abs(nave->getPosY() - mPosY) > 5) {
            if (nave->getPosY() > mPosY ) vy = 3;
            else vy = -3;
        }
        a = mover(0, vy, jugadores);
        if(a != -1) return a;
        disparo = false;
        if(fireRate.transcurridoEnSegundos() > 2 && nave->isAlive() && turret) {
            disparo = true;
            misil_posX = mPosX - getRadio();
            misil_posY = mPosY;
            fireRate.finalizar();
            fireRate.iniciar();
        }
        return a;
    } else{
        turret = false;
        //printf("nave seguida = %d", nave_seguida);
        NaveJugador* nave = jugadores[nave_seguida];
        int vx = 0;
        int vy = 0;
        if( getDistanciaNaveEnX(nave) > 250) vx = 3;
        else vx = 1;
        if(abs(nave->getPosY() - mPosY) > 250) {
            if (nave->getPosY() > mPosY ) vy = 3;
            else vy = -3;
        } else if (abs(nave->getPosY() - mPosY) > 1) {
            if (nave->getPosY() > mPosY ) vy = 1;
            else vy = -1;
        }
        return mover(vx, vy, jugadores);
    }
}
