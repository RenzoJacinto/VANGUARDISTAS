#ifndef MANEJODEJSON_H_
#define MANEJODEJSON_H_

#include <iostream>
#include <fstream>
using namespace std;

//#include "json.hpp"

class ManejoDeJson{
    public:

        ManejoDeJson();
        bool abrir_archivo();
        int get_nivel_de_log();
        int get_cantidad_enemigos_enemigo1();
        int get_cantidad_enemigos_enemigo2();
        char* get_sprite_MenuBG();
        char* get_sprite_MapaBG();
        char* get_sprite_Ciudad();
        char* get_sprite_Planeta();
        char* get_sprite_Nave_Jugador();
        char* get_sprite_Nave_Enemiga();

    private:

        ifstream archivo_json;
};

#endif
