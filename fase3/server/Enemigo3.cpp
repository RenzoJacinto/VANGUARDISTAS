#include "Enemigo3.h"

Enemigo3::Enemigo3(int x, int y){

    disparo = false;

    std::string mensaje =  ">>>> CARGANDO LA NAVE ENEMIGO3 ....";
    logger.info(mensaje.c_str());

    if(crearNave(x, y)){
        mensaje = "Se creo el enemigo3";
        logger.debug(mensaje.c_str());
    }

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
    int nave_seguida = obtenerNaveSeguidaMasCercana(jugadores);
    NaveJugador* nave = jugadores[nave_seguida];
    int distanciaNave = getDistanciaNaveEnX(nave);

    int ok = -1;
    int distY = abs(nave->getPosY() - mPosY);
    if(distY < distanciaNave) ok = seguirNave(nave, distanciaNave, jugadores);
    else ok = acomodarseEnEjeY(nave->getPosY(), jugadores);
    return ok;
}
