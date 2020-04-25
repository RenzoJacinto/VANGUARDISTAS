#include "Menu.h"
#include "BotonIniciar.h"
#include "global.h"
#include "ManejoDeSDL.h"

Menu::Menu(){}

bool Menu::cargarImagen(){
	//Loading success flag
	bool ok = true;

    // Cargar la textura de la nave
	if( !gMenuBGTexture.loadFromFile( "sprites/menuBG.png" ) ){
		ok = false;
	}

	return ok;
}

void Menu::procesar(){
    bool quit = false;
    BotonIniciar bt;
    // Mientras que siga corriendo la app
    while( usuarioNoRequieraSalir(quit) ){
        SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( sdl.getRenderer() );
        gMenuBGTexture.render( 0, 0 );

        //gBotonIniciarTexture.render(0, 0);

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

