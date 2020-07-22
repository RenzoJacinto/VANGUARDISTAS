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

        //para todos
        nlohmann::json& searchValue(nlohmann::json& j_aux, const char* key);

        //para los de config
        std::string get_sprite_menu(const char* sp);
        int get_nivel_de_log();
        std::string get_sprite_mapa(char const* key, char const* sp);
        std::string get_sprite_nave(char const* key, char const* tipo, char const *sp);

        //Para los de default
        std::string get_sprite_menu_default(const char* sp);
        int get_nivel_de_log_default();
        std::string get_sprite_mapa_default(char const* key, char const* sp);
        std::string get_sprite_nave_default(char const* key, char const* tipo, char const *sp);

        //Para errores de encontrar sprite
        std::string get_imagen_default(const char* sp);

    private:

        ifstream archivo_json;
        using json = nlohmann::json;
        json j;
        json def;
};

#endif
