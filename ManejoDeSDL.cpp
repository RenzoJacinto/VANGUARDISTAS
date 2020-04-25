#include "global.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

ManejoDeSDL::ManejoDeSDL(){
    setScreenWidth(SCREEN_WIDTH);
    setScreenHeight(SCREEN_HEIGHT);
    setWindow(NULL);
    setRenderer(NULL);
}

bool ManejoDeSDL::iniciarSDL(){
	//Initialization flag
	bool ok = true;

	//Initialize SDL
	if( huboErrorAlIniciarSDL() ){
        if(logger.seDebeInformarError()) logger.informar(SDL_GetError());
		ok = false;
	}
	else{
        if (logger.seDebeInformarDebug()) logger.informar("Se inicializó SDL");
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) && logger.seDebeInformarError() ){ logger.informar("Warning: Linear texture filtering not enabled!");
        }else{ if (logger.seDebeInformarDebug()) logger.informar("Filtrado de textura lineal seteado");}
		//Create window
		setWindow( SDL_CreateWindow( "GLEY LANCER", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN ) );
		if( gWindow == NULL ){
			if(logger.seDebeInformarError()) logger.informar(SDL_GetError());
			ok = false;
		}
		else{
            if (logger.seDebeInformarDebug()) logger.informar("Se creó la ventana");
			//Create vsynced renderer for window
			setRenderer( SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ) );
			if( gRenderer == NULL ){
				if(logger.seDebeInformarError()) logger.informar(SDL_GetError());
				ok = false;
			}
			else{
                if (logger.seDebeInformarDebug()) logger.informar("Se creó vSync para la ventana");
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				if (logger.seDebeInformarDebug()) logger.informar("Se inicializó el renderizado de color");

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ){
					if(logger.seDebeInformarError()) logger.informar(IMG_GetError());
					ok = false;
				}else{
				if (logger.seDebeInformarDebug()) logger.informar("Se inicializó la carga de imagenes PNG");
				}
			}
		}
	}
	return ok;
}

void ManejoDeSDL::cerrar()
{
	SDL_DestroyRenderer( gRenderer );
	if (logger.seDebeInformarDebug()) logger.informar("Se liberó memoria del renderizador");
	SDL_DestroyWindow( gWindow );
	if (logger.seDebeInformarDebug()) logger.informar("Se liberó memoria de la ventana");
	setWindow(NULL);
	if (logger.seDebeInformarDebug()) logger.informar("Se referenció la ventana como nula");
	setRenderer(NULL);
	if (logger.seDebeInformarDebug()) logger.informar("Se referenció el renderizador como nulo");

	IMG_Quit();
	if (logger.seDebeInformarDebug()) logger.informar("Se cerró IMG");
	SDL_Quit();
	if (logger.seDebeInformarDebug()) logger.informar("Se cerró SDL");
}

bool ManejoDeSDL::huboErrorAlIniciarSDL(){
    return SDL_Init( SDL_INIT_VIDEO ) < 0;
}

void ManejoDeSDL::setWindow(SDL_Window* window){
    gWindow = window;
}

void ManejoDeSDL::setRenderer(SDL_Renderer* renderer){
    gRenderer = renderer;
}

SDL_Window* ManejoDeSDL::getWindow(){
    return gWindow;
}

SDL_Renderer* ManejoDeSDL::getRenderer(){
    return gRenderer;
}

void ManejoDeSDL::setScreenWidth(int width){
    screenWidth = width;
}

void ManejoDeSDL::setScreenHeight(int height){
    screenHeight = height;
}

int ManejoDeSDL::getScreenWidth(){
    return screenWidth;
}

int ManejoDeSDL::getScreenHeight(){
    return screenHeight;
}
