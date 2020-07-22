#include "Escenario.h"
#include "global.h"

Escenario::Escenario(){}

void Escenario::cargarImagen(const char* nivel, const char* sprite, TextureW* textura){

    string sSP(sprite);
    std::string mensaje = "## CARGANDO " + sSP + " ....";
    logger.info(mensaje.c_str());

    std::string imagen = json.get_sprite_mapa(nivel, sprite);
    // Cargar la textura de la nave
	if(! textura->loadFromFile(imagen.c_str())){
        mensaje = "No se encontro el sprite: " + imagen + ", se carga la imagen que muestra el error";
        logger.error(mensaje.c_str());
        imagen = json.get_imagen_default("escenario");
        textura->loadFromFile(imagen.c_str());
    }
    string img(imagen);
    mensaje = "Se cargo con la imagen: " + img;
    logger.debug(mensaje.c_str());
}

bool Escenario::usuarioNoRequieraSalir(bool quit){
    return !quit;
}

bool Escenario::hayEventos(){
    return SDL_PollEvent( &e ) != 0;
}

bool Escenario::eventoEsSalir(){
    return e.type == SDL_QUIT;
}

