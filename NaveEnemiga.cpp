#include "NaveEnemiga.h"

NaveEnemiga::NaveEnemiga( int x, int y){
    std::string enemigo1 = json.get_sprite_nave("enemigas", "enemigo1");
    sEnemigo1 = enemigo1.c_str();
    std::string enemigo2 = json.get_sprite_nave("enemigas", "enemigo2");
    sEnemigo2 = enemigo2.c_str();
    std::string enemigo3 = json.get_sprite_nave("enemigas", "enemigo3");
    sEnemigo3 = enemigo3.c_str();
    std::string enemigo4 = json.get_sprite_nave("enemigas", "enemigo4");
    sEnemigo4 = enemigo4.c_str();
    seleccionarImagen();
    crearNave(x, y, getImagen());
    radio=getAltoImagen()/2;
}

void NaveEnemiga::mover( NaveJugador* jugador ){

    if (!desplazamientoPositivo) {

        setPosX(getPosX()-1);

        if( checkCollision( jugador , this ) ){
        setPosX(getPosX()+1);
        }

        if ( getPosX() - getRadio() < 0 ) {
            int posicionX = sdl.getScreenWidth();
            int posicionY = rand() % sdl.getScreenHeight();
            setPosX( posicionX );
            setPosY( posicionY );
        }

    } else {

        setPosX(getPosX()+1);

        if( checkCollision( jugador , this ) ){
            setPosX(getPosX()-1);
        }

        if ( getPosX() + getRadio() > sdl.getScreenWidth() ){
            int posicionX = 0;
            int posicionY = rand() % sdl.getScreenHeight();
            setPosX( posicionX );
            setPosY( posicionY );
        }
    }

        //Esta comentado por ahora, porque la nave solo se mueve en el eje X
//    //Move the dot up or down
//    mPosY += mVelY;
//	desplazarColicionador();
//
//    //If the dot collided or went too far up or down
//    if( ( getPosY() - getColicionador().r < 0 ) || ( getPosY() + getColicionador().r > SCREEN_HEIGHT ) || checkCollision( getColicionador(), enemigo.getCollider() ) )
//    {
//        //Move back
//        mPosY -= mVelY;
//		desplazarColicionador();
//    }
}

void NaveEnemiga::renderizar(){
	gNaveTexture.render(getPosX() - getRadio(), getPosY() - getRadio());
}

int NaveEnemiga::getRadio(){
    return radio;
}

void NaveEnemiga::seleccionarImagen(){

    const char* ruta;
    int seleccion = 1 + rand() % 5;
    switch(seleccion) {

    case 1 :
      ruta = sEnemigo1;
      desplazamientoPositivo=false;
      break;

    case 2:
      ruta = sEnemigo2;
      desplazamientoPositivo=false;
      break;

    case 3 :
      ruta = sEnemigo3;
      desplazamientoPositivo=true;
      break;

    case 4:
      ruta = sEnemigo4;
      desplazamientoPositivo=true;
      break;

    default :
        //Alguna otra imagen por default
        break;
    }

    imagen = ruta;
}

const char* NaveEnemiga::getImagen(){
    return imagen;
}

int NaveEnemiga::getAltoImagen(){
	return gNaveTexture.getHeight();
}

int NaveEnemiga::getAnchoImagen(){
	return gNaveTexture.getWidth();
}
