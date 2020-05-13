#include "Nivel1.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Nave.h"
#include <list>

Nivel1::Nivel1(){}

void Nivel1::cargarNivel(){
    sMapaBG = json.get_sprite_mapa("nivel1", "bg");
    sCiudad = json.get_sprite_mapa("nivel1", "ciudad");
    sPlaneta = json.get_sprite_mapa("nivel1", "planeta");

    cargarImagen(sMapaBG, &gBGTexture);
    cargarImagen(sCiudad, &gCiudadTexture);
    cargarImagen(sPlaneta, &gPlanetaTexture);

    scrollingOffsetBG = 0;
    scrollingOffsetCity = 0;
    tierraInicial = 850;

    dataBG.h = 600;
    dataBG.w = 800;
    dataBG.x = 0;
    dataBG.y = 0;

    dataCiudad.h = 450;
    dataCiudad.w = 2048;
    dataCiudad.x = 0;
    dataCiudad.y = 0;
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

	SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( sdl.getRenderer() );

	gBGTexture.render( scrollingOffsetBG, 0, &dataBG );
	gBGTexture.render( scrollingOffsetBG + dataBG.w, 0, &dataBG );

	gPlanetaTexture.render(tierraInicial, -50);
    tierraInicial -= 0.2;

	gCiudadTexture.render(scrollingOffsetCity, 150, &dataCiudad);
	gCiudadTexture.render(scrollingOffsetCity + dataCiudad.w, 150, &dataCiudad);
}
