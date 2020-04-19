#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

#include "ManejoDeSDL.h"
#include "Nave.h"
#include "Fondo.h"
#include "global.h"

ManejoDeSDL sdl;

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !sdl.iniciarSDL() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !sdl.cargarImagen() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
            sdl.proceso();
        }
    }

	//Free resources and close SDL
	sdl.cerrar();

	return 0;
}
