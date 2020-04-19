#include "Fondo.h"

//Inicia SDL y crea una ventana
bool iniciarSDL();

//Carga una imagen
bool cargarImagen();

//Libera espacio en memoria y cierra SDL
void cerrar();

void proceso();

SDL_Renderer* getRenderer();

int getScreenWidth();

int getScreenHeight();

Fondo getDotTexture();

//void aplicarImagen();
//
//void actualizarSuperficie();
//
//bool hayEventos();
//
//bool usuarioRequirioSalir();
