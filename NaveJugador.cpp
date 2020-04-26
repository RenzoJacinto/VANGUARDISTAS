#include "NaveJugador.h"

NaveJugador::NaveJugador( int x, int y, const char* imagen){
    crearNave(x, y, imagen, NAVE_WIDTH);
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

void NaveJugador::mover( NaveEnemiga* enemigo ){
    // Mueve la nave a la izquierda o la derecha
    setPosX(getPosX()+getVelX());
	//desplazarColicionador();

	if( ( getPosX() - (getAncho()/2) < 0 ) || ( getPosX() + (getAncho()/2) > sdl.getScreenWidth() )  || checkCollision( this , enemigo ) ){
        setPosX(getPosX()-getVelX());
		//desplazarColicionador();
    }

    setPosY(getPosY()+getVelY());
	//desplazarColicionador();

    if( ( getPosY() - (getAncho()/2) < 0 ) || ( getPosY() + (getAncho()/2) > sdl.getScreenHeight() ) || checkCollision( this , enemigo ) ){
        // Vuelve a la anterior posicion
        setPosY(getPosY()-getVelY());
		//desplazarColicionador();
    }
}

void NaveJugador::renderizar(){
	gNaveTexture.render(getPosX() - (getAncho()/2), getPosY() - (getAncho()/2));
}

int NaveJugador::getAlto(){
    return alto;
}

int NaveJugador::getAncho(){
    return ancho;
}
