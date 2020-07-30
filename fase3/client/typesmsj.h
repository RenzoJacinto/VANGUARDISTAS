#ifndef TYPESMSJ_H_
#define TYPESMSJ_H_

#include <string>
#include <time.h>
#include <ctime>

#include "ColaMultihilo.h"

typedef struct posicionesR {
    int id;
    char descrip[15];
    int posX;
    int posY;
    int energiaActual;
    int vidas;
    int score;
} posicionesR_t;

typedef struct posiciones {
    int id;
    char descrip[15];
    int posX;
    int posY;
} posiciones_t;

typedef struct velocidades {
    int id;
    char descrip[5];
    int VelX;
    int VelY;
} velocidades_t;

typedef struct Credenciales {
    char id[28];
    char pass[28];
} credenciales_t;

static const int LOGIN_CORRECTO = 0;
static const int ERROR_LOGIN = 1;
static const int ID_YA_LOGUEADA = -1;
static const int JUEGO_LLENO = 2;
static const int ID_NO_LOGUEADA_RECON = -2;

static const int SCREEN_WIDTH = 800;
static const int SCREEN_HEIGHT = 600;

#endif
