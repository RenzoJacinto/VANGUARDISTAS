#include "Enemigo4.h"
#include "time_nanoseconds.h"

Enemigo4::~Enemigo4(){}

Enemigo4::Enemigo4(int x, int y){

    disparo = false;

    std::string mensaje =  ">>>> CARGANDO LA NAVE ENEMIGO4 ....";
    logger.info(mensaje.c_str());

    if(crearNave(x, y)){
        mensaje = "Se creo el enemigo4";
        logger.debug(mensaje.c_str());
    }

    disparoTriple = false;

    strcpy(imagenActual, "enemigo4");
    strcpy(imagenEspejo, "enemigo2");

    alto = 63;
    ancho = 63;
    score = 1000;
    energia_actual = 200;
    energia_total = 200;
    DISTANCIA_DE_COMBATE_INICIAL = 500;

    vidas = 1;

    damage = 25;

    strcpy(clave, imagenActual);
    radio=alto/2;

    mensaje = "<<<< SE CARGO LA NAVE ENEMIGO4" ;
    logger.info(mensaje.c_str());
    fireRate.iniciar();
}

//intenta chocar al enemigo mas cercano, si le erra se da vuelta y le dispara cada 1 seg
int Enemigo4::procesarAccion(vector<NaveJugador*> jugadores){
    int nave_seguida = naveDerechaCercana(jugadores);
    if(nave_seguida == -1){
        int ok = -1;
        if(mPosX < 600) ok = mover(5, 0, jugadores);
        else turret = true;
        if(ok != -1) return ok;
        int idx = obtenerNaveSeguidaMasCercana(jugadores);
        NaveJugador* nave = jugadores[idx];
        int vy = 0;
        if (abs(nave->getPosX() + 80 - mPosX) < radio){
            if (nave->getPosY() > mPosY ) vy = 1;
            else vy = -1;
        } else if (abs(nave->getPosY() - mPosY) > 3) {
            if (nave->getPosY() > mPosY ) vy = 3;
            else vy = -3;
        }
        ok = mover(0, vy, jugadores);
        if(ok != -1) return ok;
        disparo = false;
        if(fireRate.transcurridoEnSegundos() > 3 && nave->isAlive() && turret) {
            disparo = true;
            misil_posX = mPosX - getRadio();
            misil_posY = mPosY;
            fireRate.finalizar();
            fireRate.iniciar();
        }
        return ok;
    } else{
        disparo = false;
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
