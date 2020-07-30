#include "NaveJugador.h"

NaveJugador::NaveJugador( int x, int y, int id){
    logger.info(">>>> CARGANDO LA NAVE JUGADOR ....");
    std::string jug = "jugador"+std::to_string(id);

    if(crearNave(x, y)) logger.debug("Se creo la nave jugador");

    energia_actual = 100;
    energia_total = 100;

    score = 0;

    damage = 50;

    radio = 0;
    vidas = 3;
    alto = NAVE_HEIGHT;
    ancho = NAVE_WIDTH;
    id_nave = id;
    conectado = true;
    logger.info("<<<< SE CARGO LA NAVE JUGADOR");
}

//En caso de encontrar algun enemigo cerca, verifica si se produce colision y en caso negativo devuelve True
int NaveJugador::encontrarEnemigos(vector<NaveEnemiga*>  enemigos ){

    int id = 0;
    vector<NaveEnemiga*>::iterator pos;
    for(pos = enemigos.begin();pos != enemigos.end();pos++){
        if(checkCollision( this , *pos ) && (*pos)->isAlive()){
            die();
            (*pos)->die();
            return id;
        }
        id++;
    }

    return -1;
}

int NaveJugador::mover( vector<NaveEnemiga*> enemigos ){

    // Mueve la nave a la izquierda o la derecha
    setPosX(getPosX()+getVelX());
    int ok = encontrarEnemigos(enemigos);
    if(ok != -1) return ok;
    if( ( getPosX() < 0 ) || ( getPosX() + getAncho() > SCREEN_WIDTH)){
        // Vuelve a la anterior posicion
        setPosX(getPosX()-getVelX());
    }

    // Mueve la nave a la izquierda o la derecha
    setPosY(getPosY()+getVelY());
    ok = encontrarEnemigos(enemigos);
    if(ok != -1) return ok;
    if( ( getPosY() < 70 ) || ( getPosY() + getAlto() > SCREEN_HEIGHT)){
        // Vuelve a la anterior posicion
        setPosY(getPosY()-getVelY());
    }

    return ok;
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

void NaveJugador::addScore(int puntaje)
{
    score += puntaje;
    printf("score = %d\n", score);
}
