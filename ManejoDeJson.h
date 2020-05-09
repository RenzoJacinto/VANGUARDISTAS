#ifndef MANEJODEJSON_H_
#define MANEJODEJSON_H_

#include <iostream>
#include <fstream>
#include <string>

#include "Configuracion.h"
#include "json.hpp"

using namespace std;

class ManejoDeJson{
    public:

        ManejoDeJson();
        bool abrir_archivo();
        bool abrir_archivo_aux(const char* filename);
        void cargar_config();

        void cargar_stages(nlohmann::json& j_aux);
        void cargar_enemigo1(nlohmann::json& j_aux);
        void cargar_enemigo2(nlohmann::json& j_aux);
        void cargar_enemigos(nlohmann::json& j_aux);

        int get_nivel_de_log();
        int get_cantidad_enemigos_enemigo1();
        int get_cantidad_enemigos_enemigo2();
        const char* get_sprite_MenuBG();
        const char* get_sprite_MapaBG();
        const char* get_sprite_Ciudad();
        const char* get_sprite_Planeta();
        const char* get_sprite_Nave_Jugador();
        const char* get_sprite_Enemigo1();
        const char* get_sprite_Enemigo2();
        const char* get_sprite_Enemigo3();
        const char* get_sprite_Enemigo4();
        const char* get_sprite_Enemigo_Default();

    private:

        ifstream archivo_json;
        using json = nlohmann::json;
        json j;
        Configuracion config;
};

#endif
