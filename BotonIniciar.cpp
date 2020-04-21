#include "BotonIniciar.h"
#include "ManejoDeSDL.h"
#include "global.h"

BotonIniciar::BotonIniciar(){
}

void BotonIniciar::handleEvent( SDL_Event& e ){
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym ){
            case SDLK_UP:
                sdl.cargarImagen();
                sdl.proceso();
                break;
        }
    }
}
