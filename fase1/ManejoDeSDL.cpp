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
        logger.error(SDL_GetError());
		ok = false;
	}
	else{
        logger.debug("Se inicializó SDL");
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" )) logger.error("Warning: Linear texture filtering not enabled!");
        else logger.debug("Filtrado de textura lineal seteado");
		//Create window
		setWindow( SDL_CreateWindow( "GLEY LANCER", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN ) );
		if( gWindow == NULL ){
			logger.error(SDL_GetError());
			ok = false;
		}
		else{
            logger.debug("Se creó la ventana");
			//Create vsynced renderer for window
			setRenderer( SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ) );
			if( gRenderer == NULL ){
				logger.error(SDL_GetError());
				ok = false;
			} else{
                logger.debug("Se creó vSync para la ventana");
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				logger.debug("Se inicializó el renderizado de color");

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ){
					logger.error(IMG_GetError());
					ok = false;
				} else logger.debug("Se inicializó la carga de imagenes PNG");

			}
		}
	}
	return ok;
}

void ManejoDeSDL::cerrar(){
	SDL_DestroyRenderer( gRenderer );
	logger.debug("Se liberó memoria del renderizador");
	SDL_DestroyWindow( gWindow );
	logger.debug("Se liberó memoria de la ventana");
	setWindow(NULL);
	logger.debug("Se referenció la ventana como nula");
	setRenderer(NULL);
	logger.debug("Se referenció el renderizador como nulo");

	IMG_Quit();
	logger.debug("Se cerró IMG");
	SDL_Quit();
	logger.debug("Se cerró SDL");
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
