#ifndef MANEJODEJSON_H_
#define MANEJODEJSON_H_

#include <iostream>
#include <fstream>
#include <string>

#include "json.hpp"
#include "Mapa.h"

using namespace std;

class ManejoDeJson{
    public:

        ManejoDeJson();
        bool abrir_archivo();
        bool abrir_archivo_aux(const char* filename);

        const char* get_sprite_menu();
        int get_nivel_de_log();
        int get_cantidad_enemigos();

        nlohmann::json& searchValue(nlohmann::json& j_aux, const char* key);
        const char* get_sprite_mapa(const char* key, const char* sp);

    private:

        ifstream archivo_json;
        using json = nlohmann::json;
        json j;
};

#endif
