#include "NaveJugador.h"

NaveJugador::NaveJugador( int x, int y, int id, std::string id_user){
    logger.info(">>>> CARGANDO LA NAVE JUGADOR ....");
    std::string jug = "jugador"+std::to_string(id);

    if(crearNave(x, y, "jugador", jug.c_str())) logger.debug("Se creo la nave jugador");

    energia_total = 100;
    energia_actual = 100;
    boomInfoX = 40;
    boomInfoY = 17;
    score = 0;
    vidas = CANT_VIDAS_JUGADOR;
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
            case SDLK_t: *misil = -1; break;
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
    render = true;
    //printf("renderizo\n");
	gNaveTexture.render(mPosX, mPosY);

	int w = NAVE_WIDTH;
    w = (int) (w * energia_actual / energia_total);

	//Renderizar vida!
	TextureW user;
    user.loadFromRenderedText(id_cliente.c_str(), "box");
    int largo = user.getWidth();
    user.render(mPosX+40-largo/2, mPosY+NAVE_HEIGHT-7);
    SDL_Rect vida = { mPosX+2, mPosY+NAVE_HEIGHT+14, w, 10 };
    SDL_SetRenderDrawColor( sdl.getRenderer(), 0x00, 0xFF, 0x00, 0xFF );
    SDL_RenderFillRect( sdl.getRenderer(), &vida );
    textureVida.render(mPosX, mPosY+NAVE_HEIGHT+14);

    //Renderizo un string que dice que esta en modo test
    if(mode_test){
        TextureW textureTest;
        textureTest.loadFromRenderedText("<Test>", "box");
        int x_test = NAVE_WIDTH - textureTest.getWidth();
        textureTest.render(mPosX + x_test/2 , mPosY - NAVE_HEIGHT/2 - 1 );

    }
    user.free();
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

std::string NaveJugador::get_name(){
    return id_cliente;
}

int NaveJugador::get_cant_vidas(){
    return vidas;
}

void NaveJugador::addScore(int puntos){
    printf("score: %d\n", score);
    score += puntos;
}

int NaveJugador::getScore(){
    return score;
}

void NaveJugador::set_modeTest(){
    // si no esta activado lo activa y visceversa
    if(! mode_test) mode_test = true;
    else mode_test = false;
}

void NaveJugador::setScore(int puntos)
{
    printf("score = %d\n", puntos);
    score = puntos;
}

void NaveJugador::setVidas(int num){
    vidas = num;
}

bool NaveJugador::inModeTest(){
    return mode_test;
}
