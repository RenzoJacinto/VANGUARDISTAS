#include "Nave.h"
#include "ManejoDeSDL.h"
#include "global.h"


Nave::Nave(){
    mPosX = 0;
    mPosY = 0;

    mVelX = 0;
    mVelY = 0;
}

void Nave::handleEvent( SDL_Event& e ){
    // Si una tecla es precionada
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        // Ajusta la velocidad
        switch( e.key.keysym.sym ){
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
        // Ajusta la velocidad
        switch( e.key.keysym.sym ){
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void Nave::move(){
    // Mover la nave a la izquierda o derecha
    mPosX += mVelX;

    // Si la nav se fue muy a la derecha o muy a la izq
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > sdl.getScreenWidth() ) ){
        // Vuelve a atras
        mPosX -= mVelX;
    }

    // Mover la nave hacia arriba o abajo
    mPosY += mVelY;

    // Si la nave fue muy arriba o muy abajo
    if( ( mPosY < 0 ) || ( mPosY + DOT_HEIGHT > sdl.getScreenHeight() ) ){
        //Move back
        mPosY -= mVelY;
    }
}

void Nave::render(){
    // Mostrar la nave
	sdl.renderNave(mPosX, mPosY);
}
