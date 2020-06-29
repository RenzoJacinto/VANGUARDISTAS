#include "Menu.h"
#include "global.h"

Menu::Menu(){
    dataMenu.h = 600;
    dataMenu.w = 800;
    dataMenu.x = 0;
    dataMenu.y = 0;
}

bool Menu::cargarImagen(){
	//Loading success flag
	bool ok = true;

	bt.cargarImagenes();

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
    logger.info("Se mostró el menú");
    bt.inicializar_credenciales();

    SDL_RenderClear( sdl.getRenderer() );
    gMenuBGTexture.render( 0, 0, &dataMenu );

    // Mientras que siga corriendo la app
    while( usuarioNoRequieraSalir(quit) ){


        SDL_StartTextInput();
        while( hayEventos() ){
            if( eventoEsSalir() ) quit = true;
            if(! bt.handleEvent( e )) quit = true;
        }
        SDL_RenderPresent( sdl.getRenderer() );
        SDL_StopTextInput();
    }
}

void Menu::cerrar(){
	gMenuBGTexture.free();
}

std::string Menu::get_id(){
    return bt.get_ID();
}

std::string Menu::get_pass(){
    return bt.get_Pass();
}


