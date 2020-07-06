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
            if(! bt.handleEvent( e )){
                quit = true;
            }
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

void Menu::renderErrorLogin(const char* intentos, int accion_recibida){
    TextureW gErrorLogin;
    std::string file;
    if (accion_recibida == ERROR_LOGIN) file = json.get_sprite_menu("errorLogin");
    else if(accion_recibida == ID_YA_LOGUEADA) file = json.get_sprite_menu("errorLoginID");
    else if(accion_recibida == ID_NO_LOGUEADA_RECON) file = json.get_sprite_menu("idNoConectada");
    else if(accion_recibida == JUEGO_LLENO) file = json.get_sprite_menu("juegoLleno");
    else{
        renderServerCaido();
        return;
    }

    if(! gErrorLogin.loadFromFile(file.c_str())){
        logger.error("No se pudo cargar la imagen de error de logueo");
        file = json.get_imagen_default("escenario");
        gErrorLogin.loadFromFile(file.c_str());
    }
    else logger.debug("Se carga la imagen de error de logueo");

    TextureW gIntentos;
    if(! gIntentos.loadFromRenderedText(intentos, "log"))
        logger.error("No se pudo cargar el texto de intentos");
    else logger.debug("Se cargo el texto de intentos");

    SDL_RenderClear(sdl.getRenderer());
    gErrorLogin.render(0,0);
    gIntentos.render(398, 366);
    SDL_RenderPresent(sdl.getRenderer());

    gErrorLogin.free();
    gIntentos.free();

    //HACE UN USLEEP DE 3 SEG
    for(int i = time(NULL) + 3; time(NULL) != i; time(NULL));
}

void Menu::renderWaitUsers(){
    TextureW textureWait;
    std::string file = json.get_sprite_menu("waitUsers");
    if(! textureWait.loadFromFile(file.c_str())){
        logger.error("No se pudo cargar la imagen de wait users");
        file = json.get_imagen_default("escenario");
        textureWait.loadFromFile(file.c_str());
    } else logger.debug("Se cargo la imagen de wait users");
    SDL_RenderClear(sdl.getRenderer());
    textureWait.render(0,0);
    SDL_RenderPresent(sdl.getRenderer());
    textureWait.free();
}

void Menu::renderServerCaido(){
    TextureW texture;
    std::string file = json.get_sprite_menu("serverCerrado");
    if(! texture.loadFromFile(file.c_str())){
        logger.error("No se pudo cargar la imagen de server caido");
        file = json.get_imagen_default("escenario");
        texture.loadFromFile(file.c_str());
    } else logger.debug("Se cargo la imagen de server caido");

    SDL_RenderClear(sdl.getRenderer());
    texture.render(0,0);
    SDL_RenderPresent(sdl.getRenderer());
    texture.free();

    //HACE UN USLEEP DE 3 SEG
    for(int i = time(NULL) + 8; time(NULL) != i; time(NULL));
}

