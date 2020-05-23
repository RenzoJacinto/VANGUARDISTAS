#ifndef MANEJODEJSON_H_
#define MANEJODEJSON_H_

#include <iostream>
#include <fstream>
#include <string>

#include "json.hpp"

using namespace std;

class ManejoDeJson{
    public:

        ManejoDeJson();
        bool abrir_archivo();
        bool abrir_archivo_aux(const char* filename);

        std::string get_sprite_menu();
        int get_nivel_de_log();
        nlohmann::json& searchValue(nlohmann::json& j_aux, const char* key);
        std::string get_sprite_mapa(char const* key, char const* sp);
        int get_cantidad_enemigo(char const* key, char const* sp);
        std::string get_sprite_nave(char const* key, char const* sp);

    private:

        ifstream archivo_json;
        using json = nlohmann::json;
        json j;
};

#endif
