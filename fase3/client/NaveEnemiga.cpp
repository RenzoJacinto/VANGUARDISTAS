#include "NaveEnemiga.h"

NaveEnemiga::NaveEnemiga(int x, int y, const char* sprite){
    std::string sp(sprite);
    std::string mensaje =  ">>>> CARGANDO LA NAVE " + sp + " ....";
    logger.info(mensaje.c_str());

    if(crearNave(x, y, "enemigas", sprite)){
        mensaje = "Se creo el " + sp;
        logger.debug(mensaje.c_str());
    }
    energia_actual = -1;
    energia_total = -1;
    boomInfoX = 0;
    boomInfoY = 0;
    vidas = CANT_VIDAS_ENEMIGO;
    turret = false;
    if(strcmp(sprite, "enemigo3")==0 || strcmp(sprite, "enemigo4")==0) desplazamiento = 1;
    else desplazamiento = -1;

    if(sp == "enemigo1" || sp == "enemigo3"){
        energia_total = 100;
        energia_actual = 100;
        score = 500;
    } else if(sp == "boss"){
        energia_total = 1000;
        energia_actual = 1000;
        score = 2000;
    } else{
        energia_total = 200;
        energia_actual = 200;
        score = 1000;
    }

    clave[0] = 0;
    strncat(clave, sprite, 15);
    radio=getAltoImagen()/2;
    mensaje = "<<<< SE CARGO LA NAVE " + sp;
    logger.info(mensaje.c_str());
}

void NaveEnemiga::mover( NaveJugador* jugador ){

    setPosX(getPosX()+desplazamiento);

    if( checkCollision( jugador , this ) ){
        setPosX(getPosX()-desplazamiento);
    }
}

void NaveEnemiga::renderizar(){
    render = true;
    gNaveTexture.render(getPosX() - getRadio(), getPosY() - getRadio());
    int w = getAnchoImagen();
    w = (int) (w * energia_actual / energia_total);
    SDL_Rect vida = { mPosX-getRadio()+5, mPosY+getRadio()+6, w, 10 };
    SDL_SetRenderDrawColor( sdl.getRenderer(), 0x00, 0xFF, 0x00, 0xFF );
    SDL_RenderFillRect( sdl.getRenderer(), &vida );
    textureVida.render(mPosX-getRadio()+5, mPosY+getRadio()+6);
}

int NaveEnemiga::getRadio(){
    return radio;
}

void NaveEnemiga::setImagen(const char* img){
    if (strcmp(clave, img) != 0){
        strcpy(clave, img);
        std::string imagen_json = json.get_sprite_nave("enemigas", img, "nave");
        gNaveTexture.loadFromFile(imagen_json);
    }
}

int NaveEnemiga::getAltoImagen(){
	return gNaveTexture.getHeight();
}

int NaveEnemiga::getAnchoImagen(){
	return gNaveTexture.getWidth();
}

char* NaveEnemiga::getClave(){
    return clave;
}

int NaveEnemiga::getScore()
{
    return score;
}

void NaveEnemiga::setTurret()
{
    if(!turret)
    {
        turret = true;
        gNaveTexture.free();
        if(strcmp(clave, "enemigo3") == 0)
        {
            std::string imagen = json.get_sprite_nave("enemigas", "enemigo1", "nave");
            std::string mensaje = "La imagen (" + imagen + ") no fue encontrada, se carga la imagen que muestra el error";

            if(!gNaveTexture.loadFromFile(imagen)){
                logger.error(mensaje.c_str());
                imagen = json.get_imagen_default("nave");
                gNaveTexture.loadFromFile(imagen.c_str());
            }
        }
        else{
            std::string imagen = json.get_sprite_nave("enemigas", "enemigo2", "nave");
            std::string mensaje = "La imagen (" + imagen + ") no fue encontrada, se carga la imagen que muestra el error";

            if(!gNaveTexture.loadFromFile(imagen)){
                logger.error(mensaje.c_str());
                imagen = json.get_imagen_default("nave");
                gNaveTexture.loadFromFile(imagen.c_str());
            }
        }
    }
}

void NaveEnemiga::quitTurret()
{
    if(turret)
    {
        turret = false;
        gNaveTexture.free();
        if(strcmp(clave, "enemigo3") == 0)
        {
            std::string imagen = json.get_sprite_nave("enemigas", "enemigo3", "nave");
            std::string mensaje = "La imagen (" + imagen + ") no fue encontrada, se carga la imagen que muestra el error";

            if(!gNaveTexture.loadFromFile(imagen)){
                logger.error(mensaje.c_str());
                imagen = json.get_imagen_default("nave");
                gNaveTexture.loadFromFile(imagen.c_str());
            }
        }
        else{
            std::string imagen = json.get_sprite_nave("enemigas", "enemigo4", "nave");
            std::string mensaje = "La imagen (" + imagen + ") no fue encontrada, se carga la imagen que muestra el error";

            if(!gNaveTexture.loadFromFile(imagen)){
                logger.error(mensaje.c_str());
                imagen = json.get_imagen_default("nave");
                gNaveTexture.loadFromFile(imagen.c_str());
            }
        }
    }
}
