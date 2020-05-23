#include "Nivel3.h"
#include "global.h"

Nivel3::Nivel3(){}

void Nivel3::cargarNivel(){
    std::string bg = json.get_sprite_mapa("nivel3", "mapaBG");
    std::string fondo1 = json.get_sprite_mapa("nivel3", "fondo1");
    std::string fondo2 = json.get_sprite_mapa("nivel3", "fondo2");
    std::string fondo3 = json.get_sprite_mapa("nivel3", "fondo3");
    std::string fondo4 = json.get_sprite_mapa("nivel3", "fondo4");
    std::string fondo5 = json.get_sprite_mapa("nivel3", "fondo5");
    std::string fondo6 = json.get_sprite_mapa("nivel3", "fondo6");

    std::string finNivel = json.get_sprite_mapa("nivel3", "finNivel");

    const char* sMapaBG = bg.c_str();
    const char* sFondo1= fondo1.c_str();
    const char* sFondo2= fondo2.c_str();
    const char* sFondo3= fondo3.c_str();
    const char* sFondo4= fondo4.c_str();
    const char* sFondo5= fondo5.c_str();
    const char* sFondo6= fondo6.c_str();

    const char* sFinNivel = finNivel.c_str();

    cargarImagen(sMapaBG, &gBGTexture);
    cargarImagen(sFondo1, &gFondo1Texture);
    cargarImagen(sFondo2, &gFondo2Texture);
    cargarImagen(sFondo3, &gFondo3Texture);
    cargarImagen(sFondo4, &gFondo4Texture);
    cargarImagen(sFondo5, &gFondo5Texture);
    cargarImagen(sFondo6, &gFondo6Texture);

    cargarImagen(sFinNivel, &gFinNivel);


    scrollingOffsetBG = 0;

    scrollingOffsetFondo1 = 0;
    scrollingOffsetFondo2 = 0;
    scrollingOffsetFondo3 = 0;
    scrollingOffsetFondo4 = 0;
    scrollingOffsetFondo5 = 0;
    scrollingOffsetFondo6 = 0;

    dataBG.h = 600;
    dataBG.w = 800;
    dataBG.x = 0;
    dataBG.y = 0;

    dataFondo1.h = 600;
    dataFondo1.w = 800;
    dataFondo1.x = 0;
    dataFondo1.y = 0;

    dataFondo2.h = 600;
    dataFondo2.w = 800;
    dataFondo2.x = 0;
    dataFondo2.y = 0;

    dataFondo3.h = 600;
    dataFondo3.w = 800;
    dataFondo3.x = 0;
    dataFondo3.y = 0;

    dataFondo4.h = 600;
    dataFondo4.w = 800;
    dataFondo4.x = 0;
    dataFondo4.y = 0;

    dataFondo5.h = 600;
    dataFondo5.w = 800;
    dataFondo5.x = 0;
    dataFondo5.y = 0;

    dataFondo6.h = 600;
    dataFondo6.w = 800;
    dataFondo6.x = 0;
    dataFondo6.y = 0;
}

void Nivel3::cerrar(){
	gBGTexture.free();
	gFondo1Texture.free();
	gFondo2Texture.free();
	gFondo3Texture.free();
	gFondo4Texture.free();
	gFondo5Texture.free();
	gFondo6Texture.free();

	gFinNivel.free();
}

bool Nivel3::renderBackground(){

    if(parallax()) return true;

	/*SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( sdl.getRenderer() ); */

	gBGTexture.render( scrollingOffsetBG, 0, &dataBG );
	gBGTexture.render( scrollingOffsetBG + dataBG.w, 0, &dataBG );

	gFondo1Texture.render( scrollingOffsetFondo1, 0, &dataFondo1 );
	gFondo1Texture.render( scrollingOffsetFondo1 + dataFondo1.w, 0, &dataFondo1 );

	gFondo2Texture.render( scrollingOffsetFondo2, 0, &dataFondo2 );
	gFondo2Texture.render( scrollingOffsetFondo2 + dataFondo2.w, 0, &dataFondo2 );

	gFondo3Texture.render( scrollingOffsetFondo3, 0, &dataFondo3 );
	gFondo3Texture.render( scrollingOffsetFondo3 + dataFondo3.w, 0, &dataFondo3);

	gFondo4Texture.render( scrollingOffsetFondo4, 0, &dataFondo4 );
	gFondo4Texture.render( scrollingOffsetFondo4 + dataFondo4.w, 0, &dataFondo4 );

	gFondo5Texture.render( scrollingOffsetFondo5, 0, &dataFondo5 );
	gFondo5Texture.render( scrollingOffsetFondo5 + dataFondo5.w, 0, &dataFondo5 );

	gFondo6Texture.render( scrollingOffsetFondo6, 0, &dataFondo6 );
	gFondo6Texture.render( scrollingOffsetFondo6 + dataFondo6.w, 0, &dataFondo6 );

    return false;
}

bool Nivel3::parallax(){

    scrollingOffsetBG -= 3;
    if( scrollingOffsetBG < -gBGTexture.getWidth() ){
        scrollingOffsetBG = 0;
        gFinNivel.render(0,0);
        return true;
    }

    scrollingOffsetFondo1 -= 15;
    if( scrollingOffsetFondo1 < -dataFondo1.w ) scrollingOffsetFondo1 = 0;

    scrollingOffsetFondo2 -= 13.5;
    if( scrollingOffsetFondo2 < -dataFondo2.w ) scrollingOffsetFondo2 = 0;

    scrollingOffsetFondo3 -= 12;
    if( scrollingOffsetFondo3 < -dataFondo3.w ) scrollingOffsetFondo3 = 0;

    scrollingOffsetFondo4 -= 10.5;
    if( scrollingOffsetFondo4 < -dataFondo4.w ) scrollingOffsetFondo4 = 0;

    scrollingOffsetFondo5 -= 9;
    if( scrollingOffsetFondo5 < -dataFondo5.w ) scrollingOffsetFondo5 = 0;

    scrollingOffsetFondo6 -= 7.5;
    if( scrollingOffsetFondo6 < -dataFondo6.w ) scrollingOffsetFondo6 = 0;

    return false;
}
