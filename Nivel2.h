#ifndef NIVEL2H
#define NIVEL2_H

#include "Nivel.h"
#include "TextureW.h"

class Nivel2: public Nivel{
    public:
        Nivel2();
        void cargarNivel();
        void cerrar();
        bool renderBackground();

    private:
        TextureW gBGTexture;
        TextureW gPlaneta1Texture;
        TextureW gPlaneta2Texture;
        TextureW gAsteroides1Texture;
        TextureW gAsteroides2Texture;

        TextureW gFinNivel;

        SDL_Rect dataAsteroides1;
        SDL_Rect dataAsteroides2;

        double corte_nivel;

        double scrollingOffsetPlaneta1;
        double scrollingOffsetPlaneta2;
        double scrollingOffsetAsteroides1;
        double scrollingOffsetAsteroides2;

};

#endif

