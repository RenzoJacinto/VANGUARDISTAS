#ifndef MANEJODEJSON_H_
#define MANEJODEJSON_H_

#include "NaveEnemiga.h"

class ManejoDeJson
{
    public:

        ManejoDeJson();
        int get_nivel_de_log();
        int get_cantidad_enemigos(NaveEnemiga enemigo);
        char* get_sprite_MenuBG();
        char* get_sprite_MapaBG();
        char* get_sprite_Ciudad();
        char* get_sprite_Planeta();
        char* get_sprite_Nave_Jugador();
        char* get_sprite_Nave_Enemiga();
};

#endif
