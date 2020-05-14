#include "Menu.h"
#include "BotonIniciar.h"
#include "global.h"
#include "ManejoDeSDL.h"

Menu::Menu(){}

bool Menu::cargarImagen(){
	//Loading success flag
	bool ok = true;

    // Cargar la textura de la nave
    std::string prueba = json.get_sprite_menu();
    const char* sMenuBG = prueba.c_str();
	if( !gMenuBGTexture.loadFromFile(sMenuBG) ){
		ok = false;
	}else logger.info("Se cargó la imagen menuBG.png");

	return ok;
}

void Menu::procesar(){
    bool quit = false;
    BotonIniciar bt;
    logger.info("Se mostró el menú");
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

