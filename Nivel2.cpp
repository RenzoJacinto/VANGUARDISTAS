#include "Nivel2.h"
#include "global.h"

Nivel2::Nivel2(){}

void Nivel2::cargarNivel(){
    std::string bg = json.get_sprite_mapa("nivel2", "mapaBG");
    std::string planeta1 = json.get_sprite_mapa("nivel2", "planeta1");
    std::string planeta2 = json.get_sprite_mapa("nivel2", "planeta2");
    std::string asteroides1 = json.get_sprite_mapa("nivel2", "asteroides1");
    std::string asteroides2 = json.get_sprite_mapa("nivel2", "asteroides2");

    std::string finNivel = json.get_sprite_mapa("nivel2", "finNivel");

    const char* sMapaBG = bg.c_str();
    const char* sPlaneta2= planeta2.c_str();
    const char* sPlaneta1 = planeta1.c_str();
    const char* sAsteroides1 = asteroides1.c_str();
    const char* sAsteroides2 = asteroides2.c_str();

    const char* sFinNivel = finNivel.c_str();

    cargarImagen(sMapaBG, &gBGTexture);
    cargarImagen(sPlaneta1, &gPlaneta1Texture);
    cargarImagen(sPlaneta2, &gPlaneta2Texture);
    cargarImagen(sAsteroides1, &gAsteroides1Texture);
    cargarImagen(sAsteroides2, &gAsteroides2Texture);

    cargarImagen(sFinNivel, &gFinNivel);

    corte_nivel = 0;

    scrollingOffsetPlaneta1 = 740;
    scrollingOffsetPlaneta2 = 850;

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

	gFinNivel.free();
}

bool Nivel2::renderBackground(){

    corte_nivel -= 1;
    if(corte_nivel < -gBGTexture.getWidth()){
        corte_nivel = 0;
        gFinNivel.render(0,0);
        return true;
    }
	scrollingOffsetAsteroides1 -= 10;
    if( scrollingOffsetAsteroides1 < -dataAsteroides1.w ) scrollingOffsetAsteroides1 = 0;

    scrollingOffsetAsteroides2 -= 5;
    if( scrollingOffsetAsteroides2 < -dataAsteroides2.w ) scrollingOffsetAsteroides2 = 0;

	SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( sdl.getRenderer() );

	gBGTexture.render(0, 0);

	gPlaneta2Texture.render(scrollingOffsetPlaneta2, -500);
    scrollingOffsetPlaneta2 -= 0.25;

	gPlaneta1Texture.render(scrollingOffsetPlaneta1, -120);
    scrollingOffsetPlaneta1 -= 0.15;

	gAsteroides2Texture.render( scrollingOffsetAsteroides2, 0, &dataAsteroides2 );
	gAsteroides2Texture.render( scrollingOffsetAsteroides2 + dataAsteroides2.w, 0, &dataAsteroides2 );

    gAsteroides1Texture.render(scrollingOffsetAsteroides1, 0, &dataAsteroides1);
	gAsteroides1Texture.render(scrollingOffsetAsteroides1 + dataAsteroides1.w, 0, &dataAsteroides1);



    return false;
}

