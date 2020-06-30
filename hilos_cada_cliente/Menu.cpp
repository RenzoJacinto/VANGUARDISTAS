#include "Menu.h"
#include "BotonIniciar.h"
#include "global.h"
#include "ManejoDeSDL.h"

Menu::Menu(){
    dataMenu.h = 600;
    dataMenu.w = 800;
    dataMenu.x = 0;
    dataMenu.y = 0;
}

bool Menu::cargarImagen(){
	//Loading success flag
	bool ok = true;

    // Cargar la textura de la nave
    std::string menu = json.get_sprite_menu("menu");
    const char* sMenuBG = menu.c_str();
	if( !gMenuBGTexture.loadFromFile(sMenuBG) ){
        logger.error("No se encontro la imagen del menu, se cargo una por defecto");
        menu = json.get_imagen_default("escenario");
        if (!gMenuBGTexture.loadFromFile(menu.c_str())) ok = false;
	}
	std::string msj = "Se cargó la imagen de menú: " + menu;
    logger.info(msj.c_str());
	return ok;
}

void Menu::procesar(){
    bool quit = false;
    BotonIniciar bt;
    logger.info("Se mostró el menú");
    // Mientras que siga corriendo la app

    SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( sdl.getRenderer() );
    gMenuBGTexture.render( 0, 0, &dataMenu );

    while( usuarioNoRequieraSalir(quit) ){
        SDL_StartTextInput();
        while( hayEventos() ){
            if( eventoEsSalir() ) quit = true;
            bt.handleEvent( e );
        }
        SDL_RenderPresent( sdl.getRenderer() );
        SDL_StopTextInput();
    }
}

void Menu::cerrar(){
	gMenuBGTexture.free();
}


