#include "NaveJugador.h"

NaveJugador::NaveJugador( int x, int y, int id){
    logger.info(">>>> CARGANDO LA NAVE JUGADOR ....");
    std::string jug = "jugador"+std::to_string(id);

    if(crearNave(x, y)) logger.debug("Se creo la nave jugador");

    energia_actual = 200;
    energia_total = 200;

    score = 0;

    vidas = 3;
    alto = NAVE_HEIGHT;
    ancho = NAVE_WIDTH;
    id_nave = id;
    conectado = true;
    logger.info("<<<< SE CARGO LA NAVE JUGADOR");
}

void NaveJugador::mover( vector<NaveEnemiga*> enemigos ){

    // Mueve la nave a la izquierda o la derecha
    setPosX(getPosX()+getVelX());

    if( ( getPosX() < 0 ) || ( getPosX() + getAncho() > SCREEN_WIDTH )  ||  encontrarEnemigos( this, enemigos ) ){
        // Vuelve a la anterior posicion
        setPosX(getPosX()-getVelX());
    }

    // Mueve la nave a la izquierda o la derecha
    setPosY(getPosY()+getVelY());

    if( ( getPosY() < 70 ) || ( getPosY() + getAlto() > SCREEN_HEIGHT ) || encontrarEnemigos( this, enemigos ) ){
        // Vuelve a la anterior posicion
        setPosY(getPosY()-getVelY());
    }
}

int NaveJugador::getAlto(){
    return alto;
}

int NaveJugador::getAncho(){
    return ancho;
}
int NaveJugador::get_id(){
    return id_nave;
}

void NaveJugador::desconectar(){
    conectado = false;
    std::string msj = "Se desconectó la nave con id: ";
    std::string id = std::to_string(id_nave);
    msj = msj + id;
    logger.info(msj.c_str());
}

bool NaveJugador::isOn(){
    return conectado;
}

void NaveJugador::conectar(){
    conectado = true;
    std::string msj = "Se conectó la nave con id: ";
    std::string id = std::to_string(id_nave);
    msj = msj + id;
    logger.info(msj.c_str());
}

void NaveJugador::set_modeTest(){
    // si no esta activado lo activa y visceversa
    if(! mode_test) mode_test = true;
    else mode_test = false;
}

int NaveJugador::getVidas(){
    return vidas;
}

