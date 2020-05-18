#include "Nivel1.h"
#include "global.h"

Nivel1::Nivel1(){}

void Nivel1::cargarNivel(){

    std::string bg = json.get_sprite_mapa("nivel1", "mapaBG");
    std::string ciudad = json.get_sprite_mapa("nivel1", "ciudad");
    std::string planeta = json.get_sprite_mapa("nivel1", "planeta");
    std::string nube1 = json.get_sprite_mapa("nivel1", "nube1");
    std::string nube2 = json.get_sprite_mapa("nivel1", "nube2");

    std::string finNivel = json.get_sprite_mapa("nivel1", "finNivel");

    const char* sMapaBG = bg.c_str();
    const char* sCiudad = ciudad.c_str();
    const char* sPlaneta = planeta.c_str();
    const char* sNube1 = nube1.c_str();
    const char* sNube2 = nube2.c_str();

    const char* sFinNivel = finNivel.c_str();

    cargarImagen(sMapaBG, &gBGTexture);
    cargarImagen(sCiudad, &gCiudadTexture);
    cargarImagen(sPlaneta, &gPlanetaTexture);
    cargarImagen(sNube1, &gNube1);
    cargarImagen(sNube2, &gNube2);

    cargarImagen(sFinNivel, &gFinNivel);

    scrollingOffsetBG = 0;
    scrollingOffsetCity = 0;
    tierraInicial = 700;
    scrollingOffsetNube1 = 120;
    scrollingOffsetNube2 = 0;

    dataBG.h = 600;
    dataBG.w = 2048;
    dataBG.x = 0;
    dataBG.y = 0;

    dataCiudad.h = 450;
    dataCiudad.w = 2048;
    dataCiudad.x = 0;
    dataCiudad.y = 0;

    dataNube1.h = 600;
    dataNube1.w = 800;
    dataNube1.x = 0;
    dataNube1.y = 0;

    dataNube2.h = 600;
    dataNube2.w = 800;
    dataNube2.x = 0;
    dataNube2.y = 0;
}

void Nivel1::cerrar(){
	gBGTexture.free();
	gCiudadTexture.free();
	gPlanetaTexture.free();
	gNube1.free();
	gNube2.free();

	gFinNivel.free();
}

bool Nivel1::renderBackground(){

    scrollingOffsetBG -= 0.5;
    if( scrollingOffsetBG < -dataBG.w ){
        scrollingOffsetBG = 0;
        gFinNivel.render(0,0);
        return true;
    }

	scrollingOffsetCity -= 5;
    if( scrollingOffsetCity < -dataCiudad.w ) scrollingOffsetCity = 0;

    scrollingOffsetNube1 -= 8;
    if( scrollingOffsetNube1 < -dataNube1.w ) scrollingOffsetNube1 = 0;

    scrollingOffsetNube2 -= 10;
    if( scrollingOffsetNube2 < -dataNube2.w ) scrollingOffsetNube2 = 0;

	SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( sdl.getRenderer() );

	gBGTexture.render( scrollingOffsetBG, 0, &dataBG );
	gBGTexture.render( scrollingOffsetBG + dataBG.w, 0, &dataBG );

	gPlanetaTexture.render(tierraInicial, -50);
    tierraInicial -= 0.2;

	gCiudadTexture.render(scrollingOffsetCity, 150, &dataCiudad);
	gCiudadTexture.render(scrollingOffsetCity + dataCiudad.w, 150, &dataCiudad);

	gNube1.render( scrollingOffsetNube1, 0, &dataNube1 );
	gNube1.render( scrollingOffsetNube1 + dataNube1.w, 0, &dataNube1 );
	gNube2.render( scrollingOffsetNube2, 0, &dataNube2 );
	gNube2.render( scrollingOffsetNube2 + dataNube2.w, 0, &dataNube2 );

	return false;
}
