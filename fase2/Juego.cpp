#include "Juego.h"
#include "global.h"
#include "TextureW.h"

Juego::Juego(){

    if(! sdl.iniciarSDL()) logger.error("Fallo la inicializacion de SDL");
    for(int i=0; i<4;i++){
        bit_enemigos[i] = VACIO;
        bit_jugadores[i] = VACIO;
    }
}

void Juego::init_menu(){
    if (menu.cargarImagen()) menu.procesar();
}

std::string Juego::get_id(){
    return menu.get_id();
}

std::string Juego::get_password(){
    return menu.get_pass();
}

void Juego::cerrarMenu(){
    menu.cerrar();
}

void Juego::cerrar(){
    sdl.cerrar();
}

void Juego::render_errorLoguin(int intentos){

    std::string intentos_rest = std::to_string(intentos);
    menu.renderErrorLoguin(intentos_rest.c_str());
}

void Juego::iniciar(position_t* pos){
    ManejoDeNiveles niveles;
    niveles.cargarNiveles();
    niveles.procesar(pos);
}
void Juego::renderNave(client_vw_t* client_view){
    TextureW texture;
    std::string sprite = obtenerSpriteNave(client_view->tipo_nave, client_view->serial);
    if(sprite == ""){
        if(client_view->tipo_nave == TIPO_JUGADOR) texture = texturesJugadores[client_view->serial - 1];
        if(client_view->tipo_nave == TIPO_ENEMIGO) texture = texturesEnemigos[client_view->serial - 1];
    } else{
        if(client_view->tipo_nave == TIPO_JUGADOR){
            std::string msj = "No se pudo cargar la imagen "+sprite+" , se carga por default";
            if(! texture.loadFromFile(sprite)){
                logger.error(msj.c_str());
                std::string nave = "nave"+client_view->serial;
                sprite = json.get_sprite_nave_default("jugador", nave.c_str());
                texture.loadFromFile(sprite);
            } else{
                msj = "Se cargo la imagen "+sprite;
                logger.debug(sprite.c_str());
            }
            texturesJugadores[client_view->serial - 1] = texture;
        } else if(client_view->tipo_nave == TIPO_ENEMIGO){
            texture.loadFromFile(sprite);
            texturesEnemigos[client_view->serial - 1] = texture;
        }
    }

    texture.render(client_view->x, client_view->y);
    SDL_RenderPresent(sdl.getRenderer());
}

std::string Juego::obtenerSpriteNave(int tipo, int serial){
    std::string sprite = "";
    if(tipo == TIPO_JUGADOR){
        if(bit_jugadores[serial] == CARGADO) sprite = "";
        else{
            std::string nave = "nave"+std::to_string(serial);
            sprite = json.get_sprite_nave("jugador", nave.c_str());
            bit_jugadores[serial - 1] = CARGADO;
        }

    } else if(tipo == TIPO_ENEMIGO){
        if(bit_enemigos[serial] == CARGADO) sprite = "";
        else{
            std::string nave = "enemigo"+std::to_string(serial);
            sprite = json.get_sprite_nave("enemigo", nave.c_str());
            bit_enemigos[serial - 1] = CARGADO;
        }
    }
    return sprite;
}
