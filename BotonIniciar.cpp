#include "BotonIniciar.h"
#include "Mapa.h"

BotonIniciar::BotonIniciar(){}

void BotonIniciar::handleEvent( SDL_Event& e ){
	if( e.type == SDL_KEYDOWN){
        // Cuando se apreta Enter
        if( e.key.keysym.sym == SDLK_RETURN){
            Mapa mapa = Mapa();
            mapa.procesar();
            mapa.cerrar();
        }
    }
}
