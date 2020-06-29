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

void Menu::renderErrorLoguin(const char* intentos){
    TextureW gErrorLoguin;
    std::string file = json.get_sprite_menu("errorLoguin");
    if(! gErrorLoguin.loadFromFile(file.c_str()))
        logger.error("No se pudo cargar la imagen de error de logueo");
    else logger.debug("Se carga la imagen de error de logueo");

    TextureW gIntentos;
    if(! gIntentos.loadFromRenderedText(intentos, "log"))
        logger.error("No se pudo cargar el texto de intentos");
    else logger.debug("Se cargo el texto de intentos");

    SDL_RenderClear(sdl.getRenderer());
    gErrorLoguin.render(0,0);
    gIntentos.render(398, 366);
    SDL_RenderPresent(sdl.getRenderer());

    //HACE UN USLEEP DE 2 SEG
    for(int i = time(NULL) + 4; time(NULL) != i; time(NULL));
}


