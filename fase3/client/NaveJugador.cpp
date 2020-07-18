#include "NaveJugador.h"

NaveJugador::NaveJugador( int x, int y, int id, std::string id_user){
    logger.info(">>>> CARGANDO LA NAVE JUGADOR ....");
    std::string jug = "jugador"+std::to_string(id);

    if(! textureID.loadFromRenderedText(id_user.c_str(), "box"))
        logger.error("No se pudo cargar el id del jugador");
    else logger.debug("Se cargo el id del jugador");

    if(crearNave(x, y, "jugador", jug.c_str())) logger.debug("Se creo la nave jugador");

    alto = NAVE_HEIGHT;
    ancho = NAVE_WIDTH;
    id_nave = id;
    conectado = true;
    logger.info("<<<< SE CARGO LA NAVE JUGADOR");
}

void NaveJugador::handleEvent( SDL_Event& e , Mix_Music* gMusic, int* misil){

	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        // Ajusta la velocidad
        switch( e.key.keysym.sym ){
            case SDLK_UP: mVelY -= NAVE_VEL; break;
            case SDLK_DOWN: mVelY += NAVE_VEL; break;
            case SDLK_LEFT: mVelX -= NAVE_VEL; break;
            case SDLK_RIGHT: mVelX += NAVE_VEL; break;
            case SDLK_m: sounds.pauseMusic(gMusic); break;
            case SDLK_s: sounds.pauseEffects(); break;
            case SDLK_RETURN: *misil = 0; break;
        }
    } else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
        // Ajusta la velocidad
        switch( e.key.keysym.sym ){
            case SDLK_UP: mVelY += NAVE_VEL; break;
            case SDLK_DOWN: mVelY -= NAVE_VEL; break;
            case SDLK_LEFT: mVelX += NAVE_VEL; break;
            case SDLK_RIGHT: mVelX -= NAVE_VEL; break;
        }
    }

}

void NaveJugador::mover( vector<NaveEnemiga*> enemigos ){

    // Mueve la nave a la izquierda o la derecha
    setPosX(getPosX()+getVelX());

    if( ( getPosX() < 0 ) || ( getPosX() + getAncho() > sdl.getScreenWidth() )  ||  encontrarEnemigos( this, enemigos ) ){
        // Vuelve a la anterior posicion
        setPosX(getPosX()-getVelX());
    }

    // Mueve la nave a la izquierda o la derecha
    setPosY(getPosY()+getVelY());

    if( ( getPosY() < 0 ) || ( getPosY() + getAlto() > sdl.getScreenHeight() ) || encontrarEnemigos( this, enemigos ) ){
        // Vuelve a la anterior posicion
        setPosY(getPosY()-getVelY());
    }
}

void NaveJugador::renderizar(){
	gNaveTexture.render(getPosX(), getPosY());
    textureID.render(getPosX()+ textureID.getWidth() - 1, getPosY() + (3/2) * NAVE_HEIGHT);
}

int NaveJugador::getAlto(){
    return alto;
}

int NaveJugador::getAncho(){
    return ancho;
}
int NaveJugador::get_id(){
    return id_nave;
}

void NaveJugador::desconectar(){
    conectado = false;
    if(!gNaveTexture.loadFromFile(json.get_sprite_nave("jugador", "jugadorOff"))){
        logger.error("La imagen de la nave desconectada no existe");
        gNaveTexture.loadFromFile(json.get_imagen_default("nave"));
    }
    std::string msj = "Se desconectó la nave con id: ";
    std::string id = std::to_string(id_nave);
    msj = msj + id;
    logger.info(msj.c_str());
}

bool NaveJugador::isOn(){
    return conectado;
}

void NaveJugador::conectar(){
    conectado = true;
    std::string jug = "jugador"+std::to_string(id_nave);
    if(!gNaveTexture.loadFromFile(json.get_sprite_nave("jugador", jug.c_str()))){
        logger.error("La imagen del jugador no existe");
        gNaveTexture.loadFromFile("nave");
    }
    std::string msj = "Se conectó la nave con id: ";
    std::string id = std::to_string(id_nave);
    msj = msj + id;
    logger.info(msj.c_str());
}

