#include "NaveJugador.h"

NaveJugador::NaveJugador( int x, int y){
    std::string jugador = json.get_sprite_nave("jugador", "jugador");
    const char* sJugador = jugador.c_str();
    if(!crearNave(x, y, sJugador)){
        jugador = json.get_sprite_nave("jugador", "default");
        sJugador = jugador.c_str();
        logger.debug("La imagen de la nave no fue encontrada, lo cual se cargo una por defecto");
        crearNave(x, y, sJugador);
    }
    alto = NAVE_HEIGHT;
    ancho = NAVE_WIDTH;
}

void NaveJugador::handleEvent( SDL_Event& e ){

	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        // Ajusta la velocidad
        switch( e.key.keysym.sym ){
            case SDLK_UP: mVelY -= NAVE_VEL; break;
            case SDLK_DOWN: mVelY += NAVE_VEL; break;
            case SDLK_LEFT: mVelX -= NAVE_VEL; break;
            case SDLK_RIGHT: mVelX += NAVE_VEL; break;
        }
    }

    else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
        // Ajusta la velocidad
        switch( e.key.keysym.sym ){
            case SDLK_UP: mVelY += NAVE_VEL; break;
            case SDLK_DOWN: mVelY -= NAVE_VEL; break;
            case SDLK_LEFT: mVelX += NAVE_VEL; break;
            case SDLK_RIGHT: mVelX -= NAVE_VEL; break;
        }
    }
}

void NaveJugador::mover( vector<NaveEnemiga*> enemigos ){

    // Mueve la nave a la izquierda o la derecha
    setPosX(getPosX()+getVelX());

    if( ( getPosX() < 0 ) || ( getPosX() + getAncho() > sdl.getScreenWidth() )  ||  encontrarEnemigos( this, enemigos ) ){
        // Vuelve a la anterior posicion
        setPosX(getPosX()-getVelX());
    }

    // Mueve la nave a la izquierda o la derecha
    setPosY(getPosY()+getVelY());

    if( ( getPosY() < 0 ) || ( getPosY() + getAlto() > sdl.getScreenHeight() ) || encontrarEnemigos( this, enemigos ) ){
        // Vuelve a la anterior posicion
        setPosY(getPosY()-getVelY());
    }
}

void NaveJugador::renderizar(){
	gNaveTexture.render(getPosX(), getPosY());
}

int NaveJugador::getAlto(){
    return alto;
}

int NaveJugador::getAncho(){
    return ancho;
}


