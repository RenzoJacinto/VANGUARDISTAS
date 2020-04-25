#include "Menu.h"
#include "BotonIniciar.h"
#include "global.h"
#include "ManejoDeSDL.h"

Menu::Menu(){}

void Menu::procesar(){
    cargarImagen("sprites/menuBG.png", gMenuBGTexture);
    bool quit = false;
    BotonIniciar bt;
    if (logger.seDebeInformarInfo()) logger.informar("Se mostró el menú");
    // Mientras que siga corriendo la app
    while( usuarioNoRequieraSalir(quit) ){
        SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( sdl.getRenderer() );
        gMenuBGTexture.render( 0, 0 );

        SDL_RenderPresent( sdl.getRenderer() );
        while( hayEventos() ){
            if( eventoEsSalir() ) quit = true;
            bt.handleEvent( e );
        }
    }

}

void Menu::cerrar(){
	gMenuBGTexture.free();
}

