#include "Nivel2.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Nave.h"
#include <list>

Nivel2::Nivel2(){}

void Nivel2::cargarNivel(){
    std::string bg = json.get_sprite_mapa("nivel2", "mapaBG");
    std::string planeta1 = json.get_sprite_mapa("nivel2", "planeta1");
    std::string planeta2 = json.get_sprite_mapa("nivel2", "planeta2");
    std::string asteroides1 = json.get_sprite_mapa("nivel2", "asteroides1");
    std::string asteroides2 = json.get_sprite_mapa("nivel2", "asteroides2");

    const char* sMapaBG = bg.c_str();
    const char* sPlaneta2= planeta2.c_str();
    const char* sPlaneta1 = planeta1.c_str();
    const char* sAsteroides1 = asteroides1.c_str();
    const char* sAsteroides2 = asteroides2.c_str();

    cargarImagen(sMapaBG, &gBGTexture);
    cargarImagen(sPlaneta1, &gPlaneta1Texture);
    cargarImagen(sPlaneta2, &gPlaneta2Texture);
    cargarImagen(sAsteroides1, &gAsteroides1Texture);
    cargarImagen(sAsteroides2, &gAsteroides2Texture);

    scrollingOffsetPlaneta1 = 850;
    scrollingOffsetPlaneta2 = 800;

    scrollingOffsetAsteroides1 = 0;
    scrollingOffsetAsteroides2 = 0;

    dataAsteroides1.h = 600;
    dataAsteroides1.w = 800;
    dataAsteroides1.x = 0;
    dataAsteroides1.y = 0;

    dataAsteroides2.h = 600;
    dataAsteroides2.w = 800;
    dataAsteroides2.x = 0;
    dataAsteroides2.y = 0;
}

void Nivel2::cerrar(){
	gBGTexture.free();
	gPlaneta1Texture.free();
	gPlaneta2Texture.free();
	gAsteroides1Texture.free();
	gAsteroides2Texture.free();
}

bool Nivel2::renderBackground(){


	scrollingOffsetAsteroides1 -= 10;
    if( scrollingOffsetAsteroides1 < -dataAsteroides1.w ) scrollingOffsetAsteroides1 = 0;

    scrollingOffsetAsteroides2 -= 5;
    if( scrollingOffsetAsteroides2 < -dataAsteroides2.w ) scrollingOffsetAsteroides2 = 0;

	SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( sdl.getRenderer() );

	gBGTexture.render(0, 0);

	gAsteroides2Texture.render( scrollingOffsetAsteroides2, 0, &dataAsteroides2 );
	gAsteroides2Texture.render( scrollingOffsetAsteroides2 + dataAsteroides2.w, 0, &dataAsteroides2 );

    gAsteroides1Texture.render(scrollingOffsetAsteroides1, 0, &dataAsteroides1);
	gAsteroides1Texture.render(scrollingOffsetAsteroides1 + dataAsteroides1.w, 0, &dataAsteroides1);

	gPlaneta2Texture.render(scrollingOffsetPlaneta2, -52);
    scrollingOffsetPlaneta2 -= 0.15;

	gPlaneta1Texture.render(scrollingOffsetPlaneta1, -74);
    scrollingOffsetPlaneta1 -= 0.25;
}

