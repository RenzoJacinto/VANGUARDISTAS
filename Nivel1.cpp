#include "Nivel1.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Nave.h"
#include <list>

Nivel1::Nivel1(){}

void Nivel1::cargarNivel(){
    std::string bg = json.get_sprite_mapa("nivel1", "mapaBG");
    std::string ciudad = json.get_sprite_mapa("nivel1", "ciudad");
    std::string planeta = json.get_sprite_mapa("nivel1", "planeta");
    std::string nube = json.get_sprite_mapa("nivel1", "nube");
    const char* sMapaBG = bg.c_str();
    const char* sCiudad = ciudad.c_str();
    const char* sPlaneta = planeta.c_str();
    const char* sNube = nube.c_str();

    cargarImagen(sMapaBG, &gBGTexture);
    cargarImagen(sCiudad, &gCiudadTexture);
    cargarImagen(sPlaneta, &gPlanetaTexture);
    cargarImagen(sNube, &gNube);

    scrollingOffsetBG = 0;
    scrollingOffsetCity = 0;
    tierraInicial = 850;
    scrollingOffsetNube = 0;

    dataBG.h = 600;
    dataBG.w = 2048;
    dataBG.x = 0;
    dataBG.y = 0;

    dataCiudad.h = 450;
    dataCiudad.w = 2048;
    dataCiudad.x = 0;
    dataCiudad.y = 0;

    dataNube.h = 600;
    dataNube.w = 800;
    dataNube.x = 0;
    dataNube.y = 0;
}

void Nivel1::cerrar(){
	gBGTexture.free();
	gCiudadTexture.free();
	gPlanetaTexture.free();
}

void Nivel1::renderBackground(){

    if( scrollingOffsetBG < -dataBG.w ) scrollingOffsetBG = 0;

	scrollingOffsetCity -= 5;
    if( scrollingOffsetCity < -dataCiudad.w ) scrollingOffsetCity = 0;

    scrollingOffsetNube -= 8;
    if( scrollingOffsetNube < -dataNube.w ) scrollingOffsetNube = 0;

	SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( sdl.getRenderer() );

	gBGTexture.render( scrollingOffsetBG, 0, &dataBG );
	gBGTexture.render( scrollingOffsetBG + dataBG.w, 0, &dataBG );

	gPlanetaTexture.render(tierraInicial, -50);
    tierraInicial -= 0.2;

	gCiudadTexture.render(scrollingOffsetCity, 150, &dataCiudad);
	gCiudadTexture.render(scrollingOffsetCity + dataCiudad.w, 150, &dataCiudad);

	gNube.render( scrollingOffsetNube, 0, &dataNube );
	gNube.render( scrollingOffsetNube + dataNube.w, 0, &dataNube );
}
