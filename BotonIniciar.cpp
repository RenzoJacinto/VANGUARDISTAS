#include "BotonIniciar.h"
#include "Menu.h"
#include "Mapa.h"
#include "global.h"

Mapa mapa;

BotonIniciar::BotonIniciar(){}

void BotonIniciar::handleEvent( SDL_Event& e ){
	if( e.type == SDL_KEYDOWN){
        // Cuando se apreta Enter
        if( e.key.keysym.sym == SDLK_RETURN){
            mapa.cargarImagen();
            mapa.procesar();
        }
    }
}
