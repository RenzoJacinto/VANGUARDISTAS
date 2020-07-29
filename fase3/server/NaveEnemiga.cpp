#include "NaveEnemiga.h"
#include "time_nanoseconds.h"

NaveEnemiga::NaveEnemiga(int x, int y, const char* sprite, vector<NaveJugador*> jugadores){
    setJugadores(jugadores);
    disparo = false;
    naveSeguida = obtenerNaveSeguidaRandom(jugadores.size());
    distanciaActual = getDistanciaNave(jugadores[naveSeguida]);
    std::string sp(sprite);
    std::string mensaje =  ">>>> CARGANDO LA NAVE " + sp + " ....";
    logger.info(mensaje.c_str());
    if(crearNave(x, y)){
        mensaje = "Se creo el " + sp;
        logger.debug(mensaje.c_str());
    }

    strcpy(imagenActual, sprite);
    if (sp == "enemigo1") strcpy(imagenEspejo, "enemigo3");
    else if (sp == "enemigo3") strcpy(imagenEspejo, "enemigo1");
    else if (sp == "enemigo2") strcpy(imagenEspejo, "enemigo4");
    else if (sp == "enemigo4") strcpy(imagenEspejo, "enemigo2");

    if(sp == "enemigo1" || sp == "enemigo3"){
        alto = 80;
        ancho = 80;
        score = 500;
        energia_actual = 100;
        energia_total = 100;
        DISTANCIA_DE_COMBATE = 0;

    } else{
        alto = 63;
        ancho = 63;
        score = 1000;
        energia_actual = 200;
        energia_total = 200;
        DISTANCIA_DE_COMBATE_INICIAL = 500;
    }

    vidas = 1;

    //clave[0] = 0;
    strcpy(clave, imagenActual);
    radio=alto/2;
    mensaje = "<<<< SE CARGO LA NAVE " + sp;
    logger.info(mensaje.c_str());
    fireRate.iniciar();
}

void NaveEnemiga::mover(int velX, int velY){

    if(!isAlive()) {
    destruirNave();
    return;}

    for(unsigned int i = 0; i < jugadores.size(); ++i){
        if( checkCollision( jugadores[i] , this ) ) return;
    }

    setPosX(getPosX()+velX);
    setPosY(getPosY()+velY);

}

int NaveEnemiga::getRadio(){
    return radio;
}

int NaveEnemiga::getAltoImagen(){
	return alto;
}

int NaveEnemiga::getAnchoImagen(){
	return ancho;
}

char* NaveEnemiga::getClave(){
    return clave;
}

void NaveEnemiga::setNaveSeguida(int id){
    naveSeguida = id;
}

int NaveEnemiga::getNaveSeguida(){
    return naveSeguida;
}

void NaveEnemiga::procesarAccion(NaveJugador* nave){
    actualizarSprite();
    int distanciaNave = getDistanciaNave(nave);
    if (distanciaNave < DISTANCIA_DE_COMBATE_INICIAL){
        DISTANCIA_DE_COMBATE = 100 + (getRadio() *(randomNumber() % 10));
        DISTANCIA_DE_COMBATE_INICIAL = 0;
    }
    disparo = false;
    int posY = getPosY();
    int navePosY = nave->getPosY();
    if (abs(distanciaNave - DISTANCIA_DE_COMBATE) > 2 || abs(posY - navePosY) > 2){
        if (abs(distanciaNave - DISTANCIA_DE_COMBATE) > 2) seguirNave(nave, distanciaNave);
        if (abs(posY - navePosY) > 2) acomodarseEnEjeY(navePosY);
    }
    else if (fireRate.transcurridoEnSegundos()>2) {
        disparo = true;
    }
}

int NaveEnemiga::getDistanciaNave(NaveJugador* nave){
    return abs(nave->getPosX() - getPosX());
}

int NaveEnemiga::getDistanciaNaveConSigno(NaveJugador* nave){
    return nave->getPosX() - getPosX();
}

void NaveEnemiga::seguirNave(NaveJugador* nave, int distanciaNave){
    if (distanciaNave > DISTANCIA_DE_COMBATE){
        if (nave->getPosX() < getPosX()) mover(-2, 0);
        if (nave->getPosX() > getPosX()) mover(2, 0);
    }else{
        if (nave->getPosX() < getPosX()) mover(1, 0);
        if (nave->getPosX() > getPosX()) mover(-1, 0);
    }
}

void NaveEnemiga::acomodarseEnEjeY(int navePosY){
    if (getPosY() < navePosY) mover(0, 2);
    else mover(0, -2);
}

void NaveEnemiga::setJugadores(vector<NaveJugador*> listaJugadores){
    jugadores = listaJugadores;
}

bool NaveEnemiga::seDisparo(){
    return disparo;
}

void NaveEnemiga::reiniciarDisparo(){
    fireRate.iniciar();
}

void NaveEnemiga::destruirNave(){
    radio=0;
    mPosX=0;
    mPosY=0;
    mVelX=0;
    mVelY=0;
}

int NaveEnemiga::obtenerNaveSeguidaRandom(int cant_naves){
    return randomNumber() % cant_naves;
}

int NaveEnemiga::obtenerNaveSeguidaPonderada(){
    int cant_jug = jugadores.size();
    int ponderacion[cant_jug];
    for(int i=0; i<cant_jug; i++){
        int score = jugadores[i]->getScore();
        int vida = jugadores[i]->getVidas();

        ponderacion[i] = score / vida;
    }
    int idx = 0;
    int min_pond = ponderacion[idx];
    for(int j=1; j<cant_jug; j++){
        if(ponderacion[j] < min_pond){
            min_pond = ponderacion[j];
            idx = j;
        }
    }
    return idx;
}

void NaveEnemiga::actualizarSprite(){
    int distanciaNueva = getDistanciaNaveConSigno(jugadores[naveSeguida]);
    if (distanciaNueva * distanciaActual < 0){
        strcpy(clave, imagenEspejo);
        char imagen[10];
        strcpy(imagen, imagenActual);
        strcpy(imagenActual, imagenEspejo);
        strcpy(imagenEspejo, imagen);
    }
    distanciaActual = distanciaNueva;
}

char* NaveEnemiga::getImagen(){
    return imagenActual;
}

