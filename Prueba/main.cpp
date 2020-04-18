#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

#include "ManejoDeSDL.h"
#include "Nave.h"
#include "Fondo.h"

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !iniciarSDL() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !cargarImagen() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
            proceso();
        }
    }

	//Free resources and close SDL
	cerrar();

	return 0;
}
