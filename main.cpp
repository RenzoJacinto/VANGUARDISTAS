#include <SDL2/SDL.h>
#include <stdio.h>

bool inicializar();
void terminar();

SDL_Window* gWindow = NULL;
SDL_Surface* gSurface = NULL;

const int ANCHO_PANTALLA = 800;
const int ALTO_PANTALLA = 600;

int main(){
    if(!inicializar()) printf("Fallo la inicializacion de SDL\n");
    SDL_FillRect(gSurface, NULL, SDL_MapRGB(gSurface->format, 0xFF, 0xFF, 0xFF ));
    SDL_UpdateWindowSurface(gWindow);
    terminar();
}

bool inicializar(){
    bool ok = true;
    if(SDL_Init( SDL_INIT_VIDEO ) < 0){
        printf("No se pudo inicializar SDL, SDL_Error: %s\n", SDL_GetError);
        ok = false;
    } else{
        gWindow = SDL_CreateWindow("GLEY LANCER", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ANCHO_PANTALLA, ALTO_PANTALLA, SDL_WINDOW_SHOWN );
        if(gWindow == NULL){
			printf( "La ventana no pudo ser creada! SDL_Error: %s\n", SDL_GetError());
			ok = false;
		} else{
			gSurface = SDL_GetWindowSurface(gWindow);
		}
    }
    return ok;
}

void terminar(){
    SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	SDL_Quit();
}
