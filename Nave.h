#ifndef NAVE_H_
#define NAVE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

class Nave
{
    public:
		static const int DOT_WIDTH = 50;
		static const int DOT_HEIGHT = 21;

		static const int DOT_VEL = 10;

		Nave();

		void handleEvent( SDL_Event& e );
		void move();
		void render();

    private:

		int mPosX, mPosY;
		int mVelX, mVelY;
};

#endif
