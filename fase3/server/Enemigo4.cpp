#include "Enemigo4.h"
#include "time_nanoseconds.h"

Enemigo4::Enemigo4(int x, int y){

    disparo = false;

    std::string mensaje =  ">>>> CARGANDO LA NAVE ENEMIGO4 ....";
    logger.info(mensaje.c_str());

    if(crearNave(x, y)){
        mensaje = "Se creo el enemigo4";
        logger.debug(mensaje.c_str());
    }

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
    if(nave_seguida == -1)
    {
        int a = -1;
        if(mPosX < 600) a = mover(10, 0, jugadores);
        if(a != -1) return a;
        int idx = obtenerNaveSeguidaMasCercana(jugadores);
        a = acomodarseEnEjeY(jugadores[idx]->getPosY(), jugadores);
        if(a != -1) return a;
        disparo = false;
        if(fireRate.transcurridoEnSegundos() > 1 && abs(mPosY - 600) < 5) {
            disparo = true;
            fireRate.finalizar();
            fireRate.iniciar();
        }
        else if(fireRate.transcurridoEnSegundos() > 5)
        {
            disparo = true;
            fireRate.finalizar();
            fireRate.iniciar();
        }
        return a;
    }
    else
    {
        NaveJugador* nave = jugadores[nave_seguida];
        int distanciaNave = getDistanciaNaveEnX(nave);
        int ok = -1;
        if(-getDistanciaNaveEnXConSigno(nave) > DISTANCIA_DE_COMBATE_INICIAL/2){
            DISTANCIA_DE_COMBATE = 50 + (getRadio() *(randomNumber() % 10));
            disparo = false;
            int posY = getPosY();
            int navePosY = nave->getPosY();
            if (abs(distanciaNave - DISTANCIA_DE_COMBATE) > 2 || abs(posY - navePosY) > 2){
                if (abs(posY - navePosY) > 2) ok = acomodarseEnEjeY(navePosY, jugadores);
            } else if (fireRate.transcurridoEnSegundos() > 1) {
                disparo = true;
            }
        } else ok = seguirNave(nave, distanciaNave, jugadores);
        return ok;
    }
}
