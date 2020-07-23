#include "NaveJugador.h"

NaveJugador::NaveJugador( int x, int y, int id, std::string id_user){
    logger.info(">>>> CARGANDO LA NAVE JUGADOR ....");
    std::string jug = "jugador"+std::to_string(id);

    if(! textureVida.loadFromFile("sprites/usuario/puntos/barraVida.png")){
        logger.error("No se pudo cargar la barra de vida");
        textureVida.loadFromFile(json.get_imagen_default("nave"));
    } else logger.debug("Se cargo la barra de vida");

    if(crearNave(x, y, "jugador", jug.c_str())) logger.debug("Se creo la nave jugador");

    id_cliente = id_user;
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
	gNaveTexture.render(mPosX, mPosY);

	//Renderizar vida!
    SDL_Rect vida = { mPosX+3, mPosY+NAVE_HEIGHT+6, NAVE_WIDTH-3, 4 };
    SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0x00, 0x00, 0xFF );
    SDL_RenderFillRect( sdl.getRenderer(), &vida );
    textureVida.render(mPosX, mPosY+NAVE_HEIGHT+2);
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
    if(!gNaveTexture.loadFromFile(json.get_sprite_nave("jugador", "jugadorOff", "nave"))){
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
    if(!gNaveTexture.loadFromFile(json.get_sprite_nave("jugador", jug.c_str(), "nave"))){
        logger.error("La imagen del jugador no existe");
        gNaveTexture.loadFromFile("nave");
    }
    std::string msj = "Se conectó la nave con id: ";
    std::string id = std::to_string(id_nave);
    msj = msj + id;
    logger.info(msj.c_str());
}

