#include "BotonIniciar.h"
#include "ManejoDeSDL.h"
#include "global.h"

BotonIniciar::BotonIniciar(){
}

void BotonIniciar::handleEvent( SDL_Event& e ){
	if( e.type == SDL_KEYDOWN){
        // Cuando se apreta Enter
        if( e.key.keysym.sym == SDLK_RETURN){
            sdl.cargarImagen();
            sdl.proceso();
        }
    }
}
