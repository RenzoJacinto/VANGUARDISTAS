#include "NaveJugador.h"
#include "global.h"

NaveJugador::NaveJugador( int x, int y, const char* imagen){
    crearNave(x, y, imagen, NAVE_WIDTH);
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

void NaveJugador::mover( NaveEnemiga* enemigo ){
    // Mueve la nave a la izquierda o la derecha
    setPosX(getPosX()+getVelX());
	desplazarColicionador();

	if( ( getPosX() - getColicionador().r < 0 ) || ( getPosX() + getColicionador().r > sdl.getScreenWidth() )  || checkCollision( getColicionador(), enemigo->getColicionador() ) ){
        setPosX(getPosX()-getVelX());
		desplazarColicionador();
    }

    setPosY(getPosY()+getVelY());
	desplazarColicionador();

    if( ( getPosY() - getColicionador().r < 0 ) || ( getPosY() + getColicionador().r > sdl.getScreenHeight() ) || checkCollision( getColicionador(), enemigo->getColicionador() ) ){
        // Vuelve a la anterior posicion
        setPosY(getPosY()-getVelY());
		desplazarColicionador();
    }
}
